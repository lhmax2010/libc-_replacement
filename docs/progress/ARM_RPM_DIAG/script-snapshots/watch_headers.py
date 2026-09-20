"""Read-only observer: preserve header prefixes of RPMs even if RPM deletes a failed output.
These are NOT complete RPMs and must never be used as installable artifacts.
"""
import ctypes,datetime,hashlib,json,os,select,struct,time
from pathlib import Path
base=Path('progress/ARM_RPM_DIAG'); dest=base/'observed-header-prefixes';dest.mkdir(exist_ok=False)
root=Path('tmp/ARM_RPM_DIAG/rpm-arm-llvm/RPMS/armv7l')
deadline=datetime.datetime.fromisoformat('2026-09-19T01:46:15+08:00').timestamp()
done=base/'raw/045_reproduce.exitcode'
events=(dest/'events.jsonl').open('x',buffering=1)
def note(**kw): events.write(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),**kw))+'\n')
while not root.exists() and not done.exists() and time.time()<deadline: time.sleep(.5)
if not root.exists(): note(result='NOT_OBSERVED_NO_RPM_DIRECTORY');raise SystemExit(0)
lib=ctypes.CDLL(None,use_errno=True); fd=lib.inotify_init1(os.O_NONBLOCK|os.O_CLOEXEC)
assert fd>=0
wd=lib.inotify_add_watch(fd,os.fsencode(root),0x100|0x8|0x200|0x80)
assert wd>=0
opened={}
def retain(name):
    if not name.endswith('.rpm') or name in opened:return
    try: opened[name]=os.open(root/name,os.O_RDONLY);note(event='opened',name=name)
    except OSError as e: note(event='open_error',name=name,error=str(e))
for f in root.glob('*.rpm'):retain(f.name)
def save(name,event):
    if name not in opened:return
    n=opened[name]; data=os.pread(n,1048576,0);s=os.fstat(n)
    filename=name+'.'+event+'.header-prefix'
    (dest/filename).write_bytes(data)
    note(event=event,name=name,output=filename,bytes_saved=len(data),observed_size=s.st_size,nlink=s.st_nlink,sha256=hashlib.sha256(data).hexdigest(),complete_rpm=False)
try:
    while not done.exists() and time.time()<deadline:
        ready,_,_=select.select([fd],[],[],.5)
        if not ready:continue
        buf=os.read(fd,65536);off=0
        while off<len(buf):
            _,mask,_,length=struct.unpack_from('iIII',buf,off);off+=16
            name=os.fsdecode(buf[off:off+length].split(b'\0',1)[0]);off+=length
            retain(name)
            if mask&0x8:save(name,'close_write')
            if mask&0x200:save(name,'deleted')
    for name in opened:save(name,'final')
finally:
    for n in opened.values():os.close(n)
    os.close(fd)
print('Header-prefix observer finished; prefixes are not installable RPMs.')
