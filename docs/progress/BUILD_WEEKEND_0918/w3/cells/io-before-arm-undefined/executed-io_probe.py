import hashlib,json,os,tempfile,time
from pathlib import Path
t=time.monotonic(); d=Path(tempfile.mkdtemp(prefix='io-',dir='tmp/STATIC_0917B')); p=d/'probe'
data=b'io-calibration\n'*262144
with p.open('wb') as f: f.write(data); f.flush(); os.fsync(f.fileno())
assert hashlib.sha256(p.read_bytes()).digest()==hashlib.sha256(data).digest()
elapsed=time.monotonic()-t; p.unlink(); d.rmdir()
print(json.dumps(dict(seconds=elapsed,bytes=len(data),deleted_only_new_probe=True)))
raise SystemExit(0 if elapsed<=30 else 30)
