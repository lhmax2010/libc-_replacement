import json
import shlex
import subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
base=json.loads((p/'private_root_command.json').read_text())
for flag in ['--uid','--gid']:
    base[base.index(flag)+1]='1000'
emul=Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/emul').resolve()
base+=['--ro-bind',str(emul),'/emul']
commands=[
    ['--','/emul/usr/bin/aarch64-tizen-linux-gnu-clang++','--version'],
    ['--','/emul/usr/bin/aarch64-tizen-linux-gnu-clang++','-###','-stdlib=libc++','-x','c++','-fsyntax-only','-'],
]
for i,suffix in enumerate(commands):
    command=base+suffix
    print(shlex.join(command),flush=True)
    r=subprocess.run(command,input='',capture_output=True,text=True,timeout=30)
    result=dict(command=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    (p/f'existing-acceleration-probe-{i}.json').write_text(json.dumps(result,indent=2))
    print(json.dumps(result,indent=2),flush=True)
    if r.returncode:
        raise SystemExit(r.returncode)
