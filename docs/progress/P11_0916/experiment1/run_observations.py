#!/usr/bin/env python3
"""复用既有夹具；调试器只观察，保存命令/输出/退出码与精确观测。"""
import csv, hashlib, json, os, pathlib, socket, subprocess, sys, time
ROOT = pathlib.Path(__file__).resolve().parents[4]
OUT = pathlib.Path(__file__).resolve().parent
OLD = pathlib.Path('/home/toolchain/development/libc++_replacement')
REC = ROOT/'docs/progress/API_0911/record.py'
ARMROOT = OLD/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'
ARMLIB = OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib'
XBUILD = OLD/'progress/R78/build/x86_64'
ABUILD = OLD/'progress/R78/build/armv7l'
RESULTS = []

def run(stem, args, timeout=100):
    rc = subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*map(str,args)],timeout=timeout).returncode
    return rc,(OUT/'raw'/(stem+'.stdout')).read_text(errors='replace'),(OUT/'raw'/(stem+'.stderr')).read_text(errors='replace')

def trace_rows(text):
    return [json.loads(s.removeprefix('P11_TRACE ')) for s in text.splitlines() if s.startswith('P11_TRACE ')]

def one(arch, repeat):
    stem = f'r78_{arch}_{repeat}'
    if arch=='x86_64':
        env = ['env','LD_LIBRARY_PATH='+str(XBUILD/'runtime')+':/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu']
        args = [str(XBUILD/'new_main'),str(XBUILD/'libold_plugin.so'),'exception']
        rc, text, err = run(stem+'_plain',['nice','-n','15','ionice','-c','3','timeout','40',*env,*args])
        assert rc==12 and 'WRONG_EXCEPTION_TYPE' in err
        rc,text,err = run(stem+'_trace',['nice','-n','15','ionice','-c','3','timeout','45',*env,'gdb','-q','-batch','-x',OUT/'trace.gdb','--args',*args])
    else:
        env = ['-L',str(ARMROOT),'-E','LD_LIBRARY_PATH='+str(ARMLIB)+':'+str(ARMROOT/'usr/lib')]
        args = [str(ABUILD/'new_main'),str(ABUILD/'libold_plugin.so'),'exception']
        rc,text,err = run(stem+'_plain',['nice','-n','15','ionice','-c','3','timeout','40','qemu-arm-static',*env,*args])
        assert rc==12 and 'WRONG_EXCEPTION_TYPE' in err
        # QEMU 的 gdb 端口仅在本次短进程生命周期内使用，不修改系统服务。
        with socket.socket() as sock:
            sock.bind(('127.0.0.1',0)); port=sock.getsockname()[1]
        qcmd = [sys.executable,str(REC),str(OUT/'raw'/(stem+'_qemu')),'nice','-n','15','ionice','-c','3',
                'timeout','55','qemu-arm-static','-g',str(port),*env,*args]
        q = subprocess.Popen(qcmd)
        try:
            time.sleep(0.3)
            # 快照派生脚本只将 run 改为 continue；断点代码本身完全相同。
            arm_script = ROOT/'tmp/P11_0916/trace-arm.gdb'
            arm_script.write_text((OUT/'trace.gdb').read_text().replace('\nrun\n','\ncontinue\n'))
            debugger=ROOT/'tmp/P11_0916/debugger/usr/bin/gdb-multiarch'
            rc,text,err = run(stem+'_trace',['timeout','45',debugger,'-q','-batch',
                '-ex','set sysroot '+str(ARMROOT),'-ex','set solib-search-path '+str(ARMLIB)+':'+str(ARMROOT/'usr/lib'),
                '-ex','file '+str(ABUILD/'new_main'),'-ex','target remote 127.0.0.1:'+str(port),'-x',arm_script])
        finally:
            q.wait(timeout=60)
    rows=trace_rows(text)
    result={'architecture':arch,'environment':'native' if arch=='x86_64' else 'QEMU user mode',
            'repeat':repeat,'gdb_exit':rc,'trace':rows,'inferior_exit_12':'P11_INFERIOR_EXIT 12' in text,
            'type_match_returns':[s for s in text.splitlines() if s.startswith('P11_RETURN')]}
    RESULTS.append(result)
    (OUT/'R78_OBSERVATIONS.json').write_text(json.dumps(RESULTS,indent=2,ensure_ascii=False)+'\n')
    assert rc==0 and result['inferior_exit_12'], (stem,err)
    assert any(r['event']=='__cxa_throw' and r['object'] for r in rows), stem
    assert any(r['event']=='__gxx_personality_v0' and 'exception_class' in r for r in rows), stem
    print('OBSERVED',arch,repeat,flush=True)

if __name__=='__main__':
    for arch in sys.argv[1:] or ['x86_64','armv7l']:
        for repeat in range(1,6): one(arch,repeat)
