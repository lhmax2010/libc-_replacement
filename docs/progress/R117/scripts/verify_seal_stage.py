from common import *
import csv
for r in csv.DictReader((OUT/'SHA256SUMS.tsv').open(),delimiter='\t'):
 p=OUT/r['path'];h=hashlib.sha256()
 with p.open('rb') as f:
  for data in iter(lambda:f.read(1024*1024),b''):h.update(data)
 assert p.stat().st_size==int(r['bytes']) and h.hexdigest()==r['sha256'],r['path']
staged=subprocess.check_output(['git','diff','--cached','--name-only','-z'],cwd=ROOT).decode().split('\0');staged=[p for p in staged if p]
assert staged and all(p.startswith('docs/progress/R117/') for p in staged),staged
assert subprocess.check_output(['git','branch','--show-current'],cwd=ROOT).decode().strip()=='codex/runtime-validation'
for file,expected in [('stdout','dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'),('stderr','94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c')]:
 assert hashlib.sha256((ROOT/('docs/progress/R115/W3/raw/033_angle_files.'+file)).read_bytes()).hexdigest()==expected
print('SEAL_AND_STAGE_PASS',len(staged),'files; only docs/progress/R117; original R115 changes preserved')
