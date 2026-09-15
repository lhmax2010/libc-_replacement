#!/usr/bin/env python3
"""仅提取真实转发层和依赖核查用源码；不覆盖平台树。"""
import hashlib,json,pathlib,sys,tarfile
root=pathlib.Path('tmp/P12/upstream'); root.mkdir(parents=True,exist_ok=True)
rows=[]
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|gz') as tf:
    for member in tf:
        if not member.isfile(): continue
        name=member.name
        wanted='/tizen_src/downloadable/' in name or name.endswith('/packaging/chromium-efl.spec')
        if not wanted:continue
        p=pathlib.PurePosixPath(name)
        assert not p.is_absolute() and '..' not in p.parts
        data=tf.extractfile(member).read(); dest=root/p
        dest.parent.mkdir(parents=True,exist_ok=True); dest.write_bytes(data)
        rows.append({'member':name,'path':str(dest),'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
pathlib.Path('docs/progress/P12_0916/SOURCE_INDEX.json').write_text(json.dumps(rows,indent=2)+'\n')
print('EXTRACTED',len(rows))
