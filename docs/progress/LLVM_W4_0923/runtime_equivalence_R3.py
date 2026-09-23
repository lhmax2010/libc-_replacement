"""R3 gate: ignore pure blank lines only, then compare exact permitted changes."""
import difflib,hashlib,json,re,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/prep-check/runtime_R3';E.mkdir(parents=True,exist_ok=False)
repo=P/'tmp/BUILD_W1BC_0911/target-fetch';root=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
candidate=P/'tmp/NIGHT_0917/specs/libcxx-runtimes.spec'
assert hashlib.sha256(candidate.read_bytes()).hexdigest()=='c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe'
old=json.loads((P/'docs/progress/ARM_W5_0921/cell_commands/w5-original.json').read_text())
common=['--rcfile',old[old.index('--rcfile')+1],'--macros',old[old.index('--macros')+1],'--target','armv7l-tizen-linux','--define','_binary_payload w5.xzdio']
records=[];results=[]
def run(label,cmd):
 start=time.time();r=subprocess.run(cmd,capture_output=True,timeout=600)
 (E/(label+'.stdout.txt')).write_bytes(r.stdout);(E/(label+'.stderr.txt')).write_bytes(r.stderr)
 records.append(dict(label=label,argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,seconds=time.time()-start));(E/'commands_R3.json').write_text(json.dumps(records,indent=2)+'\n');assert r.returncode==0
 return r.stdout.decode()
baseline=E/'baseline_R3.spec';baseline.write_text(run('source',['git','-C',str(repo),'show','f203923a:packaging/libcxx-runtimes.spec']))
bw=['bwrap','--unshare-user','--uid','1000','--gid','1000','--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--ro-bind',str(baseline),'/tmp/baseline.spec','--ro-bind',str(candidate),'/tmp/candidate.spec','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C','--']
flags=['LIBCXX_ENABLE_STATIC','LIBCXX_INSTALL_STATIC_LIBRARY','LIBCXXABI_ENABLE_STATIC','LIBCXXABI_INSTALL_STATIC_LIBRARY']
def nonblank(s):return [x for x in s.splitlines(True) if x.strip()]
for mode,args in [('default',[]),('override-gcc',['--define','_toolchain_override gcc']),('toolchain-gcc',['--define','_toolchain gcc']),('undefined',['--undefine','_toolchain'])]:
 a=run(mode+'-baseline',bw+['rpmspec','-P',*common,*args,'/tmp/baseline.spec'])
 b=run(mode+'-candidate',bw+['rpmspec','-P',*common,*args,'/tmp/candidate.spec'])
 (E/(mode+'-raw.diff')).write_text(''.join(difflib.unified_diff(a.splitlines(True),b.splitlines(True),fromfile='baseline',tofile='candidate')))
 (E/(mode+'-nonblank.diff')).write_text(''.join(difflib.unified_diff(nonblank(a),nonblank(b),fromfile='baseline-nonblank',tofile='candidate-nonblank')))
 normalized=b
 counts={f:dict(ON=b.count('-D'+f+'=ON'),OFF=b.count('-D'+f+'=OFF')) for f in flags}
 for f in flags:normalized=normalized.replace('-D'+f+'=ON','-D'+f+'=OFF')
 archives=[x.strip() for x in b.splitlines() if x.strip() in ['/usr/lib/libc++.a','/usr/lib/libc++abi.a']]
 normalized=''.join(x for x in normalized.splitlines(True) if x.strip() not in archives)
 enabled=mode=='default'
 passed=nonblank(normalized)==nonblank(a) and all(v==dict(ON=int(enabled),OFF=int(not enabled)) for v in counts.values()) and len(archives)==(2 if enabled else 0)
 results.append(dict(mode=mode,result='PASS' if passed else 'FAIL',blank_lines_ignored=True,extra_blank_lines=sum(not x.strip() for x in b.splitlines())-sum(not x.strip() for x in a.splitlines()),static_flags=counts,archive_file_entries=archives,nonblank_identical=nonblank(a)==nonblank(b)))
 (E/'RESULT_R3.json').write_text(json.dumps(results,indent=2)+'\n');print(mode,results[-1],flush=True);assert passed,'STOP: unexpected difference'
