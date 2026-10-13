"""Read fixed refs; export baselines and verify four real-RPM macro combinations."""
import difflib, hashlib, json, pathlib, shlex, subprocess, sys
ROOT = pathlib.Path.cwd()
E = ROOT / 'docs/progress/QB_1194606_FIX'
T = ROOT / 'tmp/QB_1194606_FIX'
S = ROOT / 'tmp/GBS-ROOT/LIBCXX-R91-x86_64-retry1/local/BUILD-ROOTS/scratch.x86_64.0'
OUT = E / 'macro-check_R3' / 'attempt3'
if len(sys.argv)>1:
    S=T/'GBS-ROOT/local/BUILD-ROOTS/scratch.x86_64.0'
    OUT=E/'macro-check_R3'/'actual-RBS-root'
OUT.mkdir(parents=True,exist_ok=True)
BASE = {'cereal':'28aedc95fdc600568c57babf28ff717e0b2bca7d','icu':'dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330','qemu-accel':'2737f3bf78ed33be4333114c0e5c741a1deca467'}
def command(label, argv):
    p = OUT / label
    p.with_suffix('.command.txt').write_text(shlex.join(argv)+'\n')
    r = subprocess.run(argv, capture_output=True)
    p.with_suffix('.stdout').write_bytes(r.stdout)
    p.with_suffix('.stderr').write_bytes(r.stderr)
    p.with_suffix('.exitcode').write_text(str(r.returncode)+'\n')
    if r.returncode: raise RuntimeError((label,r.returncode,r.stderr.decode(errors='replace')))
    return r.stdout.decode()
conf=(ROOT/'docs/progress/QB_1194606/evidence/project_config.from-log').read_text()
macros=[x[1:] for x in conf.splitlines() if x.startswith(('%_toolchain ','%toolchain_is() '))]
assert len(macros)==2
common=['bwrap','--ro-bind',str(S),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--ro-bind',str(T),'/tmp/task','--setenv','PATH','/usr/bin:/bin','--','rpmspec','-P','--define','_binary_payload w5.xzdio']
for macro in macros: common+=['--define',macro]
rows=[]
for pkg,sha in BASE.items():
    repo=T/'repos'/pkg
    for spec in sorted((repo/'packaging').glob('*.spec')):
        baseline=T/'baselines'/pkg/spec.name
        baseline.parent.mkdir(parents=True,exist_ok=True)
        baseline.write_bytes(subprocess.check_output(['git','-C',str(repo),'show',sha+':packaging/'+spec.name]))
        for mode,extra in [('default',[]),('override-gcc',['--define','_toolchain_override gcc']),('toolchain-gcc',['--define','_toolchain gcc']),('undefined',['--undefine','_toolchain'])]:
            vals=[]
            for kind,path in [('baseline',baseline),('candidate',spec)]:
                text=command(f'{spec.stem}-{mode}-{kind}',common+extra+['/tmp/task/'+str(path.relative_to(T))])
                vals.append([line for line in text.splitlines() if line.strip()])
            before,after=vals
            diff='\n'.join(difflib.unified_diff(before,after,fromfile='baseline',tofile='candidate',lineterm=''))+'\n'
            (OUT/f'{spec.stem}-{mode}.diff').write_text(diff)
            # Expected changes only: no general whitespace normalization, only blank lines.
            if pkg=='cereal':
                value='ON' if mode=='default' else 'OFF'
                expect=[line.replace('cmake .. -DSKIP_PORTABILITY_TEST=ON',f'cmake .. -DCLANG_USE_LIBCPP={value} -DSKIP_PORTABILITY_TEST=ON') for line in before]
                ok=expect==after and sum('-DCLANG_USE_LIBCPP='+value in x for x in after)==1
            elif pkg=='icu':
                removed=[x for x in after if not ('Use_iquote_for_tz2icu_source_directory.patch' in x or ('Patch #0 ' in x))]
                # RPM emits a marker and one patch invocation; retain exact full diff for review.
                ok=removed==before
            else:
                start=next(i for i,x in enumerate(after) if x=='# Include copied libc++ SONAME paths in the generated file list.')
                end=next(i for i in range(start,len(after)) if after[i]=='done')
                expected=['# Include copied libc++ SONAME paths in the generated file list.',
                    'for library in libc++.so.1 libc++abi.so.1; do',
                    '  if test -e "${buildroot_emul_path}/usr/lib64/$library"; then',
                    '    echo "/usr/lib64/$library" >> clang.list','  fi','done']
                ok=after[start:end+1]==expected and after[:start]+after[end+1:]==before
            rows.append(dict(package=pkg,spec=spec.name,mode=mode,result='PASS' if ok else 'FAIL'))
            (OUT/'results.json').write_text(json.dumps(rows,indent=2)+'\n')
            print(rows[-1],flush=True)
            if not ok: raise RuntimeError('Macro equivalence failure; inspect original diff, do not alter recipe automatically')
