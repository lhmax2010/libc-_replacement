"""Read-only queries of actual build-root RPM macro paths and packages."""
import json
import shlex
import subprocess
from pathlib import Path

p = Path.cwd()
out = p/'progress/BUILD_STATIC_0917B/resume_0917'
records=[]
macrofiles='/usr/lib/rpm/macros:/usr/lib/rpm/macros.d/macros.*:/usr/lib/rpm/platform/%{_target}/macros:/usr/lib/rpm/fileattrs/*.attr:/usr/lib/rpm/tizen/macros:/etc/rpm/macros.*:/etc/rpm/macros:/etc/rpm/%{_target}/macros:/home/abuild/.rpmmacros'
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    records.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'bpf_mode_queries.json').write_text(json.dumps(records,indent=2))
    assert r.returncode==0
    return r.stdout
for arch in ('armv7l','aarch64'):
    root=p/f'tmp/GBS-ROOT/BUILD-W2C-bpftrace-{arch}-gcc/local/BUILD-ROOTS/scratch.{arch}.0'
    base=['bwrap','--unshare-user','--uid','1000','--gid','1000',
          '--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp',
          '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--']
    packages=run(base+['rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
    print(arch, '\n'.join(x for x in packages.splitlines() if x.startswith(('python','llvm','clang','libc++','bcc-'))), flush=True)
    common=['rpm','--rcfile','/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc','--macros',macrofiles,'--target',arch+'-tizen-linux']
    modes={
      'gcc':['--define','_toolchain_override gcc'],
      'undefined':['--undefine','_toolchain','--define','__cc %{_host}-gcc','--define','__cxx %{_host}-g++',
                   '--define','__ar gcc-ar','--define','__ranlib gcc-ranlib','--define','__optflags_toolchain %{__optflags_toolchain_gcc}']}
    for mode,args in modes.items():
        text=run(base+common+args+['--eval','%{defined _toolchain}|%{?toolchain_is:%{toolchain_is clang}}|%{__cc}|%{__cxx}|%{optflags}'])
        print(arch,mode,text,flush=True)
        if mode=='undefined':
            assert text.startswith('0|'), 'undefined macro path not actually undefined'
        assert '-stdlib=libc++' not in text and '-lc++abi' not in text
