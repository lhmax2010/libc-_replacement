"""Prepare an isolated RPM output set, reusing the approved arm LLVM tree."""
import hashlib,json
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; task=p/'tmp/WEEKEND_0918'
original=p/'progress/BUILD_STATIC_0917B/resume_0917/cell_commands/llvm-arm-rpm-serial.json'
cmd=json.loads(original.read_text())
cmd=[x.replace(str(p/'tmp/STATIC_0917B'),str(task)) for x in cmd]
for part in ('RPMS','SRPMS','BUILDROOT','TMP'):
    (task/'rpm-arm-llvm'/part).mkdir(parents=True,exist_ok=False)
spec=task/'specs/llvm.spec'; old=p/'tmp/STATIC_0917B/specs/llvm.spec'
assert spec.read_bytes()==old.read_bytes()
assert 'rsync' not in spec.read_text()
assert '_binary_payload w5T1.xzdio' in cmd
trace=out/'llvm-arm-rpm-failed-syscalls.strace'
# Include failed reads as well as writes: a generic cpio error does not prove
# which underlying I/O operation failed. FD annotations identify the file.
# No successful data buffers, sudo, host setting changes or source edits.
cmd=['strace','-f','-yy','-e','trace=%file,read,pread64,write,pwrite64,lseek,ioctl,copy_file_range,sendfile',
     '-e','status=failed','-s','160','-o',str(trace),*cmd]
(out/'cell_commands/llvm-arm-whole-retry.json').write_text(json.dumps(cmd,indent=2))
(out/'llvm-arm-retry-inputs.json').write_text(json.dumps({'original_command':str(original),
    'spec_sha256':hashlib.sha256(spec.read_bytes()).hexdigest(),'output':str(task/'rpm-arm-llvm'),
    'existing_18_rpms_preserved':True,'full_rpmbuild_bb':True,'prep_reused_from_approved_tree':True,
    'rsync_in_spec':False,'note':'Previous whole-build cpio ENOSYS and input-copy rsync EPERM are separate observations.'},indent=2))
print('Prepared complete arm RPM retry; no build yet')
