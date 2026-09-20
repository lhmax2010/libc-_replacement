import hashlib,json,pathlib,shutil
b=pathlib.Path('progress/ARM_REPRO_0920')
assert json.loads((b/'STEP1_RESULT.json').read_text())['decision']=='STEP_1_EXACT_FAILURE_REPRODUCED'
assert json.loads((b/'step1-failure-preserved-manifest.json').read_text())['all_verified']
old=json.loads((b/'cell_commands/original-argv-repro.json').read_text());new=old.copy();out_index=new.index('-o')+1
new[out_index]=str((b/'host-vv-original-config.strace').resolve())
new.insert(new.index('rpmbuild')+1,'-vv')
normalized=new.copy();normalized.remove('-vv');normalized[out_index]=old[out_index];assert normalized==old
assert hashlib.sha256(pathlib.Path('tmp/WEEKEND_0918/specs/llvm.spec').read_bytes()).hexdigest()=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
(b/'cell_commands/vv-only-repro.json').write_text(json.dumps(new,indent=2)+'\n')
(b/'vv-only-difference.json').write_text(json.dumps({'build_variable_added':'-vv','trace_output_old':old[out_index],'trace_output_new':new[out_index],'all_other_arguments_byte_identical':True},indent=2)+'\n')
deadlines=json.loads((b/'deadlines.json').read_text());deadlines['W2']=deadlines['task_total_deadline'];(b/'deadlines.json').write_text(json.dumps(deadlines,indent=2)+'\n')
snap=b/'script-snapshots-vv';snap.mkdir(exist_ok=True)
for name in ('guard.py','capture_failure.py','prepare_vv.py'):
    shutil.copy2(b/name,snap/name)
(snap/'SHA256.json').write_text(json.dumps({p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in snap.iterdir() if p.suffix=='.py'},indent=2)+'\n')
print(json.dumps({'variable':'-vv only (plus per-run trace output filename)','no_guest_trace':True,'host_strace_filter_unchanged':True,'deadline':deadlines['W2']}))
