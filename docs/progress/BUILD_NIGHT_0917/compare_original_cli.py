import difflib,hashlib,json,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); rows=[]
c=json.loads((p/'bpftrace-aarch64-commands.json').read_text())['build']; base=c[:c.index('--')]
f=Path('tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace')
for arg in ['--version','--help','--info']:
    cmd=base+['--','/tmp/night/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace',arg]
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=60)
    rows.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
new=json.loads((p/'bpftrace-aarch64-cli.json').read_text()); comp=[]
for old,n in zip(rows,new):
    arg=old['command'][-1]
    delta=list(difflib.unified_diff(old['stdout'].splitlines(),n['stdout'].splitlines(),fromfile='original',tofile='candidate',lineterm=''))
    comp.append(dict(argument=arg,original_exitcode=old['exitcode'],candidate_exitcode=n['exitcode'],stdout_equal=not delta,difference=delta))
(p/'original_aarch64_cli.json').write_text(json.dumps(rows,indent=2))
(p/'original_cli_comparison.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(f.read_bytes()).hexdigest(),comparisons=comp,functional_equivalence='NOT_OBSERVED: --info requires root; no probe execution'),indent=2))
print(json.dumps(comp,indent=2))
