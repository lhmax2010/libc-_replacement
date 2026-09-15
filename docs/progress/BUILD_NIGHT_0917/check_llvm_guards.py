import json,subprocess
from pathlib import Path
root=Path.cwd(); out=root/'progress/BUILD_NIGHT_0917'; task=root/'tmp/NIGHT_0917'
macro=next(x[1:] for x in (root/'codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if x.startswith('%toolchain_is() '))
sysroot=root/'tmp/GBS-ROOT/BUILD-W2-bcc-x86-gcc/local/BUILD-ROOTS/scratch.x86_64.0'
base=['bwrap','--ro-bind',str(sysroot),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--bind',str(task),'/tmp/night','--setenv','PATH','/bin:/usr/bin','--','rpmspec','-P','--define',macro]
texts={}; logs=[]
for mode in ['clang','gcc','undefined']:
    for version in ['baseline','candidate']:
        name='llvm.baseline.spec' if version=='baseline' else 'llvm.spec'
        cmd=base+([] if mode=='undefined' else ['--define','_toolchain '+mode])+['/tmp/night/specs/'+name]
        r=subprocess.run(cmd,capture_output=True,text=True); logs.append(dict(command=cmd,exitcode=r.returncode,stderr=r.stderr))
        (out/f'llvm_{version}_{mode}.expanded.spec').write_text(r.stdout)
        assert r.returncode==0,r.stderr
        texts[mode,version]='\n'.join(x for x in r.stdout.splitlines() if x.strip())
(out/'llvm_guard_expansion_commands.json').write_text(json.dumps(logs,indent=2))
assert texts['gcc','candidate']==texts['gcc','baseline']
assert texts['undefined','candidate']==texts['undefined','baseline']
assert '-DLIBCLANG_BUILD_STATIC=ON' in texts['clang','candidate']
assert texts['clang','candidate'].replace('    -DLIBCLANG_BUILD_STATIC=ON \\\n','')==texts['clang','baseline']
print('PASS: GCC and undefined unchanged; Clang adds only LIBCLANG_BUILD_STATIC=ON.')
