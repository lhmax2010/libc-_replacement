#!/usr/bin/env python3
"""逐项输出可追溯上下文；样本只用于找线索，不用于阴性分类。"""
import csv,json,pathlib,re,sys
base=pathlib.Path(__file__).resolve().parents[1]
first=base.parent/'R119_batch1'
with (base/'W1/BATCH_100.tsv').open() as f:order=list(csv.DictReader(f,delimiter='\t'))
start,end=map(int,sys.argv[1:3]);pattern=re.compile(sys.argv[3]) if len(sys.argv)>3 else None
queue=json.loads((first/'W1/full_elf/CANDIDATE_QUEUE.json').read_text())
for row in order:
    if not start<=int(row['rank'])<=end:continue
    name=row['entity'];path=base/'W1/entity_full'/(name.replace('::','_')+'.json')
    items=json.loads(path.read_text()) if path.exists() else []
    print('\nCANDIDATE',row['rank'],name,'total_unique_contexts',len(items))
    filtered=[r for r in items if not r['sourcerpm'].startswith(('gcc-','llvm-')) and (not pattern or pattern.search(json.dumps(r)))]
    filtered.sort(key=lambda r:(r['private_path']=='YES',r['package'].startswith('boost-'),len(r['text'])>300))
    seen=set();shown=0
    for r in filtered:
        key=r['sourcerpm']
        if key in seen and not pattern:continue
        seen.add(key);shown+=1
        print(r['sourcerpm'],r['installed_path']+':'+str(r['line']),r['context'])
        if shown>=(20 if pattern else 3):break
    for edge in queue.get(name,[])[:3]:
        print('SYMBOL_CANDIDATE',edge['consumer_id'],edge['provider_id'],edge['consumer']['sourcerpm'],'->',edge['provider']['sourcerpm'],edge['symbol'])
    print('LIMIT: 限定名样本与符号定位队列不是定界结果。')
