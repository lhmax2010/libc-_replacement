"""Collect all payload observations before evaluating gates; keep stripped/unstripped evidence distinct."""
import hashlib,json,re,shlex,struct,subprocess,sys,tarfile
from pathlib import Path
p=Path.cwd(); arch,mode=sys.argv[1:]; label=f'bpf-{arch}-{mode}'
out=p/'progress/BPF_W1_0921'/f'verify-{label}'; out.mkdir(exist_ok=False)
task=p/'tmp/BPF_W1_0921'; top=task/f'rpm-{label}'; payload=task/f'payload-{label}'; payload.mkdir()
records=[]; result=dict(arch=arch,mode=mode,rpms=[],checks={},observations={})
def sha(f):
    h=hashlib.sha256()
    with f.open('rb') as s:
        for b in iter(lambda:s.read(1048576),b''): h.update(b)
    return h.hexdigest()
def save(): (out/'result.json').write_text(json.dumps(result,indent=2))
def run(cmd,cwd=None):
    r=subprocess.run(cmd,cwd=cwd,capture_output=True,text=True,errors='replace')
    records.append(dict(command=shlex.join(cmd),cwd=str(cwd or p),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (out/'commands.json').write_text(json.dumps(records,indent=2))
    assert r.returncode==0,(cmd,r.stderr)
    return r.stdout
for f in sorted((top/'RPMS').rglob('*.rpm')):
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
    name=identity.split('\t')[0]; run(['rpm','-K','--nosignature',str(f)])
    compression=run(['rpm','-qp','--qf','%{PAYLOADCOMPRESSOR}|%{PAYLOADFLAGS}',str(f)])
    files=run(['rpm','-qpl',str(f)]); assert all('..' not in Path(x).parts for x in files.splitlines())
    (out/f'{name}.files.txt').write_text(files)
    (out/f'{name}.scripts.txt').write_text(run(['rpm','-qp','--scripts',str(f)]))
    result['rpms'].append(dict(path=str(f),identity=identity,sha256=sha(f),bytes=f.stat().st_size,payload=compression,file_count=len(files.splitlines())))
    d=payload/name; d.mkdir()
    run(['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -id --quiet --no-absolute-filenames','extract',str(f)],d)
    save()
checks=result['checks']; obs=result['observations']
cell_argv=json.loads((p/'progress/BPF_W1_0921/cell_commands'/f'{label}.json').read_text())
sources=Path(cell_argv[cell_argv.index('/tmp/bpf-sources')-1])
source1002=sources/'prebuilt-static-bpftrace.tar.gz'
source_arch='arm' if arch=='armv7l' else arch
with tarfile.open(source1002) as t:
    members=[m for m in t.getmembers() if m.isfile() and m.name.lstrip('./').startswith(f'prebuilt-static-bpftrace/{source_arch}/')]
    obs['source1002']=dict(path=str(source1002),sha256=sha(source1002),bytes=source1002.stat().st_size,arch_regular_files=[dict(path=m.name,bytes=m.size) for m in members])
    bm=next(m for m in members if m.name.endswith('/usr/bin/bpftrace'))
    stream=t.extractfile(bm); h=hashlib.sha256()
    for b in iter(lambda:stream.read(1048576),b''): h.update(b)
    original_in_tar_sha=h.hexdigest()
obs['static_rpm_regular_files']=[dict(path=str(f.relative_to(payload/'bpftrace-static')),bytes=f.stat().st_size) for f in sorted((payload/'bpftrace-static').rglob('*')) if f.is_file() and not f.is_symlink()]
checks['three_packages']={x['identity'].split('\t')[0] for x in result['rpms']}=={'bpftrace','bpftrace-common','bpftrace-static'}
checks['payload_no_T']=all(x['payload']=='xz|5' for x in result['rpms'])
def elf(f):
    dynamic=run(['readelf','-dW',str(f)]); header=run(['readelf','-hW',str(f)])
    return dict(path=str(f),sha256=sha(f),bytes=f.stat().st_size,needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',dynamic),arch_correct=('AArch64' if arch=='aarch64' else 'ARM') in header)
original=p/'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace'/('arm' if arch=='armv7l' else arch)/'usr/bin/bpftrace'
main=payload/'bpftrace/usr/bin/bpftrace'; binary=payload/'bpftrace-static/usr/bin/bpftrace-static'
obs['main']=elf(main); obs['static']=elf(binary); obs['original']=elf(original)
checks['original_matches_this_cell_source1002']=original_in_tar_sha==obs['original']['sha256']
checks['elf_arch']=all(obs[k]['arch_correct'] for k in ('main','static','original'))
def markers(f,tag):
    strings=task/f'{label}-{tag}.strings.txt'; command=['strings','-a',str(f)]
    with strings.open('wb') as s: r=subprocess.run(command,stdout=s,stderr=subprocess.PIPE)
    records.append(dict(command=shlex.join(command),exitcode=r.returncode,stderr=r.stderr.decode(errors='replace'),stdout_path=str(strings),stdout_sha256=sha(strings)))
    (out/'commands.json').write_text(json.dumps(records,indent=2)); assert r.returncode==0
    counts=dict(std1=0,cxx11=0,cxx11_broad=0); excerpt=[]
    for line in strings.open(errors='replace'):
        counts['std1']+='_ZNSt3__1' in line; counts['cxx11']+='_ZNSt7__cxx11' in line
        counts['cxx11_broad']+='St7__cxx11' in line or 'std::__cxx11' in line
        if '_ZNSt3__1' in line and len(excerpt)<10: excerpt.append(line)
    (out/f'{tag}-namespace-excerpt.txt').write_text(''.join(excerpt))
    return counts
obs['static_markers']=markers(binary,'rpm'); obs['original_markers']=markers(original,'original')
def sections(path):
    with path.open('rb') as f:
        h=f.read(64); wide=h[4]==2; e='<' if h[5]==1 else '>'
        off=struct.unpack_from(e+('Q' if wide else 'I'),h,40 if wide else 32)[0]
        sz,n,ns=struct.unpack_from(e+'HHH',h,58 if wide else 46)
        f.seek(off); t=f.read(sz*n); ss=[struct.unpack_from(e+('IIQQQQIIQQ' if wide else 'IIIIIIIIII'),t,i*sz) for i in range(n)]
        f.seek(ss[ns][4]); names=f.read(ss[ns][5]); output={}
        for s in ss:
            name=names[s[0]:].split(b'\0',1)[0].decode()
            if name in ('.text','.rodata'):
                f.seek(s[4]); data=f.read(s[5]); assert len(data)==s[5]
                output[name]=dict(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())
        assert len(output)==2; return output
if mode=='libcxx':
    needed=obs['static']['needed']
    checks['libgcc_dynamic']='libgcc_s.so.1' in needed
    checks['no_dynamic_cxx_llvm']=not any(x.startswith(('libc++.so','libc++abi.so','libLLVM','libstdc++')) for x in needed)
    checks['main_libcxx']='libc++.so.1' in obs['main']['needed'] and 'libstdc++.so.6' not in obs['main']['needed']
    allowed={'libgcc_s.so.1','libc.so.6','libm.so.6','libdl.so.2','librt.so.1','libpthread.so.0','ld-linux-aarch64.so.1','ld-linux.so.3','ld-linux-armhf.so.3'}
    obs['extra_dynamic_dependencies']=sorted(set(needed)-allowed)
    checks['static_goal_no_extra_dynamic_deps']=not obs['extra_dynamic_dependencies']
    raw=top/'BUILD/bpftrace-0.24.2/build-static/src/bpftrace'
    obs['unstripped_markers']=markers(raw,'unstripped')
    obs['unstripped_sections']=sections(raw); obs['rpm_sections']=sections(binary)
    checks['code_sections_match']=obs['unstripped_sections']==obs['rpm_sections']
    checks['unstripped_libcxx_markers']=obs['unstripped_markers']['std1']>0 and obs['unstripped_markers']['cxx11_broad']==0
    checks['rpm_literal_std1_marker']=obs['static_markers']['std1']>0
    checks['rpm_cxx11_absent']=obs['static_markers']['cxx11_broad']==0
else:
    checks['source1002_byte_identical']=binary.read_bytes()==original.read_bytes()
    checks['main_gcc_runtime']='libstdc++.so.6' in obs['main']['needed'] and not any(x.startswith(('libc++.so','libc++abi.so')) for x in obs['main']['needed'])
result['status']='PASS' if all(checks.values()) else 'RPM_COMPLETE_GATES_NOT_ALL_CLOSED'
result['board']='NOT_OBSERVED (out of scope)'; save(); print(json.dumps(result,indent=2))
sys.exit(0 if all(checks.values()) else 1)
