#!/usr/bin/env python3
"""每次状态检查自行保存原命令、退出码与结果；中间统计不冒充最终 JSON。"""
import collections,csv,datetime,json,pathlib,re,subprocess,sys
o=pathlib.Path('docs/progress/IMPL_0909/W2')
if '--inner' not in sys.argv:
    stem=o/'raw/monitor'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%f')
    rc=subprocess.call([sys.executable,'docs/progress/R114/code/record.py',str(stem),sys.executable,__file__,'--inner'])
    print(pathlib.Path(str(stem)+'.stdout').read_text(),end='');raise SystemExit(rc)
def key(name):
    suite='libcxxabi' if 'libc++abi' in name.split(' :: ',1)[0] else 'libcxx'
    name=name.split(' :: ',1)[-1]
    for marker in ['/libcxx/test/','/libcxxabi/test/']:
        if marker in name:name=name.split(marker,1)[1]
    return suite,name
baseline={(x['suite'],key(x['test'])[1]):x['historical_code'] for x in csv.DictReader(open('docs/progress/IMPL_0908/W3/historical_baseline.tsv'),delimiter='\t') if x['arch']=='armv7l'}
p=o/'raw/lit_resume3';pattern=r'^(PASS|FAIL|XFAIL|XPASS|UNSUPPORTED|TIMEOUT|UNRESOLVED): (.+) \((\d+) of (\d+)\)$'
matches=re.findall(pattern,(p/'stdout').read_text(errors='replace'),re.M)
control_key=next(k for k,v in baseline.items() if v=='PASS')
control_name=('llvm-libc++abi-shared.cfg.in' if control_key[0]=='libcxxabi' else 'llvm-libc++-shared.cfg.in')+' :: '+control_key[1]
control=re.findall(pattern,'FAIL: '+control_name+' (1 of 1)',re.M)
assert len(control)==1 and baseline[key(control[0][1])]!=control[0][0]
counts=collections.Counter(x[0] for x in matches)
candidates=[[key(name),baseline.get(key(name),'NOT_AVAILABLE'),code] for code,name,_,_ in matches if code in ['FAIL','XPASS','TIMEOUT','UNRESOLVED'] and baseline.get(key(name))!=code]
print(json.dumps({'time':datetime.datetime.now(datetime.timezone.utc).isoformat(),'run':'lit_resume3','records':len(matches),'last_position':matches[-1][2] if matches else 0,'counts':dict(counts),'new_failure_candidates':candidates,'synthetic_new_failure_positive_control':'PASS (not a test result)','json_ready':(p/'result.json').exists(),'connection_failure':(p/'executor/CONNECTION_FAILURE').exists()},ensure_ascii=False))
