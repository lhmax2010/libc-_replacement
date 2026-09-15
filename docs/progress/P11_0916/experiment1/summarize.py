#!/usr/bin/env python3
import collections, csv, hashlib, json, pathlib, shutil
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(4*1024*1024),b''): h.update(b)
    return h.hexdigest()
def load(name):return json.loads((OUT/name).read_text())
obs=load('R78_OBSERVATIONS.json')
assert len(obs)==10
for row in obs:
    assert row['inferior_exit_12']
    trace=row['trace']
    assert all('libc++abi' in s['object'] for s in trace if s['event'] in ('__cxa_throw','__gxx_personality_v0'))
    assert all(s['exception_class']=='0x434c4e47432b2b00' for s in trace if 'exception_class' in s)
    matches=[s for s in trace if 'can_catch' in s['event']]
    assert len(matches)==1
    match=matches[0]
    assert match['catch_type']['name']==match['thrown_type']['name']=='13BoundaryError'
    assert match['catch_type']['address']!=match['thrown_type']['address']
    assert match['catch_type']['name_address']!=match['thrown_type']['name_address']
    if row['architecture']=='armv7l':assert any('"value": "false"' in s for s in row['type_match_returns'])
rep=load('R14B_REPEATS.json'); assert len(rep)==480
counts=collections.Counter((r['scheme'],r['scope'],r['direction'].split('_')[0]==r['direction'].split('_')[1],r['exit_code']) for r in rep)
grouped=[]
for key,n in sorted(counts.items()):grouped.append({'scheme':key[0],'scope':key[1],'same_side':key[2],'exit':key[3],'runs':n})
(OUT/'R14B_SUMMARY.json').write_text(json.dumps(grouped,indent=2)+'\n')
historical=list(csv.DictReader((OLD/'progress/R14b/L2/tables/matrix_all.tsv').open(),delimiter='\t'))
assert len(historical)==96
config=load('ABI_CONFIG_RESULTS.json');assert len(config)==40
for row in config:
    if row['scenario']=='normal':assert row['exit']==0 and 'OUTPUT=old:ping OLD_GUARD=1 ASSERT=PASS' in row['stdout']
    elif row['abi']=='libsupc++':assert row['exit']==0 and 'CAUGHT=77 OLD_GUARD=1 ASSERT=PASS' in row['stdout']
    else: assert row['exit']==12 and 'WRONG_EXCEPTION_TYPE' in row['stderr']
    if row['scenario']=='exception':
        raises=[r for r in row['trace'] if r['event']=='_Unwind_RaiseException']
        assert raises
        wanted='libunwind.so' if row['unwinder_setting']=='LLVM-preload' else 'libgcc_s.so'
        assert all(wanted in r['object'] for r in raises)
        for r in raises:assert r['exception_class']==('0x474e5543432b2b00' if row['abi']=='libsupc++' else '0x434c4e47432b2b00')
traces=load('R14B_TRACES.json');assert len(traces)==40
summary={'r78_observer_runs':len(obs),'r14b_original_cells':len(historical),'r14b_plain_repeats':len(rep),
         'r14b_trace_runs':len(traces),'abi_configuration_program_runs':len(config),'abi_configuration_trace_runs':20,
         'r14b_exit_counts':dict(collections.Counter(str(r['exit_code']) for r in rep)),
         'historical_signal_different_runs':sum(r['exit_code']!=r['historical_return'] for r in rep),
         'r78_native_exception_class':'0x434c4e47432b2b00','assertions':'PASS'}
(OUT/'SUMMARY.json').write_text(json.dumps(summary,indent=2,ensure_ascii=False)+'\n')
snap=OUT/'snapshots';snap.mkdir(exist_ok=True)
sources={
 'R14b_matrix_all.tsv':OLD/'progress/R14b/L2/tables/matrix_all.tsv',
 'R14b_build_l2.log':OLD/'progress/R14b/L2/commands/01_build_l2.log',
 'R14b_report.md':ROOT/'docs/progress/R14b/R14b_report.md',
 'R78_report.md':ROOT/'docs/progress/R78/REPORT.md',
 'supc_CMakeCache.txt':ROOT/'tmp/P11_0916/build-supc-clang/CMakeCache.txt',
 'supc_build.ninja':ROOT/'tmp/P11_0916/build-supc-clang/build.ninja',
 'original_abi_CMakeCache.txt':OLD/'tmp/R68/build-patched-x86_64/CMakeCache.txt',
 'record.py':ROOT/'docs/progress/API_0911/record.py',
 'resource_gate.sh':ROOT/'tools/resource_gate.sh',
 'trace-arm.gdb':ROOT/'tmp/P11_0916/trace-arm.gdb',
 'private_typeinfo.cpp':ROOT/'codes/llvm/libcxxabi/src/private_typeinfo.cpp',
 'cxa_personality.cpp':ROOT/'codes/llvm/libcxxabi/src/cxa_personality.cpp',
 'cxa_exception.h':ROOT/'codes/llvm/libcxxabi/src/cxa_exception.h',
 'typeinfo':ROOT/'codes/llvm/libcxx/include/typeinfo',
 'HandleLibCXXABI.cmake':ROOT/'codes/llvm/libcxx/cmake/Modules/HandleLibCXXABI.cmake'}
for name in ['boundary_error.hpp','new_main.cpp','old_plugin.cpp']:
    sources['R78_'+name]=ROOT/'docs/progress/R78/code'/name
for name in ['side.cpp','main.c','shared.hpp']:
    sources['R14b_'+name]=ROOT/'docs/progress/R14b/experiment_sources'/name
identity=[]
for name,p in sources.items():
    shutil.copyfile(p,snap/name)
    assert sha(p)==sha(snap/name)
    identity.append({'source':str(p),'snapshot':str((snap/name).relative_to(OUT)),'sha256':sha(p)})
artifacts=[OLD/'progress/R78/build/x86_64/new_main',OLD/'progress/R78/build/x86_64/libold_plugin.so',
 OLD/'progress/R78/build/x86_64/runtime/libc++.so.1',OLD/'progress/R78/build/x86_64/runtime/libc++abi.so.1',
 OLD/'progress/R78/build/armv7l/new_main',OLD/'progress/R78/build/armv7l/libold_plugin.so',
 OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib/libc++.so.1',
 OLD/'tmp/R68/build-patched-armv7l/libcxx/test-suite-install/lib/libc++abi.so.1',
 ROOT/'tmp/P11_0916/build-supc-clang/lib/libc++.so.1',ROOT/'tmp/P11_0916/supc_main',
 OLD/'tmp/R14b/runtime/B/libunwind.so.1.0']
artifacts+=list((ROOT/'tmp/P11_0916/controls').iterdir())
for p in artifacts:
    if p.is_file():identity.append({'source':str(p),'sha256':sha(p),'bytes':p.stat().st_size,'binary_not_committed':True})
(OUT/'IDENTITY.json').write_text(json.dumps(identity,indent=2)+'\n')
print(json.dumps(summary,indent=2))
