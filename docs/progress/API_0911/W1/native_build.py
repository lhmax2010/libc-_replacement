#!/usr/bin/env python3
import pathlib, csv, gzip, subprocess, sys, re, json, hashlib
out=pathlib.Path('docs/progress/API_0911/W1'); tmp=pathlib.Path('tmp/API_0911/W1').resolve();tmp.mkdir(parents=True,exist_ok=True)
cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
def run(stem,cmd):
    return subprocess.run([sys.executable,'docs/progress/API_0911/record.py',str(out/'raw'/stem),*cmd]).returncode
with gzip.open('docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
    headers=[x for x in csv.DictReader(f,delimiter='\t') if x['arch']=='x86_64']
includes=[]; evidence=[]
def add(name):
    matches=[x for x in headers if pathlib.Path(x['path']).name==name and x['path'].startswith('/usr/include/')]
    if not matches: print('HEADER_NOT_AVAILABLE',name,flush=True);return False
    # 同一文件的多个拥有者：内容相同才可合并，否则不替人工选变体。
    valid=[x for x in matches if x.get('file_sha256')]
    if len({x['file_sha256'] for x in valid})!=1:
        print('HEADER_AMBIGUOUS',name,[(x['name'],x.get('file_sha256')) for x in matches],flush=True);return False
    x=valid[0];h=x['rpm_sha256'];p=cache/h[:2]/h/x['path'].lstrip('/')
    assert hashlib.sha256(p.read_bytes()).hexdigest()==x['file_sha256']
    if p.parent not in includes:includes.append(p.parent)
    evidence.append(dict(name=name,package=x['name'],path=str(p),sha256=x['file_sha256']))
    return True
for name in ['app_control.hpp','app_common.hpp','service_app.hpp','app.hpp']:
    assert add(name)
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
b=pathlib.Path('tmp/IMPL_0908/build-native').resolve()
result=[]
for kind in ['gnu','cxx']:
    for case in range(1,5):
        for attempt in range(1,16):
            stem=f'native_r2_{kind}_{case}_build_{attempt}'
            cmd=([compiler,'-nostdinc++','-I'+str(b/'include/c++/v1')] if kind=='cxx' else ['g++'])
            cmd+=['-std=c++17','-O0','-g','-fPIC','-DCASE='+str(case),*[a for p in includes for a in ['-I',str(p)]],'-c',str(out/'native_probe.cpp'),'-o',str(tmp/f'native_{kind}_{case}.o')]
            rc=run(stem,['nice','-n','15','ionice','-c','3',*cmd])
            if rc==0:break
            err=(out/'raw'/(stem+'.stderr')).read_text()
            missing=re.search(r'(?:fatal error: |fatal error: [\x27])([^\s\x27:]+\.h(?:pp|h)?)[:\x27]',err)
            if not missing or not add(missing[1]):break
        result.append(dict(kind=kind,case=case,compile_exit=rc,last_log=stem))
        if rc==0:
            run(f'native_r2_{kind}_{case}_undefined',['nm','-uC',str(tmp/f'native_{kind}_{case}.o')])
            run(f'native_r2_{kind}_{case}_relocations',['readelf','-Wr',str(tmp/f'native_{kind}_{case}.o')])
(out/'native_build_results.json').write_text(json.dumps(result,indent=2)+'\n')
(out/'native_header_identity.json').write_text(json.dumps(evidence,indent=2)+'\n')
print(json.dumps(result,indent=2),flush=True)
