"""Keep a read-only descriptor; preserve the failed output after its producer exits."""
import hashlib,json,os,time
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG')
p=Path('tmp/ARM_RPM_DIAG/rpm-arm-llvm/RPMS/armv7l/lldb-debuginfo-22.1.8-1.armv7l.rpm')
fd=os.open(p,os.O_RDONLY)
print('Opened read-only descriptor for',p,flush=True)
while not (b/'raw/045_reproduce.exitcode').exists():time.sleep(1)
s=os.fstat(fd)
result={'source':str(p),'bytes':s.st_size,'nlink':s.st_nlink}
if s.st_nlink==0:
    assert s.st_size<1024**3,'Preservation size exceeds 1 GiB diagnostic bound'
    target=b/'failed-lldb-debuginfo.incomplete-rpm';h=hashlib.sha256()
    with target.open('xb') as out:
        while data:=os.read(fd,1024*1024):out.write(data);h.update(data)
    result.update(output=str(target),sha256=h.hexdigest(),not_installable=True)
else:result['preservation']='Not deleted; original output remains in place'
os.close(fd)
(b/'failed-output-retention.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result),flush=True)
