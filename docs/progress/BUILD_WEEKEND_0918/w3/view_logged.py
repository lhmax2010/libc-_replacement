"""Show completed diagnostic output while retaining the literal command log."""
import subprocess,sys
from pathlib import Path
label,*args=sys.argv[1:]
root=Path('progress/BUILD_WEEKEND_0918')
r=subprocess.run([sys.executable,str(root/'log.py'),label,*args])
for suffix,stream in (('.stdout.txt',sys.stdout),('.stderr.txt',sys.stderr)):
    f=root/'raw'/(label+suffix)
    if f.exists():
        stream.write(f.read_text(errors='replace')); stream.flush()
raise SystemExit(r.returncode)
