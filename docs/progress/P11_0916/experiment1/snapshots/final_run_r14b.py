#!/usr/bin/env python3
"""在原二进制上逐格复跑，沿用 R14b 数据和清理断言，不把 A/B 称作 ABI 设置对照。"""
import csv, json, pathlib, subprocess, sys
ROOT = pathlib.Path(__file__).resolve().parents[4]
OUT = pathlib.Path(__file__).resolve().parent
OLD = pathlib.Path('/home/toolchain/development/libc++_replacement')
REC = ROOT/'docs/progress/API_0911/record.py'
rows = list(csv.DictReader((OLD/'progress/R14b/L2/tables/matrix_all.tsv').open(),delimiter='\t'))
results=[]
for row in sorted(rows,key=lambda x:0 if x['stage']=='same' else 1):
    runtime=OLD/'tmp/R14b/runtime'/('L2'+row['scheme'])
    exe=OLD/'tmp/R14b/L2/bin'/('main_dlopen' if row['mode']=='dlopen' else 'main_dt_'+row['scheme']+'_'+row['order'])
    env=['env','LD_LIBRARY_PATH='+str(runtime),'LD_DEBUG=bindings,versions']
    if row['binding']!='lazy':env+=['LD_BIND_NOW=1']
    args=[str(exe),row['scheme'],row['mode'],row['order'],row['scope'],row['direction']]
    for repeat in range(1,6):
        stem=OUT/'raw'/('r14b_'+row['case_id']+'_r'+str(repeat))
        command=['nice','-n','15','ionice','-c','3','timeout','15',*env,*args]
        subprocess.run([sys.executable,str(REC),str(stem),*command],check=False)
        rc=int(pathlib.Path(str(stem)+'.exitcode').read_text())
        stdout=pathlib.Path(str(stem)+'.stdout').read_text()
        results.append({'case':row['case_id'],'repeat':repeat,'architecture':'x86_64 native',
            'scheme':row['scheme'],'direction':row['direction'],'scope':row['scope'],'mode':row['mode'],
            'order':row['order'],'binding':row['binding'],'exit_code':rc,
            'historical_return':int(row['actual_return']),
            'stdout':stdout,'record':str(stem.relative_to(OUT))})
        if row['stage']=='same':
            assert rc==0 and 'FAIL' not in stdout and 'PASS' in stdout,(row['case_id'],stdout)
    (OUT/'R14B_REPEATS.json').write_text(json.dumps(results,indent=2,ensure_ascii=False)+'\n')
print('COMPLETE',len(rows),'cells',len(results),'runs')
