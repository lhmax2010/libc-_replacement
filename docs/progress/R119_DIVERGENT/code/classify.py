#!/usr/bin/env python3
"""对冻结逐处结果补充独立的类型一致性门槛，不改旧产物。"""
from common import *
from collections import Counter
import re
def checked_no_object(d):
    if d['result']!='RESULT_NO_OBJECT':return dict(d)
    if d['basis']=='STATIC_DEFINITION_CHAIN':
        assert d['result_type']=='const bool；谓词值 NOT_OBSERVED'
        actual=['const bool','const bool']
    else:
        assert d['basis']=='MEASURED_BOTH'
        actual=[]
        for lib,ref in zip(['gnu','cxx'],d['measurement']):
            m=json.loads((OLD/ref).read_text());assert m['status']=='MEASURED' and m['repeat_count']==5
            actual.append(re.search(r'TYPE=.*?\[T = (.*?)\]',m['result'])[1])
            assert actual[-1]==d['result_type'][lib]
            assert any(s in m['result'] for s in ['kind=void','kind=builtin','empty=1'])
    out=dict(d)
    if len(set(actual))!=1:
        out.update(result='RESULT_TYPE_DIVERGENT',reason='两侧均不承载对象不等于类型相同；两库实际投影类型不同',identity_check=dict(actual=actual,equal=False))
        return out
    assert len(set(actual))==1,'NO_OBJECT requires identical actual types'
    out['identity_check']=dict(actual=actual,equal=True)
    return out
def main():
    gate('reclassify_gate');start=time.time()
    frozen={str(p.relative_to(ROOT)):sha(p) for p in OLD.rglob('*') if p.is_file() and '__pycache__' not in p.parts}
    save(OUT/'PROJECTION_FROZEN_SHA256.json',frozen)
    verified={};changes=[];summary=[];counts=Counter();oldcounts=Counter()
    for p in sorted((OLD/'results').glob('*.json.gz')):
        with gzip.open(p,'rt') as f:rows=json.load(f)
        local=Counter()
        for h in rows:
            for src in h['references']:
                path=src['cache_path']
                if path not in verified:verified[path]=sha(path)
                assert verified[path]==src['sha256'],path
            before=h['decision'];after=checked_no_object(before);oldcounts[before['result']]+=1;counts[after['result']]+=1;local[after['result']]+=1
            h['previous_decision']=before;h['decision']=after
            if before['result']!=after['result']:
                changes.append(dict(occurrence_id=h['id'],candidate=h['candidate'],source=h['references'][0]['installed_path'],line=h['line'],gnu=after['result_type']['gnu'],cxx=after['result_type']['cxx'],expression=after['requested_expression'],old=before['result'],new=after['result'],evidence='results/'+p.name))
        dest=OUT/'results'/p.name;dest.parent.mkdir(parents=True,exist_ok=True)
        with gzip.open(dest,'wt') as f:json.dump(rows,f,ensure_ascii=False)
        summary.append(dict(candidate=rows[0]['candidate'],file='results/'+p.name,positions=len(rows),no_object=local['RESULT_NO_OBJECT'],divergent=local['RESULT_TYPE_DIVERGENT'],object=local['RESULT_STDLIB_OBJECT'],unresolved=local['RESULT_UNRESOLVED']))
    assert len(changes)==10 and Counter(r['candidate'] for r in changes)=={'std::numeric_limits':1,'std::decay':9}
    assert all(r['old']=='RESULT_NO_OBJECT' and r['new']=='RESULT_TYPE_DIVERGENT' for r in changes)
    # 单元对照：空类型但类型不同必须拒绝；静态 const bool 必须仍通过。
    static=dict(result='RESULT_NO_OBJECT',basis='STATIC_DEFINITION_CHAIN',result_type='const bool；谓词值 NOT_OBSERVED')
    assert checked_no_object(static)['identity_check']['equal']
    tsv(OUT/'RECLASSIFIED_CHANGES.tsv',changes);tsv(OUT/'RECLASSIFIED_CANDIDATES.tsv',summary)
    save(OUT/'RECLASSIFIED_COUNTS.json',dict(before=dict(oldcounts),after=dict(counts),changed=len(changes),candidate_count=len(summary),verified_source_paths=len(verified),elapsed_seconds=time.time()-start))
    save(OUT/'VERIFIED_HEADERS.json',verified)
    for p,digest in frozen.items():assert sha(ROOT/p)==digest,p
    print(json.dumps(dict(counts=counts,changes=changes),ensure_ascii=False,indent=2))
if __name__=='__main__':main()
