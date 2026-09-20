#!/usr/bin/env python3
"""汇出 B 通道完整既有命中，不以 ELF 名字零交集作结论。"""
from common import *
plan=json.loads((OUT/'code/plan.json').read_text()); table={x['rank']:x for x in inputs()}
for rank in plan['B_REVIEW']:
    name=table[rank]['row']['candidate']; p=ROOT/'docs/progress/R119_batch2/W1/entity_full'/(name.replace('::','_')+'.json')
    matches=json.loads(p.read_text()) if p.exists() else []
    print('\nRANK',rank,name,'qualified_contexts',len(matches))
    for i,h in enumerate(matches):
        print(i,h['package'],h['arch'],h['installed_path'],h['line'],h['context'])
    u=ROOT/'docs/progress/R119_batch2/W1/unqualified'/(name.split('::')[-1]+'.json')
    if u.exists():
        obj=json.loads(u.read_text());print('UNQUALIFIED_SCHEMA',str(obj)[:220])
