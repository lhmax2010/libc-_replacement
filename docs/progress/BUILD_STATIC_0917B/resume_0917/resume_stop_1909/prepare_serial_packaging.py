"""Correct the observed RPM XZ thread setting, preserving the stopped attempt."""
import datetime
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917'
task=p/'tmp/STATIC_0917B'
assert (out/'raw/149_arm_llvm_build.exitcode').read_text().strip()=='241'
assert not Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/build-static-0917-arm-llvm.scope').exists()
old=task/'rpm-arm-llvm/RPMS'
saved=task/'rpm-arm-llvm/RPMS_stopped_parallel_1459'
assert not saved.exists()
rows=[]
for f in sorted(old.rglob('*.rpm')):
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    rows.append({'path':str(f),'bytes':f.stat().st_size,'sha256':sha})
old.rename(saved)
old.mkdir()
cmd=json.loads((out/'arm_llvm_command.json').read_text())
i=cmd.index('--')
cmd[i+2:i+2]=['--define','_binary_payload w5T1.xzdio']
(out/'cell_commands/llvm-arm-rpm-serial.json').write_text(json.dumps(cmd,indent=2))
query=cmd[:]
query[query.index('rpmbuild')]='rpm'
for x in ('--nosignature','--noprep','--noclean','-bb','/tmp/task/specs/llvm.spec'):
    query.remove(x)
query+=['--eval','%{_binary_payload}']
r=subprocess.run(query,capture_output=True,text=True)
result={'time':datetime.datetime.now().astimezone().isoformat(),'command':shlex.join(query),
        'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,
        'stopped_rpms_preserved_at':str(saved),'stopped_files':rows,
        'reason':'Observed w5T.xzdio requested 20 XZ threads; override w5T1.xzdio limits payload compression to one.'}
(out/'serial_packaging_correction.json').write_text(json.dumps(result,indent=2))
assert r.returncode==0 and r.stdout.strip()=='w5T1.xzdio'
print(json.dumps(result,indent=2))
