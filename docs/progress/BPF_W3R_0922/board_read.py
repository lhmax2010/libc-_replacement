"""Read-only board identity, mount and RPM-header observations. No install."""
import json,re,shlex,subprocess,time
from pathlib import Path
O=Path('progress/BPF_W3R_0922/board-read'); O.mkdir(exist_ok=False)
S=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']; rows=[]
def run(label,cmd):
    argv=S+['shell','('+cmd+'); task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"']
    t=time.monotonic(); r=subprocess.run(argv,capture_output=True,timeout=90)
    (O/(label+'.out')).write_bytes(r.stdout);(O/(label+'.err')).write_bytes(r.stderr)
    text=r.stdout.decode(errors='replace').replace('\r\n','\n'); codes=re.findall(r'^TASK_REMOTE_RC=(\d+)$',text,re.M)
    rows.append(dict(label=label,command=shlex.join(argv),exitcode=r.returncode,remote_exitcode=int(codes[0]) if len(codes)==1 else None,seconds=time.monotonic()-t))
    (O/'commands.json').write_text(json.dumps(rows,indent=2));assert r.returncode==0 and len(codes)==1
    return int(codes[0]), re.sub(r'\nTASK_REMOTE_RC=\d+\n?','',text)
rc,k=run('uname-r','uname -r'); assert rc==0 and 'rpi4' in k
rc,m=run('uname-m','uname -m'); assert rc==0 and m.strip()=='armv7l'
rc,osr=run('os-release','cat /etc/os-release'); assert rc==0 and re.search(r'^ID=["\x27]?tizen["\x27]?$',osr,re.M|re.I)
(O/'IDENTITY.json').write_text(json.dumps(dict(kernel=k.strip(),machine=m.strip(),os_release=osr,status='TRIPLE_ASSERTION_PASS'),indent=2))
for label,cmd in [
 ('id','id'),('mounts','cat /proc/mounts'),('resolve','readlink -f /var /var/tmp /tmp'),
 ('stat-root','stat -f /'),('stat-work','stat -f /var/tmp'),('rpm-version','rpm --version'),
 ('rpm-info','rpm -qip --nosignature /var/tmp/bpf_static_0922/bpftrace-static-0.24.2-1.armv7l.rpm'),
 ('rpm-size',"rpm -qp --nosignature --qf '%{SIZE}\\n' /var/tmp/bpf_static_0922/bpftrace-static-0.24.2-1.armv7l.rpm"),
 ('df','df -Pk / /var/tmp/bpf_static_0922'),('tools','command -v rpm2cpio; command -v cpio; command -v timeout'),
 ('files','ls -la /var/tmp/bpf_static_0922'),('system-before','ls -l /usr/bin/bpftrace*'),
 ('packages-before',"rpm -qa --qf '%{NAME}\\n' | grep bpftrace"),('processes','ps -eo pid,ppid,comm,args')]:
    rc,s=run(label,cmd);print(label,rc,s[:3000])
