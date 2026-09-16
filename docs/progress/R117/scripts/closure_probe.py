from common import *
import shlex
for lib,header in [('libcxx',LLVM/'vector'),('libstdcxx',GCC/'std/vector')]:
 row=run('deps_'+lib,[CC,*FLAGS[lib],'-std=c++23','-x','c++-header','-M',header],allowed=(1,))
 if row['exit']:continue
 text=(ROOT/(row['raw']+'.stdout')).read_text().replace('\\\n',' ')
 paths=shlex.split(text.split(':',1)[1]);roots=[LLVM] if lib=='libcxx' else [GCC,gnu_public,gnu_config,ROOT/'codes/gcc/libstdc++-v3/libsupc++']
 files=[p for p in paths if any(pathlib.Path(p).is_relative_to(r) for r in roots) and pathlib.Path(p).suffix not in ['.inc','.def']]
 files=list(dict.fromkeys([str(header),*files]))
 graph=TMP/(lib+'_closure.json')
 result=run('closure_'+lib,[CC,*FLAGS[lib],'-std=c++23','-x','c++-header','-extract-api',*files,'-o',graph],allowed=(1,))
 print(lib,len(files),result)
 if graph.exists():
  data=json.loads(graph.read_text());print('symbols',len(data['symbols']));print([s.get('pathComponents') for s in data['symbols'] if s['names']['title']=='vector'][:8])
