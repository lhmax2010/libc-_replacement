import hashlib
import json
import subprocess
import time
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
t=Path('tmp/STATIC_0917B').resolve()
root=t/'llvm-root-minimal'
old=Path('tmp/NIGHT_0917').resolve()
assert root.is_dir()
assert not Path('password.md').exists(), 'Credential must be removed after copy/chown.'
base=['bwrap','--unshare-user','--uid','0','--gid','0','--bind',str(root),'/',
      '--dev','/dev','--proc','/proc','--tmpfs','/tmp',
      '--bind',str(t),'/tmp/task','--ro-bind',str(old),'/tmp/night',
      '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin',
      '--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1','--setenv','LC_ALL','C']
for name in ['clang','libllvm','llvm','lldb']:
    payload=Path(f'tmp/BUILD_W2_0912/llvm-local-repo/aarch64/{name}-22.1.8-108.2.aarch64.rpm').resolve(strict=True)
    base+=['--ro-bind',str(payload),'/tmp/llvm-resolved-inputs/'+name+'.rpm']
(p/'private_root_command.json').write_text(json.dumps(base,indent=2))

def run(name,command,timeout=300):
    name='v6-'+name
    started=time.time()
    r=subprocess.run(command,capture_output=True,text=True,errors='replace',timeout=timeout)
    (p/(name+'.json')).write_text(json.dumps(dict(command=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,seconds=time.time()-started),indent=2))
    print(name,r.returncode,flush=True)
    if r.returncode:
        raise SystemExit(r.returncode)
    return r

run('private-root-id',base+['--','id'])
run('private-root-packages',base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
rpms=[]
for q in sorted((old/'finish_validation/rpm/RPMS/aarch64').glob('*.rpm')):
    rpms.append((q,'/tmp/night/'+str(q.relative_to(old))))
for name in ['clang','libllvm','llvm','lldb']:
    q=Path(f'tmp/BUILD_W2_0912/llvm-local-repo/aarch64/{name}-22.1.8-108.2.aarch64.rpm').resolve(strict=True)
    rpms.append((q,'/tmp/llvm-resolved-inputs/'+name+'.rpm'))
(p/'private_root_rpm_inputs.json').write_text(json.dumps([dict(path=str(q),sha256=hashlib.file_digest(q.open('rb'),'sha256').hexdigest(),bytes=q.stat().st_size) for q,_ in rpms],indent=2))
identity=run('private-root-input-architectures',base+['--','rpm','-qp','--qf','%{NAME}\t%{ARCH}\n',*[path for _,path in rpms]])
assert all(line.split('\t')[-1]=='aarch64' for line in identity.stdout.splitlines())
# Cross-root installation only: retain dependency checks; host uname is x86_64.
run('private-root-install-test',base+['--','rpm','-Uvh','--ignorearch','--test',*[path for _,path in rpms]])
run('private-root-install',base+['--','rpm','-Uvh','--ignorearch',*[path for _,path in rpms]])
run('private-root-clang',base+['--','aarch64-tizen-linux-gnu-clang++','--version'])
