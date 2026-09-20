#!/usr/bin/env python3
"""只从两份冻结限定名索引取全量，不使用旧 plan 的表达式。"""
from common import *
from collections import Counter, defaultdict
start=time.time();gate('inventory_gate');cs=candidates(); names={r['candidate']:r for r in cs}; groups=defaultdict(list)
paths=[ROOT/f'docs/progress/R119_batch{i}/W1/header_occurrences.tsv.gz' for i in [1,2]]
for path in paths:
 with gzip.open(path,'rt') as f:
  for line,r in enumerate(csv.DictReader(f,delimiter='\t'),2):
   if r['entity'] in names:
    r['index_path']=str(path.relative_to(ROOT));r['index_line']=line;groups[r['entity']].append(r)
frozen={str(p.relative_to(ROOT)):sha(p) for p in paths}
for p in OLD.rglob('*'):
 if p.is_file() and '__pycache__' not in p.parts:frozen[str(p.relative_to(ROOT))]=sha(p)
save(OUT/'FROZEN_INPUT_SHA256.json',frozen)
rows=[]
for c in cs:
 name=c['candidate'];rank=int(c['rank']);matches=groups[name];assert matches,name
 previous=json.loads((OLD/f'items/{rank:03}.json').read_text())['platform_samples']
 prior={(s['source'].get('sha256',s['current_sha256']),int(s['source'].get('line',0))) for s in previous}
 unique={(h['sha256'],int(h['line'])) for h in matches}
 assert prior<=unique,(name,prior-unique)
 p=OUT/'inventory'/f'{rank:03}.json.gz';p.parent.mkdir(parents=True,exist_ok=True)
 with gzip.open(p,'wt') as f:json.dump(matches,f,ensure_ascii=False)
 rows.append(dict(rank=rank,candidate=name,total_index_rows=len(matches),unique_sha_line=len(unique),previous_bound_entries=len(previous),previous_unique_sha_line=len(prior),new_unique_sha_line=len(unique-prior),index_rows_corresponding_to_previous=sum((h['sha256'],int(h['line'])) in prior for h in matches),evidence=str(p.relative_to(OUT))))
tsv(OUT/'COVERAGE_INVENTORY.tsv',rows)
save(OUT/'INVENTORY_SUMMARY.json',dict(candidates=65,total_index_rows=sum(r['total_index_rows'] for r in rows),unique_candidate_sha_line=sum(r['unique_sha_line'] for r in rows),previous_bound_entries=sum(r['previous_bound_entries'] for r in rows),previous_unique_candidate_sha_line=sum(r['previous_unique_sha_line'] for r in rows),new_unique_candidate_sha_line=sum(r['new_unique_sha_line'] for r in rows),elapsed_seconds=time.time()-start))
print((OUT/'INVENTORY_SUMMARY.json').read_text());print('\n'.join(f"{r['rank']} {r['candidate']} index={r['total_index_rows']} unique={r['unique_sha_line']}" for r in rows))
