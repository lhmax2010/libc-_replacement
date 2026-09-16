from common import *
import csv,gzip,collections
sets=collections.defaultdict(set);side_sources=[];namespace_aliases=[]
statuses=[];inventories={}
for batch in ['headers','headers_supplement','headers_repair','headers_extra','headers_wrapper','headers_legacy_variables']:
 for r in json.loads((OUT/batch/'INVENTORY.json').read_text()):inventories[(r['library'],r['path'])]=r
 statuses+=json.loads((OUT/batch/'PARSE_STATUS.json').read_text())
 for line in gzip.open(OUT/batch/'declarations.jsonl.gz','rt'):
  d=json.loads(line);sets[d['entity']].add(d['library'])
  if d['library']=='libcxx' and d['location'][0].startswith('codes/gcc/') or d['library']=='libstdcxx' and d['location'][0].startswith('codes/llvm/'):
   if len(side_sources)<20:side_sources.append(d)
  if d['kind']=='NAMESPACE_ALIAS':namespace_aliases.append(d)
controls={}
for name in ['std::vector','std::string','std::is_same_v','std::is_literal_type_v','std::ranges::sort','std::same_as']:
 controls[name]={'expected':['libcxx','libstdcxx'],'observed':sorted(sets.get(name,[])),'pass':sets.get(name)=={'libcxx','libstdcxx'}}
controls['__gnu_pbds::tree']={'expected':['libstdcxx'],'observed':sorted(sets.get('__gnu_pbds::tree',[])),'pass':'libstdcxx' in sets.get('__gnu_pbds::tree',set()),'note':'positive extension control only; not proof of absence in libcxx'}
assert all(r['pass'] for r in controls.values()),controls
save('headers/FINAL_CONTROLS.json',controls);save('headers/CROSS_LIBRARY_IMPORT_SAMPLES.json',side_sources)
with gzip.open(OUT/'headers/namespace_aliases.jsonl.gz','wt') as f:
 for d in namespace_aliases:f.write(json.dumps(d,ensure_ascii=False)+'\n')
group=collections.defaultdict(list)
for r in statuses:group[(r['library'],r['path'])].append(r)
coverage=[];errors=[]
for key,h in sorted(inventories.items()):
 s=group[key];good=sorted({r['mode'] for r in s if r['status']=='PARSED'});bad=sorted({r['mode'] for r in s if r['status']!='PARSED'})
 coverage.append(dict(**h,parsed_modes=';'.join(good),error_modes=';'.join(bad),status='PARSED_AT_LEAST_ONE_MODE' if good else 'NOT_COMPLETELY_PARSED',max_declarations=max((r['declarations'] for r in s),default=0)))
 for r in s:
  for d in r.get('diagnostics',[]):
   if d['severity']>=3:errors.append(dict(library=r['library'],path=r['path'],header=r['header'],mode=r['mode'],diagnostic=d['text']))
for name,rows in [('HEADER_COVERAGE.tsv',coverage),('HEADER_ERRORS.tsv',errors)]:
 with (OUT/name).open('w',newline='') as f:
  w=csv.DictWriter(f,rows[0].keys(),delimiter='\t');w.writeheader();w.writerows(rows)
save('headers/COVERAGE_SUMMARY.json',{'physical_entries':len(coverage),'category_counts':dict(collections.Counter(r['library']+':'+r['category'] for r in coverage)),'never_fully_parsed':[r['path'] for r in coverage if r['status']=='NOT_COMPLETELY_PARSED'],'namespace_alias_records':len(namespace_aliases),'cross_library_import_samples':len(side_sources),'controls':'PASS'})
print('entries',len(coverage),'never_fully_parsed',sum(r['status']=='NOT_COMPLETELY_PARSED' for r in coverage),'controls PASS',flush=True)
