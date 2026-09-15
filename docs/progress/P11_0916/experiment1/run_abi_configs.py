#!/usr/bin/env python3
"""两种实际 ABI 构建 × 默认 GNU / 预加载 LLVM 展开器；不冒充完整平台迁移。"""
import json, pathlib, subprocess, sys
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
TMP=ROOT/'tmp/P11_0916'
X=OLD/'progress/R78/build/x86_64'
BUILD=TMP/'build-supc-clang'
LLVMUNWIND=OLD/'tmp/R14b/runtime/B/libunwind.so.1.0'
def run(stem,cmd):
    subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*map(str,cmd)])
    return int((OUT/'raw'/(stem+'.exitcode')).read_text()),(OUT/'raw'/(stem+'.stdout')).read_text(),(OUT/'raw'/(stem+'.stderr')).read_text()
rc,_,err=run('config_build_supc_main',[OUT/'clang-cxx.sh','-std=c++17','-O1','-g','-nostdinc++','-I',BUILD/'include/c++/v1',
       ROOT/'docs/progress/R78/code/new_main.cpp','-nostdlib++',BUILD/'lib/libc++.so.1','-lgcc_s','-pthread','-ldl','-o',TMP/'supc_main'])
assert rc==0,err
results=[]
for abi,exe,lib in [('libcxxabi',X/'new_main',X/'runtime'),('libsupc++',TMP/'supc_main',BUILD/'lib')]:
    for unwind in ['GNU-default','LLVM-preload']:
        env=['env','LD_LIBRARY_PATH='+str(lib)]
        if unwind=='LLVM-preload':env+=['LD_PRELOAD='+str(LLVMUNWIND)]
        for scenario in ['normal','exception']:
            for n in range(1,6):
                stem='config_'+abi.replace('+','p')+'_'+unwind+'_'+scenario+'_'+str(n)
                args=[exe,X/'libold_plugin.so',scenario]
                rc,text,err=run(stem,['nice','-n','15','ionice','-c','3','timeout','30',*env,*args])
                row={'abi':abi,'unwinder_setting':unwind,'scenario':scenario,'repeat':n,'exit':rc,'stdout':text,'stderr':err,'raw':stem}
                results.append(row)
                (OUT/'ABI_CONFIG_RESULTS.json').write_text(json.dumps(results,indent=2)+'\n')
                if scenario=='normal':assert rc==0 and 'OUTPUT=old:ping OLD_GUARD=1 ASSERT=PASS' in text,(stem,text,err)
                if scenario=='exception':
                    trc,trace,terr=run(stem+'_gdb',['nice','-n','15','ionice','-c','3','timeout','40',*env,'gdb','-q','-batch',
                         '-x',OUT/'trace.gdb','--args',*args])
                    row['gdb_exit']=trc
                    row['trace']=[json.loads(s[10:]) for s in trace.splitlines() if s.startswith('P11_TRACE ')]
                    row['inferior_exit']=[s for s in trace.splitlines() if s.startswith('P11_INFERIOR_EXIT')]
                    (OUT/'ABI_CONFIG_RESULTS.json').write_text(json.dumps(results,indent=2)+'\n')
                    assert trc==0 and row['trace'],(stem,terr)
print('完成',len(results),'次配置程序运行；异常格另各一份实际调用轨迹')
