#!/usr/bin/env python3
"""保留旧有效记录并追加续测，直接逐项对照 R81；基础设施错误不伪装回归。"""
import collections,csv,hashlib,json,pathlib
r=pathlib.Path.cwd();out=r/'docs/progress/IMPL_0909/W2';old=r/'docs/progress/IMPL_0908/W3'
def key(name):
    suite='libcxxabi' if 'libc++abi' in name.split(' :: ',1)[0] else 'libcxx'
    test=name.split(' :: ',1)[-1]
    for root in ['/libcxx/test/','/libcxxabi/test/']:
        if root in test:test=test.split(root,1)[1]
    return suite,test
baseline={};sources=[]
for suite,file in [('libcxx','aggregate_result_owner_normalized.json'),('libcxxabi','batch-001/result.json')]:
    p=r/'docs/progress/R81/raw/phase2/results/baseline_armv7l'/suite/file
    data=json.loads(p.read_text());sources.append([str(p),hashlib.sha256(p.read_bytes()).hexdigest()])
    for e in data['tests']:baseline[(suite,key(e['name'])[1])]=e['code']
assert len(baseline)==11402
old_data=json.loads((old/'ARM_COMBINED_RESULT.json').read_text())
selected={key(e['name']):(e,str(old/'ARM_COMBINED_RESULT.json')) for e in old_data['tests']}
assert len(selected)==7848
infra=[];per_run=[]
for label in ['lit_resume','lit_resume2','lit_resume3']:
    p=out/'raw'/label/'result.json';assert (p.parent/'exitcode').exists(),('STILL_RUNNING',label)
    if not p.exists():per_run.append([label,0,0,'RESULT_NOT_AVAILABLE']);continue
    data=json.loads(p.read_text());valid=bad=0
    for e in data['tests']:
        output=e.get('output','')
        if any(x in output for x in ['CONNECTION_FAILURE already recorded','EXECUTOR_ERROR:','RuntimeError: remote exit marker absent','CLEANUP_ERROR']):
            infra.append([*key(e['name']),label,e['code'],'INFRASTRUCTURE_INCOMPLETE']);bad+=1;continue
        k=key(e['name']);assert k not in selected,('VALID_TEST_RERUN',k)
        selected[k]=(e,str(p));valid+=1
    per_run.append([label,valid,bad,'RECORDED'])
counts=collections.defaultdict(collections.Counter);rows=[];changes=[];detail=out/'failure_details';detail.mkdir(exist_ok=True)
for k,(e,source) in sorted(selected.items()):
    previous=baseline[k];current=e['code'];counts[k[0]][current]+=1
    if current==previous:kind='SAME_AS_R81'
    elif current in ('FAIL','TIMEOUT','UNRESOLVED','XPASS'):kind='NEW_FAILURE_REQUIRES_ANALYSIS'
    else:kind='CHANGED_STATUS'
    evidence=''
    if kind!='SAME_AS_R81':
        p=detail/(hashlib.sha256(('/'.join(k)).encode()).hexdigest()[:16]+'.txt')
        p.write_text('TEST='+ '/'.join(k)+'\nR81='+previous+'\nCURRENT='+current+'\nSOURCE='+source+'\n'+e.get('output',''))
        evidence=str(p.relative_to(r))
        changes.append([*k,previous,current,kind,evidence])
    rows.append([*k,previous,current,kind,source,evidence])
def table(name,header,values):
    with (out/name).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(header);w.writerows(values)
table('ARM_R81_COMPARISON.tsv',['suite','test','r81_code','current_code','comparison','source','detail'],rows)
table('ARM_CHANGED_STATUS.tsv',['suite','test','r81_code','current_code','comparison','detail'],changes)
table('ARM_REMAINING.tsv',['suite','test','state'],[[*k,'NOT_OBSERVED'] for k in sorted(set(baseline)-set(selected))])
table('INFRASTRUCTURE_RECORDS.tsv',['suite','test','run','raw_code','classification'],infra)
table('R81_SOURCE_IDENTITIES.tsv',['source','sha256'],sources)
summary={'kind':'DERIVED_UNION_NOT_SINGLE_RUN','arch':'armv7l','environment':'PHYSICAL_BOARD','original_preserved':7848,'valid_total':len(selected),'new_valid':len(selected)-7848,'missing':11402-len(selected),'counts':dict(counts),'per_run':per_run,'new_failure_candidates':[x for x in changes if x[4]=='NEW_FAILURE_REQUIRES_ANALYSIS'],'changed_status_count':len(changes),'baseline_total':11402}
(out/'ARM_SUMMARY.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
print('BASELINE_KEYS_POSITIVE_CONTROL=11402; no old valid test rerun; raw FAIL records untouched')
