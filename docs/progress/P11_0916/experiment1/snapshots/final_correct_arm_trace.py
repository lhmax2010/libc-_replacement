#!/usr/bin/env python3
import json,pathlib,socket,subprocess,sys,time
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
TMP=ROOT/'tmp/P11_0916'
REC=ROOT/'docs/progress/API_0911/record.py'
SYS=OLD/'tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0'
LLVM=OLD/'tmp/R11/extracted/9b/9bda3e524b7ff5384c44baa1d85808ec71cc78db7c17bf71726e24c6c6bd4c99/usr/lib/libunwind.so.1.0'
script=TMP/'trace-arm-corrected.gdb'
script.write_text((OUT/'trace.gdb').read_text().replace('\nrun\n','\ncontinue\n'))
rows=[]
for abi,exe,lib in [('libcxxabi',OLD/'progress/R78/build/armv7l/new_main',OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib'),
                     ('libsupc++',TMP/'supc_arm_main',TMP/'build-supc-arm/lib')]:
    for n in range(1,6):
        stem='arm_corrected_'+abi.replace('+','p')+'_'+str(n)
        with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
        q=subprocess.Popen([sys.executable,str(REC),str(OUT/'raw'/(stem+'_qemu')),'timeout','55','qemu-arm-static','-g',str(port),
            '-L',str(SYS),'-E','LD_LIBRARY_PATH='+str(lib)+':'+str(SYS/'usr/lib'),'-E','LD_PRELOAD='+str(LLVM),
            str(exe),str(OLD/'progress/R78/build/armv7l/libold_plugin.so'),'exception'])
        try:
            time.sleep(.3)
            pid=q.pid
            for _ in range(5):
                child=pathlib.Path(f'/proc/{pid}/task/{pid}/children').read_text().split()
                if not child:break
                assert len(child)==1
                pid=int(child[0])
            cmdline=pathlib.Path(f'/proc/{pid}/cmdline').read_bytes().replace(b'\0',b' ')
            assert b'qemu-arm-static' in cmdline and str(port).encode() in cmdline
            args=['timeout','45',str(TMP/'debugger/usr/bin/gdb-multiarch'),'-q','-batch',
                '-ex',f'set $p11_host_qemu_pid={pid}','-ex','set sysroot '+str(SYS),
                '-ex','set solib-search-path '+str(lib)+':'+str(SYS/'usr/lib')+':'+str(LLVM.parent),
                '-ex','file '+str(exe),'-ex','target remote 127.0.0.1:'+str(port),'-x',str(script)]
            rc=subprocess.run([sys.executable,str(REC),str(OUT/'raw'/stem),*args]).returncode
            text=(OUT/'raw'/(stem+'.stdout')).read_text()
            trace=[json.loads(s[10:]) for s in text.splitlines() if s.startswith('P11_TRACE ')]
            assert 'HOST_QEMU_PROC_MAPS' in text
            for expected in ('libc++.so','libstdc++.so','libold_plugin.so','libunwind.so'):assert expected in text
            rows.append({'abi':abi,'repeat':n,'debugger_exit':rc,'qemu_pid':pid,'qemu_cmdline':cmdline.decode(),
                         'trace':trace,'signal_abort':'Program received signal SIGABRT' in text,'raw':stem})
            (OUT/'ARM_CORRECTED_TRACES.json').write_text(json.dumps(rows,indent=2)+'\n')
        finally:q.wait(timeout=60)
print('CORRECTED_TRACES',len(rows))
