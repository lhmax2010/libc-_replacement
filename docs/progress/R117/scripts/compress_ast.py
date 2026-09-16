from common import *
import gzip,shutil
rows=[]
for p in sorted((OUT/'raw').glob('ast_*.stdout')):
 datahash=hashlib.sha256(p.read_bytes()).hexdigest()
 dest=pathlib.Path(str(p)+'.gz')
 with p.open('rb') as f,gzip.open(dest,'wb',compresslevel=6) as g:shutil.copyfileobj(f,g)
 checked=hashlib.sha256()
 with gzip.open(dest,'rb') as f:
  for chunk in iter(lambda:f.read(1024*1024),b''):checked.update(chunk)
 assert checked.hexdigest()==datahash
 rows.append({'original':str(p.relative_to(ROOT)),'bytes':p.stat().st_size,'sha256':datahash,'archive':str(dest.relative_to(ROOT)),'archive_sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'restoration':'gzip -dc ARCHIVE > ORIGINAL; exact byte preservation verified'})
 p.unlink() # recoverable byte-for-byte from checked gzip; no platform data removed
save('headers/AST_ARCHIVES.json',rows)
