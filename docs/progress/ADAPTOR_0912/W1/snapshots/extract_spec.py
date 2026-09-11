#!/usr/bin/env python3
import tarfile,sys,pathlib,hashlib,json
rows=[]
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|') as t:
 for m in t:
  if m.isfile() and m.name.endswith('.spec'):
   data=t.extractfile(m).read();p=pathlib.Path('tmp/ADAPTOR_0912/W1')/pathlib.PurePosixPath(m.name).name;p.write_bytes(data)
   rows.append(dict(member=m.name,path=str(p),sha256=hashlib.sha256(data).hexdigest()))
pathlib.Path('docs/progress/ADAPTOR_0912/W1/spec_index.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(rows))
