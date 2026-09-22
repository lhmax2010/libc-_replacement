"""Existing cancellation probe, static libc++ and libbpf, shared libgcc; no source edit."""
import hashlib,json,re,shutil,subprocess,time
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_LIBBPF_0922'; T=P/'tmp/BPF_LIBBPF_0922'; O=E/'probe'; O.mkdir(exist_ok=False)
def sha(f):
    with f.open('rb') as s: return hashlib.file_digest(s,'sha256').hexdigest()
old=json.loads((P/'progress/BUILD_NIGHT_0917/bpftrace-x86_64-commands.json').read_text())['build']; base=old[:old.index('--')]
for i,x in enumerate(base):
    if x=='--bind': base[i]='--ro-bind'
base+=['--bind',str(T),'/tmp/lbpf0922']
def run(label,argv,timeout=90):
    start=time.time()
    try:
        r=subprocess.run(argv,capture_output=True,text=True,errors='replace',timeout=timeout); d=dict(command=argv,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    except subprocess.TimeoutExpired as x: d=dict(command=argv,exitcode='TIMEOUT',stdout=str(x.stdout),stderr=str(x.stderr))
    d.update(start_epoch=start,duration=time.time()-start); (O/(label+'.json')).write_text(json.dumps(d,indent=2)); print(label,d['exitcode'],flush=True); return d
source=P/'docs/progress/BUILD_STATIC_0916B/inputs/condition_cancel_probe.cpp'; shutil.copy2(source,T/source.name)
root=Path(base[2]); files=[source,root/'usr/lib64/libelf.a',P/'tmp/NIGHT_0917/deps/x86_64/usr/lib64/libbpf.a',P/'tmp/NIGHT_0917/deps/x86_64/usr/lib64/libz.a',P/'tmp/NIGHT_0917/builds/runtime-x86_64-libcxx/lib/libc++.a',P/'tmp/NIGHT_0917/builds/runtime-x86_64-libcxx/lib/libc++abi.a']
(O/'inputs.json').write_text(json.dumps([dict(path=str(f),sha256=sha(f),bytes=f.stat().st_size) for f in files],indent=2))
libs='/tmp/night/builds/runtime-x86_64-libcxx/lib'; dep='/tmp/night/deps/x86_64/usr/lib64'; exe='/tmp/lbpf0922/cancel-static-libbpf-shared-gcc'
common=['x86_64-tizen-linux-gnu-clang++','-std=c++20','-O0','-g','-fno-inline','/tmp/lbpf0922/condition_cancel_probe.cpp','-pthread','-latomic','-stdlib=libc++','-nostdinc++','-I/tmp/night/builds/runtime-x86_64-libcxx/include/c++/v1','-L'+libs,'-shared-libgcc','-static-libstdc++','-Wl,-Map,'+exe+'.map','-Wl,--start-group',libs+'/libc++.a',libs+'/libc++abi.a','-Wl,--whole-archive',dep+'/libbpf.a','-Wl,--no-whole-archive','/usr/lib64/libelf.a',dep+'/libz.a','-Wl,--end-group','-o',exe]
assert run('compiler',base+['--',common[0],'--version'])['exitcode']==0
assert run('driver',base+['--']+common+['-###'])['exitcode']==0
assert run('compile',base+['--']+common)['exitcode']==0
r=run('needed',base+['--','readelf','-d',exe]); needed=re.findall(r'Shared library: \[(.*?)\]',r['stdout'])
assert r['exitcode']==0 and 'libgcc_s.so.1' in needed and not any(x in needed for x in ('libbpf.so.1','libc++.so.1','libc++abi.so.1','libstdc++.so.6'))
r=run('defined',base+['--','nm','--defined-only',exe]); assert r['exitcode']==0
assert re.search(r'\b[Tt] bpf_object__open\b',r['stdout']), 'libbpf extraction not demonstrated'
matrix=[]
for wait in ('wait','system_clock','steady_clock','custom_clock'):
    for n in range(1,6):
        r=run(f'{wait}-{n}',base+['--',exe,wait],20)
        ok=r['exitcode']==0 and all(s in r['stdout'] for s in ('event=wait_confirmed','event=cleanup','cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1')) and 'event=terminate' not in r['stdout']
        matrix.append(dict(wait=wait,repeat=n,pass_assertions=ok,exitcode=r['exitcode']))
        (O/'matrix.json').write_text(json.dumps(matrix,indent=2))
mapfile=T/'cancel-static-libbpf-shared-gcc.map'; lines=mapfile.read_text(errors='replace').splitlines()
(O/'map-excerpts.txt').write_text('\n'.join(x for x in lines if any(t in x for t in ('libgcc_s','libgcc_eh','libunwind','libbpf.a','cxa_personality')))+'\n')
result=dict(status='PASS' if all(x['pass_assertions'] for x in matrix) else 'FAIL',passed=sum(x['pass_assertions'] for x in matrix),total=len(matrix),DT_NEEDED=needed,exe_sha256=sha(T/'cancel-static-libbpf-shared-gcc'),source_sha256=sha(source),scope='x86_64 existing probe, whole-archive libbpf retained, shared libgcc; not ARM board validation')
(O/'result.json').write_text(json.dumps(result,indent=2)); print(json.dumps(result),flush=True)
raise SystemExit(0 if result['status']=='PASS' else 1)
