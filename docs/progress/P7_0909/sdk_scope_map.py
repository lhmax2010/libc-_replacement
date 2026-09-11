#!/usr/bin/env python3
"""解析冻结包携带的 public/internal SDK 配方；匹配包名和头文件路径，不外推产品支持。"""
import collections
import csv
import fnmatch
import hashlib
import json
import pathlib
import subprocess
import sys
import xml.etree.ElementTree as ET

ROOT=pathlib.Path.cwd();OUT=ROOT/'docs/progress/P7_0909'
def load(name):return list(csv.DictReader((OUT/'tables'/name).open(),delimiter='\t'))
def write(name,fields,rows):
    with (OUT/'tables'/name).open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n',extrasaction='ignore');w.writeheader();w.writerows(rows)
records=list(csv.DictReader((OUT/'inputs/binary_package_records.tsv').open(),delimiter='\t'))
logical=load('logical_packages.tsv');snaps=load('sdk_text_snapshots.tsv')
names={'app-rootstrap-native-data-public','app-rootstrap-native-data-internal','app-rootstrap-csharp-data-common'}
checks=[]
for idx,r in enumerate(x for x in logical if x['name'] in names):
    rpm=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/rpms')/r['repo_id']/r['arch']/(r['name']+'-'+r['version']+'-'+r['release']+'.'+r['arch']+'.rpm')
    actual=hashlib.sha256(rpm.read_bytes()).hexdigest();assert actual==r['checksum']
    stem=OUT/'raw'/f'sdk_{idx:02d}_rpm_digest'
    subprocess.run([sys.executable,str(OUT/'record.py'),str(stem),'rpm','-qp','--qf','[%{FILENAMES}\t%{FILEDIGESTS}\n]',str(rpm)],check=True)
    expected=dict(line.split('\t',1) for line in pathlib.Path(str(stem)+'.stdout').read_text().splitlines())
    for s in snaps:
        if s['package']==r['name']:
            file=OUT/s['snapshot'];got=hashlib.sha256(file.read_bytes()).hexdigest()
            assert got==expected[s['path']],s
            checks.append(dict(package=r['name'],path=s['path'],snapshot=s['snapshot'],file_sha256=got,rpm_sha256=actual,result='MATCH'))
write('sdk_recipe_identity.tsv',['package','path','snapshot','file_sha256','rpm_sha256','result'],checks)
recipes=[];accepts=[];patterns=[]
for s in snaps:
    if s['package'] not in names or not s['path'].endswith('.xml'):continue
    tree=ET.parse(OUT/s['snapshot']);root=tree.getroot()
    kind='PUBLIC' if s['package'].endswith('-public') else ('INTERNAL' if s['package'].endswith('-internal') else 'CSHARP')
    group=root.attrib.get('name','NOT_OBSERVED')
    row=dict(kind=kind,group=group,package=s['package'],snapshot=s['snapshot'],rpm_names=';'.join(sorted({e.attrib.get('accept','') for e in root.iter('rpm')})),include_paths=';'.join(e.text or '' for e in root.iter('include_path')))
    recipes.append(row)
    for e in root.iter('rpm'):
        accepts.append(dict(kind=kind,group=group,accept=e.attrib.get('accept',''),arch=e.attrib.get('arch','NOT_OBSERVED'),snapshot=s['snapshot']))
    for e in root.findall('./necessary/file'):
        if e.text:patterns.append(dict(kind=kind,group=group,pattern=e.text.strip(),snapshot=s['snapshot']))
write('sdk_recipes.tsv',['kind','group','package','snapshot','rpm_names','include_paths'],recipes)
write('sdk_recipe_rpm_accepts.tsv',['kind','group','accept','arch','snapshot'],accepts)
write('sdk_recipe_required_paths.tsv',['kind','group','pattern','snapshot'],patterns)
matched=[];missing=[]
for a in accepts:
    byname=[r for r in logical if fnmatch.fnmatchcase(r['name'],a['accept'])]
    if not byname:missing.append(a);continue
    for r in byname:
        matched.append(dict(**a,name=r['name'],selected_arch=r['arch'],source_rpm=r['sourcerpm'],exact_arch_present='YES' if a['arch'] in r['available_arches'].split(',') else 'NO'))
write('sdk_recipe_package_matches.tsv',['kind','group','accept','arch','snapshot','name','selected_arch','source_rpm','exact_arch_present'],matched)
write('sdk_recipe_unmatched_rpm_names.tsv',['kind','group','accept','arch','snapshot'],missing)
print('recipe_counts',dict(collections.Counter(r['kind'] for r in recipes)),'package_name_matches',len(matched),'unmatched_accept_entries',len(missing))
print('PUBLIC_GROUPS',','.join(sorted({r['group'] for r in recipes if r['kind']=='PUBLIC'})))
