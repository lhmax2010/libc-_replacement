#!/usr/bin/env python3
"""封存官方测试原始双流、JSON、逐次执行记录；验证归档内每个文件摘要。"""
import csv,hashlib,pathlib,tarfile
base=pathlib.Path('docs/progress/IMPL_0909/W2')
for name in ['lit_resume','lit_resume2','lit_resume3']:assert (base/'raw'/name/'exitcode').exists()
files=[]
for name in ['config_executor','lit_resume','lit_resume2','lit_resume3']:
    files.extend(x for x in sorted((base/'raw'/name).rglob('*')) if x.is_file())
archive=base/'OFFICIAL_RESUME_EVIDENCE.tgz';assert not archive.exists()
expected={str(p.relative_to(base)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files}
with tarfile.open(archive,'w:gz') as tar:
    for p in files:tar.add(p,arcname=str(p.relative_to(base)),recursive=False)
with tarfile.open(archive,'r:gz') as tar:
    observed={m.name:hashlib.sha256(tar.extractfile(m).read()).hexdigest() for m in tar.getmembers() if m.isfile()}
assert expected==observed
with (base/'OFFICIAL_RESUME_MANIFEST.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['archive_member','sha256']);w.writerows(sorted(expected.items()))
print('ARCHIVE_MEMBERS_VERIFIED',len(files),'SHA256',hashlib.sha256(archive.read_bytes()).hexdigest(),'PATH',archive,'SIZE',archive.stat().st_size)
print('Original expanded files retained locally; no core ZIPs or environment dumps included')
