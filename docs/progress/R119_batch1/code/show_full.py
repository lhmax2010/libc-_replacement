from common import *
import json,gzip
for n in sys.argv[1:]:
    p=OUT/'W1/entity_full'/(n.replace('::','_')+'.json')
    rows=json.loads(p.read_text())if p.exists()else json.loads(gzip.open(p.with_suffix('.json.gz'),'rt').read())
    print('\nENTITY',n,'COUNT',len(rows))
    for i,r in enumerate(rows,1):print(i,r['sourcerpm'],r['installed_path'],r['line'],r['context'])
