#!/usr/bin/env python3
"""复用既有探针，只在独立目录构建；每条子命令分别留证。"""
import pathlib, subprocess, sys

root = pathlib.Path.cwd()
arch = sys.argv[1]
base = pathlib.Path('/home/toolchain/development/libc++_replacement')
out = root / 'tmp/R115-W3' / arch
raw = root / 'docs/progress/R115/W3/raw' / ('build_' + arch)
out.mkdir(parents=True, exist_ok=True)
counter = 0
def run(label, args):
    global counter
    counter += 1
    subprocess.run([sys.executable, 'docs/progress/R114/code/record.py',
                    str(raw / f'{counter:03}_{label}'), 'nice', '-n', '15', 'ionice', '-c', '3',
                    *map(str,args)], check=True)

compiler = base / ('progress/R36/tools/armv7l-clang++' if arch == 'armv7l' else 'progress/R33/tools/tizen-clang++')
headers = base / f'tmp/R68/build-baseline-{arch}/libcxx/test-suite-install/include/c++/v1'
runtime = base / f'tmp/R69/build-{arch}/lib'
overlay = out / 'overlay'
run('overlay', [sys.executable,'docs/progress/R113/code/make_removed_overlay.py','--header',
               headers/'__condition_variable/condition_variable.h','--source',
               root/'codes/llvm/libcxx/src/condition_variable.cpp','--output',overlay])
common = ['-std=c++20','-O0','-g','-fno-inline','-fPIC','-nostdinc++']
link = ['-nostdlib++','-L',runtime,'-Wl,-rpath-link,'+str(runtime),'-lc++','-lc++abi','-latomic','-pthread','-ldl']
objects=[]
for name in ['condition_variable','mutex','shared_mutex','future','system_error']:
    source = overlay/'condition_variable_removed.cpp' if name=='condition_variable' else root/f'codes/llvm/libcxx/src/{name}.cpp'
    obj=out/(name+'.o'); objects.append(obj)
    run('compile_'+name,[compiler,*common,'-I',overlay/'include','-I',headers,'-c',source,'-o',obj])
run('link_overlay',[compiler,'-shared',*objects,*link,'-Wl,-soname,libremoved_r115.so','-o',out/'libremoved_r115.so'])
cases=[('wait_retained','R111/code/condition_cancel_probe.cpp',False,[],False),
       ('wait_removed','R111/code/condition_cancel_probe.cpp',True,[],True),
       ('facility','R113/code/item1_facility_cancel.cpp',True,[],True)]
for name,defs in [('old_noisy',[]),('old_quiet',['-DWORKER_NO_PUTS']),
                  ('old_fno',['-fno-exceptions']),('old_fno_quiet',['-fno-exceptions','-DWORKER_NO_PUTS'])]:
    cases.append((name,'R112/code/item1_oldcaller_cancel.cpp',False,['-DEXPECT_CLEAN=-1',*defs],True))
for name,source,new_header,defs,new_lib in cases:
    includes = ['-I',overlay/'include','-I',headers] if new_header else ['-I',headers]
    obj=out/(name+'.o')
    run('compile_'+name,[compiler,*common,*includes,*defs,'-c',root/'docs/progress'/source,'-o',obj])
    extra=['-L',out,'-Wl,--no-as-needed','-lremoved_r115'] if new_lib else []
    run('link_'+name,[compiler,obj,*extra,*link,'-Wl,-export-dynamic','-o',out/name])
run('identity',['file',*[out/name for name,*_ in cases],out/'libremoved_r115.so'])
run('hashes',['sha256sum',*sorted(out.glob('*.o')),*[out/name for name,*_ in cases],out/'libremoved_r115.so'])
