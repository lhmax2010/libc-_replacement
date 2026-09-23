"""Read-only macro queries; optionally prep into an independent directory, never build."""
import json,os,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/prep-check';E.mkdir(exist_ok=True)
T=P/'tmp/LLVM_W4_0923';T.mkdir(exist_ok=True)
root=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
old=json.loads((P/'docs/progress/ARM_W5_0921/cell_commands/w5-original.json').read_text())
macro=old[old.index('--macros')+1];rcfile=old[old.index('--rcfile')+1]
bw=['bwrap','--unshare-user','--uid','1000','--gid','1000','--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(T),'/tmp/task','--ro-bind',str(repo/'packaging'),'/tmp/packaging','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C','--']
common=['--rcfile',rcfile,'--macros',macro,'--target','armv7l-tizen-linux','--define','_binary_payload w5.xzdio']
records=[]
def run(label,cmd):
 start=time.time()
 with (E/(label+'.stdout.txt')).open('xb') as out,(E/(label+'.stderr.txt')).open('xb') as err:r=subprocess.run(cmd,stdout=out,stderr=err,timeout=1200)
 records.append(dict(label=label,command=shlex.join(cmd),argv=cmd,exitcode=r.returncode,seconds=time.time()-start));(E/'commands.json').write_text(json.dumps(records,indent=2));return r.returncode
counts={}
for mode,extra in [('clang',[]),('gcc',['--define','_toolchain_override gcc']),('undefined',['--undefine','_toolchain'])]:
 rc=run(mode,bw+['rpmspec','-P',*common,*extra,'/tmp/packaging/llvm.spec'])
 text=(E/(mode+'.stdout.txt')).read_text();counts[mode]=dict(exitcode=rc,static_option_count=text.count('-DLIBCLANG_BUILD_STATIC=ON'),libcxx_option_count=text.count('-DLLVM_ENABLE_LIBCXX=ON'))
(E/'macro-result.json').write_text(json.dumps(counts,indent=2));print(json.dumps(counts,indent=2))
