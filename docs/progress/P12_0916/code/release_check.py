#!/usr/bin/env python3
"""移除测试入口的发布候选；沿用真实转发源码编译对象，不运行 RPM 安装。"""
import json,pathlib,subprocess,sys
import build as b
original=b.BUILD
b.BUILD=original/'release';b.BUILD.mkdir(exist_ok=True)
back=original/'ewk_back_reverse.o'
front=b.compile('ewk_front_reverse',b.SRC/'ewk_front.cpp','cxx')
embedded=b.compile('ewk_front_embedded',b.SRC/'ewk_front.cpp','cxx',['-DP12_EMBEDDED'])
adaptor=b.shared('ewk_adaptor_reverse',[front,back],b.SRC/'ewk_release.exports',['-Wl,-soname,libewk-cookie-adaptor.so.1'])
alias=b.BUILD/'libewk-cookie-adaptor.so.1'
if not alias.exists():alias.symlink_to(adaptor.name)
for mode in ['separate','embedded']:
 objects=[original/'forwarder_main.o',original/('forwarder_'+mode+'.o')]
 objects+=([adaptor] if mode=='separate' else [embedded,back])
 b.shared('forwarder_'+mode,objects,b.SRC/'forwarder_release.exports',[str(b.SDK/'closure_ewk/libdlog.so.0'),'-Wl,-rpath,$ORIGIN'])
rows=[]
for side,mode in [('gnu','control'),('cxx','separate'),('cxx','embedded'),('cxx','control')]:
 for n in range(1,6):
  entry=(original if mode=='control' else b.BUILD)/('forwarder_'+mode+'.so')
  stem=b.OUT/'raw'/('release_'+side+'_'+mode+'_'+str(n))
  assert not pathlib.Path(str(stem)+'.command.txt').exists()
  cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),original/('ewk_app_'+side),entry,b.PROVIDER]
  subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','40',*map(str,cmd)])
  rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());out=pathlib.Path(str(stem)+'.stdout').read_text();err=pathlib.Path(str(stem)+'.stderr').read_text()
  negative=side=='cxx' and mode=='control'
  assert (rc!=0 if negative else rc==0 and 'SUMMARY EXACT=12/12 DESTROYED=12/12' in out)
  assert 'MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in out
  assert 'P12_ADAPTOR_ENTRY' not in err
  rows.append(dict(side=side,mode=mode,repeat=n,exit=rc,raw=stem.name))
for name in ['ewk_adaptor_reverse.so','forwarder_separate.so','forwarder_embedded.so']:
 b.run('release_dynamic_'+name,['readelf','-dWs',b.BUILD/name])
 exports=subprocess.check_output(['nm','-D','--defined-only',str(b.BUILD/name)],text=True)
 assert 'p12_test_' not in exports
(b.OUT/'RELEASE_RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
