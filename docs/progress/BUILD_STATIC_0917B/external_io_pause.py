"""Apply the agreed I/O pause to the identified owned build group only."""
import datetime
import json
import os
import signal
import subprocess
import time
from pathlib import Path
p=Path('progress/BUILD_STATIC_0917B')
events=[json.loads(x) for x in (p/'llvm-aarch64-native-rpm.events.jsonl').read_text().splitlines()]
start=next(e for e in reversed(events) if e['event']=='start')
monitor=next(e['pid'] for e in reversed(events) if e['event']=='resource_verified' and e['label']=='monitor-before-build')
deadline=start['deadline']
children=Path(f'/proc/{monitor}/task/{monitor}/children').read_text().split()
matches=[]
for value in children:
    pid=int(value)
    args=Path(f'/proc/{pid}/cmdline').read_bytes().rstrip(b'\0').split(b'\0')
    if [x.decode() for x in args]==start['command']:
        matches.append(pid)
assert len(matches)==1, matches
pid=matches[0]
assert os.getpgid(pid)==pid
identity=Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()[19]
log=(p/'external-io-pause.events.jsonl').open('a',buffering=1)
def record(event,**kw):
    row=dict(time=datetime.datetime.now().astimezone().isoformat(),event=event,**kw)
    log.write(json.dumps(row)+'\n'); print(json.dumps(row),flush=True)
def send(sig):
    assert Path(f'/proc/{pid}/stat').read_text().rsplit(')',1)[1].split()[19]==identity
    os.killpg(pid,sig)
    record('signal',pgid=pid,signal=sig.name)
def stop(reason):
    record('stop',reason=reason)
    send(signal.SIGTERM); send(signal.SIGCONT)
    for _ in range(20):
        if not Path(f'/proc/{pid}').exists():
            raise SystemExit(75)
        time.sleep(1)
    if Path(f'/proc/{pid}').exists(): send(signal.SIGKILL)
    raise SystemExit(75)
assert (p/'raw/225_io_probe.exitcode').read_text().strip()=='124'
record('identity',pid=pid,monitor=monitor,start_ticks=identity,command=start['command'],initial_probe='raw/225_io_probe',deadline=deadline)
send(signal.SIGSTOP)
for attempt in range(1,4):
    end=time.time()+600
    record('pause',attempt=attempt,seconds=600)
    while time.time()<end:
        if time.time()>=deadline: stop('fixed stage deadline during I/O pause')
        time.sleep(1)
    cmd=['python3','progress/BUILD_STATIC_0917B/io_probe.py']
    t=time.monotonic()
    child=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,start_new_session=True)
    try:
        stdout,stderr=child.communicate(timeout=min(30,max(0.1,deadline-time.time())))
        rc=child.returncode
    except subprocess.TimeoutExpired:
        os.killpg(child.pid,signal.SIGKILL)
        stdout,stderr=child.communicate()
        rc=124
    record('probe',attempt=attempt,command=cmd,exitcode=rc,stdout=stdout,stderr=stderr,seconds=time.monotonic()-t)
    if time.time()>=deadline: stop('fixed stage deadline after I/O probe')
    if rc==0:
        send(signal.SIGCONT); record('resumed',attempt=attempt)
        raise SystemExit(0)
stop('I/O probes failed after three ten-minute pauses')
