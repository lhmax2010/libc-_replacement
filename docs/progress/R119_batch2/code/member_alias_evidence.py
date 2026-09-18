#!/usr/bin/env python3
"""第二批复用包边所需的精确类型别名和存储成员源码。"""
import hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[4]
base=root/'docs/progress/R119_batch2'
files={'bits/ios_base.h':[(360,369),(468,475),(576,583),(640,652)],'std/sstream':[(100,113),(1009,1018)],'bits/chrono.h':[(874,934),(992,1004),(1260,1280)],'std/iosfwd':[(128,150)]}
rows=[]
for name,ranges in files.items():
    path=root/'codes/gcc/libstdc++-v3/include'/name
    data=path.read_bytes();lines=data.decode().splitlines()
    row=dict(path=str(path.relative_to(root)),sha256=hashlib.sha256(data).hexdigest(),excerpts=['\n'.join(f'{i}: {lines[i-1]}' for i in range(a,b+1)) for a,b in ranges])
    rows.append(row);print(row['path'],*row['excerpts'],sep='\n')
(base/'W1/MEMBER_ALIAS_EVIDENCE.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
