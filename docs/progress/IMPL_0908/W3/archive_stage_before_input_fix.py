#!/usr/bin/env python3
"""冻结阶段脚本、构建配置与提交差异；二进制只记录身份。"""
import hashlib,pathlib,subprocess,sys,shutil
stage=sys.argv[1];r=pathlib.Path('docs/progress/IMPL_0908');out=r/stage;src=pathlib.Path('tmp/IMPL_0908/source')
dest=out/'snapshots';dest.mkdir(exist_ok=True)
for p in (r/'code').iterdir():
 if p.is_file():shutil.copyfile(p,dest/p.name)
builds=['build-native','build-baseline-native']
if stage=='W3':builds+=['build-armv7l','build-baseline-armv7l']
for kind in builds:
 for name in ['CMakeCache.txt','build.ninja']:
  p=pathlib.Path('tmp/IMPL_0908')/kind/name
  if p.exists():shutil.copyfile(p,dest/(kind+'-'+name))
commits=subprocess.check_output(['git','-C',str(src),'rev-list','--reverse','c3f8578a4db871d9d6de96d751f4c2ea7b1638fa..HEAD']).decode().splitlines()
for i,sha in enumerate(commits,1):
 data=subprocess.check_output(['git','-C',str(src),'format-patch','-1','--stdout',sha]);(dest/f'{i:02}_{sha}.patch').write_bytes(data)
 print('COMMIT',sha)
for kind in builds:
 for name in ['libc++.so.1','libc++abi.so.1']:
  p=pathlib.Path('tmp/IMPL_0908')/kind/'lib'/name
  print('BINARY_SHA256',p,hashlib.sha256(p.read_bytes()).hexdigest())
if stage=='W3':
 for p in [pathlib.Path('docs/progress/R111/code/condition_cancel_probe.cpp'),pathlib.Path('docs/progress/R112/code/item1_oldcaller_cancel.cpp'),pathlib.Path('docs/progress/R113/code/item1_facility_cancel.cpp'),pathlib.Path('docs/progress/R114/code/record.py'),pathlib.Path('tools/resource_gate.sh')]:
  shutil.copyfile(p,dest/('input_'+p.parts[-3]+'_'+p.name))
 for p in out.glob('*before*'):
  if p.is_file():shutil.copyfile(p,dest/p.name)
with (out/'SNAPSHOT_SHA256SUMS').open('w') as f:
 for p in sorted(dest.iterdir()):f.write(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+str(p)+'\n')
print('SNAPSHOT_FILES',len(list(dest.iterdir())))
