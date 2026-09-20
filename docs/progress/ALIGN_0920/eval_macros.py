"""Present-day read-only query with historical argv's macros/env. Never builds."""
import json,pathlib,subprocess,sys
which=sys.argv[1];assert which in ('old','new')
p=pathlib.Path('progress/ALIGN_0920/comparison')
cmd=json.loads((p/('argv-'+which+'.json')).read_text());bwrap=cmd[cmd.index('bwrap'):cmd.index('--')+1]
bwrap=['--ro-bind' if x=='--bind' else x for x in bwrap]
start=cmd.index('/usr/bin/rpmbuild')+1 if '/usr/bin/rpmbuild' in cmd else cmd.index('rpmbuild')+1
args=cmd[start:];query=[];i=0
while i<len(args):
    if args[i] in ('--define','--rcfile','--macros','--target'):query.extend(args[i:i+2]);i+=2
    else:i+=1
assert '-bb' not in query
script='printf "QEMU_RESERVED_VA=%s\\n" "$QEMU_RESERVED_VA"; exec /usr/bin/rpm "$@"'
actual=bwrap+['/bin/sh','-c',script,'rpm-read-only-query',*query,'--eval','_binary_payload=%{_binary_payload}\n_target=%{_target}\n_build_create_debug=%{_build_create_debug}']
print('ACTUAL_ARGV='+json.dumps(actual),flush=True)
r=subprocess.run(actual,capture_output=True,text=True)
record={'scope':'Present-day query replay; NOT a historical /proc snapshot','actual_command':actual,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr}
(p/(which+'-actual-eval.json')).write_text(json.dumps(record,indent=2)+'\n')
print(r.stdout);print(r.stderr);sys.exit(r.returncode)
