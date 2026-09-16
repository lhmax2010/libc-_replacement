"""筛选复核队列，不自动将符号命中定为跨包。"""
from common import *
import csv,gzip,re,collections
d=OUT/'W1'
rows=collections.defaultdict(list); seen=collections.defaultdict(set)
reject=collections.Counter()
with gzip.open(d/'symbol_candidate_edges.tsv.gz','rt') as f:
    for r in csv.DictReader(f,delimiter='\t'):
        s=r['demangled']; n=r['entity']
        if r['runtime_provider']=='True': reject[(n,'RUNTIME_CALL')]+=1; continue
        if s.startswith(('std::','typeinfo ','vtable ','VTT ','guard variable ','non-virtual thunk to std::','virtual thunk to std::')):
            reject[(n,'STANDARD_IMPLEMENTATION_OR_METADATA')]+=1;continue
        key=(r['provider_source'],s)
        if key in seen[n]:continue
        seen[n].add(key);rows[n].append(r)
save(d/'BUSINESS_SYMBOL_QUEUE.json',rows)
with (d/'BATCH_100.tsv').open() as f: order=list(csv.DictReader(f,delimiter='\t'))
for r in order:
    n=r['entity'];print('\n###',r['rank'],n,'unique_business_candidates',len(rows[n]),'runtime_rows',reject[n,'RUNTIME_CALL'])
    for e in rows[n][:5]:print(e['provider_source'],'->',e['consumer_source'],e['demangled'])
