from common import *
import csv,gzip,re,json,collections
dest=OUT/'W1'; specs={
'inference-engine-interface':['IETensorBuffer'], 'cert-svc':['UriList'],
'dali2-adaptor':['MatchRule','WebEnginePageLoadCallback'], 'dali2':['TimePoint'],
'isf':['WideString'], 'libzypp':['LocaleSet'],'abseil-cpp':['using std::string_view']}
names={}
with (ROOT/'docs/progress/P7_0909/inputs/source_package_records.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):names[r['location']]=r['name']
with (ROOT/'docs/progress/P7_0909/tables/packages.tsv').open()as f:
    devel={r['checksum']for r in csv.DictReader(f,delimiter='\t')if r['devel_name']=='YES'}
headers=[];seen=set();alias=[];graph=collections.defaultdict(list)
allowed=set(specs)|{'dali2-toolkit','gtest','protobuf'}
with gzip.open(ROOT/'docs/progress/P7_0909/tables/headers.tsv.gz','rt')as f:
    for h in csv.DictReader(f,delimiter='\t'):
        family=names.get(h['sourcerpm']);key=(family,h['path'])
        if family not in allowed or h['status']!='PASS' or h['rpm_sha256']not in devel or key in seen:continue
        seen.add(key);p=pathlib.Path(h['cache_path'])
        if not p.is_file():continue
        assert digest(p)==h['file_sha256']
        lines=p.read_text(errors='replace').splitlines();headers.append(h)
        for i,line in enumerate(lines):
            m=re.match(r'\s*#\s*include\s*[<"]([^>"]+)[>"]',line)
            if m:graph[pathlib.Path(m[1]).name].append({'from':h['path'],'to':m[1],'line':i+1,'metadata':h})
            if any(t in line for t in specs.get(family,[])) and re.search(r'\b(typedef|using)\b',line):
                alias.append({'family':family,'path':h['path'],'line':i+1,'metadata':h,'context':'\n'.join(f'{j+1}: {lines[j]}'for j in range(max(0,i-2),min(len(lines),i+12)))})
save(dest/'ALIASES.json',alias)
for a in alias:print('ALIAS',a['family'],a['path'],a['context'])
for key,consumer,targets in [
('dali_match','com.samsung.dali-demo',['actor-accessible.h','collection.h']),
('gtest_ostream','app-core',['gtest-printers.h']),
('gtest_stringstream','enlightenment',['gtest-string.h']),
('scim_wstring','ise-engine-anthy',['scim_utility.h']),
('protobuf_view','aurum',['arenastring.h'])]:
    reverse={t:[]for t in targets};todo=collections.deque(targets)
    while todo:
        t=todo.popleft()
        if len(reverse[t])>=8:continue
        for edge in graph[t]:
            source=pathlib.Path(edge['from']).name
            if source in reverse:continue
            reverse[source]=[edge]+reverse[t];todo.append(source)
    pattern=r'^\s*#\s*include\s*[<"](?:[^>"\n]*/)?(?:'+ '|'.join(re.escape(n)for n in reverse)+r')[>"]'
    rows=[]
    for root in sorted((OLD/'tmp/corpus/extracted').glob(consumer+'-*.src.rpm')):
        rc,txt,raw=record('indirect_include_'+key,['rg','-n','-uuu','--glob','*.h','--glob','*.hpp','--glob','*.hh','--glob','*.cc','--glob','*.cpp','--glob','*.cxx',pattern,str(root/'unpacked')])
        for line in txt.splitlines():
            m=re.search(r'#\s*include\s*[<"]([^>"]+)[>"]',line)
            if m and pathlib.Path(m[1]).name in reverse:
                chain=reverse[pathlib.Path(m[1]).name]
                rows.append({'source_include':line,'requested_header':m[1],'candidate_chain':chain,'record':raw})
    save(dest/'positive'/(key+'_include_chain.json'),rows)
    print('CHAIN',key,'matches',len(rows))
    if rows:
        print(rows[0]['source_include'])
        for edge in rows[0]['candidate_chain']:print(' ->',edge['from'],edge['line'],edge['to'])
