import hashlib,json,shlex,subprocess,sys
from pathlib import Path
arch,mode=sys.argv[1:]; p=Path('progress/BPF_W1_0921'); dest=p/f'checks-{arch}-{mode}'; dest.mkdir(exist_ok=False)
records=[]
for name,args in [('verify.py',[arch,mode]),('audit_bpf_cell.py',[arch,mode,f'bpf-{arch}-{mode}']),('verify_main_elfs.py',[arch,mode])]:
    f=p/name; snapshot=dest/name; snapshot.write_bytes(f.read_bytes())
    cmd=['/usr/bin/python3',str(f),*args]; print('COMMAND',shlex.join(cmd),flush=True)
    r=subprocess.run(cmd)
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,script_sha256=hashlib.sha256(snapshot.read_bytes()).hexdigest()))
    (dest/'commands.json').write_text(json.dumps(records,indent=2))
    print('EXITCODE',r.returncode,flush=True)
sys.exit(1 if any(r['exitcode'] for r in records) else 0)
