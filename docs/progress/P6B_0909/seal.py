#!/usr/bin/env python3
"""只封存 P6B 证据，校验授权改动范围；不执行 Git 写操作。"""
import csv
import hashlib
import pathlib
import re
import subprocess

root = pathlib.Path.cwd()
out = root/'docs/progress/P6B_0909'
def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

changed = list(csv.DictReader((out/'CHANGED_FILES.tsv').open(),delimiter='\t'))
allowed = {r['path'] for r in changed}
protected = {
 'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c',
 'docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c',
}
for p,expected in protected.items():
    assert sha(root/p) == expected
for row in changed:
    assert sha(root/row['path']) == row['after_sha256']
actual = set(subprocess.check_output(['git','diff','--name-only']).decode().splitlines())
assert actual <= allowed | set(protected), actual - allowed - set(protected)
for p in out.rglob('*.exitcode'):
    if 'raw' in p.relative_to(out).parts:
        assert p.read_text().strip() == '0',p
for p in (out/'raw').rglob('*.command.txt'):
    text = p.read_text()
    assert not re.search(r'--force(?:\s|=|-|$)|\s--mirror(?:\s|$)',text),p
for p in [out/'REPORT.md',out/'BRANCH_MAPPING.md',out/'PRE_DELETE_CONFIRMATION.md']:
    for link in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if link.startswith(('https://','http://')):
            continue
        assert (p.parent/link.split('#',1)[0]).resolve().exists(), (p,link)

# 未跟踪材料的补充范围检查：历史 raw/交付回执及本轮审计不充当现行取件入口。
others = subprocess.check_output(['git','ls-files','--others','--exclude-standard','-z','--','docs','HQ_files']).decode().split('\0')
supplement = []
for name in others:
    if not name or name.startswith('docs/progress/P6B_0909/'):
        continue
    p = root/name
    parts = p.relative_to(root).parts
    if any(x in parts for x in ['raw','delivery','delivery_resume','__pycache__']):
        continue
    if not p.is_file():
        continue
    data = p.read_bytes()
    if b'\0' in data:
        continue
    text = data.decode('utf-8')
    count = text.count('libcxx-noexcept-relief')
    assert count == 0, name
    supplement.append((name,sha(p),count))
with (out/'UNTRACKED_CURRENT_CHECK.tsv').open('w',newline='') as f:
    w = csv.writer(f,delimiter='\t',lineterminator='\n')
    w.writerow(['path','sha256','old_branch_occurrences'])
    w.writerows(supplement)

scripts = sorted(p for p in out.rglob('*.py') if not any(x in p.relative_to(out).parts for x in ['before_current','delivery']))
(out/'SCRIPT_SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(out)}\n' for p in scripts))
paths = sorted(p for p in out.rglob('*') if p.is_file() and 'delivery' not in p.relative_to(out).parts and p.name not in {'SHA256SUMS','EVIDENCE_INDEX.tsv'})
# before_current 的既有 EVIDENCE_INDEX 也是证据，不能因为同名而遗漏。
paths += sorted(p for p in (out/'before_current').rglob('EVIDENCE_INDEX.tsv'))
paths = sorted(set(paths))
with (out/'EVIDENCE_INDEX.tsv').open('w',newline='') as f:
    w = csv.writer(f,delimiter='\t',lineterminator='\n')
    w.writerow(['path','bytes','sha256'])
    w.writerows((str(p.relative_to(out)),p.stat().st_size,sha(p)) for p in paths)
paths.append(out/'EVIDENCE_INDEX.tsv')
(out/'SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(out)}\n' for p in sorted(paths)))
print(f'SCOPE=PASS files={len(changed)} protected_dirty_files=2 historical_raw_exitcodes=PASS force_commands=0 untracked_current_files={len(supplement)} evidence_files={len(paths)} scripts={len(scripts)}')
