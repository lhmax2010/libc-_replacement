from common import *
import shutil
files=['FINAL.md','STATUS.md','QUESTIONS.md','W3/ERROR_CONTRACT.md','W3/DEPLOYMENT_UPDATE.md']
rows=[]
for rel in files:
 p=ROOT/'docs/progress/P13_0917'/rel;dest=OUT/'W3/before_P13'/rel
 assert not dest.exists();dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,dest)
 rows.append({'path':str(p.relative_to(ROOT)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
save('W3','BEFORE_IDENTITY.json',rows)
