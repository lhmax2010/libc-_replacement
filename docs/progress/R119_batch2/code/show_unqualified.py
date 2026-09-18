#!/usr/bin/env python3
import collections,json,pathlib,sys
base=pathlib.Path(__file__).resolve().parents[1]
for name in sys.argv[1:]:
    rows=json.loads((base/'W1/unqualified'/(name+'.json')).read_text())
    grouped=collections.defaultdict(list)
    for r in rows:
        m=r['metadata']
        if '/c++/' in m['path']:continue
        grouped[(m['sourcerpm'],m['path'])].append(r)
    print('\nTOKEN',name)
    for key,items in grouped.items():
        print(*key,'lines',','.join(str(r['line']) for r in items))
        if name in ['future_status','directory_iterator','smatch']:
            for r in items[:2]:print(r['context'])
