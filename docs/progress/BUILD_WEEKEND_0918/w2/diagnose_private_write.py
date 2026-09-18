import json,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
base=json.loads((out/'bpf-private-aarch64-base.json').read_text())
cmd=['strace','-f','-e','trace=mkdir,mkdirat,openat,symlinkat','-o',str(out/'private-write.strace'),*base,'--','sh','-c',
     'id; mkdir /usr/lib64/.weekend-write-probe && rmdir /usr/lib64/.weekend-write-probe']
r=subprocess.run(cmd,capture_output=True,text=True)
(out/'private-write-diagnostic.json').write_text(json.dumps({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr},indent=2))
print(r.stdout,r.stderr,'exitcode',r.returncode)
