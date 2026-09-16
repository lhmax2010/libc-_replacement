"""Bounded W2 copy only: inventory, size gate, limited cp, I/O pause control.

No credential input is opened here. sudo alone consumes stdin. Ownership
restoration and credential removal are performed by separate recorded commands.
"""
import datetime
import json
import os
from pathlib import Path
import resource
import selectors
import shlex
import signal
import subprocess
import sys
import time

workspace=Path('/home/toolchain/development/libc++_replacement')
source=workspace/'tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0'
dest=workspace/'tmp/STATIC_0917B/llvm-root-minimal'
names=['usr','etc','var','.build','emul','bin','sbin','lib','lib64']
paths=[str(source/name) for name in names]
limit_kib=int(next(line.split()[1] for line in Path('/proc/meminfo').read_text().splitlines() if line.startswith('MemTotal:')))//2
limit_bytes=limit_kib*1024
deadline=datetime.datetime.fromisoformat((workspace/'progress/BUILD_STATIC_0917B/raw/037_sudo_copy.started.txt').read_text().strip()).timestamp()+4*3600

def note(event,**fields):
    print(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),event=event,**fields)),flush=True)

def run(command):
    note('command',command=shlex.join(command))
    r=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,timeout=max(1,deadline-time.time()))
    print(r.stdout,end='',flush=True)
    print(r.stderr,end='',file=sys.stderr,flush=True)
    note('exit',exitcode=r.returncode)
    if r.returncode:
        raise SystemExit(r.returncode)
    return r.stdout

assert os.geteuid()==0
assert resource.getrlimit(resource.RLIMIT_AS)==(limit_bytes,limit_bytes)
assert os.getpriority(os.PRIO_PROCESS,0)==19
assert not dest.exists() and not dest.is_symlink(), 'Never overwrite an existing destination'
assert source.resolve()==source
note('limits_after_sudo',method='prlimit RLIMIT_AS',bytes=limit_bytes,soft=resource.getrlimit(resource.RLIMIT_AS)[0],hard=resource.getrlimit(resource.RLIMIT_AS)[1])
run(['/usr/bin/ionice','-p',str(os.getpid())])
run(['/usr/bin/du','-sh','--',*paths])
sizes=run(['/usr/bin/du','-s','--block-size=1','--',*paths])
total=sum(int(line.split()[0]) for line in sizes.splitlines())
note('size_gate',allocated_bytes=total,maximum_bytes=3*1024**3,excluded=['home','tmp','root','installed-pkg','opt','apps','boot','product','vendor','history/source/cache files'])
if total>3*1024**3:
    note('stop',reason='More than 3 GiB; no copy started')
    raise SystemExit(30)
if time.time()>=deadline:
    raise SystemExit(124)

dest.mkdir(mode=0o755)
for name in ['home/abuild','root','dev','proc','sys','tmp','run']:
    (dest/name).mkdir(parents=True,exist_ok=True)
probe_code="""import hashlib,json,os,sys,tempfile,time
from pathlib import Path
t=time.monotonic(); fd,name=tempfile.mkstemp(prefix='copy-io-',dir=sys.argv[1]); p=Path(name)
try:
 data=b'io-calibration\\n'*262144
 with os.fdopen(fd,'wb') as f: f.write(data); f.flush(); os.fsync(f.fileno())
 assert hashlib.sha256(p.read_bytes()).digest()==hashlib.sha256(data).digest()
 seconds=time.monotonic()-t; print(json.dumps(dict(seconds=seconds,bytes=len(data))),flush=True)
finally: p.unlink(missing_ok=True)
raise SystemExit(0 if seconds<=30 else 30)
"""
pending_probe=None
def probe():
    global pending_probe
    if pending_probe is not None and pending_probe.poll() is None:
        note('probe_still_exiting',pid=pending_probe.pid)
        return False
    command=['/usr/bin/python3','-c',probe_code,str(dest/'tmp')]
    note('io_command',command=shlex.join(command))
    pending_probe=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,start_new_session=True)
    try:
        out,err=pending_probe.communicate(timeout=30)
    except subprocess.TimeoutExpired:
        os.killpg(pending_probe.pid,signal.SIGTERM)
        note('io_timeout',pid=pending_probe.pid,exitcode='NOT_OBSERVED',seconds=30)
        return False
    note('io_result',exitcode=pending_probe.returncode,stdout=out,stderr=err)
    return pending_probe.returncode==0

command=['/usr/bin/cp','-av','--reflink=auto','--one-file-system','--',*paths,str(dest)]
note('copy_command',command=shlex.join(command))
proc=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,start_new_session=True)
note('copy_child',pid=proc.pid,limits=Path(f'/proc/{proc.pid}/limits').read_text())
selector=selectors.DefaultSelector(); selector.register(proc.stdout,selectors.EVENT_READ)
entries=0; next_probe=0; last_status=0; buffer=b''

def stop(reason):
    note('copy_stop',reason=reason,pid=proc.pid)
    try:
        os.killpg(proc.pid,signal.SIGCONT)
        os.killpg(proc.pid,signal.SIGTERM)
    except ProcessLookupError:
        pass
    try:
        rc=proc.wait(timeout=20)
        note('copy_exit',exitcode=rc)
    except subprocess.TimeoutExpired:
        os.killpg(proc.pid,signal.SIGKILL)
        note('copy_exit',exitcode='NOT_OBSERVED',signal_sent='SIGKILL')
    raise SystemExit(124)

while proc.poll() is None:
    if time.time()>=deadline:
        stop('W2 four-hour deadline')
    for key,_ in selector.select(timeout=1):
        chunk=os.read(key.fd,65536)
        if not chunk:
            selector.unregister(key.fileobj)
            continue
        buffer+=chunk
        while b'\n' in buffer:
            line,buffer=buffer.split(b'\n',1)
            print(line.decode(errors='replace'),flush=True)
            entries+=1
    if entries>=next_probe:
        next_probe=entries+500
        if not probe():
            try: os.killpg(proc.pid,signal.SIGSTOP)
            except ProcessLookupError: break
            note('copy_paused',entries=entries)
            for attempt in range(1,4):
                until=time.time()+600
                while time.time()<until:
                    if time.time()>=deadline: stop('deadline during I/O pause')
                    time.sleep(1)
                if probe():
                    os.killpg(proc.pid,signal.SIGCONT)
                    note('copy_resumed',attempt=attempt)
                    break
                note('io_pause_failed',attempt=attempt)
            else:
                stop('I/O still slow after three ten-minute pauses')
    if time.time()-last_status>=45:
        note('copy_progress',entries=entries)
        last_status=time.time()
remaining=proc.stdout.read()
if buffer or remaining: print((buffer+remaining).decode(errors='replace'),end='',flush=True)
note('copy_exit',exitcode=proc.returncode,entries=entries)
raise SystemExit(proc.returncode)
