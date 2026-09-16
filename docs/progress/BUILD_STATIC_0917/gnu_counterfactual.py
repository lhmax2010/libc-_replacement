import json
from probes import base,p,run,inside

original=json.loads((p/'gnu-static-compile.json').read_text())['command']
cmd=original.copy(); cmd.remove('-static-libgcc')
old=inside+'/gnu-static'; new=inside+'/gnu-static-std-shared-gcc'
cmd=[new if x==old else x.replace('-Wl,-Map,'+old+'.map','-Wl,-Map,'+new+'.map') for x in cmd]
r=run('gnu-counterfactual-compile',cmd)
assert r['exitcode']==0
dep=run('gnu-counterfactual-needed',base+['--','readelf','-d',new])
assert 'libgcc_s.so.1' in dep['stdout'] and 'libstdc++.so.6' not in dep['stdout']
matrix=[]
for wait in ['wait','system_clock','steady_clock','custom_clock']:
    for n in range(1,6):
        r=run(f'gnu-counterfactual-{wait}-{n}',base+['--',new,wait],20)
        ok=r['exitcode']==0 and 'event=cleanup' in r['stdout'] and 'cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in r['stdout'] and 'event=wait_confirmed' in r['stdout']
        matrix.append(dict(wait=wait,repetition=n,exitcode=r['exitcode'],assertions_pass=ok))
(p/'gnu-counterfactual-matrix.json').write_text(json.dumps(matrix,indent=2))
print('PASS',sum(x['assertions_pass'] for x in matrix),'/',len(matrix))
