#!/usr/bin/env python3
from common import *
from collections import Counter
changes={r['rank']:r for name in ['A_RECLASSIFIED.json','B_RECLASSIFIED.json'] for r in json.loads((OUT/name).read_text())}
rows=[]
for item in inputs():
    old=item['row'];rank=item['rank'];change=changes.get(rank)
    rows.append(change or dict(rank=rank,candidate=old['candidate'],original=old['result'],new=old['result'],subtype='',scope='原阳性证据范围' if old['result']=='CROSS_PACKAGE' else 'NOT_OBSERVED',reason='既有证据链不变' if old['result']=='CROSS_PACKAGE' else item['dossier']['review'].get('missing',''),evidence=str(item['dossier_path'].relative_to(ROOT))))
tsv(OUT/'RECLASSIFIED_001_200.tsv',rows)
save(OUT/'DISTRIBUTION.json',dict(overall=dict(Counter(r['new'] for r in rows)),first100=dict(Counter(r['new'] for r in rows[:100])),second100=dict(Counter(r['new'] for r in rows[100:])),subtypes=dict(Counter(r['subtype'] for r in rows if r['new']=='NO_STDLIB_OBJECT')),original_unknowns=156,reclassified_unknowns=len(changes),remaining_unknowns=sum(r['new']=='NOT_OBSERVED' for r in rows)))
for p,digest in json.loads((OUT/'INPUT_SHA256.json').read_text()).items():assert sha(ROOT/p)==digest
print((OUT/'DISTRIBUTION.json').read_text())
