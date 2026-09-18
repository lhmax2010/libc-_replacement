"""Supplementary platform checks with an existing, payload-bearing BUILDROOT."""
import hashlib,json,shlex,subprocess,sys
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; arch,mode=sys.argv[1:]
assert arch in ('aarch64','armv7l') and mode in ('libcxx','gcc','undefined')
label=f'bpf-{arch}-{mode}'
if mode=='undefined': command_file=out/'cell_commands'/f'{label}-harness.json'
elif arch=='aarch64' and mode=='libcxx': command_file=out/'cell_commands/bpf-aarch64-libcxx-checkers.json'
elif mode=='gcc': command_file=out/'cell_commands'/f'{label}-linkerfixed.json'
else: command_file=out/'cell_commands'/f'{label}.json'
cmd=json.loads(command_file.read_text()); base=cmd[:cmd.index('--')]
entry=cmd[-1]; assert entry.startswith('/tmp/task/specs/')
entry_local=p/'tmp/WEEKEND_0918/specs'/Path(entry).name
roots=[x for x in (p/f'tmp/WEEKEND_0918/rpm-{label}/BUILDROOT').iterdir()
       if x.is_dir() and (x/'usr/bin/bpftrace').is_file()]
assert len(roots)==1,('Expected exactly one payload-bearing build root',roots)
root=roots[0]
directory=p/f'tmp/WEEKEND_0918/hal-spec-v2-{arch}-{mode}'; directory.mkdir(exist_ok=False)
spec=directory/'bpftrace.spec'; spec.write_bytes(entry_local.read_bytes())
base+=['--setenv','HOME','/home/abuild']
commands=[]
for checker in ('/etc/hal/rootstrap/hal-rootstrap-checker.sh','/etc/app-rootstrap/app-rootstrap-checker.sh'):
    args=base+['--',checker,f'/tmp/task/{directory.name}',
               '/tmp/task/'+str(root.relative_to(p/'tmp/WEEKEND_0918')),arch]
    r=subprocess.run(args,capture_output=True,text=True)
    commands.append({'command':shlex.join(args),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    print(r.stdout,r.stderr,flush=True)
    (out/f'hal-candidate-v2-{arch}-{mode}.json').write_text(json.dumps({'commands':commands,
        'entry_spec':str(entry_local),'entry_spec_sha256':hashlib.sha256(spec.read_bytes()).hexdigest(),
        'checked_buildroot':str(root),'main_payload_sha256':hashlib.sha256((root/'usr/bin/bpftrace').read_bytes()).hexdigest(),
        'scope':'Supplementary unchanged platform checkers against an existing payload and the actual entry spec. Checker macro evaluation remains separate from the full build-command macro audit.'},indent=2))
raise SystemExit(0 if all(x['exitcode']==0 for x in commands) else 1)
