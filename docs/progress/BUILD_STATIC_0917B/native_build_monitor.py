"""One owned process group, job=1, inherited limits; record and enforce deadline/I/O pauses."""
import argparse, datetime, hashlib, json, os, re, signal, subprocess, time
from pathlib import Path

p=argparse.ArgumentParser(); p.add_argument('--label',required=True); p.add_argument('--deadline',required=True,type=float); p.add_argument('command',nargs=argparse.REMAINDER); a=p.parse_args()
cmd=a.command[1:] if a.command[:1]==['--'] else a.command
out=Path('progress/BUILD_STATIC_0917B'); log=out/(a.label+'.build.log')
if log.exists():
    n=1
    while (out/(a.label+f'.previous-{n}.build.log')).exists(): n+=1
    log.rename(out/(a.label+f'.previous-{n}.build.log'))
events=(out/(a.label+'.events.jsonl')).open('a',buffering=1)
def note(kind,**kw):
    row=dict(time=datetime.datetime.now().astimezone().isoformat(),event=kind,**kw); events.write(json.dumps(row)+'\n'); print(json.dumps(row),flush=True)
note('start',command=cmd,deadline=a.deadline,monitor_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
MEMORY_MAX=16536449024
SCOPE='build-static-0917b-native-rpm.scope'
def resource_check(pid, label):
    catcmd=['cat',f'/proc/{pid}/cgroup']
    r=subprocess.run(catcmd,capture_output=True,text=True)
    note('resource_command',label=label,command=catcmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    assert r.returncode==0
    cg=next(line.split(':',2)[2] for line in r.stdout.splitlines() if line.startswith('0::'))
    assert SCOPE in cg
    memorycmd=['cat','/sys/fs/cgroup'+cg+'/memory.max']
    r=subprocess.run(memorycmd,capture_output=True,text=True)
    note('resource_command',label=label,command=memorycmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    assert r.returncode==0 and int(r.stdout.strip())==MEMORY_MAX
    io=subprocess.run(['ionice','-p',str(pid)],capture_output=True,text=True)
    note('resource_command',label=label,command=['ionice','-p',str(pid)],exitcode=io.returncode,stdout=io.stdout,stderr=io.stderr)
    assert io.returncode==0 and io.stdout.strip()=='idle'
    assert os.getpriority(os.PRIO_PROCESS,pid)==19
    note('resource_verified',label=label,pid=pid,cgroup=cg,memory_max=MEMORY_MAX,nice=19,ionice='idle')
    return cg
scope_path=resource_check(os.getpid(),'monitor-before-build')
stream=log.open('wb',buffering=0); proc=subprocess.Popen(cmd,stdout=stream,stderr=subprocess.STDOUT,start_new_session=True)
def stop(reason):
    note('stop',reason=reason)
    try: os.killpg(proc.pid,signal.SIGCONT); os.killpg(proc.pid,signal.SIGTERM)
    except ProcessLookupError: pass
    try: proc.wait(timeout=20)
    except subprocess.TimeoutExpired: os.killpg(proc.pid,signal.SIGKILL); proc.wait()
    raise SystemExit(124)
def probe():
    path=out/(a.label+'.io.log')
    with path.open('ab') as f:
        test=subprocess.Popen(['python3','progress/BUILD_STATIC_0917B/io_probe.py'],stdout=f,stderr=f,start_new_session=True)
        try: rc=test.wait(timeout=30)
        except subprocess.TimeoutExpired:
            os.killpg(test.pid,signal.SIGTERM); note('io_timeout',pid=test.pid); return False
    note('io_result',exitcode=rc); return rc==0
reader=log.open(errors='replace'); target=0; next_check=0; last=0; heartbeat=0; ninja_verified=False
try:
    while proc.poll() is None:
        if time.time()>=a.deadline: stop('stage deadline')
        if not ninja_verified:
            for rawpid in Path('/sys/fs/cgroup'+scope_path+'/cgroup.procs').read_text().split():
                pid=int(rawpid)
                try:
                    args=Path(f'/proc/{pid}/cmdline').read_bytes().split(b'\0')
                except FileNotFoundError:
                    continue
                is_ninja=bool(args) and (Path(args[0].decode()).name in ('ninja','ninja.real') or (b'qemu-' in args[0] and any(x.endswith(b'/ninja.real') for x in args)))
                if is_ninja and b'-t' not in args:
                    note('ninja_observed',pid=pid,command=[x.decode() for x in args if x])
                    if b'--version' in args or b'--help' in args:
                        continue
                    one=(b'-j1' in args or (b'-j' in args and args[args.index(b'-j')+1]==b'1'))
                    if not one: stop('actual Ninja is not -j1')
                    try:
                        resource_check(pid,'actual-ninja')
                    except Exception as error:
                        note('resource_failed',error=str(error)); stop('actual Ninja resource guard failed')
                    note('ninja_command_verified',pid=pid,command=[x.decode() for x in args if x])
                    ninja_verified=True
                    break
        for line in reader.readlines():
            m=re.search(r'\[(\d+)/(\d+)\]',line)
            if m: target=max(target,int(m[1]))
        if target>=next_check:
            next_check+=500
            if not probe():
                os.killpg(proc.pid,signal.SIGSTOP); note('paused',target=target)
                for attempt in range(1,4):
                    end=time.time()+600
                    while time.time()<end:
                        if time.time()>=a.deadline: stop('deadline while paused')
                        time.sleep(1)
                    if probe(): os.killpg(proc.pid,signal.SIGCONT); note('resumed',attempt=attempt); break
                else: stop('three I/O pauses remained slow')
        if time.time()-last>=45: note('running',completed=target); last=time.time()
        if time.time()-heartbeat>=3600:
            with (out/'HOURLY_STATUS.md').open('a') as f: f.write(f'\n- {datetime.datetime.now().astimezone().isoformat()}: {a.label}, completed={target}, running; deadline={a.deadline}.\n')
            heartbeat=time.time()
        time.sleep(1)
    note('finished',exitcode=proc.returncode,completed=target)
    raise SystemExit(proc.returncode)
finally:
    if proc.poll() is None:
        try: os.killpg(proc.pid,signal.SIGCONT); os.killpg(proc.pid,signal.SIGTERM)
        except ProcessLookupError: pass
