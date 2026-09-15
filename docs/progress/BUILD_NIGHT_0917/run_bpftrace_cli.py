import json,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); arch=sys.argv[1]
cmd=json.loads((p/f'bpftrace-{arch}-commands.json').read_text())['build']; base=cmd[:cmd.index('--')]
f=f'/tmp/night/builds/bpftrace-{arch}-static/src/bpftrace'
rows=[]
for args in [['--version'],['--help'],['--info']]:
    command=base+['--',f]+args
    r=subprocess.run(command,capture_output=True,text=True,errors='replace',timeout=60)
    rows.append(dict(command=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    print(args,r.returncode,r.stdout[:100],r.stderr[:200])
(p/f'bpftrace-{arch}-cli.json').write_text(json.dumps(rows,indent=2))
assert rows[0]['exitcode']==0 and '0.24.2' in rows[0]['stdout']
assert rows[1]['exitcode']==0
