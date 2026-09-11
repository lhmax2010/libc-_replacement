#!/usr/bin/env python3
import pathlib,subprocess,sys,json,hashlib,os
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';b=r/'tmp/IMPL_0908/build-native/lib'
cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
cores={'gnu':cache/'b8/b81d60c07222ae83cb728efe33675a21d09db564bc9d4bdd60e6a6b236816691/usr/lib64/libdali2-core.so.2.0.0','cxx':t/'dali_core_cxx/libdali2-core.so.2.0.0'}
full=cache/'38/38113041d61d0b68d0a65af809402ff283177071a8fa0b76bb23c188dfa86dcd/usr/lib64/libdali2-csharp-binder.so.2.0.0'
cases=[(kind,provider,t/('binder_property_'+kind+'.so'),'real_source_subset') for kind in ['gnu','cxx'] for provider in ['gnu','cxx']]
cases += [('gnu',provider,full,'full_rpm_binder') for provider in ['gnu','cxx']]
rows=[];identities={}
tag=os.environ.get('API_DALI_ATTEMPT','')
for consumer,provider,binder,scope in cases:
 if not binder.is_file():rows.append(dict(consumer=consumer,provider=provider,scope=scope,status='NOT_OBSERVED',reason='binding build unavailable'));continue
 core=cores[provider]
 for p in [binder,core,t/'dali_probe']:
  if str(p) not in identities:identities[str(p)]=hashlib.sha256(p.read_bytes()).hexdigest()
 for repeat in range(1,6):
  stem=f'dali_{scope}_{consumer}_to_{provider}_{repeat}{tag}'
  path=':'.join(map(str,[core.parent,t/'deps_binder',t/'closure_ewk',b]))
  cmd=['nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',path,str(t/'dali_probe'),str(binder),str(core)]
  rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),*cmd]).returncode
  out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
  rows.append(dict(consumer=consumer,provider=provider,scope=scope,repeat=repeat,returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 binder=1 core=1' in out,exact='EXACT_VALUES=7 CONSUMER_DELETE_COMPLETED=7 CALLBACK_EXCEPTIONS=0' in out,raw=stem))
(o/('dali_matrix'+tag+'.json')).write_text(json.dumps(rows,indent=2)+'\n')
(o/('dali_binary_identity'+tag+'.json')).write_text(json.dumps(identities,indent=2)+'\n')
