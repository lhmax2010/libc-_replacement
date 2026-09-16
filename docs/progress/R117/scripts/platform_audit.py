from common import *
import csv,gzip,collections
status=json.loads((OUT/'platform/STATUS.json').read_text());assert len(status)==371
families={r['source_rpm']:r['package_name'] for r in csv.DictReader((ROOT/'docs/progress/R32/tables/scan_status.tsv').open(),delimiter='\t')}
filecounts=collections.Counter();examples=[]
with gzip.open(OUT/'platform/files.tsv.gz','rt') as f:
 for row in csv.DictReader(f,delimiter='\t'):
  filecounts[row['status']]+=1
  if row['status']!='READ':examples.append(row)
with (OUT/'platform/unread_files.tsv').open('w',newline='') as f:
 w=csv.DictWriter(f,['source_rpm','path','size','status','sha256'],delimiter='\t');w.writeheader();w.writerows(examples)
using_pkgs=set();using_count=0
with gzip.open(OUT/'platform/unqualified_contexts.tsv.gz','rt') as f:
 for row in csv.DictReader(f,delimiter='\t'):using_count+=1;using_pkgs.add(row['source_rpm'])
summary={'source_rpm_identities':len(status),'unique_package_names':len({families[s['source_rpm']] for s in status}),'eligible_files':sum(s['eligible_files'] for s in status),'read_files':sum(s['read_files'] for s in status),'read_bytes':sum(s['bytes'] for s in status),'token_hits':sum(s['token_hits'] for s in status),'package_status':dict(collections.Counter(s['status'] for s in status)),'extraction_status':dict(collections.Counter(s['extraction_status'] for s in status)),'file_status':dict(filecounts),'using_namespace_contexts':using_count,'using_namespace_source_rpms':len(using_pkgs),'exact_semantic_usage':'NOT_OBSERVED','scope':'冻结 manifest 的 C/C++/ObjC 文本扩展名；非全部产品、非所有构建配置'}
save('platform/SUMMARY.json',summary)
print(json.dumps(summary,ensure_ascii=False,indent=2))
