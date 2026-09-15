"""Prepare private dependency prefix and commands; no original root/source changes."""
import datetime,hashlib,json,shlex,shutil,subprocess,sys
from pathlib import Path
root=Path.cwd(); p=root/'progress/BUILD_NIGHT_0917'; task=root/'tmp/NIGHT_0917'
arch=sys.argv[1]; assert arch in ['x86_64','aarch64']
log=(p/f'bpftrace-{arch}-prepare.commands.jsonl').open('a',buffering=1)
def run(cmd,**kw):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',**kw)
    log.write(json.dumps(dict(command=shlex.join([str(x) for x in cmd]),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n')
    assert r.returncode==0,(cmd,r.stderr); return r.stdout
old=root/'progress/BUILD_NIGHT_0916'; records=json.loads((old/'repository_static_packages.json').read_text())['records']
rpms=[x for x in records if x['arch']==arch and x['package'] in ['libbpf-static','liblzma-static','zlib-devel-static','libffi-devel-static']]
bcc=next(x for x in json.loads((old/'static_packages_v2.json').read_text())['bcc_static_rpms'] if x['arch']==arch)
rpms.append(bcc)
if arch=='x86_64':
    f=root/bcc['path']; devel=f.with_name(f.name.replace('-static-','-devel-')); assert devel.is_file()
    rpms.append(dict(path=str(devel),sha256=hashlib.sha256(devel.read_bytes()).hexdigest()))
    dl=task/'downloads'; dl.mkdir(exist_ok=True)
    for name in ['cereal-devel-1.3.2-1.1.x86_64.rpm','vim-9.1.1918-1.1.x86_64.rpm']:
        url='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/x86_64/'+name
        f=dl/name
        if not f.exists(): run(['curl','-q','--fail','--location','--silent','--show-error','--max-time','120','--output',str(f),url])
        rpms.append(dict(path=str(f),sha256=hashlib.sha256(f.read_bytes()).hexdigest(),url=url))
prefix=task/'deps'/arch; prefix.mkdir(parents=True,exist_ok=True)
for row in rpms:
    f=root/row['path']; assert hashlib.sha256(f.read_bytes()).hexdigest()==row['sha256']
    run(['rpm','--checksig','--nosignature',str(f)])
    names=run(['rpm','-qpl',str(f)]).splitlines()
    assert all('..' not in Path(x).parts for x in names)
    # Only extract into this new private prefix. Names lose leading slashes.
    run(['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -id --quiet --no-absolute-filenames','extract',str(f)],cwd=prefix)
(p/f'bpftrace-{arch}-inputs.json').write_text(json.dumps(rpms,indent=2))
oldcmd=json.loads((p/'libclang_commands.json').read_text())[arch+'-libcxx']['build']; base=oldcmd[:oldcmd.index('--')]
src=root/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'; assert run(['git','-C',str(src),'rev-parse','HEAD']).strip()=='f895f8c0373d224847fc7d3ecbeaac3bf926a1a1'
assert not run(['git','-C',str(src),'status','--porcelain']).strip()
base+=['--ro-bind',str(src),'/tmp/night/bpftrace-source']
guest='/tmp/night'; dep=f'{guest}/deps/{arch}/usr'; d=f'{guest}/builds/bpftrace-{arch}-static'; rt=f'{guest}/builds/runtime-{arch}-libcxx/lib'
triple=arch+'-tizen-linux-gnu'
args=['cmake','-G','Ninja','-S',guest+'/bpftrace-source','-B',d,'-DCMAKE_BUILD_TYPE=Release','-DBUILD_TESTING=OFF','-DSTATIC_LINKING=ON','-DCMAKE_C_COMPILER='+triple+'-clang','-DCMAKE_CXX_COMPILER='+triple+'-clang++','-DCMAKE_CXX_FLAGS=-stdlib=libc++', '-DCMAKE_EXE_LINKER_FLAGS=-stdlib=libc++ -L'+rt,'-DCMAKE_PREFIX_PATH='+dep,'-DCMAKE_LIBRARY_PATH='+dep+'/lib64;'+rt,'-DLIBCLANG_STATIC_PATH=../builds/libclang-'+arch+'-libcxx/lib64/libclang.a','-DLIBBCC_LIBRARIES='+dep+'/lib64/libbcc.a','-DLIBBCC_BPF_LIBRARIES='+dep+'/lib64/libbcc_bpf.a','-DLIBBCC_LOADER_LIBRARY_STATIC='+dep+'/lib64/libbcc-loader-static.a','-DLIBBPF_LIBRARIES='+dep+'/lib64/libbpf.a','-DLIBLZMA_LIBRARIES='+dep+'/lib64/liblzma.a','-DLIBELF_LIBRARIES=/usr/lib64/libelf.a','-DLIBZ_LIBRARIES='+dep+'/lib64/libz.a','-DZLIB_LIBRARY='+dep+'/lib64/libz.a']
# CACHE PATH normalizes relative CLI values against the process directory before
# the project's own source-relative concatenation. Explicit STRING preserves it.
args=[s.replace('-DLIBCLANG_STATIC_PATH=', '-DLIBCLANG_STATIC_PATH:STRING=') for s in args]
args=[s+(' -L'+dep+'/lib64 -Wl,-Map,'+d+'/bpftrace.map' if s.startswith('-DCMAKE_EXE_LINKER_FLAGS=') else '') for s in args]
args+=['-DCMAKE_CXX_STANDARD_LIBRARIES=-Wl,--start-group '+rt+'/libc++.a '+rt+'/libc++abi.a -Wl,--end-group']
if arch=='x86_64': args+=['-DLIBBCC_INCLUDE_DIRS='+dep+'/include']
path=dep+'/bin:'+guest+'/tools'+('/aarch64/python/bin:'+guest+'/tools/aarch64' if arch=='aarch64' else '')+':/bin:/usr/bin:/sbin:/usr/sbin'
common='set -eu\nexport PATH='+path+'\nexport CMAKE_BUILD_PARALLEL_LEVEL=1\n'
conf=task/'specs'/f'configure-bpftrace-{arch}-static.sh'; conf.write_text(common+triple+'-clang++ --version\n'+shlex.join(args)+'\n')
build=task/'specs'/f'build-bpftrace-{arch}-static.sh'; build.write_text(common+f'ninja -C {d} -j1 bpftrace\n')
cmds=dict(configure=base+['--','/bin/sh',guest+'/specs/'+conf.name],build=base+['--','/bin/sh',guest+'/specs/'+build.name])
(p/f'bpftrace-{arch}-commands.json').write_text(json.dumps(cmds,indent=2)); print('Prepared',arch,'without changing source or original build root')
