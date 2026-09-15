#!/usr/bin/env python3
import json, pathlib, socket, subprocess, sys, time
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
TMP=ROOT/'tmp/P11_0916'
SYS=OLD/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'
NEW=OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib'
ABI=OLD/'progress/R78/build/armv7l'
BUILD=TMP/'build-supc-arm'
LLVM=OLD/'tmp/R11/extracted/9b/9bda3e524b7ff5384c44baa1d85808ec71cc78db7c17bf71726e24c6c6bd4c99/usr/lib/libunwind.so.1.0'
def run(stem,cmd):
    subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*map(str,cmd)],timeout=65)
    return int((OUT/'raw'/(stem+'.exitcode')).read_text()),(OUT/'raw'/(stem+'.stdout')).read_text(),(OUT/'raw'/(stem+'.stderr')).read_text()
rc,text,err=run('config_build_arm_main',[OLD/'progress/R68/tools/armv7l_lit_host_clangxx.sh','-std=c++17','-O1','-g',
    '-nostdinc++','-I',BUILD/'include/c++/v1',ROOT/'docs/progress/R78/code/new_main.cpp','-nostdlib++',BUILD/'lib/libc++.so.1',
    '-lgcc_s','-pthread','-ldl','-latomic','-o',TMP/'supc_arm_main'])
assert rc==0,err
result=[]
for abi,exe,lib in [('libcxxabi',ABI/'new_main',NEW),('libsupc++',TMP/'supc_arm_main',BUILD/'lib')]:
    for unw in ['GNU-default','LLVM-preload']:
        preload=str(TMP/'controls/maps-arm.so')
        if unw=='LLVM-preload':preload=str(LLVM)+':'+preload
        env=['-L',str(SYS),'-E','LD_LIBRARY_PATH='+str(lib)+':'+str(SYS/'usr/lib'),'-E','LD_PRELOAD='+preload]
        for scenario in ['normal','exception']:
            for n in range(1,6):
                stem='arm_config_'+abi.replace('+','p')+'_'+unw+'_'+scenario+'_'+str(n)
                args=[str(exe),str(ABI/'libold_plugin.so'),scenario]
                rc,text,err=run(stem,['nice','-n','15','ionice','-c','3','timeout','30','qemu-arm-static',*env,*args])
                row={'abi':abi,'unwinder_setting':unw,'scenario':scenario,'repeat':n,'environment':'armv7l QEMU user mode',
                     'exit':rc,'stdout':text,'stderr':err,'raw':stem}
                result.append(row)
                (OUT/'ARM_ABI_CONFIG_RESULTS.json').write_text(json.dumps(result,indent=2)+'\n')
                if scenario=='normal':
                    assert rc==0 and 'OUTPUT=old:ping OLD_GUARD=1 ASSERT=PASS' in text,(stem,text,err)
                else:
                    with socket.socket() as sock:
                        sock.bind(('127.0.0.1',0));port=sock.getsockname()[1]
                    q=subprocess.Popen([sys.executable,str(REC),str(OUT/'raw'/(stem+'_qemu')),'timeout','55',
                        'qemu-arm-static','-g',str(port),*env,*args])
                    try:
                        time.sleep(.3)
                        gcmd=['timeout','45',TMP/'debugger/usr/bin/gdb-multiarch','-q','-batch',
                            '-ex','set sysroot '+str(SYS),'-ex','set solib-search-path '+str(lib)+':'+str(SYS/'usr/lib')+':'+str(LLVM.parent),
                            '-ex','file '+str(exe),'-ex','target remote 127.0.0.1:'+str(port),'-x',TMP/'trace-arm.gdb']
                        trc,trace,terr=run(stem+'_trace',gcmd)
                        row['gdb_exit']=trc
                        row['trace']=[json.loads(s[10:]) for s in trace.splitlines() if s.startswith('P11_TRACE ')]
                        row['inferior_exit']=[s for s in trace.splitlines() if s.startswith('P11_INFERIOR_EXIT')]
                    finally:q.wait(timeout=60)
                    (OUT/'ARM_ABI_CONFIG_RESULTS.json').write_text(json.dumps(result,indent=2)+'\n')
print('ARM CONFIG RUNS',len(result))
