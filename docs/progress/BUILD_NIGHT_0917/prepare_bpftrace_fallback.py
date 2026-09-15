import hashlib,json,re,shlex,subprocess,tarfile
from pathlib import Path
root=Path.cwd(); p=root/'progress/BUILD_NIGHT_0917'; task=root/'tmp/NIGHT_0917'; guest='/tmp/night'
cmd=json.loads((p/'libclang_commands.json').read_text())['aarch64-gcc']['build']; base=cmd[:cmd.index('--')]
src=root/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'; base+=['--ro-bind',str(src),guest+'/bpftrace-source']
macro=next(x[1:] for x in (root/'codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if x.startswith('%toolchain_is() '))
texts={}; logs=[]; commands={}
for mode in ['clang','gcc','undefined']:
    for version in ['baseline','candidate']:
        name='bpftrace.baseline.spec' if version=='baseline' else 'bpftrace.spec'
        c=base+['--','rpmspec','-P','--target','aarch64','--define',macro,'--define','_smp_mflags -j1']+([] if mode=='undefined' else ['--define','_toolchain '+mode])+[guest+'/specs/'+name]
        r=subprocess.run(c,capture_output=True,text=True); logs.append(dict(command=c,exitcode=r.returncode,stderr=r.stderr)); assert r.returncode==0,r.stderr
        texts[mode,version]=r.stdout
        (p/f'bpftrace_{version}_{mode}.expanded.spec').write_text(r.stdout)
(p/'bpftrace_guard_commands.json').write_text(json.dumps(logs,indent=2))
for mode in ['gcc','undefined']:
    normalize=lambda s:'\n'.join(x for x in s.splitlines() if x.strip())
    assert normalize(texts[mode,'candidate'])==normalize(texts[mode,'baseline'])
    assert '-stdlib=libc++' not in texts[mode,'candidate'] and '-lc++abi' not in texts[mode,'candidate']
    install=[l for l in texts[mode,'candidate'].splitlines() if l.startswith('install -m 0755 prebuilt-static-bpftrace/')]; assert len(install)==1
    # Use the unchanged main CMake configuration, an out-of-source Ninja build,
    # and explicit real GCC. This is a binary/all-target build, not rpmbuild.
    b=guest+'/builds/bpftrace-aarch64-'+mode
    c=['cmake','-G','Ninja','-S',guest+'/bpftrace-source','-B',b,'-DCMAKE_BUILD_TYPE=Release','-DCMAKE_VERBOSE_MAKEFILE=ON','-DBUILD_TESTING=OFF','-DSTATIC_LINKING=OFF','-DCMAKE_SKIP_RPATH:BOOL=ON','-DCMAKE_INSTALL_PREFIX:PATH=/usr','-DCMAKE_C_COMPILER=aarch64-tizen-linux-gnu-gcc','-DCMAKE_CXX_COMPILER=aarch64-tizen-linux-gnu-g++']
    common='set -eu\nexport PATH=/tmp/night/tools/aarch64/python/bin:/tmp/night/tools/aarch64:/bin:/usr/bin:/sbin:/usr/sbin\nexport CMAKE_BUILD_PARALLEL_LEVEL=1\nunset CFLAGS CXXFLAGS LDFLAGS\n'
    cf=task/'specs'/f'configure-bpftrace-aarch64-{mode}.sh'; cf.write_text(common+'aarch64-tizen-linux-gnu-g++ --version\n'+shlex.join(c)+'\n')
    bf=task/'specs'/f'build-bpftrace-aarch64-{mode}.sh'; bf.write_text(common+f'ninja -C {b} -j1\n')
    # Exact source-relative asset path from the expanded candidate install line.
    parts=shlex.split(install[0]); original=parts[-2]; assert original=='prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace',original
    dest=guest+'/fallback-'+mode+'/usr/bin/bpftrace-static'
    inf=task/'specs'/f'install-bpftrace-static-{mode}.sh'; inf.write_text(common+'mkdir -p '+str(Path(dest).parent)+'\ninstall -m 0755 '+guest+'/original_assets/'+original+' '+dest+'\nsha256sum '+guest+'/original_assets/'+original+' '+dest+'\n')
    commands[mode]=dict(configure=base+['--','/bin/sh',guest+'/specs/'+cf.name],build=base+['--','/bin/sh',guest+'/specs/'+bf.name],install=base+['--','/bin/sh',guest+'/specs/'+inf.name])
tar=src/'packaging/prebuilt-static-bpftrace.tar.gz'; digest=hashlib.sha256(tar.read_bytes()).hexdigest(); assert digest=='3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123'
dest=task/'original_assets'; dest.mkdir(exist_ok=True)
with tarfile.open(tar) as f: f.extractall(dest,filter='data')
(p/'bpftrace_fallback_commands.json').write_text(json.dumps(commands,indent=2))
(p/'bpftrace_fallback_guards.json').write_text(json.dumps(dict(gcc_identical=True,undefined_identical=True,forbidden_flags=0,source1002_sha256=digest,scope='expanded spec equality; builds run separately'),indent=2))
print('PASS: both fallback expanded specs unchanged; actual build commands prepared.')
