"""Make only this task's owned private input directories writable for RPM."""
import json,os,stat,sys
from pathlib import Path
arch=sys.argv[1]; assert arch in ('aarch64','armv7l')
root=Path.cwd()/f'tmp/WEEKEND_0918/input-{arch}-v3'
assert root.is_dir()
rows=[]
for part in ('usr','etc','rpmdb','root'):
    for directory,dirs,files in os.walk(root/part,followlinks=False):
        f=Path(directory); info=f.lstat()
        assert info.st_uid==os.getuid() and not f.is_symlink()
        before=stat.S_IMODE(info.st_mode); after=before|stat.S_IWUSR
        if before!=after:
            os.chmod(f,after)
            rows.append({'path':str(f),'before':oct(before),'after':oct(after)})
Path(f'progress/BUILD_WEEKEND_0918/input-{arch}-directory-permission-changes.json').write_text(json.dumps(rows,indent=2))
print('Task-owned directories made owner-writable',len(rows),'source unchanged')
