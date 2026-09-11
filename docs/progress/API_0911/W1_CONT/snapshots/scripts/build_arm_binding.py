#!/usr/bin/env python3
import pathlib,subprocess,sys,csv,json,hashlib,shlex,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT/arm';b=r/'tmp/IMPL_0908/build-armv7l';base=pathlib.Path('/home/toolchain/development/libc++_replacement')
with (r/'docs/progress/P7_0909/inputs/binary_package_records.tsv').open() as f:records=list(csv.DictReader(f,delimiter='\t'))
def run(stem,cmd):
 rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/(stem+os.environ.get('API_ARM_BIND_ATTEMPT',''))),'nice','-n','15','ionice','-c','3',*cmd]).returncode
 if rc:raise SystemExit(rc)
packages=[]
for name in ['dali2-devel','dali2','dali2-adaptor-devel']:
 matches=[x for x in records if x['name']==name and x['arch']=='armv7l'];assert len(matches)==1
 x=matches[0];rpm=base/'tmp/R11/rpms'/x['repo_id']/x['location'];assert hashlib.sha256(rpm.read_bytes()).hexdigest()==x['checksum']
 dest=t/'sdk'/name;dest.mkdir(parents=True,exist_ok=True)
 run('arm_binding_extract_'+name,['bash','-o','pipefail','-c','cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(str(rpm))+' | cpio -idmu --quiet --no-absolute-filenames'])
 packages.append(dict(name=name,rpm=str(rpm),rpm_sha256=x['checksum']))
umbrella=list((t/'sdk/dali2-adaptor-devel').rglob('dali.h'));assert len(umbrella)==1;sdk=umbrella[0].parent.parent
src=base/'tmp/corpus/extracted/dali2-csharp-binder-2.5.26-1.src.rpm/unpacked/dali2-csharp-binder-2.5.26.tar.xz/dali2-csharp-binder-2.5.26'
core=base/'tmp/corpus/extracted/dali2-2.5.26-1.src.rpm/unpacked/dali2-2.5.26.tar.gz/dali2-2.5.26'
units=[src/'dali-csharp-binder/common/common.cpp',src/'dali-csharp-binder/dali-core/property-value-wrap.cpp']
gnu_base=t/'sdk/libstdc++-devel/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/include/c++';gnu_lib=t/'sdk/libstdc++/usr/lib/libstdc++.so.6'
cc=[str(base/'progress/R33/tools/tizen-clang++'),'--target=armv7l-tizen-linux-gnueabi','--sysroot='+str(base/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'),'--ld-path='+str(base/'progress/R36/tools/armv7l-ld'),'-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb','-std=c++17','-O0','-g','-fPIC','-shared','-nostdinc++','-nostdlib++','-I'+str(src),'-I'+str(core),'-I'+str(sdk)]
for kind in ['gnu','cxx']:
 inc=['-I'+str(gnu_base),'-I'+str(gnu_base/'armv7l-tizen-linux-gnueabi')] if kind=='gnu' else ['-I'+str(b/'include/c++/v1')]
 provider=t/'sdk/dali2/usr/lib/libdali2-core.so.2.0.0' if kind=='gnu' else t/'dali_core_cxx/libdali2-core.so.2.0.0'
 run('arm_binding_build_'+kind,[*cc,*inc,*map(str,units),str(provider),'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',str(gnu_lib),'-Wl,-z,defs','-o',str(t/('binder_property_'+kind+'.so'))])
(o/'arm_binding_identity.json').write_text(json.dumps(dict(packages=packages,sources=[dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in units],umbrella=dict(path=str(umbrella[0]),sha256=hashlib.sha256(umbrella[0].read_bytes()).hexdigest())),indent=2)+'\n')
