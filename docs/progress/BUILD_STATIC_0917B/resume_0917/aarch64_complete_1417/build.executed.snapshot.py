"""Resume the existing RPM build; enforce actual cgroup, serial jobs and I/O gates."""
import datetime, hashlib, json, os, re, signal, subprocess, threading, time
from pathlib import Path

ROOT = Path('/home/toolchain/development/libc++_replacement')
os.chdir(ROOT)
OUT = ROOT / 'progress/BUILD_STATIC_0917B/resume_0917'
DEADLINE = datetime.datetime.fromisoformat('2026-09-17T22:48:19+08:00').timestamp()
SCOPE = 'build-static-0917-resume.scope'
MEM = int(next(x.split()[1] for x in Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:'))) * 1024 // 2 // 4096 * 4096
os.environ['PATH'] = '/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.local/bin'
events = (OUT/'events.jsonl').open('a', buffering=1)
def note(event, **kw):
    row = dict(time=datetime.datetime.now().astimezone().isoformat(), event=event, **kw)
    events.write(json.dumps(row)+'\n')
    print(json.dumps(row), flush=True)
def query(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    note('command', command=cmd, exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr)
    if r.returncode: raise RuntimeError('resource query failed')
    return r.stdout
def resources(pid):
    cg = next(x.split(':',2)[2] for x in query(['cat',f'/proc/{pid}/cgroup']).splitlines() if x.startswith('0::'))
    assert SCOPE in cg
    value = query(['cat','/sys/fs/cgroup'+cg+'/memory.max']).strip()
    assert int(value) == MEM
    assert query(['ionice','-p',str(pid)]).strip() == 'idle'
    assert os.getpriority(os.PRIO_PROCESS,pid) == 19
    note('resource_verified',pid=pid,cgroup=cg,memory_max=int(value),nice=19,ionice='idle')
    return cg
cg = resources(os.getpid())
cmd = json.loads((OUT.parent/'native_rpm_serial_command.json').read_text())
note('start',command=cmd,deadline=DEADLINE,script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
stream = (OUT/'llvm-aarch64.log').open('xb',buffering=0)
proc = subprocess.Popen(cmd,stdout=stream,stderr=subprocess.STDOUT,start_new_session=True)
def send(sig):
    try: os.killpg(proc.pid,sig)
    except ProcessLookupError: pass
def hard_stop():
    send(signal.SIGTERM); send(signal.SIGCONT)
    time.sleep(15)
    if proc.poll() is None: send(signal.SIGKILL)
timer = threading.Timer(max(0,DEADLINE-time.time()-20),hard_stop)
timer.daemon=True; timer.start()
def stop(reason):
    send(signal.SIGTERM); send(signal.SIGCONT)
    note('stop',reason=reason)
    try: proc.wait(timeout=15)
    except subprocess.TimeoutExpired: send(signal.SIGKILL); proc.wait()
    raise SystemExit(124)
def probe():
    testcmd=['timeout','-k','2s','30s','python3',str(OUT.parent/'io_probe.py')]
    t=time.monotonic()
    r=subprocess.run(testcmd,capture_output=True,text=True)
    note('io_probe',command=testcmd,exitcode=r.returncode,seconds=time.monotonic()-t,stdout=r.stdout,stderr=r.stderr)
    return r.returncode==0
reader=(OUT/'llvm-aarch64.log').open(errors='replace')
verified=set(); target=0; next_probe=500; last=0; hourly=0
try:
    while proc.poll() is None:
        if time.time()>=DEADLINE-20: stop('fourteen-hour deadline')
        for rawpid in Path('/sys/fs/cgroup'+cg+'/cgroup.procs').read_text().split():
            pid=int(rawpid)
            if pid in verified: continue
            try:
                args=[x.decode(errors='replace') for x in Path(f'/proc/{pid}/cmdline').read_bytes().split(b'\0') if x]
                cwd=os.readlink(f'/proc/{pid}/cwd')
            except (FileNotFoundError,ProcessLookupError,PermissionError): continue
            if not args: continue
            ninja=Path(args[0]).name in ('ninja','ninja.real') or ('qemu-' in args[0] and any(x.endswith('/ninja.real') for x in args))
            if not ninja or any(x in args for x in ('-t','--version','--help')): continue
            one='-j1' in args or any(args[i:i+2]==['-j','1'] for i in range(len(args)-1))
            if not one: stop('observed Ninja not serial')
            try: resources(pid)
            except FileNotFoundError: continue
            note('ninja_verified',pid=pid,command=args,cwd=cwd)
            verified.add(pid)
        for line in reader.readlines():
            m=re.search(r'\[(\d+)/(\d+)\]',line)
            if m: target=max(target,int(m[1]))
        if target>=next_probe:
            next_probe=(target//500+1)*500
            if not probe():
                send(signal.SIGSTOP); note('paused',completed=target)
                for attempt in range(1,4):
                    end=time.time()+600
                    while time.time()<end:
                        if time.time()>=DEADLINE-20: stop('deadline during I/O pause')
                        time.sleep(1)
                    if probe():
                        send(signal.SIGCONT); note('resumed',attempt=attempt); break
                    note('pause_retry_failed',attempt=attempt)
                else: stop('three consecutive I/O pause retries failed')
        if time.time()-last>=45:
            note('running',completed=target); last=time.time()
        if time.time()-hourly>=3600:
            with (OUT/'HOURLY_STATUS.md').open('a') as f:
                f.write(f'\n- {datetime.datetime.now().astimezone().isoformat()}: LLVM aarch64 ongoing, observed progress {target}; no RPM success claimed.\n')
            hourly=time.time()
        time.sleep(1)
    note('finished',exitcode=proc.returncode,completed=target)
    raise SystemExit(proc.returncode)
finally:
    timer.cancel()
    if proc.poll() is None: hard_stop()
