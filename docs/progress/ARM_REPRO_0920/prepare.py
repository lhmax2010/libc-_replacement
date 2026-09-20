import datetime,hashlib,json,pathlib,shutil,subprocess
base=pathlib.Path('progress/ARM_REPRO_0920');base.joinpath('cell_commands').mkdir(exist_ok=True)
source=pathlib.Path('progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json')
old=json.loads(source.read_bytes());new=old.copy();i=new.index('-o')+1
new[i]=str((base/'host-original-config.strace').resolve())
assert [(j,x,y) for j,(x,y) in enumerate(zip(old,new)) if x!=y]==[(i,old[i],new[i])]
(base/'original-argv.json').write_bytes(source.read_bytes())
(base/'cell_commands/original-argv-repro.json').write_text(json.dumps(new,indent=2)+'\n')
(base/'argv-only-difference.json').write_text(json.dumps({'index':i,'old':old[i],'new':new[i],'all_other_arguments_byte_identical':True,'original_json_sha256':hashlib.sha256(source.read_bytes()).hexdigest()},indent=2)+'\n')
spec=pathlib.Path('tmp/WEEKEND_0918/specs/llvm.spec');assert hashlib.sha256(spec.read_bytes()).hexdigest()=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
deadline='2026-09-20T17:15:44+08:00'
(base/'deadlines.json').write_text(json.dumps({'W1':deadline,'task_total_deadline':'2026-09-21T02:15:44+08:00'},indent=2)+'\n')
for name in ('guard.py','capture_failure.py','prepare.py'):
    p=base/name;snapshot=base/'script-snapshots'/name;snapshot.parent.mkdir(exist_ok=True);shutil.copy2(p,snapshot)
print(json.dumps({'single_argv_change':new[i],'stage_deadline':deadline,'spec_unchanged':True}))
