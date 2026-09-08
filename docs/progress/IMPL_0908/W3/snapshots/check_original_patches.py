#!/usr/bin/env python3
"""逐文件核对既有四补丁覆盖内容未被本轮改变，含五处头文件。"""
import hashlib,pathlib,re,subprocess
src=pathlib.Path('tmp/IMPL_0908/source');base='c3f8578a4db871d9d6de96d751f4c2ea7b1638fa';files=set()
patchdir=pathlib.Path('docs/progress/R69/code/patches')
for patch in sorted(patchdir.glob('*.patch')):
 text=patch.read_text();files.update(re.findall(r'^diff --git a/(\S+) b/\S+',text,re.M))
 print('PATCH',patch.name,hashlib.sha256(patch.read_bytes()).hexdigest())
headers=[]
for name in sorted(files):
 before=subprocess.check_output(['git','-C',str(src),'show',base+':'+name]);after=(src/name).read_bytes()
 assert before==after,name
 if '/include/' in name:headers.append(name)
 print('UNCHANGED',name,hashlib.sha256(after).hexdigest())
assert len(headers)==5,headers
print('FIVE_HEADER_CONTENT_CHECK=PASS',','.join(headers))
print('ALL_EXISTING_PATCH_FILES_UNCHANGED=PASS',len(files))
