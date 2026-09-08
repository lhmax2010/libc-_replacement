#!/usr/bin/env python3
"""无损归档官方大日志；保留每个原文件路径、大小与 SHA256，不删原件。"""
import csv,hashlib,pathlib,tarfile
r=pathlib.Path('docs/progress/IMPL_0908/W3');raw=r/'raw'
dirs=sorted(p for p in raw.glob('lit_*') if p.is_dir())
assert dirs
for p in dirs:assert (p/'exitcode').is_file(),('run still active',p)
files=sorted(p for d in dirs for p in d.rglob('*') if p.is_file())
dest=r/'official_evidence.tar.gz';assert not dest.exists()
with tarfile.open(dest,'w:gz') as archive:
 for p in files:archive.add(p,arcname=str(p.relative_to(r)),recursive=False)
with (r/'OFFICIAL_EVIDENCE_MANIFEST.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['path','bytes','sha256'])
 for p in files:w.writerow([str(p.relative_to(r)),p.stat().st_size,hashlib.sha256(p.read_bytes()).hexdigest()])
# Verify every compressed member against its original bytes before handoff.
with tarfile.open(dest,'r:gz') as archive:
 for member in archive.getmembers():
  original=r/member.name
  assert hashlib.sha256(archive.extractfile(member).read()).digest()==hashlib.sha256(original.read_bytes()).digest()
print('OFFICIAL_ARCHIVE_VERIFIED=PASS files='+str(len(files)))
print('ARCHIVE_BYTES',dest.stat().st_size,'SHA256',hashlib.sha256(dest.read_bytes()).hexdigest())
print('ORIGINALS_RETAINED_UNTRACKED=YES; archive stores complete original command/output/exit bytes')
