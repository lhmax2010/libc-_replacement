from common import *
import csv
def read(path):
    with path.open() as f:return list(csv.DictReader(f,delimiter='\t'))
candidates=read(ROOT/'docs/progress/R117/platform/INTERFACE_CANDIDATES.tsv')
assert len(candidates)==495
skeleton={r['ID']:r for r in read(ROOT/'docs/progress/R117/SKELETON.tsv')}
for c in candidates:c['skeleton_family_count']=int(skeleton[c['id']]['去重包族数'])
candidates.sort(key=lambda c:(-c['skeleton_family_count'],c['id']))
rows=[dict(rank=i+1,**r)for i,r in enumerate(candidates)]
dest=OUT/'W1';dest.mkdir(parents=True,exist_ok=True)
for name,data in [('ORDER_495.tsv',rows),('BATCH_100.tsv',rows[:100])]:
    with (dest/name).open('w',newline='')as f:
        w=csv.DictWriter(f,fieldnames=list(data[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(data)
save(dest/'INPUTS.json',[dict(path=str(p.relative_to(ROOT)),sha256=digest(p)) for p in [ROOT/'docs/progress/R117/platform/INTERFACE_CANDIDATES.tsv',ROOT/'docs/progress/R117/SKELETON.tsv']])
for r in rows[:100]:print(r['rank'],r['entity'],r['skeleton_family_count'],r['package_family_count'])
