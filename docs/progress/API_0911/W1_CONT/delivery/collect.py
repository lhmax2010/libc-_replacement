#!/usr/bin/env python3
import pathlib,shutil,hashlib
base=pathlib.Path('docs/progress/API_0911');out=base/'W1_CONT/delivery';raw=out/'raw';raw.mkdir(parents=True,exist_ok=True)
for p in sorted((base/'delivery').glob('W1_CONT*')):
 if p.is_file() and 'receipt' not in p.name:shutil.copyfile(p,raw/p.name)
files=sorted(p for p in out.rglob('*') if p.is_file() and p.name!='SHA256SUMS')
(out/'SHA256SUMS').write_text(''.join(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+str(p.relative_to(out))+'\n' for p in files))
print('DELIVERY_FILES',len(files))
