#!/usr/bin/env python3
import pathlib,subprocess,sys,csv,gzip,json,hashlib,re,shlex,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT/arm';t.mkdir(exist_ok=True)
b=r/'tmp/IMPL_0908/build-armv7l';cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted');cc='/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-clang++'
compiler=[cc]
if os.environ.get('API_ARM_NATIVE')=='1':
 compiler=['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','--target=armv7l-tizen-linux-gnueabi','--sysroot=/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0','--ld-path=/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-ld']
tag=os.environ.get('API_ARM_ATTEMPT','')
def run(stem,cmd):return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/(stem+tag)),'nice','-n','15','ionice','-c','3',*cmd]).returncode
with (r/'docs/progress/P7_0909/inputs/binary_package_records.tsv').open() as f:records=list(csv.DictReader(f,delimiter='\t'))
includes=[];identity=[];sdk={}
def package(name):
 if name in sdk:return sdk[name]
 matches=[x for x in records if x['arch']=='armv7l' and x['name']==name];assert len(matches)==1,(name,len(matches))
 x=matches[0];rpm=cache.parent/'rpms'/x['repo_id']/x['location'];assert hashlib.sha256(rpm.read_bytes()).hexdigest()==x['checksum']
 dest=t/'sdk'/name;dest.mkdir(parents=True,exist_ok=True)
 command='cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(str(rpm))+' | cpio -idmu --quiet --no-absolute-filenames'
 assert run('arm_extract_'+name,['bash','-o','pipefail','-c',command])==0
 sdk[name]=dest;identity.append(dict(package=name,rpm=str(rpm),rpm_sha256=x['checksum']));return dest
for x in json.loads((o/'ewk_header_identity.json').read_text()):
 native=[z for z in records if z['checksum']==x['rpm_sha256']];assert len(native)==1
 relative=str(pathlib.Path(x['path']).relative_to(cache/x['rpm_sha256'][:2]/x['rpm_sha256']))
 p=package(native[0]['name'])/relative;assert p.is_file()
 includes.append(p.parent);identity.append(dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
gnu=package('libstdc++-devel');gnu_headers=[p for p in gnu.rglob('vector') if p.parent.name=='c++'];assert len(gnu_headers)==1
gnu_base=gnu_headers[0].parent;gnu_configs=list(gnu.rglob('c++config.h'));assert len(gnu_configs)==1
gnu_target=gnu_configs[0].parent.parent
gnu_runtime=package('libstdc++');gnu_libs=list(gnu_runtime.rglob('libstdc++.so.6'));assert len(gnu_libs)==1
results=[]
for kind in ['gnu','cxx']:
 cmd=[*compiler,'-std=c++17','-O0','-g','-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb','-include',str(o/'arm_monitor.h')]
 cmd+=['-nostdinc++','-I'+str(gnu_base),'-I'+str(gnu_target),'-nostdlib++'] if kind=='gnu' else ['-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
 cmd += ['-I'+str(p) for p in includes]+[str(o/'ewk_probe.cpp'),'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',str(gnu_libs[0]),'-ldl','-o',str(t/('ewk_'+kind))]
 rc=run('arm_ewk_build_'+kind,cmd);results.append(dict(target='ewk_'+kind,returncode=rc))
cmd=[*compiler,'-x','c','-std=c11','-O0','-g','-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb','-include',str(o/'arm_monitor.h'),str(o/'dali_probe.c'),'-ldl','-o',str(t/'dali_probe')]
results.append(dict(target='dali_probe',returncode=run('arm_dali_build',cmd)))
(o/'arm_build_results.json').write_text(json.dumps(results,indent=2)+'\n');(o/'arm_header_identity.json').write_text(json.dumps(identity,indent=2)+'\n')
