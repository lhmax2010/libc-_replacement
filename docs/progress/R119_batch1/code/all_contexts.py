"""全部一百候选的已收集限定名上下文，SHA/行去重；不是语义扫描。"""
from common import *
import csv,gzip,collections,json
csv.field_size_limit(20_000_000)
rows=collections.defaultdict(list);seen=collections.defaultdict(set)
with gzip.open(OUT/'W1/header_occurrences.tsv.gz','rt')as f:
    for r in csv.DictReader(f,delimiter='\t'):
        n=r['entity']
        if r['sourcerpm'].startswith(('gcc-','llvm-')):continue
        key=(r['sha256'],r['line'])
        if key in seen[n]:continue
        seen[n].add(key);rows[n].append(r)
for n,items in rows.items():save(OUT/'W1/entity_full'/(n.replace('::','_')+'.json'),items)
save(OUT/'W1/entity_full/COUNTS.json',{n:len(items)for n,items in rows.items()})
print(json.dumps({n:len(items)for n,items in rows.items()},ensure_ascii=False))
