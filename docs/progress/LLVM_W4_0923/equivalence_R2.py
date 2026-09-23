"""Real ARM macro expansion; independent outputs and exact allowed-difference gate."""
import difflib, hashlib, json, shlex, subprocess, time
from pathlib import Path
P=Path.cwd(); E=P/'docs/progress/LLVM_W4_0923'; T=P/'tmp/LLVM_W4_0923/equivalence_R2'
T.mkdir(parents=True,exist_ok=True)
repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
root=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0'
old=json.loads((P/'docs/progress/ARM_W5_0921/cell_commands/w5-original.json').read_text())
common=['--rcfile',old[old.index('--rcfile')+1],'--macros',old[old.index('--macros')+1],'--target','armv7l-tizen-linux','--define','_binary_payload w5.xzdio']
runtime=P/'tmp/NIGHT_0917/specs/libcxx-runtimes.spec'
assert hashlib.sha256(runtime.read_bytes()).hexdigest()=='c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe'
records=[]; results=[]
def run(label,cmd,outdir):
    start=time.time(); r=subprocess.run(cmd,capture_output=True,timeout=600)
    (outdir/(label+'.stdout.txt')).write_bytes(r.stdout); (outdir/(label+'.stderr.txt')).write_bytes(r.stderr)
    records.append(dict(label=label,argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,seconds=time.time()-start))
    (E/'prep-check/commands_R2.json').write_text(json.dumps(records,indent=2)+'\n')
    assert r.returncode==0,(label,r.returncode)
    return r.stdout.decode()
for package,candidate,subdir in [('llvm',repo/'packaging/llvm.spec','equivalence_R2'),('libcxx-runtimes',runtime,'runtime_R2')]:
    out=E/'prep-check'/subdir; out.mkdir(parents=True,exist_ok=True)
    base=run(package+'-source',['git','-C',str(repo),'show','f203923a:packaging/'+package+'.spec'],out)
    baseline=T/(package+'-baseline.spec'); baseline.write_text(base)
    bw=['bwrap','--unshare-user','--uid','1000','--gid','1000','--ro-bind',str(root),'/', '--dev','/dev','--proc','/proc','--tmpfs','/tmp','--ro-bind',str(baseline),'/tmp/baseline.spec','--ro-bind',str(candidate),'/tmp/candidate.spec','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C','--']
    for mode,args in [('default',[]),('override-gcc',['--define','_toolchain_override gcc']),('toolchain-gcc',['--define','_toolchain gcc']),('undefined',['--undefine','_toolchain'])]:
        a=run(mode+'-baseline',bw+['rpmspec','-P',*common,*args,'/tmp/baseline.spec'],out)
        b=run(mode+'-candidate',bw+['rpmspec','-P',*common,*args,'/tmp/candidate.spec'],out)
        diff=''.join(difflib.unified_diff(a.splitlines(True),b.splitlines(True),fromfile='baseline',tofile='candidate'))
        (out/(mode+'.diff')).write_text(diff)
        if package=='llvm':
            clean=''.join(x for x in b.splitlines(True) if x.strip()=='-DLIBCLANG_BUILD_STATIC=ON \\')
            normalized=''.join(x for x in b.splitlines(True) if x.strip()!='-DLIBCLANG_BUILD_STATIC=ON \\')
            passed=normalized==a and len(clean.splitlines())<=1
        else:
            normalized=b
            for flag in ['LIBCXX_ENABLE_STATIC','LIBCXX_INSTALL_STATIC_LIBRARY','LIBCXXABI_ENABLE_STATIC','LIBCXXABI_INSTALL_STATIC_LIBRARY']:
                normalized=normalized.replace('-D'+flag+'=ON','-D'+flag+'=OFF')
            normalized=''.join(x for x in normalized.splitlines(True) if x.strip() not in ['/usr/lib/libc++.a','/usr/lib/libc++abi.a'])
            passed=normalized==a
        results.append(dict(package=package,mode=mode,result='PASS' if passed else 'FAIL',identical=a==b,diff=str(out/(mode+'.diff')),baseline_sha256=hashlib.sha256(a.encode()).hexdigest(),candidate_sha256=hashlib.sha256(b.encode()).hexdigest()))
        (E/'prep-check/EQUIVALENCE_R2.json').write_text(json.dumps(results,indent=2)+'\n')
        print(package,mode,results[-1]['result'],'IDENTICAL' if a==b else 'ALLOWED_DIFF',flush=True)
        assert passed,(package,mode,'STOP: unexpected difference')
