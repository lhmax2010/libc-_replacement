#!/usr/bin/env python3
import collections,hashlib,json,pathlib,shutil
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
rows=json.loads((OUT/'ARM_ABI_CONFIG_RESULTS.json').read_text())
assert len(rows)==40
for r in rows:
    if r['scenario']=='normal':assert r['exit']==0 and 'OUTPUT=old:ping OLD_GUARD=1 ASSERT=PASS' in r['stdout']
    elif r['abi']=='libcxxabi':assert r['exit']==12 and 'WRONG_EXCEPTION_TYPE' in r['stderr']
    elif r['unwinder_setting']=='GNU-default':assert r['exit']==0 and 'CAUGHT=77 OLD_GUARD=1 ASSERT=PASS' in r['stdout']
    else:assert r['exit']==-6 and 'personality function returned unknown result 5' in r['stderr']
corrected=json.loads((OUT/'ARM_CORRECTED_TRACES.json').read_text())
assert len(corrected)==10
for r in corrected:
    personality=[x for x in r['trace'] if x['event']=='__gxx_personality_v0']
    assert personality
    expected='474e5543432b2b00' if r['abi']=='libsupc++' else '002b2b43474e4c43'
    assert all(x['header_bytes']==expected for x in personality)
    for x in r['trace']:
        if x['event']=='_Unwind_RaiseException':assert 'libunwind.so' in x['object'] and 'class_read' in x
    if r['abi']=='libsupc++':assert r['signal_abort']
groups=collections.Counter((r['abi'],r['unwinder_setting'],r['scenario'],r['exit']) for r in rows)
summary={'environment':'armv7l QEMU user mode, not physical board','program_runs':40,'initial_debugger_runs':20,
         'corrected_llvm_trace_runs':10,'invalid_initial_raise_header_reads':10,
         'correction':'ARM LLVM RaiseException 源码行断点处 r0 已变化；原十条 header 读数无效。修正后该处参数 NOT_OBSERVED；以 personality 收到的 header 字节为依据。',
         'results':[dict(abi=k[0],unwinder=k[1],scenario=k[2],exit=k[3],runs=v) for k,v in groups.items()],
         'value_and_failure_assertions':'PASS'}
(OUT/'ARM_SUMMARY.json').write_text(json.dumps(summary,indent=2,ensure_ascii=False)+'\n')
paths={
 'arm_supc_CMakeCache.txt':ROOT/'tmp/P11_0916/build-supc-arm/CMakeCache.txt',
 'arm_supc_build.ninja':ROOT/'tmp/P11_0916/build-supc-arm/build.ninja',
 'trace-arm-corrected.gdb':ROOT/'tmp/P11_0916/trace-arm-corrected.gdb'}
identity=[]
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
    return h.hexdigest()
for name,p in paths.items():
    dest=OUT/'snapshots'/name;shutil.copyfile(p,dest)
    identity.append({'source':str(p),'snapshot':str(dest.relative_to(OUT)),'sha256':sha(p)})
for p in [ROOT/'tmp/P11_0916/build-supc-arm/lib/libc++.so.1',ROOT/'tmp/P11_0916/supc_arm_main',
          OLD/'tmp/R11/extracted/9b/9bda3e524b7ff5384c44baa1d85808ec71cc78db7c17bf71726e24c6c6bd4c99/usr/lib/libunwind.so.1.0',
          ROOT/'tmp/gdb-multiarch_15.1-1ubuntu1~24.04.1_amd64.deb',ROOT/'tmp/P11_0916/debugger/usr/bin/gdb-multiarch']:
    identity.append({'source':str(p),'sha256':sha(p),'binary_not_committed':True})
(OUT/'ARM_IDENTITY.json').write_text(json.dumps(identity,indent=2)+'\n')
print(json.dumps(summary,indent=2,ensure_ascii=False))
