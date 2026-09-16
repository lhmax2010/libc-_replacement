"""只发布人工已写复核记录的候选；未闭合保持 NOT_OBSERVED。"""
from common import *
import csv,json,gzip,collections,re,datetime
d=OUT/'W1';end=int(sys.argv[1]);notes={}
for p in sorted((OUT/'code').glob('review_notes_*.json')):notes.update(json.loads(p.read_text()))
with (d/'BATCH_100.tsv').open()as f:order=list(csv.DictReader(f,delimiter='\t'))
runtime=collections.defaultdict(list)
with gzip.open(d/'expanded_symbol_edges.tsv.gz','rt')as f:
    for r in csv.DictReader(f,delimiter='\t'):
        if r['runtime_provider']=='True':runtime[r['entity']].append(r)
header_samples=json.loads((d/'DIVERSE_HEADER_REVIEW.json').read_text())
reviewed=[];rows=[]
for src in order:
    n=src['entity'];rank=int(src['rank']);note=notes.get(n)if rank<=end else None
    status='NOT_OBSERVED_PENDING_REVIEW';provider=consumer=shape=own=exception='NOT_OBSERVED';refs=[];seconds='NOT_OBSERVED'
    if note:
        status='NOT_OBSERVED';refs=['code/'+next(p.name for p in sorted((OUT/'code').glob('review_notes_*.json'))if n in json.loads(p.read_text()))]
        if note.get('proof'):
            p=d/'positive'/(note['proof']+'.json');proof=json.loads(p.read_text());refs.append(str(p.relative_to(OUT)))
            assert proof['elf_matches'] and proof['header_matches'],n
            for h in proof['header_matches']:assert digest(h['metadata']['cache_path'])==h['metadata']['file_sha256']
            direct=any(i['exit']==0 and i['matches']for i in proof['source_includes'])
            chain=d/'positive'/(note['proof']+'_include_chain.json')
            if chain.exists():
                direct=direct or bool(json.loads(chain.read_text()));refs.append(str(chain.relative_to(OUT)))
            if note['proof'] in ['zypp_set','zypp_istream']:
                raw=OUT/'raw/074_swig_declarations.stdout'
                assert ('ZConfig.h'if note['proof']=='zypp_set'else 'CheckSum.h')in raw.read_text()
                direct=True;refs.append('raw/074_swig_declarations')
            if direct:status='CROSS_PACKAGE'
            else:status='NOT_OBSERVED_INCLUDE_CHAIN'
            provider=proof['spec']['provider'];consumer=proof['spec']['consumer'];shape=note['shape'];own=note['ownership'];seconds=round(proof['verification_seconds'],3)
        reviewed.append(n)
        # 程序耗时仅是证据关联用时，不伪造人工逐候选时间。
        dossier={'entity':n,'status':status,'review':note,'header_samples':header_samples.get(n,[]),'runtime_index_rows':len(runtime[n]),'runtime_sample':runtime[n][:1], 'limits':'样本不是全量阴性证明；NOT_OBSERVED 不等于不跨包；静态 ELF 引用不等于已经运行到该接口。','proof_paths':refs}
        save(d/'reviewed'/f'{rank:03d}.json',dossier)
    row=dict(rank=rank,candidate=n,result=status,provider_package=provider,consumer_package=consumer,shape=shape,ownership=own,exceptions=exception,runtime_reference='RUNTIME_CALL'if runtime[n]else'NOT_OBSERVED',runtime_index_rows=len(runtime[n]),evidence_link_seconds=seconds,individual_manual_review_seconds='NOT_OBSERVED',evidence=';'.join(refs)or'NOT_OBSERVED')
    rows.append(row)
dest=d/f'REVIEW_001_{end:03d}.tsv'
with dest.open('w',newline='')as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows[:end])
with (d/'REVIEW_PROGRESS_100.tsv').open('w',newline='')as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
counts=collections.Counter(r['result']for r in rows[:end]);save(d/f'CHECKPOINT_{end:03d}.json',{'timestamp_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'review_notes_written':len(reviewed),'review_range':[1,end],'statuses':counts,'scope_complete':False,'unreviewed':100-len(reviewed),'timing_limit':'证据关联脚本有实测用时；人工逐项计时未记录，不以脚本秒数替代10–30分钟人工定界时间，也不据此估算后四批。'})
print(json.dumps(dict(reviewed=len(reviewed),statuses=counts),ensure_ascii=False))
