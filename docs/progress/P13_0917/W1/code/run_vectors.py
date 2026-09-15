#!/usr/bin/env python3
"""构造的数据层接收器，不冒充真实浏览器回调。每格五次、逐字节断言。"""
import json,os,pathlib,re,subprocess,sys
import build as b
prefix=os.environ.get('P12_RUN_TAG','')
rows=[]
for side,entry,provider,expected in [
    ('gnu','vector_provider_gnu','gnu',8),('cxx','vector_provider_cxx','cxx',8),
    ('gnu','vector_gnu_to_cxx','cxx',8),('cxx','vector_cxx_to_gnu','gnu',8),
    ('gnu','vector_gnu_to_cxx_bad','cxx',2)]:
    for n in range(1,6):
        tag=prefix+'vector_run_'+entry+'_'+str(n)
        stem=b.OUT/'raw'/tag
        assert not pathlib.Path(str(stem)+'.command.txt').exists(),tag
        command=['prlimit','--core=0','--','timeout','40',str(b.SDK/'closure_ewk/ld-linux-x86-64.so.2'),
          '--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib')+':'+str(b.BUILD),
          str(b.BUILD/('vector_app_'+side)),str(b.BUILD/(entry+'.so')),str(b.BUILD/('vector_provider_'+provider+'.so'))]
        subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','15','ionice','-c','3',*command])
        rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());out=pathlib.Path(str(stem)+'.stdout').read_text()
        summary=re.findall(r'SUMMARY EXACT=(\d+)/8 DESTROYED=(\d+)/8',out)
        assert summary==[(str(expected),'8')],(tag,rc,summary)
        assert rc==(0 if expected==8 else 28)
        assert 'MAP_ASSERT gnu=1 cxx=1 entry=1 provider=1' in out
        rows.append({'entry':entry,'consumer':side,'receiver':provider,'repeat':n,'exit':rc,
                     'scenarios':8,'exact':expected,'destroyed':8,'environment':'x86_64 native',
                     'receiver_kind':'constructed data-layer only','raw':tag})
        (b.OUT/(prefix+'VECTOR_RESULTS.json')).write_text(json.dumps(rows,indent=2)+'\n')
print('VECTOR_PROGRAM_RUNS',len(rows))
