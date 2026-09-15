#!/usr/bin/env python3
"""候选打包入口。配置必须由产品工具链提供；不猜测迁移后 ABI/路径。"""
import argparse,hashlib,json,os,pathlib,resource,shutil,subprocess,time
p=argparse.ArgumentParser();p.add_argument('config');p.add_argument('output');p.add_argument('--mode',choices=['separate','embedded'],required=True);a=p.parse_args()
c=json.loads(pathlib.Path(a.config).read_text());out=pathlib.Path(a.output).resolve();out.mkdir(parents=True,exist_ok=False)
src=pathlib.Path(__file__).resolve().parent/'src'
required=['compiler','gnu_flags','libcxx_flags','includes','link_flags','provider_path','provider_abi_evidence','consumer_abi_evidence','direction']
assert all(c.get(k) for k in required),'Missing explicit product ABI/toolchain configuration'
assert c['direction'] in ['production','reverse-mechanism']
assert pathlib.Path(c['provider_path']).is_absolute()
assert shutil.disk_usage(out).free>=1024**3,'Less than 1 GiB free for small target; stop'
mem_kib=int(next(s.split()[1] for s in pathlib.Path('/proc/meminfo').read_text().splitlines() if s.startswith('MemTotal:')))
cap=min(4*1024**3,mem_kib*1024//2)
def limited():resource.setrlimit(resource.RLIMIT_AS,(cap,cap));os.nice(19)
steps=[]
def run(args):
 n=len(steps)+1;cmd=['ionice','-c','3',*map(str,args)];base=out/('%03d'%n)
 base.with_suffix('.command.json').write_text(json.dumps(cmd)+'\n')
 t=time.time()
 with base.with_suffix('.stdout').open('wb') as stdout,base.with_suffix('.stderr').open('wb') as stderr:
  r=subprocess.run(cmd,stdout=stdout,stderr=stderr,preexec_fn=limited)
 base.with_suffix('.exitcode').write_text(str(r.returncode)+'\n')
 steps.append({'command':cmd,'exit':r.returncode,'seconds':time.time()-t})
 assert r.returncode==0,'Build failed; inspect '+str(base)
common=['-std=c++17','-O2','-fPIC','-fvisibility=hidden','-fexceptions','-D_GNU_SOURCE','-I'+str(src),'-include',str(src/'abi_guard.h'),*c['includes']]
front='gnu' if c['direction']=='production' else 'libcxx';back='libcxx' if front=='gnu' else 'gnu'
for part,side in [('front',front),('back',back)]:
 flags=c[side+'_flags'];assert isinstance(flags,list)
 defines=['-DP12_BUILD_GNU' if side=='gnu' else '-DP12_BUILD_LIBCXX']
 if part=='front' and a.mode=='embedded':defines+=['-DP12_EMBEDDED']
 if part=='back':defines+=['-DP12_PROVIDER_PATH="'+c['provider_path']+'"']
 run([c['compiler'],*flags,*common,*defines,'-c',src/('ewk_'+part+'.cpp'),'-o',out/(part+'.o')])
if a.mode=='separate':
 run([c['compiler'],'-shared',out/'front.o',out/'back.o',*c['link_flags'],'-Wl,-z,defs,-z,relro,-z,now','-Wl,-soname,libewk-cookie-adaptor.so.1','-Wl,--version-script='+str(src/'ewk_release.exports'),'-o',out/'libewk-cookie-adaptor.so.1'])
 inputs=[str(out/'libewk-cookie-adaptor.so.1')]
else:inputs=[str(out/'front.o'),str(out/'back.o')]
# GN accepts linker inputs as absolute paths; the source patch additionally
# records them as build inputs. Runtime flags must match the product ABI.
(out/'gn.args').write_text('ewk_cookie_adaptor_enabled=true\newk_cookie_adaptor_link_inputs='+json.dumps(inputs)+'\newk_cookie_adaptor_runtime_flags='+json.dumps(c['link_flags'])+'\n')
(out/'manifest.json').write_text(json.dumps({'mode':a.mode,'config':c,'memory_max_bytes':cap,'parallelism':1,'steps':steps,'sha256':{f.name:hashlib.sha256(f.read_bytes()).hexdigest()for f in out.iterdir()if f.suffix=='.o' or f.name.endswith('.so.1')}},indent=2)+'\n')
