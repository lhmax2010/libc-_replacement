import hashlib,json,subprocess
from pathlib import Path
from probes import base,run,p
run('toolchain-runtime-identities',base+['--','/bin/sh','-c','rpm -q glibc libgcc libstdc++; x86_64-tizen-linux-gnu-g++ --version; rpm -qf /usr/lib64/gcc/x86_64-tizen-linux-gnu/14.2.0/libgcc_eh.a /lib64/libgcc_s.so.1; sha256sum /usr/lib64/gcc/x86_64-tizen-linux-gnu/14.2.0/libgcc_eh.a /lib64/libgcc_s.so.1'])
source=Path('tmp/BUILD_W1BC_0911/target-fetch')
for name,rel,start,end in [('llvm-openmp','openmp/runtime/src/z_Linux_util.cpp',432,458),('llvm-lldb','lldb/source/Host/posix/HostThreadPosix.cpp',31,59)]:
    run(name+'-context',['awk',f'NR>={start} && NR<={end} {{printf "%d:%s\\n", NR,$0}}',str(source/rel)])
run('gcc-source-identity',['git','-C','codes/gcc','rev-parse','HEAD'])
run('gcc-register-state-source',['rg','-n','-A','12','-B','5','dwarf_reg_size_table|_Unwind_SetGR|__gthread_once','codes/gcc/libgcc/unwind-dw2.c'])
run('gcc-personality-setgr-source',['rg','-n','-A','8','-B','6','_Unwind_SetGR','codes/gcc/libstdc++-v3/libsupc++/eh_personality.cc'])
run('search-positive-control',['rg','-n','pthread_cancel|pthread_setcanceltype',str(source/'lldb/source/Host/posix/HostThreadPosix.cpp')])
for arch in ['x86_64','aarch64']:
    run(arch+'-linked-cancel-components',['rg','-n','-m','20','libomp|liblldb|pthread_cancel|pthread_setcanceltype',f'tmp/NIGHT_0917/builds/bpftrace-{arch}-static/bpftrace.map'],90)
    binary=Path(f'tmp/NIGHT_0917/builds/bpftrace-{arch}-static/src/bpftrace')
    cmd=['nm','-u',str(binary)]
    r=run(arch+'-undefined-symbols',cmd)
    (p/(arch+'-cancel-import-check.json')).write_text(json.dumps(dict(matches=[x for x in r['stdout'].splitlines() if 'pthread_cancel' in x or 'pthread_setcanceltype' in x],source=arch+'-undefined-symbols.json'),indent=2))
tar=Path('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push/packaging/prebuilt-static-bpftrace.tar.gz')
run('original-tar-sha256',['sha256sum',str(tar)])
run('original-tar-members',['tar','tzf',str(tar)],180)
