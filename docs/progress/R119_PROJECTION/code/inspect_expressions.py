#!/usr/bin/env python3
from common import *
from expressions import request
from collections import Counter,defaultdict
seen={};stats=Counter();examples=defaultdict(list)
for c in candidates():
 p=OUT/f'occurrences/{int(c["rank"]):03}.json.gz'
 if not p.exists():continue
 with gzip.open(p,'rt') as f:rows=json.load(f)
 for h in rows:
  expr,why=request(h);stats['concrete_requests' if expr else 'context_needed']+=1
  if expr:seen.setdefault(expr,h)
  elif len(examples[c['candidate']])<2:examples[c['candidate']].append((h.get('raw_template_arguments'),why))
save(OUT/'CONCRETE_QUEUE.json',[dict(expression=e,source_id=h['id'],candidate=h['candidate'],context=h.get('context',''),references=h['references']) for e,h in seen.items()])
print(dict(stats),'distinct concrete',len(seen))
for e in seen:print('EXPR',e)
for n,v in examples.items():print('UNRESOLVED',n,v)
