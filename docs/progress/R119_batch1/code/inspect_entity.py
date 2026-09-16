from common import *
import csv,gzip,json
n=sys.argv[1]; seen=set();rows=[]
with gzip.open(OUT/'W1/header_occurrences.tsv.gz','rt')as f:
    for r in csv.DictReader(f,delimiter='\t'):
        if r['entity']!=n or r['sourcerpm'].startswith(('gcc-','llvm-')):continue
        key=(r['sha256'],r['line'])
        if key in seen:continue
        seen.add(key);rows.append(r)
save(OUT/'W1/entity_full'/(n.replace('::','_')+'.json'),rows)
for i,r in enumerate(rows,1):print(i,r['package'],r['installed_path'],r['line'],r['context'])
print('UNIQUE_SHA_LINE_COUNT',len(rows))
