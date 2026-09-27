from common import *
import argparse,csv
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
ap=argparse.ArgumentParser()
ap.add_argument('--root',type=pathlib.Path,default=TMP/'root')
ap.add_argument('--rpm-manifest',type=pathlib.Path,default=OUT/'RPMS.json')
ap.add_argument('--compiler',default='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++')
ap.add_argument('--stdlib',choices=['libstdc++','libc++'],default='libstdc++')
ap.add_argument('--config',type=pathlib.Path,default=OUT/'fixtures.json')
ap.add_argument('--expected-symbols',type=pathlib.Path)
ap.add_argument('--edges',nargs='*',type=int)
a=ap.parse_args();root=a.root.resolve();gate()
edges={int(r['edge']):r for r in csv.DictReader((OUT/'EDGES.tsv').open(),delimiter='\t')}
if a.stdlib=='libc++' and not a.expected_symbols:raise SystemExit('libc++ requires an independently verified symbol map; do not reuse GNU manglings')
symbols={int(r['edge']):r['symbol'] for r in csv.DictReader(a.expected_symbols.open(),delimiter='\t')} if a.expected_symbols else {k:v['x86_raw_symbol'].split('@')[0] for k,v in edges.items()}
config=json.loads(a.config.read_text());rpmhash=sha(a.rpm_manifest)
libdirs=[root/x for x in ('usr/lib64','lib64','usr/lib','lib','usr/lib64/hal','usr/lib/hal')];libpath=':'.join(map(str,libdirs))
loader=next(p for p in (root/'lib64/ld-linux-x86-64.so.2',root/'usr/lib64/ld-linux-x86-64.so.2') if p.is_file())
tag=str(time.time_ns());dest=OUT/'runs'/tag;dest.mkdir(parents=True)
build=TMP/'build'/tag;build.mkdir(parents=True)
run('compiler_version',[a.compiler,'--version']);results=[]
for row in config:
 e=row['edge']
 if a.edges and e not in a.edges:continue
 gate();result=dict(edge=e,stdlib=a.stdlib,rpm_manifest=str(a.rpm_manifest),rpm_manifest_sha256=rpmhash,rounds=[],fixture=row.get('fixture'),status='NOT_AVAILABLE',config_sha256=sha(a.config),driver_sha256=sha(__file__),common_sha256=sha(OUT/'code/common.py'),support_sha256={p.name:sha(p) for p in (OUT/'fixtures').glob('*.h')})
 if not row.get('fixture'):
  result['reason']=row['reason'];results.append(result);save(str(dest.relative_to(OUT))+'/results.json',results);continue
 provider=(root/row['provider'].lstrip('/')).resolve();fixture=OUT/row['fixture'];exe=build/f'edge{e:02}'
 assert provider.is_relative_to(root),provider
 psha=sha(provider);symbol=symbols[e]
 result.update(provider=str(provider),provider_sha256=psha,symbol=symbol,fixture_sha256=sha(fixture))
 cmd=[a.compiler,'--sysroot='+str(root),'--gcc-toolchain='+str(root/'usr'),'-std=c++17','-O0','-g','-fno-inline','-stdlib='+a.stdlib,'-pthread','-I'+str(root/'usr/include'),'-I'+str(OUT/'fixtures')]
 cmd+=['-I'+str(root/d.lstrip('/')) for d in row.get('includes',[])]
 cmd+=[str(fixture),'-o',str(exe)]+['-L'+str(d) for d in libdirs]+['-Wl,-rpath-link,'+libpath,'-Wl,--no-as-needed','-Wl,-z,lazy']+['-l'+lib for lib in row['libs']]
 r=run('build_edge'+str(e),cmd,timeout=180,check=False);result['build_record']=r['record']
 if r['exit']:
  result['reason']='BUILD_FAILED';result['build_exit']=r['exit']
 else:
  result['executable_sha256']=sha(exe)
  for i in range(1,6):
   gate();nm=run(f'und_{e}_{i}',['nm','-D','--undefined-only',exe]);matched=[s for s in nm['stdout'].splitlines() if s.split() and s.split()[-1].split('@')[0]==symbol]
   item=dict(round=i,und_record=nm['record'],und_exact=bool(matched),und_lines=matched)
   if not matched:item.update(valid=False,reason='EXACT_UND_ABSENT')
   else:
    assert sha(provider)==psha
    environment=['env','-u','LD_BIND_NOW','-u','LD_PRELOAD','-u','LD_AUDIT']
    if row.get('headless'):environment+=['-u','DISPLAY','-u','WAYLAND_DISPLAY','XDG_CACHE_HOME='+str(build/'cache')]
    r=run(f'run_{e}_{i}',environment+['LD_DEBUG=bindings',loader,'--library-path',libpath,exe,str(provider),psha],cwd=build,timeout=30,check=False)
    assert sha(provider)==psha
    maps=[s[4:] for s in r['stdout'].splitlines() if s.startswith('MAP ')]
    hashes={p:sha(p) for p in maps if pathlib.Path(p).is_file()}
    provider_ok=str(provider) in maps
    gnu=any('libstdc++.so' in p for p in maps);cxx=any('libc++.so' in p for p in maps)
    bindings=[s for s in r['stderr'].splitlines() if ('binding file '+str(exe)+' ') in s and symbol in s]
    item.update(exit=r['exit'],record=r['record'],maps=hashes,provider_mapped=provider_ok,gnu_mapped=gnu,cxx_mapped=cxx,target_bindings=bindings)
    item['valid']=r['exit']==0 and provider_ok and bool(bindings) and ('LIFETIME ' in r['stdout']) and ('VALUES ' in r['stdout']) and (gnu and not cxx if a.stdlib=='libstdc++' else cxx)
   result['rounds'].append(item);save(str(dest.relative_to(OUT))+f'/edge{e:02}.json',result)
  if all(x['valid'] for x in result['rounds']):result.update(status='GNU_BASELINE_5_OF_5' if a.stdlib=='libstdc++' else 'MEASURED_OTHER_CONFIGURATION',reason='')
  else:result['reason']='UND_OR_RUNTIME_OR_ASSERTION_FAILED'
 results.append(result);save(str(dest.relative_to(OUT))+'/results.json',results)
 print('EDGE_RESULT',e,result['status'],result.get('reason'),flush=True)
save('LATEST_RUN.json',dict(path=str(dest.relative_to(OUT)),edges=[r['edge'] for r in results]))
