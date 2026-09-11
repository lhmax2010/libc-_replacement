#!/usr/bin/env python3
"""候选头文件证据索引：保留行号与上下文，不把词法命中当导出 ABI 证明。"""
import csv,hashlib,pathlib,re,collections,json,sys
P=pathlib.Path('docs/progress/P7_0909');O=P/'stage2';sys.path.insert(0,str(P))
from scan_scope import TOKEN,shape
H=list(csv.DictReader((O/'tables/headers.tsv').open(),delimiter='\t'))
groups={'DALI_NUI','WEB'};hits=[];summary=[];native=[]
for r in H:
    if r['group'] not in groups and not (r['group']=='NATIVE_APP_MIDDLEWARE' and r['public_recipes']):continue
    p=pathlib.Path(r['local_path'])
    if r['status']!='PASS':continue
    raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()==r['file_sha256']
    text=raw.decode(errors='replace');lines=text.splitlines()
    clean=TOKEN.sub(lambda m:''.join('\n' if c=='\n' else ' ' for c in m.group()),text)
    private=bool(re.search(r'internal|private|implementation|integration-api|devel-api',r['path'],re.I))
    types=sorted(set(re.findall(r'\bstd\s*::\s*(\w+)',clean)))
    form=shape(text,r['path']);assert form['form']==r['form']
    row=dict(family=r['family'],package=r['name'],path=r['path'],scope='INTERNAL_NAMED_SKIP' if private else 'PUBLIC_PATH_CANDIDATE_SDK_CONFIRM',public_recipes=r['public_recipes'],internal_recipes=r['internal_recipes'],form=r['form'],extern_c_marker=r['extern_c_marker'],std_tokens=';'.join(types),sha256=r['file_sha256'])
    summary.append(row)
    if r['group']=='NATIVE_APP_MIDDLEWARE':native.append(row)
    if r['group'] not in groups or private:continue
    for n,line in enumerate(clean.splitlines(),1):
        tags=[]
        if re.search(r'\bstd\s*::',line):tags.append('STD_TOKEN_REVIEW_REQUIRED')
        if re.search(r'\bthrow\b',line):tags.append('THROW_OR_SPEC_REVIEW_REQUIRED')
        if re.search(r'\b(?:new|delete|Create|Destroy|Release|Reference|Unreference)\b',line):tags.append('LIFECYCLE_REVIEW_REQUIRED')
        if tags:hits.append(dict(family=r['family'],package=r['name'],path=r['path'],line=n,tags=';'.join(tags),text=lines[n-1].strip(),context=' | '.join(lines[max(0,n-3):n+2]),file_sha256=r['file_sha256']))
def write(name,rows):
    with (O/'tables'/name).open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
write('boundary_headers.tsv',summary);write('boundary_signal_index.tsv',hits);write('native_refined_scan.tsv',native)
stats=[]
for family in sorted({r['family'] for r in native}):
    allrows=[r for r in native if r['family']==family];public=[r for r in allrows if r['scope']!='INTERNAL_NAMED_SKIP']
    stats.append(dict(family=family,public_recipe_headers=len(allrows),internal_named_headers=len(allrows)-len(public),public_name_candidates=len(public),c_shape=sum(r['form']=='C_DECLARATION_SHAPE' for r in public),cpp_signal=sum(r['form']=='CPP_DECLARATION_SIGNAL' for r in public),unresolved=sum(r['form']=='UNRESOLVED_OR_INCLUDE_MACRO_ONLY' for r in public),cpp_paths=';'.join(r['path'] for r in public if r['form']=='CPP_DECLARATION_SIGNAL')))
write('native_summary.tsv',stats)
# 独立的小型正负样本，防“零命中”扫描无效；不宣称这证明真实头的完整预处理语义。
controls=[('c','extern "C" { int f(const char*); }','C_DECLARATION_SHAPE'),('cpp','namespace x { class A { public: std::string f(); }; }','CPP_DECLARATION_SIGNAL'),('comment','/* std::string f(); */\nint g(void);','C_DECLARATION_SHAPE'),('mixed','extern "C" { int f(); } namespace x { class A {}; }','CPP_DECLARATION_SIGNAL')]
result=[]
for name,text,expected in controls:
    got=shape(text,name+'.h')['form'];assert got==expected;result.append(dict(name=name,source=text,expected=expected,observed=got))
(O/'tables/scan_controls.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(dict(header_rows=len(summary),signal_rows=len(hits),native_families=len(stats),controls='4/4')))
