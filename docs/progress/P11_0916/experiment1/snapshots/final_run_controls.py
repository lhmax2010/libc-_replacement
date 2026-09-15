#!/usr/bin/env python3
import json, pathlib, subprocess, sys
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
TMP=ROOT/'tmp/P11_0916/controls'
X=OLD/'progress/R78/build/x86_64'
ARMROOT=OLD/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'
ARMNEW=OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib'
cases=[('gnu_same',[TMP/'gnu_main',X/'libold_plugin.so','exception']),
       ('llvm_same',[X/'new_main',TMP/'llvm_plugin.so','exception']),
       ('export_cross',[TMP/'export_main',X/'libold_plugin.so','exception'])]
result=[]
for name,args in cases:
    for n in range(1,6):
        stem='control_'+name+'_'+str(n)
        cmd=['nice','-n','15','ionice','-c','3','timeout','30','env','LD_LIBRARY_PATH='+str(X/'runtime'),*map(str,args)]
        subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*cmd])
        text=(OUT/'raw'/(stem+'.stdout')).read_text();rc=int((OUT/'raw'/(stem+'.exitcode')).read_text())
        result.append({'case':name,'repeat':n,'exit':rc,'stdout':text})
        if name in ('gnu_same','export_cross'):
            assert rc==0 and 'CAUGHT=77 OLD_GUARD=1 ASSERT=PASS' in text,(stem,text)
for n in range(1,6):
    stem='control_arm_maps_'+str(n)
    cmd=['nice','-n','15','ionice','-c','3','timeout','30','qemu-arm-static','-L',str(ARMROOT),
         '-E','LD_LIBRARY_PATH='+str(ARMNEW)+':'+str(ARMROOT/'usr/lib'),'-E','LD_PRELOAD='+str(TMP/'maps-arm.so'),
         str(OLD/'progress/R78/build/armv7l/new_main'),str(OLD/'progress/R78/build/armv7l/libold_plugin.so'),'exception']
    subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*cmd])
    rc=int((OUT/'raw'/(stem+'.exitcode')).read_text());text=(OUT/'raw'/(stem+'.stderr')).read_text()
    assert rc==12 and 'WRONG_EXCEPTION_TYPE' in text and 'P11_PROC_SELF_MAPS_END' in text
    for expected in ('libc++.so','libc++abi.so','libstdc++.so','libold_plugin.so'):assert expected in text,expected
    result.append({'case':'arm_maps','repeat':n,'exit':rc,'maps_libraries':'PASS'})
(OUT/'CONTROLS.json').write_text(json.dumps(result,indent=2)+'\n')
print('CONTROLS',len(result))
