"""Run the unmodified platform HAL checker against the actual candidate spec."""
import hashlib,json,shlex,subprocess,sys
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; arch,mode=sys.argv[1:]
assert arch in ('aarch64','armv7l') and mode in ('libcxx','gcc','undefined')
label=f'bpf-{arch}-{mode}'; commands=[]
if arch=='aarch64' and mode=='libcxx': command_file=out/'cell_commands/bpf-aarch64-libcxx-checkers.json'
else: command_file=out/'cell_commands'/f'{label}.json'
cmd=json.loads(command_file.read_text()); base=cmd[:cmd.index('--')]
directory=p/f'tmp/WEEKEND_0918/hal-spec-{arch}-{mode}'; directory.mkdir(exist_ok=False)
spec=directory/'bpftrace.spec'; spec.write_bytes((p/'tmp/WEEKEND_0918/specs/bpftrace.spec').read_bytes())
base+=['--setenv','HOME','/home/abuild']
for checker in ('/etc/hal/rootstrap/hal-rootstrap-checker.sh','/etc/app-rootstrap/app-rootstrap-checker.sh'):
    args=base+['--',checker,f'/tmp/task/{directory.name}',
               f'/tmp/task/rpm-{label}/BUILDROOT/bpftrace-0.24.2-1.{arch}',arch]
    r=subprocess.run(args,capture_output=True,text=True)
    commands.append({'command':shlex.join(args),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    print(r.stdout,r.stderr)
result={'commands':commands,
        'spec_sha256':hashlib.sha256(spec.read_bytes()).hexdigest(),
        'scope':'Supplementary platform checker against actual candidate. Checker uses its own RPM macro evaluation; not proof of every externally overridden macro configuration.'}
(out/f'hal-candidate-{arch}-{mode}.json').write_text(json.dumps(result,indent=2))
raise SystemExit(0 if all(x['exitcode']==0 for x in commands) else 1)
