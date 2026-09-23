"""Only %prep in a fresh writable output directory; retained ARM root is read-only."""
import hashlib,json,os,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/prep-check/runtime_R3';T=P/'tmp/LLVM_W4_0923/runtime-prep_R3'
T.mkdir(exist_ok=False)
for d in ['BUILD','BUILDROOT','RPMS','SRPMS','SPECS','SOURCES','TMP']:(T/d).mkdir()
root=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0';repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
spec=repo/'packaging/libcxx-runtimes.spec';assert hashlib.sha256(spec.read_bytes()).hexdigest()=='c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe'
old=json.loads((P/'docs/progress/ARM_W5_0921/cell_commands/w5-original.json').read_text())
common=['--rcfile',old[old.index('--rcfile')+1],'--macros',old[old.index('--macros')+1],'--target','armv7l-tizen-linux','--define','_binary_payload w5.xzdio']
cmd=['bwrap','--unshare-user','--uid','1000','--gid','1000','--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(T),'/tmp/task','--ro-bind',str(repo/'packaging'),'/tmp/packaging','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C','--','rpmbuild',*common,'--define','_topdir /tmp/task','--define','_sourcedir /home/abuild/rpmbuild/SOURCES','--define','_tmppath /tmp/task/TMP','--define','_smp_mflags -j1','--noclean','-bp','/tmp/packaging/libcxx-runtimes.spec']
cg=Path('/proc/self/cgroup').read_text();rel=next(x[3:] for x in cg.splitlines() if x.startswith('0::'));limit=(Path('/sys/fs/cgroup')/rel.lstrip('/')/'memory.max').read_text().strip()
expected=(int(next(x.split()[1] for x in Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024//2//4096)*4096
assert int(limit)==expected and os.getpriority(os.PRIO_PROCESS,0)==19
io=subprocess.run(['ionice','-p',str(os.getpid())],capture_output=True,text=True);assert io.returncode==0 and 'idle' in io.stdout
(E/'prep-resources_R3.json').write_text(json.dumps(dict(pid=os.getpid(),cgroup=cg,memory_max=limit,nice=19,ionice=io.stdout),indent=2)+'\n')
(E/'prep-command_R3.json').write_text(json.dumps(dict(argv=cmd,command=shlex.join(cmd),status='RUNNING'),indent=2)+'\n')
start=time.time()
with (E/'prep.stdout.txt').open('xb') as out,(E/'prep.stderr.txt').open('xb') as err:r=subprocess.run(cmd,stdout=out,stderr=err,timeout=1200)
(E/'prep-command_R3.json').write_text(json.dumps(dict(argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,seconds=time.time()-start),indent=2)+'\n')
print('prep exit',r.returncode);raise SystemExit(r.returncode)
