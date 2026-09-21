"""Verify prior source contents remain unchanged after publication and audit."""
import hashlib,json,pathlib,subprocess
b=pathlib.Path('progress/ARM_LZMA_0921B')
cg=pathlib.Path('/proc/self/cgroup').read_text()
rel=next(x.split(':',2)[2] for x in cg.splitlines() if x.startswith('0::'))
limit=(pathlib.Path('/sys/fs/cgroup')/rel.lstrip('/')/'memory.max').read_text().strip()
assert limit=='16536457216'
manifest=json.loads(pathlib.Path('docs/progress/ARM_LZMA_0921/DELIVERY_MANIFEST.json').read_text())
for record in manifest['records']:
 p=pathlib.Path(record['original']);h=hashlib.sha256()
 with p.open('rb') as f:
  for chunk in iter(lambda:f.read(1024*1024),b''):h.update(chunk)
 assert h.hexdigest()==record['sha256'],str(p)
assert len(list(p for p in pathlib.Path('progress/ARM_LZMA_0921').rglob('*') if p.is_file()))==manifest['files']
local=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
assert local=='7c31ad73f48878cf77375fe137277d248be56481'
r={'original_files_unchanged':manifest['files'],'local_commit':local,'remote_receipt':'raw/009_remote_receipt.stdout.txt','this_check_actual_cgroup':cg,'this_check_memory_max':limit,'no_reproduction_launched':True,'scope':'Read-only comparison of previously published source contents'}
(b/'DELIVERY_VERIFIED.json').write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(r,ensure_ascii=False,indent=2))
