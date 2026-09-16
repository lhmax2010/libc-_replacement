from common import *
import csv,gzip,re,collections
s=json.loads((OUT/'SUMMARY.json').read_text());p=json.loads((OUT/'platform/SUMMARY.json').read_text())
rows=list(csv.DictReader((OUT/'SKELETON.tsv').open(),delimiter='\t'));ids={r['ID'] for r in rows};names={r['类型/函数'] for r in rows}
checks={}
def check(name,value):checks[name]=bool(value);assert value,name
check('unique_entity_rows',len(rows)==len(ids)==len(names)==s['entities'])
check('markdown_same_row_count',sum(line.startswith('| ') for line in (OUT/'SKELETON.md').open())==len(rows)+1)
check('priority_total',sum(s['priority'].values())==len(rows))
check('intersection_partition',s['both_entities']+s['only_observed_one_side']==len(rows))
check('371_manifest_and_347_names',p['source_rpm_identities']==371 and p['unique_package_names']==347)
check('file_counts',p['read_files']+p['file_status']['NOT_OBSERVED_SYMLINK_NOT_FOLLOWED']+p['file_status']['NOT_OBSERVED_NUL_ENCODING']==p['eligible_files'])
check('header_positive_controls',all(r['pass'] for r in json.loads((OUT/'headers/FINAL_CONTROLS.json').read_text()).values()))
check('platform_positive_and_negative_controls',json.loads((OUT/'platform/CONTROLS.json').read_text())['status']=='PASS')
bad=[];sourcecache={}
for r in rows:
 refs=[r[k] for k in ['libcxx源码','libstdcxx源码'] if r[k]!='NOT_OBSERVED']
 if not refs:bad.append(r['ID']);continue
 for ref in refs:
  path,line=ref.rsplit(':',1);f=ROOT/path
  if path not in sourcecache:sourcecache[path]=len(f.read_bytes().splitlines()) if f.is_file() else 0
  if not (0<int(line)<=sourcecache[path]):bad.append(ref)
 for n in ['去重包族数','生产样式包族数']:
  if r[n]!='NOT_OBSERVED' and not (0<=int(r[n])<=347):bad.append(r['ID']+n)
 if r['平台使用包数'][0].isdigit() and int(r['平台使用包数'].split('（')[0])>371:bad.append(r['ID']+'RPM_count')
 if '仅 ' in r['两边是否都有'] and 'NOT_OBSERVED' not in r['两边是否都有']:bad.append(r['ID']+'absence_claim')
check('every_entity_has_existing_header_line_and_bounded_counts',not bad)
cross=list(csv.DictReader((OUT/'EXISTING_MATERIAL_CROSSWALK.tsv').open(),delimiter='\t'));groups=collections.Counter(r['group'] for r in cross)
check('p9_10_r66_25_abi_5_cancel_7',groups['P9 十个类型']==10 and groups['R66 二十五项']==25 and groups['ABI 报告第四节']==5 and groups['七个问题']==7)
check('crosswalk_ids_resolve',all(x.split(':',1)[0] in ids for r in cross for x in r['skeleton_rows'].split(';') if x))
high=list(csv.DictReader((OUT/'HIGH_PRIORITY_SCENARIOS.tsv').open(),delimiter='\t'))
check('high_scenarios_complete',len(high)==len(s['high_entities']) and all(all(r[k] for k in ['construct_assign_copy_move','iteration_order','compare_hash','nested_element_map_key','thread_dso','c_api','errors','additional','possible_differences','operation_sources']) for r in high))
maxsize=max((f.stat().st_size,str(f.relative_to(OUT))) for f in OUT.rglob('*') if f.is_file())
check('no_github_100MiB_file',maxsize[0]<100*1024*1024)
check('scope_not_claimed_complete','PARTIAL_SCOPE' in (OUT/'SCOPE.md').read_text())
save('SELF_CHECK.json',{'status':'PASS','checks':checks,'largest_file':{'bytes':maxsize[0],'path':maxsize[1]},'source_file_paths_checked':len(sourcecache),'crosswalk_counts':dict(groups),'boundary':'检查生成清单与计数一致性；不证明声明全集、语义使用数或兼容性'})
print('SELF_CHECK PASS',len(checks),'checks',s['entities'],'rows')
