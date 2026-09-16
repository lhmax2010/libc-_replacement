import hashlib,json,shutil,subprocess
from pathlib import Path
p=Path('progress/BUILD_STATIC_0916B'); task=Path('tmp/NIGHT_0917').resolve(); dest=task/'finish_validation/llvm-root'
source=Path('tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0').resolve()
assert not dest.exists(), 'Refuse to overwrite an existing root'
dest.mkdir(); rows=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    rows.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (p/'llvm_root_prepare.json').write_text(json.dumps(rows,indent=2))
    if r.returncode: raise RuntimeError(r.stderr)
    return r
for name in ['usr','etc','var','.build','bin','sbin','lib','lib64']:
    run(['cp','-a','--reflink=auto','--no-preserve=ownership',str(source/name),str(dest/name)])
for name in ['home/abuild','root','dev','proc','sys','tmp','run']: (dest/name).mkdir(parents=True,exist_ok=True)
incoming=task/'finish_validation/llvm-input-rpms'; incoming.mkdir(exist_ok=True)
rpms=list((task/'finish_validation/rpm/RPMS/aarch64').glob('*.rpm'))
for name in ['clang','libllvm']:
    src=Path(f'tmp/BUILD_W2_0912/llvm-local-repo/aarch64/{name}-22.1.8-108.2.aarch64.rpm').resolve(strict=True)
    dst=incoming/src.name; shutil.copy2(src,dst); rpms.append(dst)
(p/'llvm_rpm_inputs.json').write_text(json.dumps([dict(path=str(r),sha256=hashlib.sha256(r.read_bytes()).hexdigest(),bytes=r.stat().st_size) for r in rpms],indent=2))
base=['bwrap','--unshare-user','--uid','0','--gid','0','--bind',str(dest),'/',
 '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(task),'/tmp/night',
 '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1']
run(base+['--','id'])
run(base+['--','rpm','-Uvh',*[str(r).replace(str(task),'/tmp/night',1) for r in rpms]])
(p/'llvm_root_base_command.json').write_text(json.dumps(base,indent=2))
print('Private root prepared, original root untouched. Namespace UID 0 is not host root.')
