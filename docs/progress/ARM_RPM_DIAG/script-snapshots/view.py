import subprocess,sys
from pathlib import Path
label,*command=sys.argv[1:]; out=Path('progress/ARM_RPM_DIAG/raw')
r=subprocess.run([sys.executable,'progress/ARM_RPM_DIAG/log.py',label,*command])
for suffix,stream in (('.stdout.txt',sys.stdout),('.stderr.txt',sys.stderr)):
    f=out/(label+suffix)
    if f.exists(): stream.write(f.read_text(errors='replace')); stream.flush()
raise SystemExit(r.returncode)
