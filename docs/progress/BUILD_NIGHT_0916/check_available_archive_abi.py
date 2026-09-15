"""Inspect available archive ABI samples. This is not a complete link or functional test."""
import hashlib
import json
from pathlib import Path
import shlex
import subprocess
import tempfile

p=Path('progress/BUILD_NIGHT_0916')
tmp=Path('tmp/BUILD_NIGHT_0916.h4taGY')
j=(p/'available_archive_commands.jsonl').open('w',buffering=1)
def run(args):
 r=subprocess.run(args,capture_output=True,text=True,errors='replace')
 j.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n')
 assert r.returncode==0,(args,r.returncode)
 return r.stdout
packages=json.loads((p/'static_packages_v2.json').read_text())['bcc_static_rpms']
inventory=json.loads((p/'static_archive_inventory.json').read_text())
records=[]
for record in packages:
 arch=record['arch'];rpm=Path(record['path'])
 assert hashlib.sha256(rpm.read_bytes()).hexdigest()==record['sha256']
 out=Path(tempfile.mkdtemp(prefix=f'{arch}-bcc-static.',dir=tmp))
 cmd1=['rpm2cpio',str(rpm.resolve())]
 cmd2=['cpio','-id','--quiet','--no-absolute-filenames']
 proc=subprocess.Popen(cmd1,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
 proc2=subprocess.run(cmd2,stdin=proc.stdout,cwd=out,capture_output=True)
 proc.stdout.close();err=proc.stderr.read();rc=proc.wait()
 j.write(json.dumps(dict(command=shlex.join(cmd1)+' | '+shlex.join(cmd2),cwd=str(out),
                         exitcodes=[rc,proc2.returncode],stdout=proc2.stdout.decode(errors='replace'),
                         stderr=err.decode(errors='replace')+proc2.stderr.decode(errors='replace')))+'\n')
 assert rc==0 and proc2.returncode==0
 archives=list(out.rglob('*.a'))
 assert len(archives)==3
 root=Path(inventory['roots'][arch])
 support=list((root/'usr').glob('lib*/libLLVMSupport.a'))
 assert len(support)==1
 for f in archives+support:
  symbols=run(['nm','-u',str(f)])
  info=run(['objdump','-f',str(f)])
  row=dict(arch=arch,path=str(f),sha256=hashlib.sha256(f.read_bytes()).hexdigest(),
           std1_lines=sum('St3__1' in x for x in symbols.splitlines()),
           stdcxx11_lines=sum('__cxx11' in x for x in symbols.splitlines()))
  records.append(row)
  print(json.dumps(row),flush=True)
(p/'available_archive_abi.json').write_text(json.dumps(dict(records=records,
scope='Only the three BCC archives and LLVMSupport per architecture; not proof of all LLVM/Clang components or final static link.'),indent=2)+'\n')
