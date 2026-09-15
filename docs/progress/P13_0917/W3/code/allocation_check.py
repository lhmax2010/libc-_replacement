#!/usr/bin/env python3
import json,pathlib,subprocess,sys
import build as b
b.run('build_allocation_contract',[b.CC,*b.flags['gnu'],*b.common,b.SRC/'allocation_contract.cpp',b.BUILD/'ewk_back_reverse.o',*b.link,'-o',b.BUILD/'allocation_contract'])
rows=[]
for n in range(1,6):
 stem=b.OUT/'raw'/('allocation_contract_'+str(n));assert not pathlib.Path(str(stem)+'.command.txt').exists()
 cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk')+':'+str(b.LIBCXX/'lib'),b.BUILD/'allocation_contract']
 subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','40',*map(str,cmd)])
 rows.append(dict(repeat=n,exit=int(pathlib.Path(str(stem)+'.exitcode').read_text()),stdout=pathlib.Path(str(stem)+'.stdout').read_text(),stderr=pathlib.Path(str(stem)+'.stderr').read_text()))
(b.OUT/'ALLOCATION_CONTRACT_RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(rows,indent=2))
