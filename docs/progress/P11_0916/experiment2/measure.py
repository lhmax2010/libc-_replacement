#!/usr/bin/env python3
"""发布 ELF 身份、版本信息和正向对照；每个所选产物静态读取五轮。"""
import csv
import json
import pathlib
import re
import subprocess
import sys
from scan_elf import OUT, CORPUS, PACKAGES, symbols, sha

ROOT = OUT.parents[3]
RECORDER = ROOT / 'docs/progress/API_0911/record.py'

def run(stem, args):
    command = [sys.executable, str(RECORDER), str(OUT / 'raw' / stem), *map(str, args)]
    subprocess.run(command, check=True)
    return OUT / 'raw' / (stem + '.stdout')

def main():
    rows = json.loads((OUT / 'MATCHES.json').read_text())
    packages = {r['checksum']: r for r in csv.DictReader(PACKAGES.open(), delimiter='\t')}
    rpm_meta = {}
    results = []
    controls = []
    for n, row in enumerate(rows, 1):
        rid = f'elf_{n:02}'
        h = row['rpm_sha256']
        package = packages[h]
        rpm = CORPUS / 'tmp/R11/rpms' / package['repo_id'] / package['location']
        path = pathlib.Path(row['local_path'])
        if h not in rpm_meta:
            actual = sha(rpm)
            assert actual == h, ('RPM身份不同', rpm, actual, h)
            meta = run(f'rpm_{n:02}_metadata', ['rpm', '-qp', '--qf',
                       '%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}\nFILEDIGESTALGO=%{FILEDIGESTALGO}\n[%{FILENAMES}\t%{FILEDIGESTS}\n]', rpm])
            lines = meta.read_text().splitlines()
            assert 'FILEDIGESTALGO=8' in lines, ('本轮仅接受SHA256载荷摘要', rpm)
            rpm_meta[h] = {line.split('\t')[0]: line.split('\t')[1] for line in lines if '\t' in line}
        before = sha(path)
        extraction_root = CORPUS / 'tmp/R11/extracted' / h[:2] / h
        resolved_member = '/' + str(path.resolve(strict=True).relative_to(extraction_root))
        expected = rpm_meta[h].get(resolved_member)
        assert before == expected, ('ELF载荷摘要不同', path, before, expected)
        out = dict(row, sample_id=rid, rpm_path=str(rpm), rpm_sha256_actual=h,
                   rpm_location=package['location'], elf_sha256=before, payload_sha256=expected,
                   symlink=path.is_symlink(), resolved_rpm_member=resolved_member, measurements=[])
        for repeat in range(1, 6):
            stem = f'{rid}_r{repeat}'
            sw = run(stem + '_Ws', ['readelf', '-Ws', '--wide', path])
            vv = run(stem + '_V', ['readelf', '-V', '--wide', path])
            matched = []
            with sw.open() as stream:
                for line in stream:
                    fields = line.split()
                    if len(fields) >= 8 and fields[0].endswith(':') and fields[7].split('@')[0] == 'ewk_parse_cookie':
                        matched.append(line.rstrip('\n'))
            assert matched, ('readelf正向对照未找到目标', path)
            custom = symbols(path)[1]
            assert custom == row['hits'], ('独立解析变化', path)
            for s in custom:
                exact = [line for line in matched if int(line.split()[0][:-1]) == s['dynsym_index']]
                assert exact and any(('UND' in line.split()) == (s['role']=='UND') for line in exact)
                if s['version_index'] in (None, 0, 1):
                    assert any('@' not in line.split()[7] for line in exact)
            out['measurements'].append({'repeat':repeat, 'readelf_Ws':str(sw.relative_to(OUT)),
                                       'readelf_V':str(vv.relative_to(OUT)), 'target_lines':matched,
                                       'parsed_symbols':custom, 'status':'PASS'})
        run(rid + '_dynamic', ['readelf', '-dW', path])
        # 真实版本化导入的正向对照，和目标使用完全相同的解析器。
        for known in (b'dlsym', b'malloc', b'__cxa_finalize'):
            found = symbols(path, known)[1]
            if any(x['role']=='UND' and x['version_index'] is not None and x['version_index']>1 for x in found):
                with (OUT / 'raw' / f'{rid}_r1_Ws.stdout').open() as f:
                    ctl_lines = [line.rstrip() for line in f if known.decode()+'@' in line and ' UND ' in line]
                assert ctl_lines
                controls.append({'sample':rid, 'symbol':known.decode(), 'parsed':found, 'readelf_lines':ctl_lines})
                break
        if row['name']=='chromium-efl' and path.name=='libchromium-ewk.so':
            run(rid + '_forwarder', ['llvm-objdump', '--disassemble-symbols=ewk_parse_cookie', path])
            run(rid + '_rodata', ['readelf', '-p', '.rodata', path])
            run(rid + '_full_disassembly', ['llvm-objdump', '-d', path])
        assert sha(path) == before
        results.append(out)
        (OUT / 'MEASUREMENTS.json').write_text(json.dumps(results, indent=2, ensure_ascii=False)+'\n')
        print('完成', rid, row['name'], row['arch'], row['path'], flush=True)
    assert controls, '必须有真实 UND＋命名版本的正向对照'
    (OUT / 'POSITIVE_CONTROLS.json').write_text(json.dumps(controls, indent=2, ensure_ascii=False)+'\n')
    print('逐 ELF 验证完成:', len(results), '正向对照:',len(controls), flush=True)

if __name__ == '__main__':
    main()
