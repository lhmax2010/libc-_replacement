"""Validate the scoped staged delivery without printing credential material."""
import base64
import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

P = Path.cwd()
E = P / 'docs/progress/LLVM_W4_0923'
C = P / 'tmp/BUILD_W1BC_0911/target-fetch'

def git(*args, cwd=P):
    return subprocess.check_output(['git', *args], cwd=cwd)

names = [n for n in git('diff', '--cached', '--name-only', '-z').decode().split('\0') if n]
assert names and all(n.startswith('docs/progress/LLVM_W4_0923/') or n == 'docs/LINE_STATUS.md' for n in names)
assert 'docs/progress/LLVM_W4_0923/SELFCHECK_R2.json' not in names
secrets, blobs = [], []
for filename in ['.quickbuild-credentials', 'password.md']:
    f = P / filename
    if f.is_file():
        lines = f.read_bytes().splitlines()
        secrets.extend(v for v in (lines[1:] if filename == '.quickbuild-credentials' else lines) if len(v) > 3)
for f in [Path.home()/'.oscrc', Path.home()/'.netrc']:
    if f.is_file():
        data = f.read_bytes()
        blobs.append(data)
        secrets.extend(m.group(1) for m in re.finditer(rb'(?im)(?:password|passwd|token|pass)\s*(?:=\s*|\s+)([^\s]+)', data) if len(m.group(1)) > 3)
ssh = Path.home()/'.ssh'
if ssh.is_dir():
    for f in ssh.iterdir():
        if f.is_file():
            data = f.read_bytes()
            if len(data) > 32:
                blobs.append(data)
            if b'PRIVATE KEY' in data:
                secrets.extend(s for s in data.splitlines() if len(s) > 40 and not s.startswith(b'-----'))
needles = [v for s in secrets for v in (s, base64.b64encode(s), s.hex().encode())] + [b for b in blobs if len(b) > 32]
total = 0
for name in names:
    data = git('show', ':'+name)
    assert not any(s in data for s in needles), 'STOP: credential match; do not commit'
    assert not re.search(rb'-----BEGIN (?:OPENSSH |RSA |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{40,}|(?im:^Authorization:\s*\S+)|(?im:^Cookie:\s*\S+)', data), 'STOP: sensitive pattern; do not commit'
    assert len(data) < 5*1024*1024, 'STOP: large artifact'
    total += len(data)
commits = ['617a210064c4559fe1152728f7c9cf5e1ab99a9f', 'db5b49afa7836db0414140208b2d3ab4f4aa7293']
base = 'f203923a1508c9344f5fc6b17bd8822f011655c4'
assert git('rev-parse', 'HEAD', cwd=C).decode().strip() == commits[-1]
assert not git('status', '--porcelain', cwd=C).strip()
assert git('rev-list', '--left-right', '--count', base+'...HEAD', cwd=C).split() == [b'0', b'2']
for commit in commits:
    assert git('show', '-s', '--format=%an <%ae>', commit, cwd=C).decode().strip() == 'Hao Lin <hao.lin@samsung.com>'
    assert b'Signed-off-by:' not in git('show', '-s', '--format=%B', commit, cwd=C)
assert hashlib.sha256((C/'packaging/libcxx-runtimes.spec').read_bytes()).hexdigest() == 'c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe'
gates = json.loads((E/'prep-check/runtime_R3/RESULT_R3.json').read_text())
assert len(gates) == 4 and all(x['result'] == 'PASS' for x in gates)
assert json.loads((E/'prep-check/runtime_R3/prep-command_R3.json').read_text())['exitcode'] == 0
preserved = ['docs/progress/LLVM_W4_0923/inputs_R2', 'docs/progress/LLVM_W4_0923/prep-check/runtime_R2', 'docs/progress/LLVM_W4_0923/equivalence_R2.py']
assert not git('diff', '542cbb6ca680b485840af512b70d24ba8cc0e436', '--', *preserved)
rows = list(csv.DictReader((E/'spec-audit_R3/LOCAL_SPECS_R3.tsv').open(), delimiter='\t'))
summary = json.loads((E/'spec-audit_R3/COMPARISON_SUMMARY_R3.json').read_text())
assert len(rows) == len({r['path'] for r in rows}) == summary['paths'] == 112
assert len(summary['other_different_ids']) == 26
assert (E/'spec-audit_R3/EXTRA_DIFFERENCES_R3.diff').stat().st_size > 0
result = dict(secret_scan='PASS', staged_files=len(names), staged_bytes=total,
              scope='LLVM_W4_0923 and LINE_STATUS only; prior SELFCHECK_R2 left untouched',
              package_clean=True, ahead=2, behind=0, package_push=False,
              runtime_equivalence='4/4 PASS', runtime_prep_exitcode=0,
              R2_preserved=True, local_spec_paths=112, other_difference_paths=26,
              task_result='PARTIAL_STOP_FOR_REVIEW')
(E/'SELFCHECK_R3.json').write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
(E/'SCRIPT_SHA256_R3.json').write_text(json.dumps({f.name: hashlib.sha256(f.read_bytes()).hexdigest() for f in sorted(E.glob('*_R3.py'))}, indent=2)+'\n')
print(json.dumps(result, ensure_ascii=False))
