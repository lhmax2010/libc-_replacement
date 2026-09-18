import datetime, os, shlex, subprocess, sys
from pathlib import Path
root=Path('/home/toolchain/development/libc++_replacement')
os.chdir(root)
os.environ['PATH']='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.local/bin:/home/toolchain/.vscode/extensions/openai.chatgpt-26.908.40401-linux-x64/bin/linux-x86_64'
out=root/'progress/BUILD_WEEKEND_0918/raw'; out.mkdir(parents=True,exist_ok=True)
label,*cmd=sys.argv[1:]
assert label and '/' not in label and cmd
p=out/label
def put(s,v):
    with Path(str(p)+s).open('x') as f: f.write(v+'\n')
put('.command.txt',shlex.join(cmd)); put('.started.txt',datetime.datetime.now().astimezone().isoformat())
with Path(str(p)+'.stdout.txt').open('xb') as so,Path(str(p)+'.stderr.txt').open('xb') as se:
    rc=subprocess.run(cmd,stdout=so,stderr=se).returncode
put('.exitcode',str(rc)); put('.finished.txt',datetime.datetime.now().astimezone().isoformat())
print(label,'exitcode='+str(rc),flush=True)
sys.exit(rc if rc>=0 else 128-rc)
