import datetime, hashlib, json, shutil, subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0916B'); t=Path('tmp/NIGHT_0917/finish_validation/rpm'); t.mkdir(parents=True,exist_ok=True)
commands=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    commands.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (p/'rpm_prepare_commands.json').write_text(json.dumps(commands,indent=2))
    if r.returncode: raise RuntimeError(str(cmd)+r.stderr)
    return r.stdout

for d in ['SOURCES','SPECS','BUILD','BUILDROOT','RPMS','SRPMS']: (t/d).mkdir(exist_ok=True)
source=Path('tmp/BUILD_W1BC_0911/target-fetch')
assert run(['git','-C',str(source),'rev-parse','HEAD']).strip()=='f203923a1508c9344f5fc6b17bd8822f011655c4'
archive=t/'SOURCES/llvm-22.1.8.tar.gz'
if not archive.exists():
    run(['git','-C',str(source),'archive','--format=tar.gz','--prefix=llvm-22.1.8/','-o',str(archive.resolve()),'HEAD'])
for name in ['libcxx-llvm22.map','libcxx-runtimes.manifest','SOURCE_PROVENANCE','llvm.manifest']:
    shutil.copy2(source/'packaging'/name,t/'SOURCES'/name)
for q in (source/'packaging').glob('mlgo*tar.gz'): shutil.copy2(q,t/'SOURCES'/q.name)
for name in ['libcxx-runtimes.spec','llvm.spec']: shutil.copy2(Path('tmp/NIGHT_0917/specs')/name,t/'SPECS'/name)
runtime=json.loads(Path('progress/BUILD_NIGHT_0917/runtime_commands.json').read_text())['aarch64-libcxx']
base=runtime[:runtime.index('--')]
macro=next(s[1:] for s in Path('codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if s.startswith('%toolchain_is() '))
cmd=base+['--','rpmbuild','-bb','--target','aarch64','--define','_topdir /tmp/night/finish_validation/rpm',
 '--define','_smp_mflags -j1','--define','_smp_build_ncpus 1','--define','mlgo_build_jobs 1',
 '--define','_host aarch64-tizen-linux-gnu','--define','_toolchain clang','--define',macro,
 '/tmp/night/finish_validation/rpm/SPECS/libcxx-runtimes.spec']
(p/'runtime_rpmbuild_command.json').write_text(json.dumps(cmd,indent=2))
manifest=[dict(path=str(q),bytes=q.stat().st_size,sha256=hashlib.sha256(q.read_bytes()).hexdigest()) for q in sorted((t/'SOURCES').iterdir())]
(p/'rpm_source_manifest.json').write_text(json.dumps(manifest,indent=2))
print('Prepared exact f203923a source archive and unchanged candidate specs; full rpmbuild -bb, not short-circuit.')
