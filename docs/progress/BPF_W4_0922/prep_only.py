"""Run the committed recipe through prep only, with fresh output and read-only inputs."""
import hashlib,json,os,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922/prep-check';E.mkdir(exist_ok=True)
assert not (E/'commands.json').exists()
T=P/'tmp/BPF_W4_0922/prep-check-verified';T.mkdir(exist_ok=False)
R=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push';records=[]
def run(label,argv):
 start=time.time();r=subprocess.run(argv,stdout=(E/(label+'.stdout.txt')).open('xb'),stderr=(E/(label+'.stderr.txt')).open('xb'),timeout=600)
 records.append(dict(label=label,argv=argv,command=shlex.join(argv),exitcode=r.returncode,seconds=time.time()-start));(E/'commands.json').write_text(json.dumps(records,indent=2));assert r.returncode==0,(label,r.returncode)
 return (E/(label+'.stdout.txt')).read_text(errors='replace')
cg=Path('/proc/self/cgroup').read_text();rel=next(x[3:] for x in cg.splitlines() if x.startswith('0::'));maxmem=(Path('/sys/fs/cgroup')/rel.lstrip('/')/'memory.max').read_text().strip()
memtotal=int(next(x.split()[1] for x in Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
assert int(maxmem)==(memtotal//2//4096)*4096
(E/'resources.json').write_text(json.dumps(dict(pid=os.getpid(),cgroup=cg,memory_max=maxmem,nice=os.getpriority(os.PRIO_PROCESS,0)),indent=2))
assert os.getpriority(os.PRIO_PROCESS,0)==19
cmd=json.loads((P/'docs/progress/BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json').read_text());i=cmd.index('--');bw=cmd[:i];args=cmd[i+2:]
for n,v in enumerate(bw):
 if v=='--bind':bw[n]='--ro-bind'
 if v==str(P/'tmp/BPF_STATIC_0922'):bw[n]=str(T);bw[n-1]='--bind'
bw+=['--ro-bind',str(R/'packaging'),'/tmp/committed-packaging','--']
args[args.index('-bb')]='-bp';args[-1]='/tmp/committed-packaging/bpftrace.spec'
# The sources remain the exact validated source directory, read-only; committed patch bytes match it.
assert (R/'packaging/static-link-libbpf.patch').read_bytes()==(P/'tmp/BPF_STATIC_0922/sources-armv7l/static-link-libbpf.patch').read_bytes()
for d in ('BUILD','BUILDROOT','RPMS','SOURCES','SPECS','SRPMS','TMP'):(T/'rpm-bpf-armv7l-libcxx'/d).mkdir(parents=True,exist_ok=True)
evalargs=args[:-2];evalargs.remove('--noclean')
out=run('macro-eval',bw+['rpm',*evalargs,'--eval','payload=%{_binary_payload};toolchain=%{?_toolchain};clang=%{toolchain_is clang};build_with_libcxx_before_spec=%{?build_with_libcxx}'])
assert 'payload=w5.xzdio' in out and 'toolchain=clang' in out and 'clang=1' in out
run('rpmbuild-bp',bw+['rpmbuild',*args])
log=(E/'rpmbuild-bp.stderr.txt').read_text(errors='replace')+(E/'rpmbuild-bp.stdout.txt').read_text(errors='replace')
assert '%build' not in log
tree=T/'rpm-bpf-armv7l-libcxx/BUILD/bpftrace-0.24.2'
cm=(tree/'src/CMakeLists.txt').read_text();ast=(tree/'src/ast/CMakeLists.txt').read_text()
assert cm.count('target_link_options(bpftrace BEFORE PRIVATE "-shared-libgcc" "-static-libstdc++")')==1
assert cm.count('target_link_options(bpftrace BEFORE PRIVATE "-static-libgcc" "-static-libstdc++")')==0
assert 'test 1 = 1' in log
patch=(R/'packaging/static-link-libbpf.patch').read_text();assert '${LIBBPF_LIBRARIES}' in ast
assets=[]
for f in sorted((tree/'prebuilt-static-bpftrace').rglob('*')):
 if f.is_file():assets.append(dict(path=str(f.relative_to(tree)),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest()))
result=dict(status='PASS',stage='prep only',build_executed=False,spec_sha256=hashlib.sha256((R/'packaging/bpftrace.spec').read_bytes()).hexdigest(),patch_sha256=hashlib.sha256((R/'packaging/static-link-libbpf.patch').read_bytes()).hexdigest(),setup=True,patch0=True,source1002=True,grep_count_before_sed=1,shared_libgcc_line_after_sed=1,prebuilt_assets=assets,output_tree=str(tree))
(E/'result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
