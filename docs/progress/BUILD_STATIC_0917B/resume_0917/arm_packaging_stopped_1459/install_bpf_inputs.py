"""Install verified static development RPMs into the private build input root.

No dependency bypass; no host install; no credentials or sudo. Existing LLVM
shared packages remain the previously tested input set, identified in rpm -qa.
"""
import hashlib
import json
import shlex
import subprocess
import sys
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917'
task=p/'tmp/STATIC_0917B'; arch=sys.argv[1]
assert arch in ('armv7l','aarch64')
base=json.loads((out/f'bpf-private-{arch}-base.json').read_text())
rows=[]; records=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    records.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/f'bpf-install-{arch}-commands.json').write_text(json.dumps(records,indent=2))
    print(shlex.join(cmd), 'exitcode='+str(r.returncode), flush=True)
    print(r.stdout,end='',flush=True); print(r.stderr,end='',file=sys.stderr,flush=True)
    assert r.returncode==0
    return r.stdout

runtime=(p/'tmp/NIGHT_0917/finish_validation/rpm/RPMS/aarch64' if arch=='aarch64'
         else task/'rpm-arm-runtime/RPMS/armv7l')
llvm=(task/'rpm-aarch64/RPMS/aarch64' if arch=='aarch64' else task/'rpm-arm-llvm/RPMS/armv7l')
files=sorted(runtime.glob('libc++*.rpm'))
assert len(files)==4
static=list(llvm.glob('llvm-static-devel-*.rpm')); assert len(static)==1
files+=static
old=json.loads((p/'progress/BUILD_NIGHT_0916/repository_static_packages.json').read_text())['records']
expected={}
for r in old:
    if r['arch']==arch and r['package'] in ('libbpf-static','liblzma-static','zlib-devel-static','libffi-devel-static'):
        f=(p/r['path']).resolve(); files.append(f); expected[f]=r['sha256']
bcc=[r for r in json.loads((p/'progress/BUILD_W2C_0915/bcc_inputs_for_bpftrace.json').read_text())
     if r['mode']=='libcxx' and r['arch']==arch and '/bcc-tools-static-' in r['path']]
assert len(bcc)==1
f=Path(bcc[0]['path']); files.append(f); expected[f]=bcc[0]['sha256']
guest=[]
for i,f in enumerate(files):
    f=f.resolve(strict=True)
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    if f in expected: assert sha==expected[f]
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
    assert identity.endswith('\t'+arch)
    run(['rpm','--checksig','--nosignature',str(f)])
    g=f'/tmp/input-rpms/{i}.rpm'; guest.append(g)
    base+=['--ro-bind',str(f),g]
    rows.append({'path':str(f),'guest_path':g,'sha256':sha,'bytes':f.stat().st_size,'identity':identity})
(out/f'bpf-install-{arch}-inputs.json').write_text(json.dumps(rows,indent=2))
# Only private candidate input roots can be downgraded to the candidate Release 1.
# Preserve all dependency and file conflict checks; do not use --nodeps/--force.
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--test',*guest])
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage',*guest])
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
libdir='lib64' if arch=='aarch64' else 'lib'
run(base+['--','ls','-l',*[f'/usr/{libdir}/{x}' for x in ('libc++.a','libc++abi.a','libclang.a')]])
(out/f'bpf-install-{arch}-result.json').write_text(json.dumps({'status':'STATIC_INPUT_RPMS_INSTALLED','arch':arch,
    'llvm_shared_input':'unchanged previously tested build-root RPM set; not the newly packaged LLVM shared RPMs',
    'sudo':False,'dependency_checks':True},indent=2))
