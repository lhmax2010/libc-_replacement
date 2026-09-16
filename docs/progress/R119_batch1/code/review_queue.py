from common import *
import csv,gzip,json,re,collections,sys
dest=OUT/'W1';candidate=json.loads((dest/'SYMBOL_REVIEW_QUEUE.json').read_text())
with (dest/'BATCH_100.tsv').open()as f:order=list(csv.DictReader(f,delimiter='\t'))
source_names={}
with (ROOT/'docs/progress/P7_0909/inputs/source_package_records.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):source_names[r['location']]=r['name']
providers={n:{r['provider_source'] for r in edges} for n,edges in candidate.items()}
queue=collections.defaultdict(dict)
with gzip.open(dest/'header_occurrences.tsv.gz','rt')as f:
    for r in csv.DictReader(f,delimiter='\t'):
        family=source_names.get(r['sourcerpm'],'NOT_OBSERVED')
        if family in ['gcc','llvm']:continue
        n=r['entity'];t=r['text'];key=(r['installed_path'],r['line'])
        rank=(family not in providers[n],r['private_path']=='YES',bool(re.search(r'\b(return|if|while|assert|static_assert|using|typedef)\b',t)),not(';'in t),not('('in t),len(t)>250)
        if key not in queue[n] or rank<queue[n][key][0]:queue[n][key]=(rank,{**r,'family':family})
        if len(queue[n])>200:queue[n]=dict(sorted(queue[n].items(),key=lambda kv:kv[1][0])[:100])
save(dest/'REVIEW_QUEUE_RANKED.json',{n:[r for _,r in sorted(d.values(),key=lambda x:x[0])[:25]]for n,d in queue.items()})
start=int(sys.argv[1]) if len(sys.argv)>1 else 1;end=int(sys.argv[2]) if len(sys.argv)>2 else 25
for r in order[start-1:end]:
    n=r['entity'];print('\n###',r['rank'],n)
    for i,(_,h) in enumerate(sorted(queue[n].values(),key=lambda x:x[0])[:4]):print('HEADER',i,h['family'],h['installed_path'],h['line'],h['context'])
    for i,e in enumerate(candidate[n][:3]):print('EDGE',i,e['provider_source'],'->',e['consumer_source'],e['demangled'])
