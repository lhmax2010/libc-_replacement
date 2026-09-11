#!/usr/bin/env python3
"""独立汇总和代表性语义形态复核；不作跨库运行验证。"""
import collections
import csv
import gzip
import hashlib
import json
import pathlib
import re
import scan_scope

OUT=pathlib.Path('docs/progress/P7_0909')
def load(name):
    p=OUT/'tables'/name
    with (p.open() if p.exists() else gzip.open(str(p)+'.gz','rt')) as f:return list(csv.DictReader(f,delimiter='\t'))
def write(name,fields,rows):scan_scope.write(name,fields,rows)
providers=load('provider_scope.tsv');packages=load('packages.tsv');recipes=load('sdk_api_groups.tsv')
by_rpm=collections.defaultdict(collections.Counter);by_category=collections.defaultdict(collections.Counter)
counts=collections.Counter();seen=set();gaps=[];public_special=[];sample_map={}
samples=load('representative_headers.tsv')
sample_keys={(r['rpm_sha256'],r['installed_path']) for r in samples}
roles={r['checksum']:r['role'] for r in providers}
p=OUT/'tables/headers.tsv';f=p.open() if p.exists() else gzip.open(str(p)+'.gz','rt')
with f:
    for r in csv.DictReader(f,delimiter='\t'):
        key=(r['rpm_sha256'],r['path']);assert key not in seen,key;seen.add(key)
        counts['all_paths']+=1;counts[r['status']]+=1
        if key in sample_keys:sample_map[key]=r
        if r['status']!='PASS':gaps.append(r)
        if r['scope']=='INCLUDE_TREE':
            by_rpm[r['rpm_sha256']][r['form']]+=1
            counts['all_include_paths']+=1
            if roles[r['rpm_sha256']]=='INCLUDE_PROVIDER':
                by_category[r['category']][r['form']]+=1;counts['provider_include_paths']+=1
for r in packages:
    c=by_rpm[r['checksum']]
    assert c['C_DECLARATION_SHAPE']==int(r['c_shape_headers'])
    assert c['CPP_DECLARATION_SIGNAL']==int(r['cpp_signal_headers'])
    assert c['UNRESOLVED_OR_INCLUDE_MACRO_ONLY']==int(r['unresolved_headers'])
    assert c['NOT_AVAILABLE']==int(r['unavailable_headers'])
for r in load('provider_category_summary.tsv'):
    c=by_category[r['category']]
    assert sum(c.values())==int(r['header_paths'])
    assert c['C_DECLARATION_SHAPE']==int(r['c_shape_headers'])
    assert c['CPP_DECLARATION_SIGNAL']==int(r['cpp_signal_headers'])
    assert c['UNRESOLVED_OR_INCLUDE_MACRO_ONLY']==int(r['unresolved_headers'])
    assert c['NOT_AVAILABLE']==int(r['unavailable_headers'])
for s in samples:
    p=OUT/s['snapshot'];data=p.read_bytes();assert hashlib.sha256(data).hexdigest()==s['sha256']
    r=sample_map[(s['rpm_sha256'],s['installed_path'])]
    assert r['file_sha256']==s['sha256']
    result=scan_scope.shape(data.decode('utf-8',errors='replace'),s['installed_path'])
    assert result['form']==r['form'],(s,result,r)
    public_special.append(dict(name=s['name'],path=s['installed_path'],form=result['form'],evidence_line=result['evidence_line'],evidence=result['evidence'],snapshot=s['snapshot'],sha256=s['sha256']))
write('representative_classifications.tsv',['name','path','form','evidence_line','evidence','snapshot','sha256'],public_special)
write('header_gaps.tsv',list(gaps[0]),gaps)
# 零命中结论的对应正向控制：同一配方匹配器能检出 Chromium，不能据无 Dali/LWE 配方推断不公开。
rs=load('sdk_recipes.tsv')
def name_hits(token):return [r for r in rs if token in r['rpm_names']]
assert name_hits('chromium-efl')
recipe_absence={token:len(name_hits(token)) for token in ['dali2','lightweight-web-engine']}
identity=list(csv.DictReader((OUT/'inputs/binary_package_records.tsv').open(),delimiter='\t'))
source_names={r['location'].rsplit('/',1)[-1]:r['name'] for r in scan_scope.load(scan_scope.R11/'tables/source_package_records.tsv')}
proposed=load('proposed_source_groups.tsv')
for r in proposed:
    assert r['source_rpm'] in source_names,r['source_rpm']
    r['source_name']=source_names[r['source_rpm']]
write('proposed_source_groups_named.tsv',list(proposed[0]),proposed)
result=dict(binary_records=len(identity),logical_binary_groups=len(load('logical_packages.tsv')),candidate_groups=len(packages),include_provider_groups=sum(r['role']=='INCLUDE_PROVIDER' for r in providers),counts=dict(counts),representative_checks=len(samples),positive_controls=len(load('positive_controls.tsv')),proposal_binary_groups=sum(r['proposal'] in {'PROPOSE_FOCUS','PROPOSE_PUBLIC_CPP'} for r in providers),proposal_source_rpm_records=len(proposed),proposal_source_names=len({r['source_name'] for r in proposed}),recipe_exact_name_absence=recipe_absence,recipe_name_positive_control='chromium-efl',stage2_status='NOT_STARTED_WAITING_FOR_USER')
(OUT/'VALIDATION.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
