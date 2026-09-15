#!/usr/bin/env python3
import difflib,hashlib,json,pathlib,shutil,sys
import p13_driver as d
b=d.b;dest=b.OUT/'code';dest.mkdir(exist_ok=True)
shutil.copytree(b.SRC,dest/'src',dirs_exist_ok=True)
scripts=['build.py','pin_runtime.py','p13_driver.py','p13_seal.py','run_ewk.py','run_boundary.py','run_units.py','run_vectors.py','allocation_check.py','allocation_control.py','package_build.py','release_check.py']
scripts += [p.name for p in b.TMP.glob('p13_*.py') if p.name not in scripts]
for name in scripts:shutil.copyfile(b.TMP/name,dest/name)
manifest={str(p.relative_to(b.OUT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sorted(dest.rglob('*'))if p.is_file()}
(b.OUT/'SNAPSHOT_SHA256.json').write_text(json.dumps(manifest,indent=2)+'\n')
before=b.ROOT/'docs/progress/P13_0917/W1/before';patch=[]
for p in sorted(dest.rglob('*')):
 if not p.is_file():continue
 relative=p.relative_to(dest);old=before/relative
 previous=old.read_text() if old.is_file() else ''
 patch.extend(difflib.unified_diff(previous.splitlines(True),p.read_text().splitlines(True),fromfile='a/tmp/P12/'+str(relative),tofile='b/tmp/P12/'+str(relative)))
(b.OUT/'ADAPTOR_CHANGES.patch').write_text(''.join(patch))
inputs=[b.PROVIDER,b.LIBCXX/'lib/libc++.so.1',b.LIBCXX/'lib/libc++abi.so.1',b.SDK/'closure_ewk/libstdc++.so.6',pathlib.Path('/usr/bin/ld.gold')]
(b.OUT/'INPUT_SHA256.json').write_text(json.dumps({str(p):hashlib.sha256(p.read_bytes()).hexdigest()for p in inputs},indent=2)+'\n')
print('STAGE',d.stage,'SNAPSHOT_FILES',len(manifest),'NO_RUNTIME_REBUILD')
