"""Read current link processes in this task's cgroup; never attach or modify them."""
import datetime,json,os
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'
cg=Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/bpf-w1-0921.scope')
rows=[]
for s in (cg/'cgroup.procs').read_text().split():
    d=Path('/proc')/s
    try:
        argv=[x.decode(errors='replace') for x in (d/'cmdline').read_bytes().split(b'\0') if x]
        cwd=os.readlink(d/'cwd')
    except (FileNotFoundError,ProcessLookupError,PermissionError): continue
    if '/rpm-bpf-armv7l-libcxx/' not in cwd or not cwd.endswith('/build-static/src'): continue
    if not any(argv[i:i+2]==['-o','bpftrace'] for i in range(len(argv)-1)): continue
    rows.append(dict(pid=int(s),cwd=cwd,argv=argv,cgroup=(d/'cgroup').read_text(),shared_libgcc='-shared-libgcc' in argv,static_libgcc='-static-libgcc' in argv))
result=dict(time=datetime.datetime.now().astimezone().isoformat(),method='read-only /proc of task cgroup',status='OBSERVED' if rows else 'NOT_OBSERVED',processes=rows)
dest=out/'ARM_STATIC_LINK_PROCESSES.json'; assert not dest.exists(); dest.write_text(json.dumps(result,indent=2))
print(result['status'],len(rows),'matching link processes; no attachment or changes')
