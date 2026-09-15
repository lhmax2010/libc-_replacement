"""Run one build in a memory-limited scope and monitor completed targets/I/O."""
import argparse
import atexit
import datetime
import json
import os
import re
import shlex
import signal
import subprocess
import time
from pathlib import Path

ROOT = Path('/home/toolchain/development/libc++_replacement')
OUT = ROOT / 'progress/BUILD_W2C_0915'
p = argparse.ArgumentParser()
p.add_argument('--unit', required=True)
p.add_argument('--stdin-file')
p.add_argument('--inner-sentinel', action='store_true')
p.add_argument('--max-ninja-total', type=int)
p.add_argument('command', nargs=argparse.REMAINDER)
a = p.parse_args()
command = a.command[1:] if a.command[:1] == ['--'] else a.command
assert command
mem_kib = int(re.search(r'MemTotal:\s+(\d+)', Path('/proc/meminfo').read_text()).group(1))
cap = mem_kib * 1024 // 2
deadline = datetime.datetime.fromisoformat('2026-09-16T03:08:00+08:00').timestamp()
log_path = OUT / (a.unit + '.build.log')
events = (OUT / (a.unit + '.events.jsonl')).open('a', buffering=1)
def note(event, **kw):
    record = dict(time=datetime.datetime.now().astimezone().isoformat(), event=event, **kw)
    events.write(json.dumps(record, ensure_ascii=False) + '\n')
    print(json.dumps(record, ensure_ascii=False), flush=True)
def ctl(*args):
    cmd = ['systemctl', '--user', *args]
    r = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=20)
    note('control', command=shlex.join(cmd), exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr)
    if r.returncode:
        raise RuntimeError('scope control failed')
    return r.stdout.strip()
unit = a.unit + '.scope'
run = ['systemd-run', '--user', '--scope', '--unit=' + a.unit,
       '-p', f'MemoryMax={cap}', 'nice', '-n', '19', 'ionice', '-c', '3', *command]
note('start', command=shlex.join(run), requested_memory_bytes=cap, memory_total_kib=mem_kib,
     stdin=a.stdin_file, deadline_unix=deadline)
stdin = open(a.stdin_file, 'rb') if a.stdin_file else subprocess.DEVNULL
log_stream = log_path.open('wb', buffering=0)
build = subprocess.Popen(run, stdin=stdin, stdout=log_stream, stderr=subprocess.STDOUT)
def emergency_cleanup():
    if build.poll() is None:
        if cg is not None and cg.exists():
            for f in cg.rglob('cgroup.procs'):
                for value in f.read_text().split():
                    try:
                        if (Path('/proc') / value).stat().st_uid == os.getuid():
                            os.kill(int(value), signal.SIGTERM)
                    except (ProcessLookupError, FileNotFoundError, PermissionError):
                        pass
        for args in (('kill', '--signal=SIGTERM', unit), ('thaw', unit)):
            try:
                r = subprocess.run(['systemctl', '--user', *args], capture_output=True,
                                   text=True, timeout=15)
                note('cleanup_control', command=list(args), exitcode=r.returncode,
                     stderr=r.stderr)
            except Exception as exc:
                note('cleanup_error', error=str(exc))
atexit.register(emergency_cleanup)
cg = None
for _ in range(30):
    if build.poll() is not None:
        break
    r = subprocess.run(['systemctl', '--user', 'show', unit, '-p', 'ControlGroup', '--value'],
                       stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True)
    if r.stdout.strip():
        cg = Path('/sys/fs/cgroup') / r.stdout.strip().lstrip('/')
        break
    time.sleep(1)
if cg is None:
    raise RuntimeError('scope did not become available')
actual_cap = int((cg / 'memory.max').read_text())
assert 0 < actual_cap <= cap
note('memory_verified', cgroup=str(cg), memory_max=actual_cap)
def snapshot():
    records = []
    if not cg.exists():
        return
    for f in cg.rglob('cgroup.procs'):
        for value in f.read_text().split():
            try:
                pid = int(value)
                proc = Path('/proc') / value
                records.append(dict(pid=pid, name=(proc/'comm').read_text().strip(),
                                    nice=os.getpriority(os.PRIO_PROCESS, pid),
                                    cgroup=(proc/'cgroup').read_text().strip()))
            except (FileNotFoundError, ProcessLookupError):
                pass
    note('scope_processes', processes=records,
         memory_current=(cg/'memory.current').read_text().strip())
def freeze():
    ctl('freeze', unit)
    for _ in range(20):
        if (cg/'cgroup.freeze').read_text().strip() == '1':
            break
        time.sleep(0.1)
    assert (cg/'cgroup.freeze').read_text().strip() == '1'
    note('pause_requested', freezer=(cg/'cgroup.events').read_text())
def thaw():
    ctl('thaw', unit)
    assert (cg/'cgroup.freeze').read_text().strip() == '0'
    note('resumed')
def stop(reason):
    note('stopping', reason=reason)
    if cg.exists():
        # Queue termination before thaw so paused tasks cannot resume normal work.
        for f in cg.rglob('cgroup.procs'):
            for value in f.read_text().split():
                try:
                    if (Path('/proc') / value).stat().st_uid == os.getuid():
                        os.kill(int(value), signal.SIGTERM)
                except (ProcessLookupError, FileNotFoundError, PermissionError):
                    pass
        if (cg/'cgroup.freeze').read_text().strip() == '1':
            thaw()
    until = time.monotonic() + 60
    while build.poll() is None and time.monotonic() < until:
        time.sleep(1)
    if build.poll() is None:
        ctl('kill', '--signal=SIGKILL', unit)
    raise RuntimeError(reason)
probe_counter = 0
probe = None
def probe_once():
    global probe, probe_counter
    if probe is not None and probe.poll() is None:
        note('probe_still_in_io_wait')
        return False
    probe_counter += 1
    file = OUT / f'{a.unit}.io_{probe_counter:02d}.log'
    cmd = ['nice', '-n', '19', 'ionice', '-c', '3', 'python3', str(OUT/'io_probe.py')]
    with file.open('wb') as stream:
        probe = subprocess.Popen(cmd, stdout=stream, stderr=subprocess.STDOUT)
    note('probe_start', command=shlex.join(cmd), log=str(file))
    until = time.monotonic() + 30
    while probe.poll() is None and time.monotonic() < until:
        time.sleep(0.25)
    if probe.poll() is None:
        note('probe_timeout', seconds=30, pid=probe.pid)
        probe.terminate()
        return False
    note('probe_end', exitcode=probe.returncode, log=str(file))
    return probe.returncode == 0
def checkpoint():
    if probe_once():
        return
    freeze()
    for pause in range(1, 4):
        note('pause_wait', attempt=pause, seconds=600)
        until = time.monotonic() + 600
        last = 0.0
        while time.monotonic() < until:
            if time.time() >= deadline:
                stop('12-hour deadline during I/O pause')
            if time.monotonic() - last >= 45:
                note('paused', attempt=pause, remaining_seconds=round(until-time.monotonic()))
                last = time.monotonic()
            time.sleep(1)
        if probe_once():
            thaw()
            return
    stop('I/O probes failed after three 10-minute pauses')

# Freezer support has been calibrated separately before starting this build.
snapshot()
reader = log_path.open('r', errors='replace')
completed = 0
make_completed = 0
next_checkpoint = 500
last_status = 0.0
early_snapshot = time.monotonic() + 15
inner_exit = None
while build.poll() is None:
    if early_snapshot and time.monotonic() >= early_snapshot:
        snapshot()
        early_snapshot = None
    if time.time() >= deadline:
        stop('12-hour deadline')
    for line in reader.readlines():
        match = re.search(r'\[(\d+)/(\d+)\]', line)
        if match and int(match[2]) >= 500:
            if a.max_ninja_total and int(match[2]) > a.max_ninja_total:
                stop('unexpected Ninja total: cached build would be restarted')
            completed = max(completed, int(match[1]))
        if re.search(r'Building (CXX|C) object|Linking (CXX|C) ', line) and not match:
            make_completed += 1
            completed = max(completed, make_completed)
        sent = re.search(r'W2C_INNER_EXIT=(\d+)', line)
        if sent:
            inner_exit = int(sent[1])
    if completed >= next_checkpoint:
        note('target_checkpoint', completed=completed, threshold=next_checkpoint)
        snapshot()
        checkpoint()
        next_checkpoint += 500
    if time.monotonic()-last_status >= 45:
        note('running', completed=completed, next_probe=next_checkpoint,
             memory_current=(cg/'memory.current').read_text().strip() if cg.exists() else None)
        last_status = time.monotonic()
    time.sleep(1)
log_stream.close()
all_text = log_path.read_text(errors='replace')
if a.inner_sentinel:
    found = re.findall(r'W2C_INNER_EXIT=(\d+)', all_text)
    inner_exit = int(found[-1]) if found else None
exitcode = build.returncode if build.returncode else (inner_exit if a.inner_sentinel else 0)
note('finished', outer_exitcode=build.returncode, inner_exitcode=inner_exit, result=exitcode)
raise SystemExit(exitcode if exitcode is not None else 90)
