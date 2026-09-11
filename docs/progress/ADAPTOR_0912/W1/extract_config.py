#!/usr/bin/env python3
"""从原始 SRPM 流提取构建配置和最小依赖说明，不改源码。"""
import tarfile, pathlib, hashlib, json, sys
dest=pathlib.Path('tmp/ADAPTOR_0912/W1/source');dest.mkdir(parents=True,exist_ok=True)
rows=[];count=0
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|gz') as tf:
 for m in tf:
  count+=1;n=m.name
  if not m.isfile():continue
  wanted=(n.endswith(('BUILD.gn','args.gn','.gni')) and any(x in n for x in ['/tizen_src/','/net/','/build/config/c++/','/base/'])) or n.endswith(('base/compiler_specific.h','base/functional/function_ref.h','net/base/net_export.h','private/ewk_private.h'))
  if not wanted:continue
  p=pathlib.PurePosixPath(n);assert not p.is_absolute() and '..' not in p.parts
  data=tf.extractfile(m).read();target=dest/p;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
  rows.append(dict(path=str(target),size=len(data),sha256=hashlib.sha256(data).hexdigest()))
pathlib.Path('docs/progress/ADAPTOR_0912/W1/config_index.json').write_text(json.dumps(dict(members=count,files=rows),indent=2)+'\n')
print('MEMBERS',count,'SELECTED',len(rows))
