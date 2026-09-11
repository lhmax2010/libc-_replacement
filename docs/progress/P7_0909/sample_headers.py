#!/usr/bin/env python3
"""形态代表性样本与独立 rg 对照，不执行接口或 ABI 测试。"""
import csv
import hashlib
import pathlib
import subprocess
import sys

OUT=pathlib.Path('docs/progress/P7_0909')
CACHE=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
packages=list(csv.DictReader((OUT/'tables/candidate_packages_before_scan.tsv').open(),delimiter='\t'))
requests=[('capi-appfw-application-devel','app.h'),('capi-appfw-application-devel','app.hpp'),('dali2-devel','actor.h'),('dali2-toolkit-devel','control.h'),('lightweight-web-engine-devel',''),('chromium-efl-devel','ewk_view.h'),('chromium-efl-devel','node.h'),('component-based-application-devel',''),('tizen-core-devel',''),('libicu-devel','unistr.h')]
rows=[]
for idx,(name,leaf) in enumerate(requests,1):
    candidates=[r for r in packages if r['name']==name]
    assert candidates,name
    r=min(candidates,key=lambda r:0 if r['arch']=='x86_64' else 1)
    root=CACHE/r['checksum'][:2]/r['checksum']
    command=['rg','--files',str(root)]
    stem=OUT/'raw'/f'sample_v2_{idx:02d}_filelist'
    subprocess.run([sys.executable,str(OUT/'record.py'),str(stem),*command],check=True)
    files=[pathlib.Path(x) for x in pathlib.Path(str(stem)+'.stdout').read_text().splitlines() if '/include/' in x and (pathlib.Path(x).name==leaf if leaf else pathlib.Path(x).suffix in {'.h','.hpp'})]
    assert files,(name,leaf)
    if leaf:files=files[:1]
    else:files=files[:8]
    for file in files:
        data=file.read_bytes();relative=file.relative_to(root)
        snapshot=OUT/'snapshots'/'examples'/name/relative;snapshot.parent.mkdir(parents=True,exist_ok=True);snapshot.write_bytes(data)
        rows.append(dict(name=name,arch=r['arch'],rpm_sha256=r['checksum'],installed_path='/'+str(relative),snapshot=str(snapshot.relative_to(OUT)),sha256=hashlib.sha256(data).hexdigest()))
with (OUT/'tables/representative_headers.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
print('representative_headers',len(rows))
