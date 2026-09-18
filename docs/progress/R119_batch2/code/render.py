#!/usr/bin/env python3
"""只将人工逐项复核且证据闭合的项归入确定类别。"""
import collections,csv,datetime,hashlib,json,pathlib,sys
base=pathlib.Path(__file__).resolve().parents[1]
first=base.parent/'R119_batch1'
end=int(sys.argv[1]);notes={}
for p in sorted((base/'code').glob('review_notes_*.json')):notes.update(json.loads(p.read_text()))
with (base/'W1/BATCH_100.tsv').open() as f:order=list(csv.DictReader(f,delimiter='\t'))
runtime=json.loads((first/'W1/RUNTIME_CALL_PROOFS.json').read_text())
rows=[]
for r in order:
    rank=int(r['rank']);n=r['entity']
    if rank>end:continue
    assert n in notes,(rank,n)
    note=notes[n];status='NOT_OBSERVED';provider=consumer=shape=ownership='NOT_OBSERVED'
    evidence=['code/'+next(p.name for p in reversed(sorted((base/'code').glob('review_notes_*.json'))) if n in json.loads(p.read_text()))]
    for extra in note.get('extra_evidence',[]):
        assert (base/extra).exists(),extra
        evidence.append(extra)
    if note.get('proof'):
        path=first/'W1/positive'/(note['proof']+'.json');p=json.loads(path.read_text())
        assert p['header_matches'] and p['elf_matches']
        for h in p['header_matches']:
            m=h['metadata'];assert hashlib.sha256(pathlib.Path(m['cache_path']).read_bytes()).hexdigest()==m['file_sha256']
        direct=any(e['exit']==0 and e['matches'] for e in p['source_includes'])
        chain=path.with_name(path.stem+'_include_chain.json')
        if chain.exists():direct=direct or bool(json.loads(chain.read_text()))
        assert direct,(rank,n)
        status='CROSS_PACKAGE';provider=p['spec']['provider'];consumer=p['spec']['consumer'];shape=note['shape'];ownership=note['ownership']
        evidence.append('../R119_batch1/W1/positive/'+path.name)
        if chain.exists():evidence.append('../R119_batch1/W1/positive/'+chain.name)
    dossier=dict(rank=rank,candidate=n,result=status,review=note,runtime_reference=runtime.get(n),limit='证据复用不等于新增实验；同一底层类型或包边的多个候选名不可当独立边相加。')
    dest=base/f'W1/reviewed/{rank:03d}.json';dest.parent.mkdir(exist_ok=True);dest.write_text(json.dumps(dossier,ensure_ascii=False,indent=2)+'\n')
    rows.append(dict(rank=rank,candidate=n,result=status,provider_package=provider,consumer_package=consumer,shape=shape,ownership=ownership,exceptions='NOT_OBSERVED',runtime_reference='RUNTIME_CALL' if n in runtime else 'NOT_OBSERVED',individual_manual_review_seconds='NOT_OBSERVED',evidence=';'.join(evidence)))
assert len(rows)==end-100
with (base/f'W1/REVIEW_101_{end}.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
counts=collections.Counter(r['result'] for r in rows)
now=datetime.datetime.now(datetime.timezone.utc)
start=datetime.datetime.fromisoformat(json.loads((base/'raw/001_headers.time.json').read_text())['start'])
report=dict(reviewed_through=end,batch_reviewed=len(rows),not_reviewed=200-end,classifications=counts,scope_complete=False,elapsed_seconds=(now-start).total_seconds(),timing_limit='含共享索引、证据复用与交付时间；逐候选人工时间未记录，不冒充完整定界的平均耗时。',timestamp_utc=now.isoformat())
(base/f'W1/CHECKPOINT_{end}.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
manifest={str(p.relative_to(base)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted((base/'code').glob('*')) if p.is_file()}
(base/'SCRIPT_SHA256.json').write_text(json.dumps(manifest,indent=2)+'\n')
text=['# 第二批候选定界状态','',f'已写第 101–{end} 项的逐项复核记录；尚有 {200-end} 项未复核。**PARTIAL，不等于全部定界完成。**','','| 分类 | 候选数 |','| --- | ---: |']
for k in ['INTERNAL_ONLY','PUBLIC_NO_CONSUMER','CROSS_PACKAGE','RUNTIME_CALL','NOT_OBSERVED']:
    text.append(f'| {k} | {counts[k]} |')
text.extend(['','零值表示尚未作出该分类，不是平台零命中结论；运行库关系列与主分类可重叠。成员/基类及别名计数不等于新的独立包边。', '',f'记录：[TSV](W1/REVIEW_101_{end}.tsv)。逐项依据和缺口见 `W1/reviewed/`。', '',f'实际累计墙钟时间 {report["elapsed_seconds"]:.1f} 秒；完整定界平均时间 NOT_OBSERVED。', '', '资源：light，串行，nice 19 + ionice 3，30% RLIMIT_AS；Git 普通 I/O；不使用板子。', '', '第一批仍有未定项，参见 ../R119_batch1/W1/SUMMARY.md。跨批建索引和复核不抹去第一批缺口。',''])
(base/'STATUS.md').write_text('\n'.join(text))
print(json.dumps(report,ensure_ascii=False))
