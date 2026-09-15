import os
import signal
import subprocess
from pathlib import Path

scope = 'build-w2c-llvm-arm-retry.scope'
cg = Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice') / scope
for f in cg.rglob('cgroup.procs'):
    for value in f.read_text().split():
        try:
            if (Path('/proc') / value).stat().st_uid == os.getuid():
                os.kill(int(value), signal.SIGTERM)
                print('SIGTERM', value, flush=True)
        except ProcessLookupError:
            pass
r = subprocess.run(['systemctl', '--user', 'thaw', scope])
print('thaw_exit', r.returncode)
raise SystemExit(r.returncode)
