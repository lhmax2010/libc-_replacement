#!/usr/bin/env python3
"""补充不以 C/C++ 头文件交付的托管层和 SDK 范围输入，仍属第一阶段。"""
import csv
import gzip
import pathlib
import re
import xml.etree.ElementTree as ET

ROOT=pathlib.Path.cwd();OUT=ROOT/'docs/progress/P7_0909'
CACHE=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
rows=list(csv.DictReader((OUT/'tables/logical_packages.tsv').open(),delimiter='\t'))
selected={r['checksum']:r for r in rows if re.match(r'(app-rootstrap|csapi-tizenfx|dali2-csharp-binder)',r['name'])}
data=[];examples=[]
for repo in ['base_binary','unified_binary']:
    with gzip.open(CACHE/'repodata'/f'{repo}.filelists.xml.gz','rb') as f:
        for _,e in ET.iterparse(f,events=('end',)):
            if not e.tag.endswith('}package'):continue
            if e.attrib['pkgid'] not in selected:e.clear();continue
            row=selected[e.attrib['pkgid']]
            for p in e:
                if not p.tag.endswith('}file') or p.attrib.get('type')=='dir':continue
                path=p.text
                data.append(dict(name=row['name'],arch=row['arch'],source_rpm=row['sourcerpm'],rpm_sha256=row['checksum'],path=path,role='SDK_SCOPE_INPUT' if row['name'].startswith('app-rootstrap') else 'MANAGED_OR_BINDING_PAYLOAD'))
                if row['name'].startswith('app-rootstrap'):
                    actual=CACHE/'extracted'/row['checksum'][:2]/row['checksum']/path.lstrip('/')
                    if actual.is_file() and not actual.is_symlink() and actual.stat().st_size < 200000:
                        raw=actual.read_bytes()
                        if b'\0' not in raw:
                            name=f'{row["name"]}__{path.replace("/","_")}'
                            target=OUT/'snapshots'/'sdk';target.mkdir(parents=True,exist_ok=True)
                            (target/name).write_bytes(raw)
                            examples.append(dict(package=row['name'],path=path,snapshot=str((target/name).relative_to(OUT))))
            e.clear()
for name,values,fields in [('managed_sdk_files.tsv',data,['name','arch','source_rpm','rpm_sha256','path','role']),('sdk_text_snapshots.tsv',examples,['package','path','snapshot'])]:
    with (OUT/'tables'/name).open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(values)
print('selected_logical_packages',len(selected),'installed_paths',len(data),'sdk_text_snapshots',len(examples))
for row in rows:
    if row['checksum'] in selected:print(row['name'],row['arch'],row['sourcerpm'])
