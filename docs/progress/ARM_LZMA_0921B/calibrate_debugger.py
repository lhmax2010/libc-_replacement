"""Non-build test: deployed rpmbuild --version, actual guest QEMU and symbols."""
import json,os,pathlib,subprocess,time
BASE=pathlib.Path('/home/toolchain/development/libc++_replacement');os.chdir(BASE)
out=BASE/'progress/ARM_LZMA_0921B/debugger-calibration';out.mkdir(exist_ok=False)
original=json.loads((BASE/'progress/ARM_REPRO_0920/original-argv.json').read_text())
cmd=original[original.index('bwrap'):original.index('--')]+['--','/usr/bin/qemu-arm-static','-g','49191','/usr/bin/rpmbuild','--version']
(out/'command.json').write_text(json.dumps(cmd,indent=2)+'\n')
env=os.environ.copy();assert 'LD_PRELOAD' not in env
env.update(OBS_PORT='49191',OBS_OUT=str(out))
with (out/'guest.log').open('xb') as f,(out/'gdb.log').open('xb') as g:
    p=subprocess.Popen(cmd,stdout=f,stderr=subprocess.STDOUT,env=env)
    try:
        time.sleep(1)
        dbg=[str(BASE/'tmp/ARM_LZMA_0921B/gdb-tools/usr/bin/gdb-multiarch'),'-q','-nx','-batch','-x',str(BASE/'progress/ARM_LZMA_0921B/gdb_observer.py')]
        (out/'gdb-command.json').write_text(json.dumps(dbg,indent=2)+'\n')
        r=subprocess.run(dbg,stdout=g,stderr=subprocess.STDOUT,env=env,timeout=45)
        (out/'gdb.exitcode').write_text(str(r.returncode)+'\n')
        code=p.wait(timeout=5);(out/'guest.exitcode').write_text(str(code)+'\n')
        print('gdb',r.returncode,'guest',code);assert r.returncode==0 and code==0
    finally:
        if p.poll() is None:p.terminate();p.wait(timeout=5)
print((out/'gdb.log').read_text())
