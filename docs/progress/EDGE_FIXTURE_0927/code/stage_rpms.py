"""后续获授权时重放输入：只下载、校验、解包，不安装、不运行 RPM 脚本。"""
from common import *
import argparse
ap=argparse.ArgumentParser();ap.add_argument('--manifest',type=pathlib.Path,required=True);ap.add_argument('--root',type=pathlib.Path,required=True);a=ap.parse_args()
root=a.root.resolve();assert root.is_relative_to(TMP.resolve()) and root!=TMP.resolve()
assert not root.exists() or not any(root.iterdir()),'Use a new empty root; never mix provider sets'
root.mkdir(parents=True,exist_ok=True);gate();rows=json.loads(a.manifest.read_text());staged=[]
cache=TMP/'replay-cache';cache.mkdir(exist_ok=True)
for i,r in enumerate(rows):
 if i%5==0:gate()
 assert r.get('arch') in ('x86_64','noarch'),r
 assert r.get('checksum_type','sha256')=='sha256'
 expected=r.get('sha256',r.get('checksum'));assert expected and len(expected)==64
 local=ROOT/r['rpm_local'] if r.get('rpm_local') else cache/(expected+'.rpm')
 if not local.exists():
  local=cache/(expected+'.rpm');run('replay_download_'+str(i),['curl','-fL','--max-time','180','-o',local,r['url']],timeout=190)
 assert sha(local)==expected,(r['name'],'RPM_HASH_MISMATCH')
 listing=run('replay_rpm_files_'+str(i),['rpm','-qpl',local])
 for p in listing['stdout'].splitlines():assert '..' not in pathlib.PurePosixPath(p).parts,p
 extraction=run('replay_extract_'+str(i),['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -idm --quiet --no-absolute-filenames','stage',local],cwd=root)
 staged.append(dict(name=r['name'],url=r['url'],sha256=expected,root=str(root),record=extraction['record']))
 save('staging/'+root.name+'.json',staged)
print(json.dumps(dict(count=len(staged),root=str(root)),ensure_ascii=False))
