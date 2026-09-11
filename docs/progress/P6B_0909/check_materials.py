#!/usr/bin/env python3
"""核验现行分支依赖、历史保全、提交引用和材料差异，失败即非零退出。"""
import csv
import hashlib
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path.cwd()
OUT = ROOT / 'docs/progress/P6B_0909'
SRC = ROOT / 'tmp/IMPL_0908/source'
phase = sys.argv[1]
OLD = 'libcxx-noexcept-relief'
NEW = 'libcxx-ehabi-backport'
history_paragraphs = {'docs/progress/IMPL_0908/W1/REPORT.md', 'docs/progress/IMPL_0908/W2/REPORT.md'}

def sha(data):
    return hashlib.sha256(data).hexdigest()

def read(name):
    return list(csv.DictReader((OUT/name).open(), delimiter='\t'))

def write(name, columns, rows):
    with (OUT/name).open('w', newline='') as f:
        writer = csv.writer(f, delimiter='\t', lineterminator='\n')
        writer.writerow(columns)
        writer.writerows(rows)

seq = 0
def git(*args):
    global seq
    seq += 1
    stem = OUT/'raw'/('materials_'+phase)/f'{seq:03d}'
    subprocess.run([sys.executable,str(OUT/'record.py'),str(stem),'git','-C',str(SRC),*args],check=True)
    return pathlib.Path(str(stem)+'.stdout').read_bytes()

rows = read('BRANCH_REFERENCES_after.tsv')
history = read('HISTORICAL_FILE_HASHES.tsv')
historical_files = {r['path'] for r in history}
for row in history:
    assert sha((ROOT/row['path']).read_bytes()) == row['sha256'], row['path']
allowed = historical_files | history_paragraphs
assert {r['path'] for r in rows} <= allowed
for p in history_paragraphs:
    before = (OUT/'before_current'/p).read_text()
    now = (ROOT/p).read_text()
    old_lines = [line for line in before.splitlines() if OLD in line]
    now_lines = [line for line in now.splitlines() if OLD in line]
    assert len(old_lines) == 1 and old_lines == now_lines
    assert '下文建立分支／推送到旧分支的叙述是原阶段操作记录' in now
exception_rows = [(r['path'],r['line'],r['occurrences'],'历史操作段落（顶端已附当前映射）' if r['path'] in history_paragraphs else '历史原文（摘要一致）',r['context']) for r in rows]
write('HISTORICAL_BRANCH_REFERENCES.tsv',['path','line','occurrences','reason','context'],exception_rows)

changed = sorted((OUT/'before_current').rglob('*'))
changed = [p for p in changed if p.is_file()]
commits = read('FIVE_COMMITS_BEFORE.tsv')
known = [r['commit'] for r in commits] + ['c3f8578a4db871d9d6de96d751f4c2ea7b1638fa']
locations = []
file_rows = []
for i,p in enumerate(changed,1):
    relative = p.relative_to(OUT/'before_current')
    current = ROOT/relative
    before, after = p.read_bytes(), current.read_bytes()
    text = after.decode()
    if str(relative) not in history_paragraphs:
        assert OLD not in text, str(relative)
    if current.suffix == '.md':
        assert NEW in text and 'BRANCH_MAPPING.md' in text
        for line_no,line in enumerate(text.splitlines(),1):
            for token in re.findall(r'(?<![0-9a-f])[0-9a-f]{7,40}(?![0-9a-f])',line):
                matches = [s for s in known if s.startswith(token)]
                if matches:
                    assert len(matches) == 1
                    locations.append([str(relative),line_no,token,matches[0],'REACHABLE'])
        for url in re.findall(r'\]\(([^)]+)\)',text):
            if url.startswith(('http://','https://')):
                continue
            target = (current.parent/url.split('#',1)[0]).resolve()
            assert target.exists(), (relative,url)
    diff = subprocess.run(['git','diff','--no-index','--',str(p),str(current)],capture_output=True)
    assert diff.returncode in (0,1)
    path = OUT/'diffs'/f'{i:02d}_{current.name}.diff'
    path.parent.mkdir(exist_ok=True)
    path.write_bytes(diff.stdout)
    pathlib.Path(str(path)+'.stderr').write_bytes(diff.stderr)
    pathlib.Path(str(path)+'.exitcode').write_text(str(diff.returncode)+'\n')
    pathlib.Path(str(path)+'.command.txt').write_text(__import__('shlex').join(diff.args)+'\n')
    file_rows.append([str(relative),sha(before),sha(after),str(path.relative_to(OUT))])
assert {r[3] for r in locations} >= set(known[:5])
for commit in sorted({r[3] for r in locations}):
    git('merge-base','--is-ancestor',commit,'refs/heads/sandbox/lhmax2025/'+NEW)
assert git('rev-parse','HEAD').decode().strip() == known[4]
assert not git('status','--porcelain').strip()
positive_rows = read('BRANCH_REFERENCES_before.tsv')
assert sum(int(r['occurrences']) for r in positive_rows if r['category']=='CURRENT_REVIEW' and r['path'] not in history_paragraphs) == 8
assert OLD in ('sandbox/lhmax2025/'+OLD)
write('CURRENT_COMMIT_REFERENCES.tsv',['path','line','token','commit','status'],locations)
write('CHANGED_FILES.tsv',['path','before_sha256','after_sha256','diff'],file_rows)
result = dict(current_old_branch_dependencies=0,positive_control='PASS',changed_files=len(file_rows),preserved_historical_files=len(history),preserved_historical_paragraphs=2,current_commit_locations=len(locations),sha_refs_reachable=True,raw_history_unchanged=True)
(OUT/('MATERIAL_CHECK_'+phase+'.json')).write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result))
