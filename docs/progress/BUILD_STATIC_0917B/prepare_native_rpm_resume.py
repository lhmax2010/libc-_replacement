import datetime
import hashlib
import json
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
assert json.loads((p/'native-equivalence-summary.json').read_text())['status']=='PASS'
assert not Path('password.md').exists()
old=json.loads((p/'llvm_full_rpmbuild_command.json').read_text())
sep=old.index('--')
emul=Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/emul').resolve()
cmd=old[:sep]+['--ro-bind',str(emul),'/emul']+old[sep:]
for i,x in enumerate(cmd):
    if x.startswith('__cc /usr/bin/') or x.startswith('__cxx /usr/bin/'):
        cmd[i]=x.replace(' /usr/bin/',' /emul/usr/bin/',1)
cmd.insert(cmd.index('-bb')+1,'--noprep')
(p/'native_rpm_resume_command.json').write_text(json.dumps(cmd,indent=2))
snapshot=Path('tmp/STATIC_0917B/native-equivalence/pre-native-build-state')
snapshot.mkdir(exist_ok=False)
b=Path('tmp/STATIC_0917B/rpm-aarch64/BUILD/llvm-22.1.8/build')
manifest=[]
for name in ['CMakeCache.txt','build.ninja','.ninja_log']:
    data=(b/name).read_bytes()
    (snapshot/name).write_bytes(data)
    manifest.append(dict(path=str(snapshot/name),sha256=hashlib.sha256(data).hexdigest()))
(p/'pre-native-build-state.json').write_text(json.dumps(manifest,indent=2))
print('Prepared full rpmbuild --noprep -bb from successfully completed prep tree; no --short-circuit, no spec changes. Compiler path changed to verified native toolchain; CMake will reconfigure.')
