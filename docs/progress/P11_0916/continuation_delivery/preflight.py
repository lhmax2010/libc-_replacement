#!/usr/bin/env python3
"""只读检查即将提交的路径、摘要、文档格式及脚本语法。"""
import json
import pathlib
import subprocess

root = pathlib.Path.cwd()
prefix = 'docs/progress/P11_0916/'
names = subprocess.check_output(['git', 'diff', '--cached', '--name-only', '-z']).decode().split('\0')
names = [n for n in names if n]
assert names
allowed_roots = ['experiment1/', 'experiment3/', 'doc_revision/', 'continuation_delivery/']
allowed_files = ['REPORT.md', 'STATUS.md', 'QUESTIONS.md', 'REVIEW_DISPOSITION.md', 'SHA256SUMS',
                 'adaptor_proposal_for_review.md', 'adaptor_proposal_for_review_en.md']
for name in names:
    if name in ['adaptor_proposal_for_review.md', 'adaptor_proposal_for_review_en.md']:
        continue
    assert name.startswith(prefix), name
    rel = name[len(prefix):]
    assert rel in allowed_files or any(rel.startswith(p) for p in allowed_roots), name
    assert '__pycache__' not in name and not name.endswith(('.so', '.o', '.a', '.deb')), name
    with (root / name).open('rb') as f:
        assert f.read(4) != b'\x7fELF', name
    if name.endswith('.py'):
        compile((root / name).read_text(), name, 'exec')
authored = [n for n in names if '/raw/' not in n and '/snapshots/' not in n and n.endswith(('.md', '.py', '.sh', '.gdb', '.c'))]
subprocess.run(['git', 'diff', '--cached', '--check', '--', *authored], check=True)
subprocess.run(['sha256sum', '--check', '--quiet', prefix + 'SHA256SUMS'], check=True)
assert subprocess.check_output(['git', 'branch', '--show-current']).decode().strip() == 'codex/runtime-validation'
print(json.dumps({'result': 'PASS', 'staged_files': len(names), 'authored_format_checked': len(authored),
                  'path_scope': 'two root reports and this continuation only', 'scripts_syntax': 'PASS',
                  'sealed_sha256': 'PASS', 'no_experimental_ELF_staged': True}, indent=2))
