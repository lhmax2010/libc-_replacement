"""Non-build smoke test: only a fresh disposable input copy, explicit new output ELF."""
import ast,hashlib,json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';T=P/'tmp/BPF_W4_0922/objcopy-smoke';T.mkdir(exist_ok=False)
tree=ast.parse((E/'build_audit.py').read_text());calls=[n for n in ast.walk(tree) if isinstance(n,ast.Call) and isinstance(n.func,ast.Name) and n.func.id=='run' and len(n.args)>1 and isinstance(n.args[1],ast.List) and any(isinstance(e,ast.Constant) and e.value=='/usr/bin/llvm-objcopy' for e in n.args[1].elts)]
assert len(calls)==1;args=calls[0].args[1];assert len(args.elts)==5 and ast.unparse(args.elts[-1])=='str(rewritten)'
records=[]
def run(argv):
 r=subprocess.run(argv,capture_output=True);records.append(dict(command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')));assert r.returncode==0
src=Path('/usr/bin/true');f=T/'input-copy';run(['cp','-a','--no-clobber','--',str(src),str(f)])
def identity(p):
 s=p.stat();return dict(sha256=hashlib.sha256(p.read_bytes()).hexdigest(),inode=s.st_ino,size=s.st_size,mtime_ns=s.st_mtime_ns,ctime_ns=s.st_ctime_ns)
before=identity(f);out=T/'text.section';rewritten=T/'output.elf';assert not out.exists() and not rewritten.exists()
run(['/usr/bin/llvm-objcopy','--dump-section','.text='+str(out),str(f),str(rewritten)])
after=identity(f);assert before==after and out.is_file() and rewritten.is_file()
(E/'FIX_SMOKE.json').write_text(json.dumps(dict(status='PASS',before=before,after=after,commands=records,scope='fresh disposable copy only; no build, no original payload conversion'),indent=2));print('SAFE_OUTPUT_SMOKE_PASS')
