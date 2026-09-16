"""Stop only this task's verified, unprivileged build process group; keep outputs."""
import json
import os
import signal
import subprocess
import time
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
pid=616478
proc=Path(f'/proc/{pid}')
args=(proc/'cmdline').read_bytes().split(b'\0')
assert proc.stat().st_uid==os.getuid()
assert args[0]==b'bwrap'
assert b'/home/toolchain/development/libc++_replacement/tmp/STATIC_0917B/llvm-root-minimal' in args
assert b'/tmp/task/rpm-aarch64/SPECS/llvm.spec' in args
assert os.getpgid(pid)==pid
identity=dict(pid=pid,uid=proc.stat().st_uid,start_ticks=(proc/'stat').read_text().rsplit(')',1)[1].split()[19],command=[x.decode() for x in args if x],reason='Await human confirmation of existing native toolchain reuse; not a build test failure or elapsed deadline')
(p/'build_pause_identity.json').write_text(json.dumps(identity,indent=2))
print(json.dumps(identity,indent=2),flush=True)
command=['/usr/bin/kill','--signal','INT','--',f'-{pid}']
print('command:',command,flush=True)
r=subprocess.run(command)
print('exitcode:',r.returncode,flush=True)
if r.returncode:
    raise SystemExit(r.returncode)
for _ in range(30):
    if not proc.exists():
        print('Verified build group leader exited; Ninja/CMake outputs retained.',flush=True)
        break
    time.sleep(1)
else:
    raise SystemExit('Build did not exit within 30 seconds; no stronger signal sent.')
