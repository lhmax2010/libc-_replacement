#!/usr/bin/env python3
from common import *
for item in inputs():
    r=item['row']
    if r['result']!='CROSS_PACKAGE':continue
    paths=[(item['base']/p).resolve() for p in r['evidence'].split(';') if '/positive/' in p and p.endswith('.json') and not p.endswith('_include_chain.json')]
    print(r['rank'],r['candidate'],r['provider_package'],r['consumer_package'])
    for p in paths:
        obj=json.loads(p.read_text()); print(p.name,'SPEC',json.dumps(obj.get('spec'),ensure_ascii=False),'MATCH_SCHEMA',json.dumps(obj.get('elf_matches',[{}])[0],ensure_ascii=False)[:1700])
