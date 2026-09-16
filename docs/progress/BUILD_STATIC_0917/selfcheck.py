import hashlib,json,re,subprocess,tarfile,time
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917'); t=Path('tmp/STATIC_0917')
checks={}
gnu=json.loads((p/'gnu-matrix.json').read_text())
checks['gnu_static_20_abort']=len([r for r in gnu if r['mode']=='static' and r['exitcode']==134 and not r['assertions_pass']])==20
checks['gnu_dynamic_20_pass']=len([r for r in gnu if r['mode']=='dynamic' and r['assertions_pass']])==20
for family in ['libcxx','gnu-counterfactual']:
    rows=json.loads((p/(family+'-matrix.json')).read_text())
    checks[family+'_20_pass']=len(rows)==20 and all(r['assertions_pass'] for r in rows)
c=json.loads((p/'candidate_checks.json').read_text())
checks['shared_libgcc_only_new_dependency']=set(c['needed'])=={'librt.so.1','libdl.so.2','libm.so.6','libpthread.so.0','libgcc_s.so.1','libc.so.6','ld-linux-x86-64.so.2'}
checks['cli_unchanged']=all(r['stdout_equal'] and r['stderr_equal'] and r['new_exitcode']==0 for r in c['cli_comparisons'])
plan=json.loads((p/'relink_plan_v2.json').read_text())
a=plan['original_tokens'].copy(); a.remove('-static-libgcc')
b=plan['new_tokens'].copy()
for seq in [a,b]:
    seq[seq.index('-o')+1]='<OUTPUT>'
    for i,s in enumerate(seq):
        if s.startswith('-Wl,-Map,'): seq[i]='<MAP>'
        if s.startswith('--dependency-file='): seq[i]='<DEPENDENCY-FILE>'
checks['link_single_semantic_change']=a==b
checks['input_objects_unchanged']=all(hashlib.sha256(Path(x['path']).read_bytes()).hexdigest()==x['sha256'] for x in json.loads((p/'reused_object_identities_v2.json').read_text()))
checks['old_candidate_unchanged']=hashlib.sha256(Path('tmp/NIGHT_0917/builds/bpftrace-x86_64-static/src/bpftrace').read_bytes()).hexdigest()=='242855842b9b07ed21d3bbb12e590601e7996b88dc29f335aa701fd3c72a5cc7'
checks['old_aarch_candidate_unchanged']=hashlib.sha256(Path('tmp/NIGHT_0917/builds/bpftrace-aarch64-static/src/bpftrace').read_bytes()).hexdigest()=='0981193d70d2a768ecef078a1980b58185f9d5904469a6de21d6532521561836'
tar=Path('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push/packaging/prebuilt-static-bpftrace.tar.gz')
checks['source1002_unchanged']=hashlib.sha256(tar.read_bytes()).hexdigest()=='3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123'
checks['restored_probe_snapshot_exact']=hashlib.sha256((p/'probes.executed_004.py').read_bytes()).hexdigest()=='4f3745bc1b0f79c3fb71e0a3c3e6fe3647b0399d58c7df0f79c80f84df93cc7f'
checks['fallback_equal']=all(r['expanded_specs_equal'] and r['forbidden_flags']==0 and r['source1002_asset_equal'] and r['previous_verified_install_equal'] for r in json.loads((p/'fallback_recheck.json').read_text()))
checks['new_binary_hash_matches']=hashlib.sha256((t/'bpftrace-dynamic-libgcc').read_bytes()).hexdigest()==c['sha256']
checks['strip_hash_matches']=hashlib.sha256((t/'bpftrace-dynamic-libgcc.stripped').read_bytes()).hexdigest()==c['stripped_sha256']
checks['actual_probe_source_unchanged']=Path('docs/progress/BUILD_STATIC_0916B/inputs/condition_cancel_probe.cpp').read_bytes()==(t/'condition_cancel_probe.cpp').read_bytes()
checks['gnu_register_table_zero']='dwarf_reg_size_table' in json.loads((p/'gnu-gdb-registers.json').read_text())['stdout']
checks['own_branch']=subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/r5-ehabi-diagnosis'
record=dict(checks=checks,all_pass=all(checks.values()),checked_epoch=time.time(),not_claimed=['original asset cancellation behavior','all platforms','package release approval','full RPM rebuild'])
(p/'selfcheck.json').write_text(json.dumps(record,indent=2))
print(json.dumps(record,indent=2)); raise SystemExit(0 if all(checks.values()) else 1)
