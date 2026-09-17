import hashlib
import json
import shlex
import subprocess
from pathlib import Path

p=Path.cwd(); root=p/'tmp/STATIC_0917B/rsync_read_probe'; root.mkdir(exist_ok=False)
source=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/usr/bin/llvm-opt-report'
out=p/'progress/BUILD_STATIC_0917B/resume_0917'
rows=[]
for mode,cmd in [('rsync',['rsync','-aH','--checksum','--no-owner','--no-group',str(source),str(root/'rsync-copy')]),
                 ('cp',['cp','--',str(source),str(root/'cp-copy')])]:
    traced=['strace','-f','-o',str(out/('copy_'+mode+'.strace')),'-e','trace=openat,read,pread64,lseek,close,mmap,munmap,write,copy_file_range,sendfile',*cmd]
    r=subprocess.run(traced,capture_output=True,text=True,timeout=40)
    rows.append({'method':mode,'command':shlex.join(traced),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'copy_read_diagnostic.json').write_text(json.dumps(rows,indent=2))
    print(mode,'exitcode',r.returncode,flush=True)
for name,f in [('source',source),('rsync',root/'rsync-copy'),('cp',root/'cp-copy')]:
    if f.is_file():
        rows.append({'file':name,'path':str(f),'sha256':hashlib.sha256(f.read_bytes()).hexdigest(),'bytes':f.stat().st_size})
(out/'copy_read_diagnostic.json').write_text(json.dumps(rows,indent=2))
print(json.dumps(rows,indent=2))
