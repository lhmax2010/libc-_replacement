import json
from probes import run,p
record=json.loads((p/'gnu-gdb-abort.json').read_text())
for mode in ['system_clock','steady_clock','custom_clock']:
    cmd=record['command'].copy(); assert cmd[-1]=='wait'; cmd[-1]=mode
    r=run('gnu-gdb-abort-'+mode,cmd,120)
    assert 'SIGABRT' in r['stdout'] and '_Unwind_SetGR.cold' in r['stdout'] and '_Unwind_ForcedUnwind' in r['stdout']
