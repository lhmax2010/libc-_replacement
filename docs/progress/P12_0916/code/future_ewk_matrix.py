#!/usr/bin/env python3
"""生产方向补测入口。真实产物及已按两侧头文件编译的探针缺失时退出 77。"""
import argparse,hashlib,json,pathlib,subprocess,sys,time
p=argparse.ArgumentParser();p.add_argument('manifest');p.add_argument('output');a=p.parse_args()
if not pathlib.Path(a.manifest).is_file():print('NOT_AVAILABLE: production manifest / real libc++ chromium-efl');sys.exit(77)
c=json.loads(pathlib.Path(a.manifest).read_text())
keys=['loader','provider','app_gnu','app_libcxx','control','separate','embedded']
if any(not c.get(k) or not pathlib.Path(c[k]).is_file() for k in keys):print('NOT_AVAILABLE: missing '+','.join(k for k in keys if not c.get(k) or not pathlib.Path(c[k]).is_file()));sys.exit(77)
assert c.get('direction')=='gnu-consumer-to-libcxx-provider'
assert c.get('provider_build_evidence') and pathlib.Path(c['provider_build_evidence']).is_file(),'Require compiler/header/namespace build provenance, not a filename assertion'
assert c.get('library_path') and c.get('sha256')
for k in keys:assert hashlib.sha256(pathlib.Path(c[k]).read_bytes()).hexdigest()==c['sha256'][k],k
out=pathlib.Path(a.output);out.mkdir(parents=True,exist_ok=False);rows=[]
for side,mode in [('libcxx','control'),('gnu','separate'),('gnu','embedded'),('gnu','control')]:
 for repeat in range(1,6):
  stem=out/(side+'_'+mode+'_'+str(repeat))
  cmd=['nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','40',c['loader'],'--library-path',c['library_path'],c['app_'+side],c[mode],c['provider']]
  stem.with_suffix('.command.json').write_text(json.dumps(cmd)+'\n');t=time.time()
  r=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
  stem.with_suffix('.stdout').write_bytes(r.stdout);stem.with_suffix('.stderr').write_bytes(r.stderr);stem.with_suffix('.exitcode').write_text(str(r.returncode)+'\n')
  s=r.stdout.decode(errors='replace');negative=side=='gnu' and mode=='control'
  map_ok='MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in s
  value_ok=r.returncode==0 and 'SUMMARY EXACT=12/12 DESTROYED=12/12' in s
  rows.append(dict(side=side,mode=mode,repeat=repeat,exit=r.returncode,maps=map_ok,values=value_ok,seconds=time.time()-t,negative_control=negative))
  (out/'results.json').write_text(json.dumps(rows,indent=2)+'\n')
  if not negative:assert map_ok and value_ok,'Production-direction failure; inspect exact fields'
  # Do not predeclare SIGSEGV: record actual negative control, including any
  # unexpectedly exact result, for review.
print('Production matrix completed; negative-control interpretation requires review')
