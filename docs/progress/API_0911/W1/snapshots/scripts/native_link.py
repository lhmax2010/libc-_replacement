#!/usr/bin/env python3
import pathlib,json,subprocess,sys,re,hashlib
out=pathlib.Path('docs/progress/API_0911/W1');tmp=pathlib.Path('tmp/API_0911/W1').resolve()
rows=json.loads((out/'provider_paths.json').read_text())
libs=[x['local_path'] for x in rows if pathlib.Path(x['path']).name in ['libcapi-appfw-app-control.so.0.26.2','libcapi-appfw-app-common.so.0.16.3','libcapi-appfw-application.so.0.10.2','libappcore-agent.so.1.7.7']]
b=pathlib.Path('tmp/IMPL_0908/build-native').resolve()
def run(stem,cmd):return subprocess.run([sys.executable,'docs/progress/API_0911/record.py',str(out/'raw'/stem),*cmd]).returncode
results=[]
for kind in ['gnu','cxx']:
 for case in range(1,5):
    stem=f'native_final_{kind}_{case}'; exe=tmp/stem
    cmd=['g++'] if kind=='gnu' else ['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-nostdlib++','-L'+str(b/'lib'),'-Wl,-rpath-link,'+str(b/'lib')]
    cmd+=['-O0',str(tmp/f'native_{kind}_{case}.o'),str(out/'native_entry.cpp'),'-Wl,--allow-shlib-undefined',*libs]
    if kind=='cxx':cmd+=['-lc++','-lc++abi']
    cmd+=['-o',str(exe)]
    rc=run(stem+'_link',['nice','-n','15','ionice','-c','3',*cmd])
    if rc==0:
        run(stem+'_dynamic',['readelf','-dW',str(exe)])
        run(stem+'_undefined',['nm','-uC',str(exe)])
        text=(out/'raw'/(stem+'_undefined.stdout')).read_text()
        platform=[l.strip().split(' U ',1)[-1] for l in text.splitlines() if re.search(r'\b(?:app_|ui_app_|service_app_)',l)]
        positive='std::' in text
        assert positive,'C++ 符号正向控制失效'
        results.append(dict(kind=kind,case=case,link_exit=rc,positive_cpp=positive,platform_imports=platform,sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),execution='NOT_OBSERVED'))
    else:results.append(dict(kind=kind,case=case,link_exit=rc))
(out/'native_final_results.json').write_text(json.dumps(results,indent=2)+'\n')
print(json.dumps(results,indent=2))
