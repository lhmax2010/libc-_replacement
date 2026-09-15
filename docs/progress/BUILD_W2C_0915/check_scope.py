"""Inspect actual scope membership and priorities, without process arguments."""
import json
import os
import re
import subprocess
import sys
from pathlib import Path

unit = sys.argv[1]
assert re.fullmatch(r'build-w2c-(bcc-tools|bpftrace)-(armv7l|aarch64)-(libcxx|gcc)', unit)
cmd = ['systemctl','--user','show',unit+'.scope','-p','ControlGroup','--value']
result = subprocess.run(cmd, text=True, capture_output=True)
print(json.dumps(dict(command=cmd, exitcode=result.returncode, stdout=result.stdout, stderr=result.stderr)))
result.check_returncode()
cg = Path('/sys/fs/cgroup') / result.stdout.strip().lstrip('/')
assert cg.is_dir()
mem_kib = int(re.search(r'MemTotal:\s+(\d+)', Path('/proc/meminfo').read_text())[1])
cap = int((cg/'memory.max').read_text())
assert 0 < cap <= mem_kib*1024//2
print(json.dumps(dict(cgroup=str(cg), memory_max=cap, memory_current=(cg/'memory.current').read_text().strip())))
observed = 0
for file in cg.rglob('cgroup.procs'):
    for value in file.read_text().split():
        proc = Path('/proc')/value
        try:
            name = (proc/'comm').read_text().strip()
            nice = os.getpriority(os.PRIO_PROCESS, int(value))
        except (FileNotFoundError, ProcessLookupError):
            print(json.dumps(dict(pid=value, status='NOT_OBSERVED', reason='process exited during inspection')))
            continue
        command = ['ionice','-p',value]
        io = subprocess.run(command, text=True, capture_output=True)
        print(json.dumps(dict(pid=value, name=name, nice=nice, command=command,
                             exitcode=io.returncode, stdout=io.stdout, stderr=io.stderr)))
        if io.returncode and not proc.exists():
            continue
        io.check_returncode()
        assert nice == 19 and io.stdout.strip() == 'idle'
        observed += 1
assert observed
print(f'PASS observed_processes={observed}')
