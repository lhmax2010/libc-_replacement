"""Fetch immutable release-source evidence; no package repository changes."""
import hashlib
import json
import shlex
import subprocess
from pathlib import Path
p=Path('progress/BUILD_W2C_0915/upstream_release')
sha=json.loads((p/'tag_v0.24.2.json').read_text())['object']['sha']
assert sha=='0e0c5cfcc7b1093b000cb516a121f21cd37a7143'
j=(p/'ci_commands.jsonl').open('w',buffering=1)
files=['.github/workflows/release.yml','.github/workflows/binary.yml',
       '.github/workflows/static.yml','.github/include/static.sh',
       'docker/Dockerfile.static','flake.nix','flake.lock','src/CMakeLists.txt']
records=[]
for name in files:
    output=p/'upstream_source'/name
    output.parent.mkdir(parents=True,exist_ok=True)
    url=f'https://raw.githubusercontent.com/bpftrace/bpftrace/{sha}/{name}'
    cmd=['curl','-q','--fail','--location','--silent','--show-error','--max-time','60',
         '--output',str(output),url]
    r=subprocess.run(cmd,capture_output=True,text=True)
    j.write(json.dumps(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n')
    r.check_returncode()
    records.append(dict(path=name,url=url,sha256=hashlib.sha256(output.read_bytes()).hexdigest()))
(p/'ci_sources.json').write_text(json.dumps(records,indent=2)+'\n')
print(json.dumps(records,indent=2))
