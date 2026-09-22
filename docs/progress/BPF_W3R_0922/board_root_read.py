import json,re,shlex,subprocess,time
from pathlib import Path
O=Path('progress/BPF_W3R_0922/board-root-read');O.mkdir(exist_ok=False)
S=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101'];rows=[]
def call(label,args):
 t=time.monotonic();r=subprocess.run(args,capture_output=True,timeout=90)
 (O/(label+'.out')).write_bytes(r.stdout);(O/(label+'.err')).write_bytes(r.stderr)
 rows.append(dict(label=label,command=shlex.join(args),exitcode=r.returncode,seconds=time.monotonic()-t));(O/'commands.json').write_text(json.dumps(rows,indent=2));assert r.returncode==0
 return r.stdout.decode(errors='replace').replace('\r\n','\n')
def shell(label,cmd):
 s=call(label,S+['shell','('+cmd+'); task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"']);m=re.findall(r'^TASK_REMOTE_RC=(\d+)$',s,re.M);assert len(m)==1
 rows[-1]['remote_exitcode']=int(m[0]);(O/'commands.json').write_text(json.dumps(rows,indent=2));return int(m[0]),re.sub(r'\nTASK_REMOTE_RC=\d+\n?','',s)
assert 'rpi4' in shell('kernel','uname -r')[1]
assert shell('arch','uname -m')[1].strip()=='armv7l'
assert re.search(r'^ID=["\x27]?tizen["\x27]?$',shell('os','cat /etc/os-release')[1],re.M|re.I)
assert 'uid=0(' not in shell('id-before','id')[1]
call('root-on',S+['root','on'])
try:
 assert 'uid=0(' in shell('id-root','id')[1]
 for label,cmd in [('resolve','for p in / /etc /usr /opt /var /var/tmp /tmp; do printf "%s: " "$p"; readlink -f "$p"; done'),('rpm-info','rpm -qip --nosignature /var/tmp/bpf_static_0922/bpftrace-static-0.24.2-1.armv7l.rpm'),('rpm-size',"rpm -qp --nosignature --qf '%{SIZE}\\n' /var/tmp/bpf_static_0922/bpftrace-static-0.24.2-1.armv7l.rpm"),('tools','for p in rpm2cpio cpio busybox tar timeout; do command -v "$p"; done'),('busybox-applets','busybox --list'),('sha','sha256sum /var/tmp/bpf_static_0922/*'),('mounts','cat /proc/mounts'),('df','df -Pk / /var/tmp/bpf_static_0922')]:
  rc,s=shell(label,cmd);print(label,rc,s[:2500])
finally:
 call('root-off',S+['root','off']);assert 'uid=0(' not in shell('id-after','id')[1]
