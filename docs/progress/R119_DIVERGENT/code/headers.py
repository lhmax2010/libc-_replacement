#!/usr/bin/env python3
"""冻结安装头全量文字命中；不把文字命中自动判为公开签名或包边。"""
from common import *
import re,collections
gate('headers_gate');start=time.time()
table=ROOT/'docs/progress/P7_0909/tables'
pkgs=list(csv.DictReader((table/'packages.tsv').open(),delimiter='\t'))
selected={(r['repo_id'],r['name'],r['arch']) for r in pkgs if r['devel_name']=='YES'}
types=[r for r in json.loads((OUT/'TYPE_RESULTS.json').read_text()) if r['result']=='DIVERGENT']
# 按完整类型表达式的公开根和嵌套成员建立过滤器；实参/局部别名关系后续人工审核。
targets=collections.defaultdict(list)
for r in types:
 expr=r['expression'];name=re.search(r'std::(?:chrono::|filesystem::|ios_base::|placeholders::)?[A-Za-z_0-9]+',expr)[0]
 targets[name].append(r['id'])
pattern=re.compile(r'(?<![\w:])(?:'+'|'.join(re.escape(s) for s in sorted(targets,key=len,reverse=True))+r')\b')
rows_by_sha=collections.defaultdict(list);issues=[];stats=collections.Counter()
with gzip.open(table/'headers.tsv.gz','rt') as f:
 for r in csv.DictReader(f,delimiter='\t'):
  stats['inventory_rows']+=1
  if (r['repo_id'],r['name'],r['arch']) not in selected or r['scope']!='INCLUDE_TREE':continue
  stats['selected_rows']+=1
  p=pathlib.Path(r['cache_path'])
  if p.is_symlink() or not p.is_file():issues.append(dict(path=r['path'],package=r['name'],reason='SYMLINK_OR_NONFILE'));continue
  rows_by_sha[r['file_sha256']].append(r)
hits=[];coverage=[];verified={}
for n,(digest,refs) in enumerate(rows_by_sha.items(),1):
 if n%10000==1:gate('headers_gate_'+str(n))
 p=pathlib.Path(refs[0]['cache_path']);actual=sha(p);assert actual==digest,(p,digest,actual)
 verified[str(p)]=actual;text=p.read_text(errors='replace');stats['unique_sha_read']+=1;stats['read_rows']+=len(refs)
 if not pattern.search(text):continue
 lines=text.splitlines()
 for i,line in enumerate(lines):
  names=sorted(set(pattern.findall(line)))
  if not names:continue
  ids=sorted({id for name in names for id in targets[name]})
  hit=dict(id=f'H{len(hits)+1:06}',sha256=digest,line=i+1,text=line,matched_roots=names,type_candidates=ids,context='\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-5),min(len(lines),i+7))),references=refs,qualification='LEXICAL_ONLY_NOT_SIGNATURE_PROOF')
  hits.append(hit)
stats['hit_positions']=len(hits)
with gzip.open(OUT/'HEADER_HITS.json.gz','wt') as f:json.dump(hits,f,ensure_ascii=False)
save(OUT/'HEADER_SCAN_SUMMARY.json',dict(stats=dict(stats),issues=issues,seconds=time.time()-start,inputs={str((table/p).relative_to(ROOT)):sha(table/p) for p in ['headers.tsv.gz','packages.tsv']},limitations='按公开根文字扫描；模板实参/宏/别名/using/返回值需人工追链；没有把候选命中当阳性边'))
save(OUT/'HEADER_SCAN_SHA256.json',verified)
counts=collections.Counter((ref['sourcerpm'],name) for h in hits for ref in h['references'][:1] for name in h['matched_roots'])
tsv(OUT/'HEADER_HIT_COUNTS.tsv',[dict(source_rpm=k[0],root=k[1],unique_content_lines=v) for k,v in sorted(counts.items())])
print(json.dumps(dict(stats=stats,issues=len(issues),seconds=time.time()-start),ensure_ascii=False))
