"""Expand real spec with the platform toolchain macro; prepare isolated build commands."""
import json,subprocess,shlex,re,hashlib
from pathlib import Path
root=Path.cwd(); task=root/'tmp/NIGHT_0917'; out=root/'progress/BUILD_NIGHT_0917'
guest='/tmp/night'; source=root/'tmp/BUILD_W1BC_0911/target-fetch'
roots={
 'x86_64': root/'tmp/GBS-ROOT/LIBCXX-R91-x86_64-retry1/local/BUILD-ROOTS/scratch.x86_64.0',
 'aarch64':root/'tmp/GBS-ROOT/R96-aarch64-llvm-final/local/BUILD-ROOTS/scratch.aarch64.0'}
macro=next(l[1:] for l in (root/'codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if l.startswith('%toolchain_is() '))
allcmd={}
for arch,sysroot in roots.items():
    cache=(sysroot/'home/abuild/rpmbuild/BUILD/llvm-22.1.8/build/CMakeCache.txt').read_text()
    flags=re.search(r'^CMAKE_ASM_FLAGS:STRING=(.*)$',cache,re.M)[1]
    triple=re.search(r'^CMAKE_CXX_COMPILER_TARGET:.*?=(.*)$',cache,re.M)[1]
    base=['bwrap','--ro-bind',str(sysroot),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(task),guest,'--ro-bind',str(source),guest+'/source','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1','--setenv','LC_ALL','C']
    for mode in ('baseline','libcxx','gcc','undefined'):
        spec='libcxx-runtimes.baseline.spec' if mode=='baseline' else 'libcxx-runtimes.spec'
        defs=['--define',macro,'--define','_smp_mflags -j1','--define',f'_host {triple}']
        if mode!='undefined': defs+=['--define','_toolchain '+('gcc' if mode=='gcc' else 'clang')]
        cmd=base+['--','rpmspec','-P',*defs,guest+'/specs/'+spec]
        r=subprocess.run(cmd,capture_output=True,text=True)
        (out/f'{arch}_{mode}.expanded.spec').write_text(r.stdout)
        (out/f'{arch}_{mode}.expand.json').write_text(json.dumps(dict(command=cmd,exitcode=r.returncode,stderr=r.stderr),indent=2))
        if r.returncode: raise RuntimeError(r.stderr)
        text=r.stdout; build=text.split('%build\n',1)[1].split('%install',1)[0]
        cm=build[build.index('cmake -G Ninja'):build.index('\ncmake --build')]
        cm=cm.replace('../runtimes',guest+'/source/runtimes')
        cm=cm.replace(str('/usr/src/packages/SOURCES/libcxx-llvm22.map'),guest+'/source/packaging/libcxx-llvm22.map')
        cm=re.sub(r'--version-script=[^ ]+', '--version-script='+guest+'/source/packaging/libcxx-llvm22.map',cm)
        directory=f'{guest}/builds/runtime-{arch}-{mode}'
        script='set -eu\nexport PATH=/bin:/usr/bin:/sbin:/usr/sbin\nexport CMAKE_BUILD_PARALLEL_LEVEL=1\n'
        script+='export CFLAGS='+shlex.quote(flags)+'\nexport CXXFLAGS='+shlex.quote(flags)+'\n'
        script+=f'mkdir -p {directory}\ncd {directory}\n{triple}-clang++ --version\n'+cm+'\n'
        script+='cmake --build . --target cxxabi cxx cxx_experimental -- -j1\n'
        script+='find lib -maxdepth 2 -type f -name "libc++*" -print\n'
        path=task/'specs'/f'runtime-{arch}-{mode}.sh'; path.write_text(script)
        allcmd[f'{arch}-{mode}']=base+['--','/bin/sh',guest+'/specs/'+path.name]
(out/'runtime_commands.json').write_text(json.dumps(allcmd,indent=2))
print('Eight spec expansions and commands prepared; GCC/undefined preserve forced Clang compiler of baseline runtime spec.')
