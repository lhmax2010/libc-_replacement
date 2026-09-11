#!/usr/bin/env python3
"""真实 Chromium GNU provider；反方向不伪造 provider。"""
import pathlib,subprocess,sys,json
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT'
rows=[]
for consumer in ['gnu','cxx']:
 for repeat in range(1,6):
  stem=f'ewk_{consumer}_to_gnu_{repeat}'
  cmd=['nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','30',str(t/'closure_ewk/ld-linux-x86-64.so.2'),'--library-path',str(t/'closure_ewk')+':'+str(r/'tmp/IMPL_0908/build-native/lib'),str(t/('ewk_'+consumer)),str(r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so')]
  rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),*cmd]).returncode
  out=(o/'raw'/(stem+'.stdout')).read_text(errors='replace')
  rows.append(dict(consumer=consumer,provider='gnu',repeat=repeat,returncode=rc,maps='MAP_ASSERT cxx=1 gnu=1 provider=1' in out,exact='EXACT_FIELDS=1' in out,destroyed='CONSUMER_OBJECTS_DESTROYED=1' in out,raw=stem))
(o/'ewk_matrix.json').write_text(json.dumps(rows,indent=2)+'\n')
