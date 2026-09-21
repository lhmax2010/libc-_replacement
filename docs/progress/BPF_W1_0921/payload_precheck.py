"""Evaluate payload in exactly the cell's isolated environment and macro context."""
import json,subprocess,shlex,sys
from pathlib import Path
out=Path('progress/BPF_W1_0921'); arch=sys.argv[1]; records=[]
for mode in ('libcxx','gcc','undefined'):
    cmd=json.loads((out/'cell_commands'/f'bpf-{arch}-{mode}.json').read_text())
    i=cmd.index('rpmbuild'); prefix=cmd[:i]; args=cmd[i+1:]
    args=args[:args.index('--noclean')]
    actual=prefix+['rpm',*args,'--eval','%{_binary_payload}']
    r=subprocess.run(actual,capture_output=True,text=True)
    records.append(dict(mode=mode,command=shlex.join(actual),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (out/f'payload-precheck-{arch}.json').write_text(json.dumps(records,indent=2))
    print(mode,r.returncode,repr(r.stdout),flush=True)
    assert r.returncode==0
    value=r.stdout.strip()
    assert value=='w5.xzdio' or (arch=='aarch64' and 'zstd' in value),value
