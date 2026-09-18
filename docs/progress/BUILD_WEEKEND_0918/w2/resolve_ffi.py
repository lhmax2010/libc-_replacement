"""Fetch declared libffi RPM dependencies from the observed repository listing."""
import hashlib,json,re,shlex,subprocess
from pathlib import Path
from urllib.parse import urljoin
out=Path('progress/BUILD_WEEKEND_0918'); dest=Path('tmp/WEEKEND_0918/dependencies'); dest.mkdir(exist_ok=True)
commands=[]; records=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'ffi-download-commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0,(cmd,r.stderr)
    return r.stdout
for arch in ('aarch64','armv7l'):
    url=f'https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/{arch}/'
    index=dest/f'index-{arch}.html'
    run(['curl','-fSL','--max-time','60','-o',str(index),url])
    links=re.findall(r'href="([^"]+)"',index.read_text())
    candidates=[x for x in links if re.fullmatch(r'libffi(?:-devel)?-3\.4\.7-[^/]+\.'+arch+r'\.rpm',x)]
    assert len(candidates)==2,candidates
    for name in candidates:
        f=dest/name; source=urljoin(url,name)
        run(['curl','-fSL','--max-time','60','-o',str(f),source])
        identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
        run(['rpm','--checksig','--nosignature',str(f)])
        requires=run(['rpm','-qpR',str(f)])
        records.append({'arch':arch,'url':source,'path':str(f.resolve()),'sha256':hashlib.sha256(f.read_bytes()).hexdigest(),
                        'identity':identity,'requires':requires,'index_sha256':hashlib.sha256(index.read_bytes()).hexdigest()})
(out/'ffi-dependencies.json').write_text(json.dumps(records,indent=2))
print('Downloaded and RPM-digest-verified',len(records))
