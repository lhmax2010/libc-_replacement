#!/usr/bin/env python3
"""按冻结清单限定材料搜索范围，不将全盘零命中视为不存在。"""
import csv, pathlib, json
r=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
names=('chromium-efl','dali2','webapi-plugins','wrt','capi-appfw-app-control','capi-appfw-app-common','capi-appfw-application','capi-appfw-service-application')
rows=[]
for x in csv.DictReader(open('docs/progress/P7_0909/inputs/binary_package_records.tsv'), delimiter='\t'):
    if x['arch']=='x86_64' and any(x['name']==n or x['name'].startswith(n+'-') for n in names):
        h=x['checksum']; p=r/'rpms'/x['repo_id']/x['location']; e=r/'extracted'/h[:2]/h
        libs=list(e.glob('usr/lib64/**/*.so*')) if e.exists() else []
        rows.append(dict(name=x['name'],rpm=str(p),rpm_exists=p.exists(),sha256=h,extracted=str(e),libraries=[str(t) for t in libs if not t.is_symlink()]))
out=pathlib.Path('docs/progress/API_0911/W1/catalog.json')
out.write_text(json.dumps(rows,indent=2)+'\n')
for x in rows: print(x['name'],x['rpm_exists'],len(x['libraries']),x['rpm'])
