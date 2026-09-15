"""Extra I/O checkpoint for an observed blocked compiler before target 500."""
import argparse
import datetime
import json
import os
import signal
import subprocess
import time
from pathlib import Path

p = Path('/home/toolchain/development/libc++_replacement/progress/BUILD_W2C_0915')
parser = argparse.ArgumentParser()
parser.add_argument('--label', required=True)
parser.add_argument('--probe-script', default='io_probe.py')
args = parser.parse_args()
assert args.label.replace('-', '').isalnum()
assert '/' not in args.probe_script
unit = 'build-w2c-llvm-confirmed.scope'
cg = Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice')/unit
deadline = datetime.datetime.fromisoformat('2026-09-16T03:51:49+08:00').timestamp()
events = (p/f'{args.label}.events.jsonl').open('x', buffering=1)
def note(event, **kwargs):
    value = dict(time=datetime.datetime.now().astimezone().isoformat(), event=event, **kwargs)
    line = json.dumps(value, ensure_ascii=False)
    events.write(line+'\n')
    print(line, flush=True)
def control(action):
    cmd = ['systemctl', '--user', action, unit]
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        note('control', command=cmd, exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr)
        assert r.returncode == 0
    except subprocess.TimeoutExpired:
        note('control_timeout', command=cmd)
        if action != 'freeze' or (cg/'cgroup.freeze').read_text().strip() != '1':
            raise
def stop():
    note('stop_after_retries_or_deadline')
    for value in (cg/'cgroup.procs').read_text().split():
        try:
            if (Path('/proc')/value).stat().st_uid == os.getuid():
                os.kill(int(value), signal.SIGTERM)
        except (FileNotFoundError, ProcessLookupError):
            pass
    control('thaw')
    raise SystemExit(75)
probe = None
def check(number):
    global probe
    if probe and probe.poll() is None:
        note('previous_probe_still_waiting')
        return False
    cmd = ['nice', '-n', '19', 'ionice', '-c', '3', 'python3', str(p/args.probe_script)]
    with (p/f'{args.label}_probe_{number}.log').open('xb') as stream:
        probe = subprocess.Popen(cmd, stdout=stream, stderr=subprocess.STDOUT)
    note('probe', number=number, command=cmd, pid=probe.pid)
    try:
        code = probe.wait(timeout=30)
        note('probe_exit', number=number, exitcode=code)
        return code == 0
    except subprocess.TimeoutExpired:
        note('probe_timeout', number=number, seconds=30)
        probe.terminate()
        return False
if check(0):
    raise SystemExit(0)
control('freeze')
note('scope_freezer', value=(cg/'cgroup.events').read_text())
for attempt in range(1, 4):
    note('pause_start', attempt=attempt, seconds=600)
    until = time.monotonic()+600
    last = 0.0
    while time.monotonic() < until:
        if time.time() >= deadline:
            stop()
        if time.monotonic()-last >= 45:
            note('paused', attempt=attempt, remaining_seconds=round(until-time.monotonic()))
            last = time.monotonic()
        time.sleep(1)
    if check(attempt):
        control('thaw')
        note('resumed')
        raise SystemExit(0)
stop()
