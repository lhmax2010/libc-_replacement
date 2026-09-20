import datetime,os,shlex,subprocess,sys
from pathlib import Path
root=Path('/home/toolchain/development/libc++_replacement'); os.chdir(root)
os.environ['PATH']='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.local/bin:/home/toolchain/.vscode/extensions/openai.chatgpt-26.908.40401-linux-x64/bin/linux-x86_64'
out=root/'progress/ARM_RPM_DIAG/raw'; out.mkdir(parents=True,exist_ok=True)
label,*command=sys.argv[1:]; assert label and '/' not in label and command
def write(s,text):
    with (out/(label+s)).open('x') as f: f.write(text+'\n')
write('.command.txt',shlex.join(command)); write('.started.txt',datetime.datetime.now().astimezone().isoformat())
with (out/(label+'.stdout.txt')).open('xb') as so,(out/(label+'.stderr.txt')).open('xb') as se:
    result=subprocess.run(command,stdout=so,stderr=se)
write('.exitcode',str(result.returncode)); write('.finished.txt',datetime.datetime.now().astimezone().isoformat())
print(label,'exitcode='+str(result.returncode),flush=True)
raise SystemExit(result.returncode if result.returncode>=0 else 128-result.returncode)
