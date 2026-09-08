#!/usr/bin/env python3
"""只打包本轮两套运行库及探针，逐文件记录 SHA256。"""
import hashlib,pathlib,shutil,tarfile,sys
r=pathlib.Path.cwd();base=r/'tmp/IMPL_0908';attempt=sys.argv[1] if len(sys.argv)>1 else '';name='payload'+('-'+attempt if attempt else '');dest=base/name;dest.mkdir(exist_ok=False)
for variant,build in [('old','build-baseline-armv7l'),('new','build-armv7l')]:
 (dest/variant).mkdir()
 for name in ['libc++.so.1','libc++abi.so.1']:
  shutil.copy2(base/build/'lib'/name,dest/variant/name)
(dest/'probes').mkdir()
for p in sorted((base/'probes-armv7l').iterdir()):
 if p.suffix!='.o':shutil.copy2(p,dest/'probes'/p.name)
files=sorted(p for p in dest.rglob('*') if p.is_file())
checks=''.join(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+str(p.relative_to(dest))+'\n' for p in files)
(dest/'SHA256SUMS').write_text(checks)
(r/('docs/progress/IMPL_0908/W3/PAYLOAD'+('_'+attempt if attempt else '')+'_SHA256SUMS')).write_text(checks)
with tarfile.open(base/(name+'.tar'),'w') as f:
 for p in sorted(dest.iterdir()):f.add(p,arcname=p.name)
print(checks,end='');print('PAYLOAD_TAR_SHA256',hashlib.sha256((base/(name+'.tar')).read_bytes()).hexdigest())
