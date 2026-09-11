#!/usr/bin/env python3
"""修正 namespace 词界与 Objective-C 误匹配；冻结字节不变，只机械更新分类。"""
import collections
import csv
import hashlib
import json
import pathlib
import re
import scan_scope

OUT=pathlib.Path('docs/progress/P7_0909');table=OUT/'tables/headers.tsv'
scan_scope.controls()
counts=collections.defaultdict(collections.Counter);changes=[];category_changes=[];tested=0
before=scan_scope.digest(table)
packages=scan_scope.load(OUT/'tables/packages.tsv')
categories={p['checksum']:p['category'] for p in packages}
for p in packages:
    if re.search(r'(^|[-_])kernel([-_]|$)',p['name']) or re.search(r'(^|[-_])linux-kernel([-_]|$)',p['sourcerpm']):
        if p['category']!='OTHER':
            category_changes.append(dict(name=p['name'],arch=p['arch'],rpm_sha256=p['checksum'],before=p['category'],after='OTHER',reason='内核/内核开发材料按完整包名识别，不因 arm/jetson 前缀归入服务'))
            categories[p['checksum']]='OTHER';p['category']='OTHER'
with table.open(newline='') as src,(OUT/'tables/headers.reclassified.tsv').open('w',newline='') as dest:
    reader=csv.DictReader(src,delimiter='\t');writer=csv.DictWriter(dest,fieldnames=reader.fieldnames,delimiter='\t',lineterminator='\n');writer.writeheader()
    for row in reader:
        old=dict(row)
        if row['form']=='CPP_DECLARATION_SIGNAL' and row['cpp_signal'] in {'namespace','class'}:
            tested+=1
            data=pathlib.Path(row['cache_path']).read_bytes()
            assert hashlib.sha256(data).hexdigest()==row['file_sha256'],row['path']
            row.update(scan_scope.shape(data.decode('utf-8',errors='replace'),row['path']))
        row['category']=categories[row['rpm_sha256']]
        if row!=old:
            for field in row:
                if str(row[field])!=str(old[field]):
                    changes.append(dict(rpm_sha256=row['rpm_sha256'],path=row['path'],field=field,before=old[field],after=row[field]))
        if row['scope']=='INCLUDE_TREE':counts[row['rpm_sha256']][row['form']]+=1
        writer.writerow(row)
(OUT/'tables/headers.reclassified.tsv').replace(table)
for p in packages:
    c=counts[p['checksum']]
    p.update(c_shape_headers=c['C_DECLARATION_SHAPE'],cpp_signal_headers=c['CPP_DECLARATION_SIGNAL'],unresolved_headers=c['UNRESOLVED_OR_INCLUDE_MACRO_ONLY'],unavailable_headers=c['NOT_AVAILABLE'])
scan_scope.write('packages.tsv',list(packages[0]),packages)
scan_scope.write('classification_changes.tsv',['rpm_sha256','path','field','before','after'],changes)
scan_scope.write('category_corrections.tsv',['name','arch','rpm_sha256','before','after','reason'],category_changes)
result=dict(before_headers_sha256=before,after_headers_sha256=scan_scope.digest(table),rechecked_headers=tested,changed_cells=len(changes),category_corrections=len(category_changes),positive_controls=8,initial_category_summary='initial_classification/category_summary.tsv',current_category_summary='tables/provider_category_summary.tsv')
(OUT/'RECLASSIFICATION.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
