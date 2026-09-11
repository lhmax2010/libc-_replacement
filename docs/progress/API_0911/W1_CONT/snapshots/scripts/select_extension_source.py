#!/usr/bin/env python3
"""按文件名扫描真实 Chromium 源码归档，定位浏览器侧回调，不修改源码。"""
import tarfile,sys,pathlib,hashlib,json
dest=pathlib.Path('tmp/API_0911/W1_CONT/chromium_extension_source');dest.mkdir(parents=True,exist_ok=True)
rows=[];hits=[];count=0;examined=0
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|gz') as archive:
 for m in archive:
  count+=1;name=m.name.lower()
  if not m.isfile() or not name.endswith(('.h','.cc','.cpp','.gn','.gni')):continue
  if not any(s in name for s in ['extension','xwalk','cookie','base/time/time','base/strings/string_util','base/strings/string_piece']):continue
  examined+=1;data=archive.extractfile(m).read()
  # Keep full selected source only in ignored tmp; publish identities and matching lines.
  relevant=b'SetSyncBinaryReply' in data or b'SyncMessagingSetSyncBinaryReply' in data
  wanted=relevant or ('tizen_src/' in name and ('extension' in name or 'xwalk' in name)) or '/net/cookies/' in name
  if not wanted:continue
  p=pathlib.PurePosixPath(m.name);assert not p.is_absolute() and '..' not in p.parts
  target=dest/p;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
  rows.append(dict(member=m.name,sha256=hashlib.sha256(data).hexdigest(),size=len(data),path=str(target)))
  if relevant:
   lines=[dict(line=i,text=line) for i,line in enumerate(data.decode(errors='replace').splitlines(),1) if 'SetSyncBinaryReply' in line]
   hits.append(dict(member=m.name,lines=lines));print(json.dumps(hits[-1]),flush=True)
o=pathlib.Path('docs/progress/API_0911/W1_CONT')
(o/'extension_source_members.json').write_text(json.dumps(dict(archive_members=count,examined_filename_candidates=examined,selected=rows,hits=hits),indent=2)+'\n')
print('COMPLETE',count,examined,len(rows),len(hits),flush=True)
