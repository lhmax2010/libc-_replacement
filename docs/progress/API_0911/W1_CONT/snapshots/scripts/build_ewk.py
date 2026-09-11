#!/usr/bin/env python3
import csv,gzip,pathlib,re,sys,subprocess,json,hashlib
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT';cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
with gzip.open(r/'docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
 headers=[x for x in csv.DictReader(f,delimiter='\t') if x['arch']=='x86_64' and x['path'].startswith('/usr/')]
includes=[];identity=[]
def add(name):
 matches=[x for x in headers if pathlib.Path(x['path']).name==name and not any(k in x['path'] for k in ['/opt/','/gcc/','/rustlib/']) and x.get('file_sha256')]
 primary=[x for x in matches if x['path'].startswith('/usr/include/')]
 if primary:matches=primary
 if len({x['file_sha256'] for x in matches})!=1:print('HEADER_UNRESOLVED',name,[(x['name'],x['path']) for x in matches],flush=True);return False
 x=matches[0];h=x['rpm_sha256'];p=cache/h[:2]/h/x['path'].lstrip('/')
 assert hashlib.sha256(p.read_bytes()).hexdigest()==x['file_sha256']
 if p.parent not in includes:includes.append(p.parent)
 identity.append(dict(path=str(p),sha256=x['file_sha256'],rpm_sha256=h));return True
assert add('ewk_cookie_parser.h')
b=r/'tmp/IMPL_0908/build-native';cc='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
for kind in ['gnu','cxx']:
 for attempt in range(1,45):
  cmd=['g++'] if kind=='gnu' else [cc,'-nostdinc++','-I'+str(b/'include/c++/v1'),'-nostdlib++']
  cmd+=['-std=c++17','-O0','-g',*[a for p in includes for a in ['-I',str(p)]],str(o/'ewk_probe.cpp'),'-L'+str(b/'lib'),'-Wl,-rpath,'+str(b/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(t/('ewk_'+kind))]
  stem=f'ewk_{kind}_build_{attempt}'
  rc=subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(o/'raw'/stem),'nice','-n','15','ionice','-c','3',*cmd]).returncode
  if rc==0:break
  err=(o/'raw'/(stem+'.stderr')).read_text();m=re.search(r"fatal error: ['\"]?([^ '\":]+):?['\"]?(?: file not found|: No such file)",err)
  if not m or not add(m[1]):print('BUILD_FAILED',stem,err[-1000:],flush=True);break
(o/'ewk_header_identity.json').write_text(json.dumps(identity,indent=2)+'\n')
