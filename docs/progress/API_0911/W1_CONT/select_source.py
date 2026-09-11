#!/usr/bin/env python3
"""流式读取真实 SRPM 的 tar 输入，仅提取待查接口源码，不展开整树。"""
import tarfile,sys,pathlib,hashlib,json
dest=pathlib.Path('tmp/API_0911/W1_CONT/chromium_source');dest.mkdir(parents=True,exist_ok=True)
rows=[];total=0
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|gz') as archive:
    for m in archive:
        total+=1;name=m.name.lower()
        wanted=('ewk_cookie_parser' in name or (('/wrt/' in name or '/xwalk/' in name) and ('extension' in name or 'plugin' in name) and name.endswith(('.h','.cc','.cpp','.gni','.gn'))))
        if not wanted or not m.isfile():continue
        p=pathlib.PurePosixPath(m.name)
        assert not p.is_absolute() and '..' not in p.parts
        data=archive.extractfile(m).read();target=dest/p;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
        row=dict(member=m.name,size=len(data),sha256=hashlib.sha256(data).hexdigest(),path=str(target));rows.append(row);print(json.dumps(row),flush=True)
out=pathlib.Path('docs/progress/API_0911/W1_CONT');(out/'source_members.json').write_text(json.dumps(dict(archive_members=total,selected=rows),indent=2)+'\n')
print('COMPLETE',total,len(rows),flush=True)
