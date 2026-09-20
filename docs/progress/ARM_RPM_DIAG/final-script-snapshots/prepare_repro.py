import hashlib,json,shlex
from pathlib import Path
p=Path.cwd(); base=p/'progress/ARM_RPM_DIAG'; task=p/'tmp/ARM_RPM_DIAG'
task.mkdir(exist_ok=False)
for part in ('RPMS','SRPMS','BUILDROOT','TMP'):
    (task/'rpm-arm-llvm'/part).mkdir(parents=True,exist_ok=False)
(base/'cell_commands').mkdir(exist_ok=True)
old=json.loads((p/'progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json').read_text())
cmd=old[old.index('bwrap'):]
# Keep the approved preparation tree; original input/spec and old outputs read-only.
i=cmd.index(str(p/'tmp/WEEKEND_0918')); assert cmd[i-1]=='--bind'; cmd[i-1]='--ro-bind'
i=cmd.index('--'); cmd[i:i]=['--bind',str(task),'/tmp/diag']
cmd=[v.replace('/tmp/task/rpm-arm-llvm/','/tmp/diag/rpm-arm-llvm/') for v in cmd]
i=cmd.index('rpmbuild')
cmd[i:i+1]=['/usr/bin/qemu-arm','-strace','-D','/tmp/diag/guest-rpmbuild.strace','/usr/bin/rpmbuild','-vv']
# Requested host calls plus open/read context; raw buffers avoid logging payloads.
cmd=['strace','-f','-tt','-yy','-s','256','-e',
     'trace=write,mknod,mknodat,link,linkat,setxattr,lsetxattr,fsetxattr,fallocate,openat,close,read,pread64,pwrite64',
     '-e','raw=write,read,pread64,pwrite64','-o',str(base/'host.strace'),*cmd]
(base/'cell_commands/llvm-arm-repro.json').write_text(json.dumps(cmd,indent=2)+'\n')
(base/'deadlines.json').write_text(json.dumps({'W1':'2026-09-19T01:46:15+08:00'})+'\n')
ident={}
for f in [p/'tmp/WEEKEND_0918/specs/llvm.spec',p/'tmp/WEEKEND_0918/ninja-serial-arm',p/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/usr/bin/qemu-arm']:
    ident[str(f)]={'sha256':hashlib.sha256(f.read_bytes()).hexdigest(),'bytes':f.stat().st_size}
(base/'repro-inputs.json').write_text(json.dumps(ident,indent=2)+'\n')
print(shlex.join(cmd));print('Fresh output directory:',task)
