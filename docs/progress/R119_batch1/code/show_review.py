from common import *
import json,csv
d=OUT/'W1';headers=json.loads((d/'DIVERSE_HEADER_REVIEW.json').read_text());biz=json.loads((d/'EXPANDED_BUSINESS_QUEUE.json').read_text())
with (d/'BATCH_100.tsv').open()as f:order=list(csv.DictReader(f,delimiter='\t'))
for r in order[int(sys.argv[1])-1:int(sys.argv[2])]:
    n=r['entity'];print('\n###',r['rank'],n)
    for h in headers.get(n,[])[:5]:print(h['family'],h['installed_path'],h['line'],h['context'])
    for e in biz.get(n,[])[:2]:print('ELF_QUEUE',e['provider_source'],'->',e['consumer_source'],e['demangled'])
