"""Probe actual directory currently blocking the build, in addition to new-file I/O."""
import json
import os
import time
from pathlib import Path
p=Path('tmp/STATIC_0917B/rpm-aarch64/BUILD/llvm-22.1.8/lldb/test/API/commands/statistics/basic')
t=time.monotonic()
with os.scandir(p) as it:
    names=sorted(entry.name for entry in it)
print(json.dumps(dict(path=str(p),seconds=time.monotonic()-t,entries=names)),flush=True)
