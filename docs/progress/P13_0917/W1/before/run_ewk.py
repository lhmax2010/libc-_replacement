#!/usr/bin/env python3
"""真实 GNU EWK；libc++ 应用→转发层→adaptor→GNU，为生产反方向。"""
import json,os,pathlib,re,subprocess,sys
import build as b
prefix=os.environ.get('P12_RUN_TAG','')
for side in ['gnu','cxx']:
    b.run('build_ewk_app_'+side,[b.CC,*b.flags[side],*b.common,b.SRC/'ewk_app.cpp',*b.link,'-o',b.BUILD/('ewk_app_'+side)])
rows=[]
for side,mode in [('gnu','control'),('cxx','separate'),('cxx','embedded'),('cxx','control')]:
    for n in range(1,6):
        tag=prefix+'ewk_run_'+side+'_'+mode+'_'+str(n);stem=b.OUT/'raw'/tag
        assert not pathlib.Path(str(stem)+'.command.txt').exists(),tag
        command=['prlimit','--core=0','--','timeout','40',str(b.SDK/'closure_ewk/ld-linux-x86-64.so.2'),
          '--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),
          str(b.BUILD/('ewk_app_'+side)),str(b.BUILD/('forwarder_'+mode+'.so')),str(b.PROVIDER)]
        subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','15','ionice','-c','3',*command])
        rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());out=pathlib.Path(str(stem)+'.stdout').read_text()
        err=pathlib.Path(str(stem)+'.stderr').read_text()
        negative=side=='cxx' and mode=='control'
        if not negative:
            assert rc==0 and 'SUMMARY EXACT=12/12 DESTROYED=12/12' in out,(tag,rc,err[-800:])
        else:assert rc!=0,(tag,'negative control unexpectedly passed; inspect values')
        assert 'MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in out
        assert 'FORWARDER_ENTRY='+str(b.BUILD/('forwarder_'+mode+'.so')) in out
        if mode!='control':assert err.count('P12_ADAPTOR_ENTRY')==13,err
        rows.append({'consumer':side,'mode':mode,'repeat':n,'exit':rc,'environment':'x86_64 native',
                     'provider':'real GNU chromium-efl','production_direction':False,
                     'scenarios_passed':12 if not negative else None,'destroyed':12 if not negative else None,
                     'raw':tag})
        (b.OUT/(prefix+'EWK_RESULTS.json')).write_text(json.dumps(rows,indent=2)+'\n')
print('EWK_PROGRAM_RUNS',len(rows))
