#!/usr/bin/env python3
import json, pathlib, subprocess, sys
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
result=[]
for scheme in ['A','B']:
    for order in ['legacy_new','new_legacy']:
        for direction in ['new_new','legacy_new']:
            for repeat in range(1,6):
                stem='r14b_trace_'+scheme+'_'+order+'_'+direction+'_r'+str(repeat)
                runtime=OLD/'tmp/R14b/runtime'/('L2'+scheme)
                args=['nice','-n','15','ionice','-c','3','timeout','40','gdb','-q','-batch',
                      '-ex','set environment LD_LIBRARY_PATH '+str(runtime),'-x',str(OUT/'trace.gdb'),
                      '--args',str(OLD/'tmp/R14b/L2/bin/main_dlopen'),scheme,'dlopen',order,'global',direction]
                rc=subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*args]).returncode
                text=(OUT/'raw'/(stem+'.stdout')).read_text()
                trace=[json.loads(s[10:]) for s in text.splitlines() if s.startswith('P11_TRACE ')]
                exitline=[s for s in text.splitlines() if s.startswith('P11_INFERIOR_EXIT')]
                result.append({'scheme':scheme,'order':order,'direction':direction,'repeat':repeat,
                               'trace':trace,'inferior_exit':exitline,'raw':stem})
                (OUT/'R14B_TRACES.json').write_text(json.dumps(result,indent=2)+'\n')
                assert rc==0 and trace and exitline,(stem,rc)
                if direction=='new_new':assert 'P11_INFERIOR_EXIT 0' in text and 'type=PASS' in text
print('完成 8 格 × 5 次 =',len(result),'次实际落点记录')
