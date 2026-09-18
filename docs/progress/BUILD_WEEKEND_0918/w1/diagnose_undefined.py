"""Inspect actual RPM option/macro processing; do not modify spec or roots."""
import json,re,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918'); dest=out/'undefined-diagnostic'; dest.mkdir(exist_ok=False)
cmd=json.loads((out/'cell_commands/bpf-armv7l-undefined-linkerfixed.json').read_text())
i=cmd.index('--'); base=cmd[:i]; args=cmd[i+2:]
args=[x for x in args if x not in ('--noclean','-bb')]; spec=args.pop()
records=[]
def run(name,extra):
    command=base+['--','rpmspec',*args,*extra]
    r=subprocess.run(command,capture_output=True,text=True)
    (dest/(name+'.stdout')).write_text(r.stdout); (dest/(name+'.stderr')).write_text(r.stderr)
    records.append({'command':shlex.join(command),'exitcode':r.returncode,'label':name})
    (dest/'commands.json').write_text(json.dumps(records,indent=2))
    print(name,'exitcode',r.returncode,flush=True)
    print('\n'.join(x for x in r.stdout.splitlines() if x.startswith(('BuildRequires:','MACRO_STATE='))),flush=True)
run('expanded',['-P',spec])
value='MACRO_STATE=%{defined _toolchain}|%{?_toolchain}|%{?build_with_libcxx}|%{__cc}|%{__cxx}'
run('evaluate',['--eval',value])
run('late_undef',['--eval','%{!?_target_cpu:0}', '--undefine','_toolchain','--eval',value])
run('lua_undef',['--eval',"%{lua:rpm.undefine('_toolchain')}",'--eval',value,'-P',spec])
