import hashlib,json,re,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); arch=sys.argv[1]
f=Path('tmp/NIGHT_0917/builds')/f'bpftrace-{arch}-static/src/bpftrace'
assert f.is_file()
commands=[]; texts={}
for key,cmd in [('dynamic',['readelf','-d',str(f)]),('symbols',['nm','-C',str(f)]),('strings',['strings',str(f)]),('file',['file',str(f)])]:
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace'); commands.append(dict(command=cmd,exitcode=r.returncode,stderr=r.stderr)); assert r.returncode==0,r.stderr
    texts[key]=r.stdout
    (p/f'bpftrace-{arch}-{key}.txt').write_text(r.stdout)
needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',texts['dynamic'])
row=dict(path=str(f),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest(),needed=needed,exact_std1_lines=sum('_ZNSt3__1' in s for s in texts['strings'].splitlines()),exact_cxx11_lines=sum('_ZNSt7__cxx11' in s for s in texts['strings'].splitlines()),broad_cxx11_lines=sum('__cxx11' in s for s in texts['symbols'].splitlines()))
row['abi_pass']=row['exact_std1_lines']>0 and row['exact_cxx11_lines']==0 and row['broad_cxx11_lines']==0
row['needed_pass']=set(needed)<=set(['libc.so.6','libm.so.6','libdl.so.2','librt.so.1','libpthread.so.0','ld-linux-x86-64.so.2','ld-linux-aarch64.so.1'])
row['needed_scope']='glibc family and libm only; raw names retained'
(p/f'bpftrace-{arch}-artifact.json').write_text(json.dumps(row,indent=2))
(p/f'bpftrace-{arch}-inspect.commands.json').write_text(json.dumps(commands,indent=2))
print(json.dumps(row,indent=2))
# Inspection returns nonzero if the actual artifact gate is not met.
raise SystemExit(0 if row['abi_pass'] and row['needed_pass'] else 1)
