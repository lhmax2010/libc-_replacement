#!/usr/bin/env python3
"""原样编译真实 binder 的 Property::Value 包装与 common；不是完整 binder 包重建。"""
import pathlib,subprocess,sys,hashlib,json,gzip,csv,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native'
corpus=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/corpus/extracted')
src=corpus/'dali2-csharp-binder-2.5.26-1.src.rpm/unpacked/dali2-csharp-binder-2.5.26.tar.xz/dali2-csharp-binder-2.5.26'
core=corpus/'dali2-2.5.26-1.src.rpm/unpacked/dali2-2.5.26.tar.gz/dali2-2.5.26'
units=[src/'dali-csharp-binder/common/common.cpp',src/'dali-csharp-binder/dali-core/property-value-wrap.cpp']
identity=[dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in units]
with gzip.open(r/'docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
 umbrella=[x for x in csv.DictReader(f,delimiter='\t') if x['arch']=='x86_64' and x['path'].endswith('/dali/dali.h')]
assert len({x['file_sha256'] for x in umbrella})==1
x=umbrella[0];h=x['rpm_sha256'];p=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')/h[:2]/h/x['path'].lstrip('/')
assert hashlib.sha256(p.read_bytes()).hexdigest()==x['file_sha256']
identity.append(dict(path=str(p),sha256=x['file_sha256'],rpm_sha256=h))
sdk=p.parent.parent
gnu=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted/b8/b81d60c07222ae83cb728efe33675a21d09db564bc9d4bdd60e6a6b236816691/usr/lib64')
def run(stem,cmd):
 return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/(stem+os.environ.get('API_BINDER_ATTEMPT',''))),'nice','-n','15','ionice','-c','3',*cmd]).returncode
for kind in ['gnu','cxx']:
 cc=['g++'] if kind=='gnu' else ['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
 lib=gnu if kind=='gnu' else t/'dali_core_cxx'
 cmd=cc+['-std=c++17','-O0','-g','-fPIC','-shared','-I'+str(src),'-I'+str(core),'-I'+str(sdk),*map(str,units),'-L'+str(lib),'-Wl,-z,defs','-Wl,--no-as-needed','-ldali2-core','-L'+str(b/'lib'),'-lc++','-lc++abi','-lstdc++','-o',str(t/('binder_property_'+kind+'.so'))]
 if run('binder_property_build_'+kind,cmd):break
for x,p in zip(identity,units):assert x['sha256']==hashlib.sha256(p.read_bytes()).hexdigest()
(o/'binder_property_source_identity.json').write_text(json.dumps(identity,indent=2)+'\n')
