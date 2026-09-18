import hashlib
import json
import shlex
import subprocess
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918/verify_runtime_armv7l'
out.mkdir(exist_ok=False)
task=p/'tmp/WEEKEND_0918'
extract=task/'verified-runtime-armv7l'; extract.mkdir(exist_ok=False)
commands=[]; rows=[]
def run(cmd,cwd=None):
    r=subprocess.run(cmd,cwd=cwd,capture_output=True,text=True,errors='replace')
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'cwd':str(cwd or p),
                     'stdout':r.stdout,'stderr':r.stderr})
    (out/'commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0
    return r.stdout
for f in sorted((task/'rpm-arm-runtime/RPMS').rglob('*.rpm')):
    run(['rpm','--checksig','--nosignature',str(f)])
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
    assert identity.endswith('\tarmv7l')
    compression=run(['rpm','-qp','--qf','%{PAYLOADCOMPRESSOR}|%{PAYLOADFLAGS}',str(f)])
    assert compression=='xz|5T1'
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    files=run(['rpm','-qpl',str(f)]).splitlines()
    rows.append({'path':str(f),'identity':identity,'sha256':sha,'bytes':f.stat().st_size,'files':files})
    assert all('..' not in Path(x).parts for x in files)
    package=identity.split('\t')[0]
    if package in ('libc++-devel','libc++abi-devel'):
        member='./usr/lib/'+('libc++.a' if package=='libc++-devel' else 'libc++abi.a')
        run(['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -id --quiet --no-absolute-filenames "$2"','extract',str(f),member],cwd=extract)
archives=[]
for name in ('libc++.a','libc++abi.a'):
    f=extract/'usr/lib'/name
    assert f.is_file()
    headers=run(['readelf','-hW',str(f)])
    assert 'ARM' in headers and 'AArch64' not in headers
    text=run(['nm','--defined-only',str(f)])
    std1=sum('_ZNSt3__1' in x for x in text.splitlines())
    cxx11=sum('_ZNSt7__cxx11' in x for x in text.splitlines())
    assert cxx11==0
    if name=='libc++.a': assert std1>0
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    archives.append({'name':name,'sha256':sha,'defined_std1_symbol_lines':std1,'defined_cxx11_symbol_lines':cxx11})
result={'status':'RPM_STATIC_ARCHIVES_VERIFIED_NOT_BOARD_TESTED','rpms':rows,'archives':archives}
(out/'result.json').write_text(json.dumps(result,indent=2))
print(json.dumps({'status':result['status'],'archives':archives,'rpm_count':len(rows)},indent=2))
