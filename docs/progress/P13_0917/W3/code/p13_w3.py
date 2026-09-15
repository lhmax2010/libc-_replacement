#!/usr/bin/env python3
import os,sys,json,subprocess,hashlib,pathlib
os.environ['P13_STAGE']='W3';os.environ['P13_PIN_ABI']='1'
import p13_driver as d
b=d.b
if sys.argv[1]=='contract':
 observer=b.BUILD/'p13_fault_allocator.so'
 b.run('build_fault_allocator',[b.CC,'-x','c','-std=c11','-O2','-fPIC','-shared','-nostdlib++','-I'+str(b.SRC),b.SRC/'p13_alloc_observer.c','-o',observer])
 for side in ['gnu','cxx']:
  link=b.link if side=='cxx' else ['-nostdlib++',str(b.SDK/'closure_ewk/libstdc++.so.6'),'-ldl','-pthread','-fuse-ld=/usr/bin/ld.gold']
  b.run('build_contract_'+side,[b.CC,*b.flags[side],*b.common,b.SRC/'p13_contract_app.cpp','-Wl,--no-as-needed',observer,*link,'-o',b.BUILD/('contract_'+side)])
 rows=[]
 for mode in ['original','separate','embedded']:
  for nth in ([1004] if os.environ.get('P13_RETENTION')=='1' else [0,1,4,8,12,16,999]):
   for n in range(1,6):
    original=mode=='original';exe=b.BUILD/('contract_gnu' if original else 'contract_cxx');entry=b.PROVIDER if original else b.BUILD/('forwarder_'+mode+'.so')
    row=d.run_case('contract_'+mode+'_'+str(nth)+'_'+str(n),exe,[entry,b.PROVIDER,int(original),nth]);rows.append(dict(mode=mode,nth=nth,repeat=n,**row))
    name=os.environ.get('P13_RUN_PREFIX','')+'CONTRACT_COMPARISON.json'
    (b.OUT/name).write_text(json.dumps(rows,indent=2)+'\n')
    # Collection is not a pass verdict: retain unpaired allocations, aborts,
    # or other failures for contract review rather than deleting those cells.
    print(mode,nth,n,'exit',row['exit'])
if sys.argv[1]=='package':
 config={'compiler':str(b.CC),'gnu_flags':b.flags['gnu'],'libcxx_flags':b.flags['cxx'],'includes':b.inc,'link_flags':b.link,'provider_path':str(b.PROVIDER),'provider_abi_evidence':'P13 fixed real GNU artifact SHA256','consumer_abi_evidence':'P13 BUILD_CONFIG headers and LLVM_22 exports','direction':'reverse-mechanism','gnu_runtimes':[str(b.SDK/'closure_ewk/libstdc++.so.6')],'libcxx_runtimes':[str(b.LIBCXX/'lib/libc++.so.1'),str(b.LIBCXX/'lib/libc++abi.so.1')],'linker':'/usr/bin/ld.gold'}
 path=b.OUT/'PACKAGE_CONFIG.json';path.write_text(json.dumps(config,indent=2)+'\n')
 for mode in ['separate','embedded']:
  b.run('package_'+mode,[sys.executable,b.TMP/'package_build.py',path,b.TMP/('package_P13_'+mode),'--mode',mode])
 b.run('release_regression',[sys.executable,b.TMP/'release_check.py'])
