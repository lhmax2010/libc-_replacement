"""Prepare full RPM builds, keeping mode selection in explicit test macros."""
import hashlib
import json
import shlex
import subprocess
import sys
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917'
task=p/'tmp/STATIC_0917B'; arch,mode=sys.argv[1:]
assert arch in ('armv7l','aarch64') and mode in ('libcxx','gcc','undefined')
label=f'bpf-{arch}-{mode}'
top=task/f'rpm-{label}'
assert not top.exists()
for d in ('BUILD','BUILDROOT','RPMS','SRPMS','TMP'):
    (top/d).mkdir(parents=True,exist_ok=True)
oldmode='libcxx' if mode=='libcxx' else 'gcc'
root=p/f'tmp/GBS-ROOT/BUILD-W2C-bpftrace-{arch}-{oldmode}/local/BUILD-ROOTS/scratch.{arch}.0'
source=root/'home/abuild/rpmbuild/SOURCES'
if mode=='libcxx':
    assert json.loads((out/f'bpf-install-{arch}-result.json').read_text())['status']=='STATIC_INPUT_RPMS_INSTALLED'
    base=json.loads((out/f'bpf-private-{arch}-base.json').read_text())
else:
    base=['bwrap','--unshare-user','--uid','1000','--gid','1000',
          '--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp',
          '--bind',str(task),'/tmp/task',
          '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin',
          '--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1','--setenv','RPM_BUILD_NCPUS','1',
          '--setenv','MAKEFLAGS','-j1','--setenv','LC_ALL','C',
          '--setenv','QEMU_RESERVED_VA','0x100000000']
base+=['--ro-bind',str(source),'/tmp/bpf-sources']
macrofiles='/usr/lib/rpm/macros:/usr/lib/rpm/macros.d/macros.*:/usr/lib/rpm/platform/%{_target}/macros:/usr/lib/rpm/fileattrs/*.attr:/usr/lib/rpm/tizen/macros:/etc/rpm/macros.*:/etc/rpm/macros:/etc/rpm/%{_target}/macros:/home/abuild/.rpmmacros'
args=['rpmbuild','--rcfile','/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc','--macros',macrofiles,
      '--target',arch+'-tizen-linux','--define',f'_topdir /tmp/task/{top.name}',
      '--define','_sourcedir /tmp/bpf-sources','--define',f'_tmppath /tmp/task/{top.name}/TMP',
      '--define','_smp_mflags -j1','--define','_smp_build_ncpus 1','--define','jobs 1',
      '--define','_binary_payload w5T1.xzdio',
      '--define','_srcdefattr (-,root,root)']
if mode=='gcc':
    args+=['--define','_toolchain_override gcc']
elif mode=='undefined':
    args+=['--undefine','_toolchain','--define','__cc %{_host}-gcc',
           '--define','__cxx %{_host}-g++','--define','__ar gcc-ar',
           '--define','__ranlib gcc-ranlib','--define','__optflags_toolchain %{__optflags_toolchain_gcc}']
elif arch=='aarch64':
    # Approved native aarch64 toolchain; only this architecture was authorized.
    native=root/'emul'
    f=native/'usr/bin/aarch64-tizen-linux-gnu-clang'
    assert f.exists()
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    assert sha=='590fa22e2859a15c3d05c46aca08958949bb0cc7ab5abf293ae8ea2f338daa1b'
    args+=['--define','__cc /emul/usr/bin/aarch64-tizen-linux-gnu-clang',
           '--define','__cxx /emul/usr/bin/aarch64-tizen-linux-gnu-clang++']
queries=[]
def query(command):
    r=subprocess.run(command,capture_output=True,text=True)
    queries.append({'command':shlex.join(command),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/f'{label}-preflight.json').write_text(json.dumps(queries,indent=2))
    assert r.returncode==0
    return r.stdout
identity=query(base+['--','rpm',*args[1:],'--eval','%{defined _toolchain}|%{?toolchain_is:%{toolchain_is clang}}|%{__cc}|%{__cxx}|%{optflags}'])
fields=identity.strip().split('|')
assert len(fields)==5
assert fields[0:2]==(['1','1'] if mode=='libcxx' else ['1','0'] if mode=='gcc' else ['0','0'])
version=query(base+['--',fields[3],'--version'])
assert ('clang version 22.1.8' in version) if mode=='libcxx' else ('14.2.0' in version)
cmd=base+['--',*args,'--noclean','-bb','/tmp/task/specs/bpftrace.spec']
(out/'cell_commands'/f'{label}.json').write_text(json.dumps(cmd,indent=2))
rows=[]
for name in ('bpftrace-0.24.2.tar.gz','prebuilt-static-bpftrace.tar.gz','bpftrace.manifest'):
    f=source/name
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    if name=='prebuilt-static-bpftrace.tar.gz':
        assert sha=='3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123'
    rows.append({'path':str(f),'sha256':sha,'bytes':f.stat().st_size})
(out/f'{label}-inputs.json').write_text(json.dumps({'source_files':rows,'mode':mode,
    'spec_sha256':hashlib.sha256((task/'specs/bpftrace.spec').read_bytes()).hexdigest(),
    'full_rpmbuild':True,'status':'PREPARED_NOT_BUILT'},indent=2))
print(label,'prepared')
