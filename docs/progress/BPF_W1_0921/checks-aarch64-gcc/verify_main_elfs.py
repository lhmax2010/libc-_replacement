import json,re,shlex,subprocess,sys
from pathlib import Path
p=Path.cwd(); arch,mode=sys.argv[1:]; label=f'bpf-{arch}-{mode}'
root=p/'tmp/BPF_W1_0921'/f'payload-{label}'/'bpftrace'; rows=[]
out=p/'progress/BPF_W1_0921'/f'verify-{label}'
assert root.is_dir()
for f in sorted(root.rglob('*')):
    if not f.is_file() or f.is_symlink(): continue
    with f.open('rb') as s: magic=s.read(4)
    if magic!=b'\x7fELF': continue
    cmd=['readelf','-dW',str(f)]; r=subprocess.run(cmd,capture_output=True,text=True)
    needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',r.stdout)
    passed=r.returncode==0 and (('libstdc++.so.6' not in needed) if mode=='libcxx' else not any(x.startswith(('libc++.so','libc++abi.so')) for x in needed))
    rows.append(dict(file=str(f.relative_to(root)),command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,needed=needed,pass_runtime_dependency_check=passed))
result=dict(arch=arch,mode=mode,elf_count=len(rows),records=rows,status='PASS' if rows and all(x['pass_runtime_dependency_check'] for x in rows) else 'FAIL')
(out/'all-main-elfs.json').write_text(json.dumps(result,indent=2))
print(json.dumps(dict(arch=arch,mode=mode,elf_count=len(rows),status=result['status'])))
sys.exit(0 if result['status']=='PASS' else 1)
