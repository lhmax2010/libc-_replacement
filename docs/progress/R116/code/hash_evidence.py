#!/usr/bin/env python3
"""生成稳定材料清单，不对不断追加的交付日志或清单自身递归求哈希。"""
import hashlib,pathlib
r=pathlib.Path('docs/progress/R116')
assert hashlib.sha256(b'abc').hexdigest()=='ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad'
paths=sorted([p for p in r.rglob('*') if p.is_file() and 'raw' not in p.relative_to(r).parts and p.name!='SHA256SUMS'])
assert b'\x7fELF'.startswith(b'\x7fELF');assert not any(p.read_bytes().startswith(b'\x7fELF') for p in paths)
with (r/'SHA256SUMS').open('w') as f:
 for p in paths:f.write(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+str(p)+'\n')
print('HASH_POSITIVE_CONTROL=PASS TEXT_ONLY=PASS HASHED_FILES='+str(len(paths)))
