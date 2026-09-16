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
reader=log.open(errors='replace'); target=0; next_check=0; last=0; heartbeat=0
try:
    while proc.poll() is None:
        if time.time()>=a.deadline: stop('stage deadline')
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
