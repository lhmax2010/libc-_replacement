from common import *
import json
for p in sorted((OUT/'W1/positive').glob('*.json')):
    if p.name=='INDEX.json':continue
    r=json.loads(p.read_text());print('\n###',p.name)
    for h in r['header_matches'][:4]:print('HEADER',h['metadata']['path'],h['context'])
    for e in r['elf_matches']:print('ELF',e['consumer']['inventory']['path'],'->',e['provider']['inventory']['path'],e['symbol']['demangled'])
    for i in r['source_includes']:
        print('INCLUDE exit',i['exit']);print('\n'.join(i['matches'][:3]))
