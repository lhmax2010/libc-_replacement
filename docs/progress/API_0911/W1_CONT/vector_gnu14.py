#!/usr/bin/env python3
"""补充冻结平台 GNU 14.2 头文件配置，避免仅凭宿主 GNU 13 头文件外推。"""
import pathlib,subprocess,sys,csv,hashlib,shlex,json
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native';base=pathlib.Path('/home/toolchain/development/libc++_replacement')
with (r/'docs/progress/P7_0909/inputs/binary_package_records.tsv').open() as f:records=list(csv.DictReader(f,delimiter='\t'))
x=next(x for x in records if x['name']=='libstdc++-devel' and x['arch']=='x86_64');rpm=base/'tmp/R11/rpms'/x['repo_id']/x['location'];assert hashlib.sha256(rpm.read_bytes()).hexdigest()==x['checksum']
dest=t/'gnu14_sdk';dest.mkdir(exist_ok=True)
def run(stem,cmd):return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
assert run('gnu14_sdk_extract',['bash','-o','pipefail','-c','cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(str(rpm))+' | cpio -idmu --quiet --no-absolute-filenames'])==0
headers=[p.parent for p in dest.rglob('vector') if p.parent.name=='c++'];configs=list(dest.rglob('c++config.h'));assert len(headers)==len(configs)==1
cmd=[str(base/'progress/R33/tools/tizen-clang++'),'-std=c++17','-O0','-g','-nostdinc++','-nostdlib++','-I'+str(headers[0]),'-I'+str(configs[0].parent.parent),str(o/'vector_representation.cpp'),'-L'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(t/'vector_repr_gnu14')]
assert run('vector_repr_build_gnu14',cmd)==0
rows=[]
for repeat in range(1,6):
 stem=f'vector_repr_gnu14_{repeat}'
 rc=run(stem,['prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',str(t/'closure_ewk')+':'+str(b/'lib'),str(t/'vector_repr_gnu14'),str(r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so')])
 out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
 rows.append(dict(library='GNU 14.2 headers / Clang 22.1.8',repeat=repeat,returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 provider=1' in out,representation=out.count('three_pointer_match=1')==4,exact_and_destroy='REPRESENTATION_AND_OWN_LIBRARY_COPY_DESTROY_PASS cases=4' in out,actual_SetSyncBinaryReply_called=False,raw=stem))
(o/'vector_gnu14_results.json').write_text(json.dumps(dict(package=x,headers=[dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in [headers[0]/'vector',configs[0]]],runs=rows),indent=2)+'\n')
