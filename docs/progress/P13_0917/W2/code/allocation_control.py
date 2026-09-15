#!/usr/bin/env python3
"""同一后端/同一分配失败用例的 GNU-only 正向对照，不更改运行库。"""
import json,pathlib,subprocess,sys
import build as b
link=['-nostdlib++',str(b.SDK/'closure_ewk/libstdc++.so.6'),'-ldl','-pthread']
b.run('build_allocation_gnu_control',[b.CC,*b.flags['gnu'],*b.common,b.SRC/'allocation_contract.cpp',b.BUILD/'ewk_back_reverse.o',*link,'-o',b.BUILD/'allocation_gnu_control'])
b.run('allocation_gnu_control_needed',['readelf','-d',b.BUILD/'allocation_gnu_control'])
rows=[]
for n in range(1,6):
 stem=b.OUT/'raw'/('allocation_gnu_control_'+str(n));assert not pathlib.Path(str(stem)+'.command.txt').exists()
 cmd=[b.SDK/'closure_ewk/ld-linux-x86-64.so.2','--library-path',str(b.SDK/'closure_ewk'),b.BUILD/'allocation_gnu_control']
 subprocess.run([sys.executable,str(b.REC),str(stem),'nice','-n','15','ionice','-c','3','prlimit','--core=0','--','timeout','40',*map(str,cmd)])
 rows.append(dict(repeat=n,exit=int(pathlib.Path(str(stem)+'.exitcode').read_text()),stdout=pathlib.Path(str(stem)+'.stdout').read_text(),stderr=pathlib.Path(str(stem)+'.stderr').read_text()))
(b.OUT/'ALLOCATION_CONTROL_RESULTS.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(rows,indent=2))
