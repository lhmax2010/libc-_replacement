"""Use upstream Clang standalone support and installed LLVM; build only named libclang target."""
import json,shlex
from pathlib import Path
root=Path.cwd(); out=root/'progress/BUILD_NIGHT_0917'; task=root/'tmp/NIGHT_0917'; guest='/tmp/night'; src=root/'tmp/BUILD_W1BC_0911/target-fetch'
roots={('x86_64','libcxx'):'BUILD-W2-bcc-x86-libcxx',('x86_64','gcc'):'BUILD-W2-bcc-x86-gcc',('aarch64','libcxx'):'BUILD-W2C-bpftrace-aarch64-libcxx',('aarch64','gcc'):'BUILD-W2C-bpftrace-aarch64-gcc'}
cmds={}
for (arch,mode),tag in roots.items():
    r=root/'tmp/GBS-ROOT'/tag/'local/BUILD-ROOTS'/f'scratch.{arch}.0'
    triple={'x86_64':'x86_64-tizen-linux-gnu','aarch64':'aarch64-tizen-linux-gnu'}[arch]
    cc=triple+('-clang' if mode=='libcxx' else '-gcc'); cxx=triple+('-clang++' if mode=='libcxx' else '-g++')
    d=f'{guest}/builds/libclang-{arch}-{mode}'
    args=['cmake','-G','Ninja','-S',guest+'/source/clang','-B',d,'-DCMAKE_BUILD_TYPE=MinSizeRel','-DCMAKE_C_COMPILER='+cc,'-DCMAKE_CXX_COMPILER='+cxx,'-DLLVM_DIR=/usr/lib64/cmake/llvm','-DLLVM_INCLUDE_TESTS=OFF','-DCLANG_INCLUDE_TESTS=OFF','-DLLVM_ENABLE_RTTI=ON','-DLLVM_LINK_LLVM_DYLIB=ON','-DCLANG_LINK_CLANG_DYLIB=ON','-DCLANG_ENABLE_ARCMT=OFF','-DCLANG_TABLEGEN=/usr/bin/clang-tblgen','-DLLVM_TABLEGEN_EXE=/usr/bin/llvm-tblgen','-DCMAKE_INSTALL_LIBDIR=lib64']
    if mode=='libcxx': args+=['-DLLVM_ENABLE_LIBCXX=ON','-DLIBCLANG_BUILD_STATIC=ON']
    # GCC keeps the OFF default, identical to the unmodified spec's choice.
    base=['bwrap','--ro-bind',str(r),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(task),guest,'--ro-bind',str(src),guest+'/source','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1','--setenv','LC_ALL','C']
    if arch=='aarch64':
        pyroot=guest+'/tools/aarch64/python'
        pystd=root/'tmp/GBS-ROOT/R96-aarch64-llvm-final/local/BUILD-ROOTS/scratch.aarch64.0/usr/lib64/python3.14'
        base+=['--ro-bind',str(pystd),pyroot+'/lib64/python3.14','--setenv','PYTHONHOME',pyroot,'--setenv','PYTHONNOUSERSITE','1','--unsetenv','PYTHONPATH','--setenv','LD_LIBRARY_PATH',pyroot+'/lib64']
        args+=['-DPython3_EXECUTABLE='+pyroot+'/bin/python3']
    target='libclang_static' if mode=='libcxx' else 'libclang'
    conf='set -eu\nexport PATH=/bin:/usr/bin:/sbin:/usr/sbin\n'+cxx+' --version\n'+shlex.join(args)+f'\nninja -C {d} -t targets all | grep -E "libclang(_static)?(:|\\.)"\nninja -C {d} -n -j1 {target}\n'
    toolpath='/tmp/night/tools'+('/aarch64' if arch=='aarch64' else '')
    if arch=='aarch64': toolpath=pyroot+'/bin:'+toolpath
    conf=conf.replace('/bin:/usr/bin:/sbin:/usr/sbin',toolpath+':/bin:/usr/bin:/sbin:/usr/sbin')
    path=task/'specs'/f'configure-libclang-{arch}-{mode}.sh'; path.write_text(conf)
    build=task/'specs'/f'build-libclang-{arch}-{mode}.sh'; build.write_text(f'set -eu\nexport PATH={toolpath}:/bin:/usr/bin:/sbin:/usr/sbin\nexport CMAKE_BUILD_PARALLEL_LEVEL=1\nninja -C {d} -j1 {target}\n')
    cmds[f'{arch}-{mode}']=dict(configure=base+['--','/bin/sh',guest+'/specs/'+path.name],build=base+['--','/bin/sh',guest+'/specs/'+build.name],target=target)
(out/'libclang_commands.json').write_text(json.dumps(cmds,indent=2)); print('Prepared standalone target-only configuration, without building LLVM itself.')
