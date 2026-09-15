#!/usr/bin/env python3
"""只快照源码/脚本/文本证据，不把 ELF、对象文件或实验安装树放进 Git。"""
import hashlib,json,pathlib,shutil,subprocess
import build as b
dest=b.OUT/'code';dest.mkdir(exist_ok=True)
for p in b.TMP.glob('*.py'):shutil.copyfile(p,dest/p.name)
for directory in ['src','upstream','testconfig']:
 shutil.copytree(b.TMP/directory,dest/directory,dirs_exist_ok=True,ignore=shutil.ignore_patterns('__pycache__','*.pyc','*.rpm'))
shutil.copyfile(b.TMP/'package_reverse.json',b.OUT/'package_reverse.json')
ref=dest/'reference';ref.mkdir(exist_ok=True)
parser=b.ROOT/'tmp/API_0911/W1_CONT/chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc'
shutil.copyfile(parser,ref/parser.name)
header=json.loads((b.ROOT/'docs/progress/API_0911/W1_CONT/ewk_header_identity.json').read_text())[0]['path']
shutil.copyfile(header,ref/'ewk_cookie_parser.h')
replay=b.TMP/'replay_verified/evidence'
assert (replay/'RELEASE_RESULTS.json').is_file() and (replay/'ALLOCATION_CONTRACT_RESULTS.json').is_file()
shutil.copytree(replay,b.OUT/'clean_replay',dirs_exist_ok=True)
testexports=subprocess.check_output(['nm','-D','--defined-only',str(b.BUILD/'ewk_adaptor_reverse.so')],text=True)
assert 'p12_test_fail_field' in testexports,'Positive control missing'
release={}
for name in ['ewk_adaptor_reverse.so','forwarder_separate.so','forwarder_embedded.so']:
 p=b.BUILD/'release'/name;exports=subprocess.check_output(['nm','-D','--defined-only',str(p)],text=True)
 assert 'p12_test_' not in exports
 release[name]={'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'exports':exports}
(b.OUT/'RELEASE_EXPORT_CHECK.json').write_text(json.dumps({'positive_control':testexports,'release':release},indent=2)+'\n')
artifacts=[]
for p in sorted(b.BUILD.glob('*')):
 if p.is_file():artifacts.append({'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(b.OUT/'FINAL_LOCAL_ARTIFACTS.json').write_text(json.dumps(artifacts,indent=2)+'\n')
hashes={}
for directory in [dest,b.OUT/'patches']:
 for p in sorted(directory.rglob('*')):
  if p.is_file():
   assert p.read_bytes()[:4]!=b'\x7fELF',p
   hashes[str(p.relative_to(b.OUT))]=hashlib.sha256(p.read_bytes()).hexdigest()
(b.OUT/'SNAPSHOT_SHA256.json').write_text(json.dumps(hashes,indent=2)+'\n')
print('SOURCE_SNAPSHOT_FILES',len(hashes),'NO_ELF_IN_SNAPSHOT=PASS','RELEASE_EXPORT_POSITIVE_CONTROL=PASS')
