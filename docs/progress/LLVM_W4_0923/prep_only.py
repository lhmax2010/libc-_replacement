import json,os,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/prep-check';T=P/'tmp/LLVM_W4_0923/prep'
T.mkdir(exist_ok=False)
for d in ('BUILD','BUILDROOT','RPMS','SRPMS','SPECS','SOURCES','TMP'):(T/d).mkdir()
source=json.loads((E/'commands.json').read_text())[0]['argv'];i=source.index('--');bw=source[:i]
cmd=bw+['--','rpmbuild',*source[i+3:-1]]
cmd+=['--define','_topdir /tmp/task/prep','--define','_sourcedir /home/abuild/rpmbuild/SOURCES','--define','_tmppath /tmp/task/prep/TMP','--define','_smp_mflags -j1','--define','mlgo_build_jobs 1','--noclean','-bp','/tmp/packaging/llvm.spec']
cg=Path('/proc/self/cgroup').read_text();rel=next(x[3:] for x in cg.splitlines() if x.startswith('0::'));limit=(Path('/sys/fs/cgroup')/rel.lstrip('/')/'memory.max').read_text().strip()
expected=(int(next(x.split()[1] for x in Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024//2//4096)*4096
assert int(limit)==expected and os.getpriority(os.PRIO_PROCESS,0)==19
(E/'prep-resources.json').write_text(json.dumps(dict(pid=os.getpid(),cgroup=cg,memory_max=limit,nice=os.getpriority(os.PRIO_PROCESS,0)),indent=2))
start=time.time()
with (E/'prep.stdout.txt').open('xb') as out,(E/'prep.stderr.txt').open('xb') as err:r=subprocess.run(cmd,stdout=out,stderr=err,timeout=1200)
(E/'prep-command.json').write_text(json.dumps(dict(argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,seconds=time.time()-start),indent=2))
print('prep exit',r.returncode);assert r.returncode==0
