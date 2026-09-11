#!/usr/bin/env python3
"""保存材料修改前的 Git 版本，原始材料不改写。"""
import pathlib
import subprocess
base=pathlib.Path('docs/progress/P6_0909/resume/before_current')
names=['docs/progress/IMPL_0909/W4/REPORT.md','docs/progress/IMPL_0909/W4/COMMIT_INDEX.tsv',
       'docs/progress/IMPL_0909/W4/EVIDENCE_INDEX.tsv','docs/progress/IMPL_0909/W4/verify_delivery.py',
       'docs/progress/IMPL_STATUS_0909.md','docs/progress/IMPL_QUESTIONS_0909.md']
names += [str(p) for p in pathlib.Path('docs/progress/IMPL_0909/W4/patches').glob('*.patch')]
for name in names:
    path=base/name;path.parent.mkdir(parents=True,exist_ok=True)
    data=subprocess.check_output(['git','show','HEAD:'+name])
    if path.exists():assert path.read_bytes()==data
    else:path.write_bytes(data)
print('PREVIOUS_GIT_VERSIONS_SAVED',len(names))
