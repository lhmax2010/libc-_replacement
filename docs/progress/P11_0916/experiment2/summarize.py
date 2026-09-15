#!/usr/bin/env python3
"""汇总已完成静态检查，生成可复核身份表及压缩的输入快照。"""
import collections
import csv
import gzip
import hashlib
import json
import pathlib
import struct
from scan_elf import OUT, INVENTORY, PACKAGES, sha

def virtual_bytes(path, address, size):
    with path.open('rb') as f:
        ident = f.read(16)
        wide = ident[4] == 2
        endian = '<' if ident[5] == 1 else '>'
        fmt = endian + ('HHIQQQIHHHHHH' if wide else 'HHIIIIIHHHHHH')
        h = struct.unpack(fmt, f.read(struct.calcsize(fmt)))
        pfmt = endian + ('IIQQQQQQ' if wide else 'IIIIIIII')
        for i in range(h[9]):
            f.seek(h[4] + i*h[8])
            p = struct.unpack(pfmt, f.read(struct.calcsize(pfmt)))
            off, va, length = (p[2],p[3],p[5]) if wide else (p[1],p[2],p[4])
            if p[0] == 1 and va <= address and address+size <= va+length:
                f.seek(off+address-va)
                return f.read(size)
    raise ValueError('虚拟地址未落在 LOAD 文件范围')

def main():
    samples = json.loads((OUT/'MEASUREMENTS.json').read_text())
    scan = [json.loads(s) for s in (OUT/'SCAN.jsonl').open()]
    controls = json.loads((OUT/'POSITIVE_CONTROLS.json').read_text())
    assert len(samples) == 18 and len(controls) == 6
    assert all(len(r['measurements']) == 5 for r in samples)
    assert all(m['status']=='PASS' for r in samples for m in r['measurements'])
    assert all(s['role']=='DEFINED' and s['version_index']==1 for r in samples for s in r['hits'])
    assert not any(s['role']=='UND' for r in scan for s in r['hits'])
    virtual_checks = []
    for r in samples:
        if r['name'] != 'chromium-efl' or not r['path'].endswith('/libchromium-ewk.so'):
            continue
        address = {'x86_64':0xf9ce, 'aarch64':0xf992, 'armv7l':0xeece}[r['arch']]
        data = virtual_bytes(pathlib.Path(r['local_path']), address, 17)
        assert data.startswith(b'ewk_parse_cookie\0')
        virtual_checks.append({'sample':r['sample_id'], 'arch':r['arch'],
                               'address':hex(address), 'bytes_hex':data.hex(),
                               'assertion':'以 ewk_parse_cookie NUL 字符串开始', 'status':'PASS'})
    (OUT/'FORWARDER_STRING_CHECKS.json').write_text(json.dumps(virtual_checks,indent=2,ensure_ascii=False)+'\n')
    with (OUT/'SAMPLES.tsv').open('w') as f:
        cols = ['sample_id','name','arch','sourcerpm','rpm_location','rpm_sha256','path','elf_sha256']
        w = csv.DictWriter(f, cols, delimiter='\t', extrasaction='ignore')
        w.writeheader(); w.writerows(samples)
    lines = ['# 产物身份与符号读取结果', '', '本表由 summarize.py 从实测 JSON 生成。5 轮是静态读取，不是执行目标函数。', '',
             '| 样本 | 包 / 架构 | RPM 内路径 | ELF SHA256 | 动态符号序号 / 版本索引 |',
             '| --- | --- | --- | --- | --- |']
    for r in samples:
        s = r['hits'][0]
        lines.append(f"| {r['sample_id']} | {r['name']} / {r['arch']} | `{r['path']}` | `{r['elf_sha256']}` | {s['dynsym_index']} / {s['version_index']} |")
    lines += ['', '## 来源 RPM', '', '| 包 / 架构 | 发布路径 | 完整 RPM SHA256 |', '| --- | --- | --- |']
    seen = set()
    for r in samples:
        if r['rpm_sha256'] in seen: continue
        seen.add(r['rpm_sha256'])
        lines.append(f"| {r['name']} / {r['arch']} | `{r['rpm_location']}` | `{r['rpm_sha256']}` |")
    lines += ['', '## readelf -Ws 原文摘录', '']
    for r in samples:
        lines += [f"### {r['sample_id']}", '', '完整 -Ws 与 -V：'+
                  f"[符号表]({r['measurements'][0]['readelf_Ws']}) / [版本表]({r['measurements'][0]['readelf_V']})。", '', '```text',
                  *r['measurements'][0]['target_lines'], '```', '']
    (OUT/'SAMPLES.md').write_text('\n'.join(lines)+'\n')
    counts = {'inventory_rows':len(scan), 'distinct_rpm_hashes':len({r['rpm_sha256'] for r in scan}),
              'by_repository':dict(collections.Counter(r['repo_id'] for r in scan)),
              'by_architecture':dict(collections.Counter(r['arch'] for r in scan)),
              'scanned_no_dynsym_table':sum(r.get('dynsym_tables')==0 for r in scan),
              'dynamic_symbol_table_rows':sum(r.get('dynsym_tables',0)>0 for r in scan),
              'target_definition_rows':len(samples),'distinct_target_elf_sha256':len({r['elf_sha256'] for r in samples}),
              'definition_rows_by_package':dict(collections.Counter(r['name'] for r in samples)),
              'measurements':sum(len(r['measurements']) for r in samples), 'positive_control_elfs':len(controls)}
    (OUT/'AUDIT_COUNTS.json').write_text(json.dumps(counts,indent=2,ensure_ascii=False)+'\n')
    snapshots = OUT/'inputs'
    snapshots.mkdir(exist_ok=True)
    identities = []
    for source in (INVENTORY, PACKAGES, OUT/'SCAN.jsonl'):
        compressed = snapshots/(source.name+'.gz')
        original_hash = hashlib.sha256()
        with source.open('rb') as src, compressed.open('wb') as dst:
            with gzip.GzipFile(filename='',mode='wb',fileobj=dst,mtime=0) as g:
                for chunk in iter(lambda:src.read(4*1024*1024), b''):
                    original_hash.update(chunk); g.write(chunk)
        with gzip.open(compressed,'rb') as check:
            decoded_hash = hashlib.sha256()
            for chunk in iter(lambda:check.read(4*1024*1024), b''):
                decoded_hash.update(chunk)
            recovered = decoded_hash.hexdigest()
        assert recovered == original_hash.hexdigest()
        identities.append({'source':str(source), 'source_sha256':recovered,
                           'archive':str(compressed.relative_to(OUT)), 'archive_sha256':sha(compressed),
                           'roundtrip':'PASS'})
    (OUT/'SNAPSHOT_IDENTITY.json').write_text(json.dumps(identities,indent=2)+'\n')
    print(json.dumps(counts,indent=2,ensure_ascii=False))
    print('地址字符串断言、18 样本 / 90 静态读取、6 个正向对照及快照往返校验通过')

if __name__ == '__main__':
    main()
