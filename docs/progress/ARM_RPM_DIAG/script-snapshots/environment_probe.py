"""Probe only a newly-created file in this diagnostic output directory."""
import json,subprocess
from pathlib import Path
base=Path('progress/ARM_RPM_DIAG')
cmd=json.loads((base/'cell_commands/llvm-arm-repro.json').read_text())
cmd=cmd[cmd.index('bwrap'):cmd.index('--')+1]
i=cmd.index('/home/abuild/rpmbuild/BUILD');cmd[i-2]='--ro-bind'
probe="""import json,os,tempfile,time
fd,path=tempfile.mkstemp(prefix='xattr-probe-',dir='/tmp/diag')
os.close(fd)
try:
    os.setxattr(path,'user.arm_rpm_diag',b'probe')
    v=os.getxattr(path,'user.arm_rpm_diag')
    print(json.dumps({'path':path,'setxattr':'PASS','getxattr':v.decode(),'attributes':os.listxattr(path)}))
    assert v==b'probe'
finally:
    os.unlink(path)
"""
tests=[['sh','-c','grep Seccomp /proc/self/status'],['mount'],['python3','-c',probe]]
failed=False
for n,args in enumerate(tests):
    full=cmd+args
    result=subprocess.run(full,capture_output=True,text=True)
    row={'command':full,'exitcode':result.returncode,'stdout':result.stdout,'stderr':result.stderr}
    (base/f'environment-{n}.json').write_text(json.dumps(row,indent=2)+'\n')
    print(json.dumps(row));failed|=result.returncode!=0
raise SystemExit(int(failed))
