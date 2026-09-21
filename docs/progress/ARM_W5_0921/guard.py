"""Resume the existing RPM build; enforce actual cgroup, serial jobs and I/O gates."""
import datetime, hashlib, json, os, re, signal, subprocess, sys, threading, time
from pathlib import Path

ROOT = Path('/home/toolchain/development/libc++_replacement')
os.chdir(ROOT)
STAGE, LABEL = sys.argv[1:]
assert STAGE in ('W1','W2','W3','W4')
assert re.fullmatch(r'[a-z0-9_-]+', LABEL)
BASE = ROOT / 'progress/ARM_W5_0921'
OUT = BASE / 'cells' / LABEL
OUT.mkdir(parents=True, exist_ok=False)
DEADLINE = datetime.datetime.fromisoformat(json.loads((BASE/'deadlines.json').read_text())[STAGE]).timestamp()
SCOPE = 'arm-w5-0921.scope'
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
    def state():
        fields=Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()
        return fields[0],fields[19]
    before=state()
    if before[0]=='Z':
        note('resource_not_observed_exited',pid=pid,state=before)
        return None
    cg = next(x.split(':',2)[2] for x in query(['cat',f'/proc/{pid}/cgroup']).splitlines() if x.startswith('0::'))
    assert SCOPE in cg
    value = query(['cat','/sys/fs/cgroup'+cg+'/memory.max']).strip()
    assert int(value) == MEM
    io=query(['ionice','-p',str(pid)]).strip()
    priority=os.getpriority(os.PRIO_PROCESS,pid)
    after=state()
    if after[0]=='Z' or before[1]!=after[1]:
        note('resource_not_observed_exited',pid=pid,before=before,after=after,ionice=io)
        return None
    assert io == 'idle'
    assert priority == 19
    note('resource_verified',pid=pid,cgroup=cg,memory_max=int(value),nice=19,ionice='idle')
    return cg
cg = resources(os.getpid())
cmd = json.loads((BASE/'cell_commands'/f'{LABEL}.json').read_text())
for arg in cmd:
    f=Path(arg)
    if f.suffix=='.py' and f.is_file():
        snap=OUT/('executed-'+f.name)
        snap.write_bytes(f.read_bytes())
        note('script_snapshot',path=str(f),snapshot=str(snap),sha256=hashlib.sha256(snap.read_bytes()).hexdigest())
note('start',command=cmd,deadline=DEADLINE,script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
log=OUT/'build.log'
if log.exists():
    backup=OUT/('build.stopped-'+datetime.datetime.now().strftime('%H%M%S')+'.log')
    log.rename(backup)
    note('previous_log_preserved',path=str(backup))
assert 'LD_PRELOAD' not in os.environ
stream = log.open('xb',buffering=0)
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
    testcmd=['timeout','-k','2s','30s','python3',str(ROOT/'progress/BUILD_STATIC_0917B/io_probe.py')]
    t=time.monotonic()
    r=subprocess.run(testcmd,capture_output=True,text=True)
    note('io_probe',command=testcmd,exitcode=r.returncode,seconds=time.monotonic()-t,stdout=r.stdout,stderr=r.stderr)
    return r.returncode==0
reader=log.open(errors='replace')
verified=set(); target=0; next_probe=500; last=0; hourly=0
last_ninja=0
last_snapshot=0
try:
    while proc.poll() is None:
        if time.time()>=DEADLINE-20: stop('stage deadline')
        if time.time()-last_snapshot>=20:
            snapcmd=['python3',str(BASE/'thread_snapshot.py'),str(OUT),cg]
            sr=subprocess.run(snapcmd,capture_output=True,text=True)
            note('thread_snapshot',command=snapcmd,exitcode=sr.returncode,stdout=sr.stdout,stderr=sr.stderr)
            if sr.returncode: stop('thread sampler failed')
            last_snapshot=time.time()
        for rawpid in Path('/sys/fs/cgroup'+cg+'/cgroup.procs').read_text().split():
            pid=int(rawpid)
            if pid in verified: continue
            try:
                args=[x.decode(errors='replace') for x in Path(f'/proc/{pid}/cmdline').read_bytes().split(b'\0') if x]
                cwd=os.readlink(f'/proc/{pid}/cwd')
            except (FileNotFoundError,ProcessLookupError,PermissionError): continue
            if not args: continue
            ninja=Path(args[0]).name in ('ninja','ninja.real') or ('qemu-' in args[0] and any(x.endswith('/ninja.real') for x in args))
            make=Path(args[0]).name in ('make','gmake') or ('qemu-' in args[0] and any(Path(x).name in ('make','gmake') for x in args[1:]))
            if not (ninja or make) or any(x in args for x in ('-t','--version','--help')): continue
            one='-j1' in args or any(args[i:i+2]==['-j','1'] for i in range(len(args)-1))
            makeflags=''
            if make:
                try:
                    makeflags=next((x.split(b'=',1)[1].decode(errors='replace') for x in Path(f'/proc/{pid}/environ').read_bytes().split(b'\0') if x.startswith(b'MAKEFLAGS=')), '')
                except (FileNotFoundError, ProcessLookupError): continue
                one=one or bool(re.search(r'(?:^|\s)-j\s*1(?:\s|$)',makeflags))
                # CMake try_compile can invoke Make without -j. GNU Make's
                # default is one job; do not confuse absent -j with Ninja's
                # parallel default. Reject any jobserver/parallel request.
                if not one and not any(x.startswith(('-j','--jobs','--jobserver')) for x in args) and not re.search(r'(?:^|\s)-?j|--jobs|jobserver',makeflags):
                    one=True
                    note('make_default_serial',pid=pid,command=args,makeflags=makeflags)
            note('scheduler_arguments',pid=pid,command=args,makeflags=makeflags,serial=one)
            if not one: stop('observed build scheduler not serial')
            try:
                checked=resources(pid)
                if checked is None:
                    verified.add(pid)
                    continue
            except (FileNotFoundError,ProcessLookupError):
                note('resource_not_observed_exited',pid=pid)
                continue
            except RuntimeError:
                if not Path(f'/proc/{pid}').exists():
                    note('resource_not_observed_exited',pid=pid)
                    continue
                stop('actual Ninja resource query failed')
            except AssertionError:
                stop('actual live Ninja resource constraint failed')
            note('build_scheduler_verified',pid=pid,command=args,cwd=cwd,makeflags=makeflags)
            verified.add(pid)
        for line in reader.readlines():
            m=re.search(r'\[(\d+)/(\d+)\]',line)
            if m:
                current=int(m[1])
                target += current-last_ninja if current>=last_ninja else current
                last_ninja=current
            elif re.search(r'\[\s*\d+%\]\s+(?:Building|Linking|Generating)',line):
                target += 1
            elif line.startswith('COPY_ENTRY '):
                target += 1
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
                f.write(f'\n- {datetime.datetime.now().astimezone().isoformat()}: {LABEL} ongoing, observed target announcements {target}; no RPM success claimed.\n')
            hourly=time.time()
        time.sleep(1)
    if proc.returncode:
        failure_dirs = [ROOT/'tmp/WEEKEND_0918/rpm-arm-llvm'/v for v in ('RPMS/armv7l','BUILDROOT','TMP','BUILDROOT/llvm-22.1.8-1.arm/usr/lib/debug/usr/lib','BUILDROOT/llvm-22.1.8-1.arm/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/lib')]
        failure_cmd=['ls','-la',*[str(v) for v in failure_dirs]]
        failure=subprocess.run(failure_cmd,capture_output=True,text=True)
        note('immediate_failure_permissions',command=failure_cmd,exitcode=failure.returncode,stdout=failure.stdout,stderr=failure.stderr)
        (OUT/'failure-permissions.txt').write_text(failure.stdout+failure.stderr)
        capture_cmd=['python3',str(BASE/'capture_failure.py'),str(OUT)]
        captured=subprocess.run(capture_cmd,capture_output=True,text=True)
        note('failure_file_snapshot',command=capture_cmd,exitcode=captured.returncode,stdout=captured.stdout,stderr=captured.stderr)
    (OUT/'exitcode').write_text(str(proc.returncode)+'\n')
    note('finished',exitcode=proc.returncode,completed=target)
    raise SystemExit(proc.returncode)
finally:
    timer.cancel()
    if proc.poll() is None: hard_stop()
