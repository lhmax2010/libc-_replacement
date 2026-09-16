"""Read-only compiler identity; five-object recompilation in separate outputs.

Run inside a MemoryMax systemd scope. A real ninja -j1 dispatches the checks.
Only the input/output filename header is normalized in objdump output.
"""
import difflib
import hashlib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from pathlib import Path

P=Path('/home/toolchain/development/libc++_replacement/progress/BUILD_STATIC_0917B')
WORK=P.parents[1]
T=WORK/'tmp/STATIC_0917B'
E=T/'native-equivalence'
B=T/'rpm-aarch64/BUILD/llvm-22.1.8/build'
GB='/tmp/task/rpm-aarch64/BUILD/llvm-22.1.8/build'
ROOT=T/'llvm-root-minimal'
EMUL=WORK/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/emul'
CAP=16536449024
TARGETS=[
 ('demangle','lib/Demangle/CMakeFiles/LLVMDemangle.dir/Demangle.cpp.o','Demangling entry points'),
 ('apint','lib/Support/CMakeFiles/LLVMSupport.dir/APInt.cpp.o','Integer arithmetic and bit operations'),
 ('md5','lib/Support/CMakeFiles/LLVMSupport.dir/MD5.cpp.o','Hash computations'),
 ('threading','lib/Support/CMakeFiles/LLVMSupport.dir/Threading.cpp.o','Thread/platform support'),
 ('blake3','lib/Support/BLAKE3/CMakeFiles/LLVMSupportBlake3.dir/blake3_portable.c.o','Portable C hashing functions'),
]

def sha(q):
    with q.open('rb') as f:
        return hashlib.file_digest(f,'sha256').hexdigest()

def run(label,cmd,timeout=180):
    dest=P/(label+'.json')
    assert not dest.exists(), str(dest)
    print('COMMAND '+shlex.join(cmd),flush=True)
    start=time.time()
    r=subprocess.run(cmd,input='',text=True,capture_output=True,timeout=timeout)
    record=dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,seconds=time.time()-start)
    dest.write_text(json.dumps(record,indent=2))
    print('EXIT '+str(r.returncode),flush=True)
    if r.returncode:
        print(r.stderr,flush=True)
        raise SystemExit(r.returncode)
    return r

def scope_check(label,ninja_required=False):
    pid=os.getpid()
    if ninja_required:
        for _ in range(12):
            args=Path(f'/proc/{pid}/cmdline').read_bytes().split(b'\0')
            if args and Path(args[0].decode()).name=='ninja':
                break
            pid=int(re.search(r'^PPid:\s*(\d+)',Path(f'/proc/{pid}/status').read_text(),re.M)[1])
        else:
            raise RuntimeError('Actual ninja ancestor not found; do not compile')
        assert any(x in args for x in [b'-j1',b'-j'])
    cg=run(label+'-cgroup',['cat',f'/proc/{pid}/cgroup']).stdout
    path=next(s.split(':',2)[2] for s in cg.splitlines() if s.startswith('0::'))
    assert 'build-static-0917b-native-equivalence.scope' in path
    memory=run(label+'-memory-max',['cat','/sys/fs/cgroup'+path+'/memory.max']).stdout.strip()
    assert int(memory)==CAP
    priority=run(label+'-priority',['ps','-p',str(pid),'-o','pid,ppid,ni,args']).stdout
    assert os.getpriority(os.PRIO_PROCESS,pid)==19
    io=run(label+'-ionice',['ionice','-p',str(pid)]).stdout.strip()
    assert io=='idle'
    result=dict(pid=pid,cgroup=path,memory_max=int(memory),nice=19,ionice=io,actual_ninja=ninja_required)
    (P/(label+'-resource-result.json')).write_text(json.dumps(result,indent=2))
    print(json.dumps(result),flush=True)

def base(native=False):
    cmd=json.loads((P/'private_root_command.json').read_text())
    for flag in ['--uid','--gid']:
        cmd[cmd.index(flag)+1]='1000'
    if native:
        cmd+=['--ro-bind',str(EMUL),'/emul']
    return cmd

def prepare():
    scope_check('native-eq-parent')
    E.mkdir(exist_ok=False)
    (E/'original').mkdir(); (E/'native').mkdir(); (E/'ok').mkdir()
    identity={}
    for name,native,path in [('qemu',False,ROOT/'usr/bin/clang-22'),('native',True,EMUL/'usr/bin/clang-22')]:
        compiler='/emul/usr/bin/aarch64-tizen-linux-gnu-clang++' if native else '/usr/bin/aarch64-tizen-linux-gnu-clang++'
        ver=run('native-eq-'+name+'-version',base(native)+['--',compiler,'--version'])
        drv=run('native-eq-'+name+'-driver',base(native)+['--',compiler,'-###','-stdlib=libc++','-x','c++','-fsyntax-only','-'])
        elf=run('native-eq-'+name+'-elf',['readelf','-h',str(path)])
        assert 'clang version 22.1.8' in ver.stdout
        triple=re.search(r'^Target: (.+)$',ver.stdout,re.M)[1]
        cpu=re.search(r'"-target-cpu" "([^"]+)"',drv.stderr)[1]
        features=re.findall(r'"-target-feature" "([^"]+)"',drv.stderr)
        assert triple=='aarch64-tizen-linux-gnu'
        identity[name]=dict(binary=str(path),sha256=sha(path),version=ver.stdout,target=triple,
                            cpu=cpu,features=features,
                            explicit_march='NOT_OBSERVED: no -march was supplied',
                            explicit_mcpu='NOT_OBSERVED: no -mcpu was supplied; cc1 default recorded above',
                            explicit_sysroot='NOT_OBSERVED: neither invocation supplies --sysroot',
                            effective_filesystem_root=str(ROOT),guest_filesystem_root='/',
                            driver_record='native-eq-'+name+'-driver.json',elf=elf.stdout)
    identity['same_target_defaults']=all(identity['qemu'][k]==identity['native'][k] for k in ['target','cpu','features'])
    (P/'native-equivalence-toolchain-identity.json').write_text(json.dumps(identity,indent=2))
    assert identity['same_target_defaults']
    log=(B/'.ninja_log').read_text()
    inputs={}
    for label,obj,reason in TARGETS:
        assert '\t'+obj+'\t' in log, obj
        original=B/obj
        saved=E/'original'/f'{label}.o'
        shutil.copy2(original,saved)
        r=run('native-eq-'+label+'-original-command',base()+['--','/bin/ninja','-C',GB,'-t','commands',obj])
        lines=[x for x in r.stdout.splitlines() if x.strip()]
        assert len(lines)==1, r.stdout
        args=shlex.split(lines[0])
        assert args[0] in ['/usr/bin/aarch64-tizen-linux-gnu-clang','/usr/bin/aarch64-tizen-linux-gnu-clang++']
        candidate=args.copy()
        candidate[0]='/emul'+args[0]
        out=f'/tmp/task/native-equivalence/native/{label}.o'
        for flag,value in [('-o',out),('-MF',out+'.d'),('-MT',out)]:
            if flag in candidate:
                candidate[candidate.index(flag)+1]=value
        inputs[label]=dict(target=obj,reason=reason,original=str(original),saved=str(saved),
                           original_sha256=sha(original),original_command=args,native_command=candidate)
    (P/'native-equivalence-inputs.json').write_text(json.dumps(inputs,indent=2))
    rule='rule compare\n  command = '+shlex.join([sys.executable,str(Path(__file__).resolve()),'compare'])+' $sample\n  description = Verify $sample\n'
    for label,_,_ in TARGETS:
        rule+=f'build {E}/ok/{label}: compare\n  sample = {label}\n'
    rule+='default '+' '.join(str(E/'ok'/l) for l,_,_ in TARGETS)+'\n'
    (E/'equivalence.ninja').write_text(rule)
    r=run('native-eq-ninja',['/usr/bin/ninja','-j1','-f',str(E/'equivalence.ninja')],timeout=600)
    results=[json.loads((P/f'native-eq-{l}-result.json').read_text()) for l,_,_ in TARGETS]
    assert all(x['pass'] for x in results)
    (P/'native-equivalence-summary.json').write_text(json.dumps(dict(status='PASS',objects=5,results=results,scope='Five sampled objects only, not a proof for every LLVM target'),indent=2))
    print('PASS: 5/5 nm and complete executable-section disassembly comparisons.',flush=True)

def compare(label):
    scope_check('native-eq-'+label+'-ninja',True)
    entry=json.loads((P/'native-equivalence-inputs.json').read_text())[label]
    assert sha(Path(entry['original']))==entry['original_sha256']
    command=base(True)+['--chdir',GB,'--',*entry['native_command']]
    run('native-eq-'+label+'-compile',command)
    nm={}; dis={}; normalized={}
    for kind in ['original','native']:
        guest=f'/tmp/task/native-equivalence/{kind}/{label}.o'
        nm[kind]=run(f'native-eq-{label}-{kind}-nm',base()+['--','/usr/bin/llvm-nm','-an',guest]).stdout
        dis[kind]=run(f'native-eq-{label}-{kind}-objdump',base()+['--','/usr/bin/llvm-objdump','-d',guest]).stdout
        normalized[kind]=dis[kind].replace(guest+':','OBJECT:')
    same_nm=nm['original']==nm['native']
    same_dis=normalized['original']==normalized['native']
    for name,data in [('nm',nm),('disassembly',normalized)]:
        delta=''.join(difflib.unified_diff(data['original'].splitlines(True),data['native'].splitlines(True),fromfile='QEMU',tofile='native'))
        (P/f'native-eq-{label}-{name}.diff').write_text(delta)
    result=dict(sample=label,target=entry['target'],nm_equal=same_nm,disassembly_equal=same_dis,
                original_sha256=entry['original_sha256'],native_sha256=sha(E/'native'/f'{label}.o'),
                original_unchanged=sha(Path(entry['original']))==entry['original_sha256'],
                nm_lines=len(nm['original'].splitlines()),disassembly_lines=len(dis['original'].splitlines()),
                normalization='Only replace the actual object filename header with OBJECT:',
                **{'pass':same_nm and same_dis})
    (P/f'native-eq-{label}-result.json').write_text(json.dumps(result,indent=2))
    print(json.dumps(result),flush=True)
    if not result['pass']:
        raise SystemExit(3)
    (E/'ok'/label).write_text('PASS\n')

if __name__=='__main__':
    if len(sys.argv)>1:
        compare(sys.argv[2])
    else:
        prepare()
