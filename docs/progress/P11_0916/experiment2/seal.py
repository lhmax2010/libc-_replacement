#!/usr/bin/env python3
"""归档校验：断言计数、命令记录完整性、内部链接与脚本/证据摘要。"""
import hashlib
import json
import pathlib
import re
from scan_elf import OUT, sha

def main():
    counts = json.loads((OUT/'AUDIT_COUNTS.json').read_text())
    assert counts['inventory_rows'] == 39166
    assert counts['dynamic_symbol_table_rows'] + counts['scanned_no_dynsym_table'] + 1 == 39166
    assert counts['target_definition_rows'] == 18 and counts['distinct_target_elf_sha256'] == 12
    assert counts['measurements'] == 90 and counts['positive_control_elfs'] == 6
    measurements = json.loads((OUT/'MEASUREMENTS.json').read_text())
    assert sum(r['name']=='chromium-efl' for r in measurements) == 6
    assert sum(r['name']=='app-rootstrap-devel' for r in measurements) == 12
    errors = []
    for p in OUT.parent.rglob('*.md'):
        for target in re.findall(r'\]\(([^)]+)\)', p.read_text()):
            if '://' in target or target.startswith('#'): continue
            q = (p.parent/target.split('#')[0]).resolve()
            if not q.exists(): errors.append([str(p),target])
    assert not errors, errors
    completed = []
    for p in sorted((OUT/'raw').glob('*.exitcode')):
        stem = str(p)[:-len('.exitcode')]
        for suffix in ('.command.txt','.stdout','.stderr','.time.json'):
            assert pathlib.Path(stem+suffix).exists(), stem+suffix
        completed.append({'stem':pathlib.Path(stem).name,'exit_code':int(p.read_text())})
    failed = [c for c in completed if c['exit_code']]
    assert {(c['stem'],c['exit_code']) for c in failed} == {
        ('007_ld_version',22),('023_summarize',1)}, failed
    # 封存已完成 raw；自身运行记录在函数返回后才完整，交付记录另存。
    selected = [p for p in OUT.rglob('*') if p.is_file()
                and '__pycache__' not in p.parts and p.name not in ('SCAN.jsonl','SHA256SUMS','SEAL.json')
                and not p.name.startswith('030_seal')]
    with (OUT/'SHA256SUMS').open('w') as f:
        for p in sorted(selected):
            f.write(sha(p)+'  '+str(p.relative_to(OUT))+'\n')
    result = {'internal_links':'PASS','counts':'PASS','completed_commands':len(completed),
              'nonzero_commands':failed,'checksummed_files':len(selected),
              'checksum_scope':'experiment2 已有文件，排除未压缩重复扫描、缓存、清单自身及正在运行的 030_seal 记录；delivery 另存',
              'uncompressed_scan_sha256':sha(OUT/'SCAN.jsonl')}
    (OUT/'SEAL.json').write_text(json.dumps(result,indent=2,ensure_ascii=False)+'\n')
    print(json.dumps(result,indent=2,ensure_ascii=False))

if __name__ == '__main__':
    main()
