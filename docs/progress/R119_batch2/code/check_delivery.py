#!/usr/bin/env python3
"""核对两批冻结顺序、逐项材料、阳性证据引用和脚本快照。"""
import collections,csv,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[4]
first=root/'docs/progress/R119_batch1';second=root/'docs/progress/R119_batch2'
with (first/'W1/ORDER_495.tsv').open() as f:order=list(csv.DictReader(f,delimiter='\t'))
reports=[]
for base,filename,start in [(first,'REVIEW_001_100.tsv',1),(second,'REVIEW_101_200.tsv',101)]:
    with (base/'W1'/filename).open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
    assert len(rows)==100
    for i,r in enumerate(rows,start):
        assert int(r['rank'])==i and r['candidate']==order[i-1]['entity']
        assert (base/f'W1/reviewed/{i:03d}.json').is_file()
        for ref in r['evidence'].split(';'):
            p=base/ref
            assert p.exists() or list(p.parent.glob(p.name+'.*')),(i,ref)
        if r['result']=='CROSS_PACKAGE':
            assert r['provider_package']!='NOT_OBSERVED' and r['consumer_package']!='NOT_OBSERVED'
    counts=collections.Counter(r['result'] for r in rows)
    reports.append(dict(batch=base.name,range=[start,start+99],records=100,classifications=counts,scope_complete=False))
    hashes={str(p.relative_to(base)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted((base/'code').glob('*')) if p.is_file()}
    hashes['../API_0911/record.py']=hashlib.sha256((root/'docs/progress/API_0911/record.py').read_bytes()).hexdigest()
    hashes['../R118/code/run.py']=hashlib.sha256((root/'docs/progress/R118/code/run.py').read_bytes()).hexdigest()
    (base/'CURRENT_SCRIPT_SHA256.json').write_text(json.dumps(hashes,indent=2)+'\n')
    if base==second:
        cross=[r for r in rows if r['result']=='CROSS_PACKAGE']
        with (base/'W1/CROSS_PACKAGE_INPUT.tsv').open('w',newline='') as f:
            w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(cross)
manifest={}
for base in [first,second]:
    for p in sorted(base.rglob('*')):
        if not p.is_file() or '__pycache__' in p.parts:continue
        if p.name in ['DELIVERY_CONTENT_SHA256.json','DELIVERY_CHECK.json']:continue
        if p.name.startswith('037_delivery_check.') or p.name.startswith('044_delivery_check.'):
            continue  # 记录器仍在写入本次核对的输出，不对未结束的记录做快照。
        manifest[str(p.relative_to(root))]=hashlib.sha256(p.read_bytes()).hexdigest()
(second/'DELIVERY_CONTENT_SHA256.json').write_text(json.dumps(manifest,indent=2)+'\n')
report=dict(batches=reports,content_hash_count=len(manifest),order_control='PASS',reference_control='PASS',scripts_sha256='CURRENT_SCRIPT_SHA256.json',limit='一致性核对不是未定项定界完成，也不是跨库运行安全验证。内容摘要为核对时的快照，不含核对命令自身的活动输出和随后 Git 交付记录。')
(second/'DELIVERY_CHECK.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(report,ensure_ascii=False,indent=2))
