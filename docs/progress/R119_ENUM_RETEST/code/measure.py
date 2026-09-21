from common import *
import ast,csv,re
PRIOR=ROOT/'docs/progress/R119_ARM_RETEST'
DIV=ROOT/'docs/progress/R119_DIVERGENT'
TMP=ROOT/'tmp/R119_ENUM_RETEST';TMP.mkdir(parents=True,exist_ok=True)
IDS=['T0008','T0009','T0010','T0011']
def main():
 gate('measure_gate')
 configs={'armv7l':json.loads((PRIOR/'CONFIGURATION.json').read_text()),'aarch64':json.loads((PRIOR/'AARCH64_CONFIGURATION.json').read_text())}
 rows=[r for r in csv.DictReader((DIV/'DIVERGENT_TYPES.tsv').open(),delimiter='\t') if r['id'] in IDS]
 assert [r['id'] for r in rows]==IDS
 p=DIV/'code/measure.py';oldmeta=json.loads((PRIOR/'PROBE_REUSE.json').read_text());assert sha(p)==oldmeta['sha256']
 tree=ast.parse(p.read_text());nodes=[n for n in tree.body if (isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='PRE' for t in n.targets)) or (isinstance(n,ast.FunctionDef) and n.name=='source')]
 assert len(nodes)==2
 ns={'re':re};exec(compile(ast.Module(body=nodes,type_ignores=[]),str(p),'exec'),ns)
 oldcpp=(PRIOR/'probes/types.cpp').read_text();assert oldcpp.startswith(ns['PRE'])
 extra=oldcpp.split('int main(){',1)[1].split('emit<int>("CONTROL_INT")',1)[0]
 code=ns['source'](rows).replace('int main(){','int main(){'+extra,1)
 src=OUT/'probes/types.cpp';src.parent.mkdir(parents=True,exist_ok=True);src.write_text(code)
 save('PROBE_REUSE.json',dict(generator=str(p.relative_to(ROOT)),generator_sha256=sha(p),prior_probe=str((PRIOR/'probes/types.cpp').relative_to(ROOT)),prior_probe_sha256=sha(PRIOR/'probes/types.cpp'),new_probe_sha256=sha(src),same_PRE=True,same_source_function=True,same_macro_print=True,selected=IDS,change='仅将原13表达式列表替换为4项；emit/TypeToken/正向对照/版本宏逻辑未改'))
 results=[];recorded_configs=[];prior_hashes={}
 for arch,clist in configs.items():
  gate('gate_'+arch)
  for c in clist:
   lib=c['library'];flags=c['flags'];links=c['links'];cc=c.get('compiler',configs['armv7l'][0]['compiler']);sysroot=c['sysroot']
   header_list=c.get('includes')
   hashes={x['path']:x['sha256'] for x in header_list} if header_list else c['headers']
   for hp,h in hashes.items():assert sha(hp)==h,hp
   prior_hashes.update(hashes)
   exe=TMP/f'{arch}_{lib}'
   b=run(f'build_{arch}_{lib}',[cc,*flags,str(src),*links,'-pthread','-o',str(exe)])
   sy=run(f'symbols_{arch}_{lib}',['readelf','-h','-Ws','-d','--wide',str(exe)])
   assert ('AArch64' if arch=='aarch64' else 'ARM') in sy['stdout']
   mac=run(f'macros_{arch}_{lib}',[cc,*flags,'-dM','-E',str(src)])
   expected='#define _GLIBCXX_RELEASE 14' if lib=='gnu' else '#define _LIBCPP_VERSION 220108'
   assert expected in mac['stdout']
   inc=run(f'includes_{arch}_{lib}',[cc,*flags,'-H','-fsyntax-only',str(src)])
   actual_headers={}
   for line in inc['stderr'].splitlines():
    m=re.match(r'^\.+ (/.+)$',line)
    if m and pathlib.Path(m[1]).is_file():actual_headers[m[1]]=sha(m[1])
   if arch=='armv7l':
    qemu=c['emulator'];loader=sysroot+'/lib/ld-linux.so.3'
    build=ROOT/'tmp/IMPL_0908/build-armv7l';sdk=ROOT/'tmp/API_0911/W1_CONT/arm/sdk'
    libpath=':'.join(map(str,[build/'lib',sdk/'libstdc++/usr/lib',pathlib.Path(sysroot)/'lib',pathlib.Path(sysroot)/'usr/lib']))
   else:qemu=c['qemu'];loader=sysroot+'/lib64/ld-linux-aarch64.so.1';libpath=sysroot+'/lib64:'+sysroot+'/usr/lib64'
   assert sha(qemu)==c.get('emulator_sha256',c.get('qemu_sha256'))
   observations=[]
   for repeat in range(1,6):
    obs=run(f'run_{arch}_{lib}_{repeat}',['timeout','30',qemu,'-L',sysroot,loader,'--library-path',libpath,str(exe)])
    assert ('LIBRARY=libstdc++ RELEASE=14' if lib=='gnu' else 'LIBRARY=libc++ VERSION=220108') in obs['stdout']
    assert ('TARGET_POINTER=4 TARGET_LONG=4 TARGET_LONG_LONG=8' if arch=='armv7l' else 'TARGET_POINTER=8 TARGET_LONG=8 TARGET_LONG_LONG=8') in obs['stdout']
    values={s.split('\t')[0]:s for s in obs['stdout'].splitlines() if '\t' in s}
    for name,enc in [('INT','i'),('LONG','l'),('LONG_LONG','x'),('CONST_REF','RKl')]:assert f'TOKEN=9TypeTokenI{enc}E\t' in values['CONTROL_'+name]
    assert set(values)==set(IDS)|{'CONTROL_INT','CONTROL_LONG','CONTROL_LONG_LONG','CONTROL_CONST_REF'}
    observations.append((values,obs['record']))
   for row in rows:
    lines=[v[row['id']] for v,r in observations];assert len(set(lines))==1
    line=lines[0];assert 'UNAVAILABLE' not in line
    m=dict(id=row['id'],expression=row['expression'],arch=arch,library=lib,environment='QEMU_USER',type=re.search(r'TYPE=.*?\[T = (.*?)\]',line)[1],token=re.search(r'TOKEN=([^\t]+)',line)[1],encoding=re.search(r'DIRECT=([^\t]+)',line)[1],size=int(re.search(r'SIZE=(\d+)',line)[1]),align=int(re.search(r'ALIGN=(\d+)',line)[1]),output=line,runs=[r for v,r in observations],repeats=5,source_sha256=sha(src),executable_sha256=sha(exe),build=b['record'],symbols=sy['record'])
    results.append(m);save(f'measurements/{arch}_{row["id"]}_{lib}.json',m)
   recorded_configs.append(dict(arch=arch,library=lib,compiler=cc,flags=flags,links=links,sysroot=sysroot,configuration_exactly_reused=True,prior_config=str((PRIOR/('CONFIGURATION.json' if arch=='armv7l' else 'AARCH64_CONFIGURATION.json')).relative_to(ROOT)),qemu=qemu,loader=loader,library_path=libpath,header_sha256=actual_headers,macro_record=mac['record']))
 save('MEASUREMENTS.json',results);save('CONFIGURATIONS.json',recorded_configs);save('PRIOR_HEADER_SHA256.json',prior_hashes)
 assert all(sha(p)==h for p,h in prior_hashes.items())
 table=[]
 for arch in configs:
  for r in rows:
   a,b=[next(x for x in results if x['arch']==arch and x['id']==r['id'] and x['library']==lib) for lib in ['gnu','cxx']]
   table.append(dict(arch=arch,expression=r['expression'],gnu_type=a['type'],cxx_type=b['type'],gnu_token=a['token'],cxx_token=b['token'],gnu_encoding=a['encoding'],cxx_encoding=b['encoding'],gnu_size=a['size'],cxx_size=b['size'],gnu_align=a['align'],cxx_align=b['align'],result='IDENTICAL' if a['token']==b['token'] else 'DIVERGENT'))
 with (OUT/'TYPES.tsv').open('w') as f:
  w=csv.DictWriter(f,fieldnames=list(table[0]),delimiter='\t');w.writeheader();w.writerows(table)
 print(json.dumps(table,ensure_ascii=False,indent=2))
if __name__=='__main__':main()
