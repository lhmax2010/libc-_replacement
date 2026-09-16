"""剩余候选的分散上下文；仅提供复核材料，不用关键词自动判阴性。"""
from common import *
import csv,gzip,json,re,collections
d=OUT/'W1';biz=json.loads((d/'BUSINESS_SYMBOL_QUEUE.json').read_text())
with (d/'BATCH_100.tsv').open()as f:order=list(csv.DictReader(f,delimiter='\t'))
families={}
with (ROOT/'docs/progress/P7_0909/inputs/source_package_records.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):families[r['location']]=r['name']
q=collections.defaultdict(dict);counts=collections.Counter()
with gzip.open(d/'header_occurrences.tsv.gz','rt')as f:
    for r in csv.DictReader(f,delimiter='\t'):
        n=r['entity'];fam=families.get(r['sourcerpm'],'NOT_OBSERVED');counts[n]+=1
        if fam in ['gcc','llvm']:continue
        text=r['text'];key=(fam,r['installed_path'],r['line'])
        rank=(fam=='boost',r['private_path']=='YES',bool(re.search(r'\b(return|throw|catch|static_assert|assert)\b',text)),bool(re.search(r'\b(if|while|for)\s*\(',text)),len(text)>230)
        q[n][key]=(rank,{**r,'family':fam})
        if len(q[n])>120:q[n]=dict(sorted(q[n].items(),key=lambda kv:kv[1][0])[:60])
result={}
for r in order:
    n=r['entity'];chosen=[];used=collections.Counter()
    for rank,h in sorted(q[n].values(),key=lambda x:x[0]):
        if used[h['family']]>=3:continue
        chosen.append(h);used[h['family']]+=1
        if len(chosen)==12:break
    result[n]=chosen
save(d/'DIVERSE_HEADER_REVIEW.json',result)
start=int(sys.argv[1]);end=int(sys.argv[2])
for r in order[start-1:end]:
    n=r['entity'];print('\n###',r['rank'],n,'occurrences',counts[n],'business candidates',len(biz.get(n,[])))
    for h in result[n][:6]:print(h['family'],h['installed_path'],h['line'],h['context'])
