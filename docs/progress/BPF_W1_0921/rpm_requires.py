"""Export actual RPM Requires for the W3 handover, without installing anything."""
import csv,json,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; rows=[]; commands=[]
for arch in ('armv7l','aarch64'):
    for mode in ('libcxx','gcc','undefined'):
        f=out/f'verify-bpf-{arch}-{mode}/result.json'
        if not f.exists(): continue
        data=json.loads(f.read_text())
        for rpm in data['rpms']:
            cmd=['rpm','-qp','--requires',rpm['path']]
            r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
            commands.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
            (out/'RPM_REQUIRES_COMMANDS.json').write_text(json.dumps(commands,indent=2))
            assert r.returncode==0
            for line in r.stdout.splitlines():
                rows.append(dict(arch=arch,path=mode,rpm=rpm['path'],rpm_sha256=rpm['sha256'],requires=line))
with (out/'RPM_REQUIRES.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=['arch','path','rpm','rpm_sha256','requires'],delimiter='\t'); w.writeheader(); w.writerows(rows)
print(len(commands),'RPM queried;',len(rows),'declared Requires rows; no installation')
