"""Snapshot previously reviewed probe/board helpers into this task, with explicit path changes."""
import hashlib,json
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; old=p/'progress/BUILD_STATIC_0917B/resume_0917'
records=[]
for name in ('prepare_arm_cancel_probes.py','board_cancel_check.py','board_functional_check.py'):
    source=old/name; original=source.read_text()
    updated=original.replace('progress/BUILD_STATIC_0917B/resume_0917','progress/BUILD_WEEKEND_0918')
    updated=updated.replace('tmp/STATIC_0917B','tmp/WEEKEND_0918')
    updated=updated.replace('build_static_0917b_cancel','build_weekend_0918_cancel')
    updated=updated.replace('build_static_0917b_function','build_weekend_0918_function')
    if name=='board_cancel_check.py':
        updated=updated.replace("shell('test ! -e '+remote)","_,initial_identity=shell('id')\ninitial_root='uid=0(' in initial_identity\nshell('test ! -e '+remote)",1)
        updated=updated.replace("command(sdb+['root','off'])\n_,identity=shell('id')\nassert 'uid=0(' not in identity", "if not initial_root: command(sdb+['root','off'])\n_,identity=shell('id')\nassert ('uid=0(' in identity)==initial_root")
        updated=updated.replace('VERIFIED_TASK_DIRECTORY_ABSENT_ROOT_OFF','VERIFIED_TASK_DIRECTORY_ABSENT_INITIAL_ROOT_STATE_RESTORED')
    dest=out/name; assert not dest.exists(); dest.write_text(updated)
    records.append({'source':str(source),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
                    'destination':str(dest),'destination_sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),
                    'changes':'Task output/temp/board paths; cancellation helper restores observed initial root state instead of assuming root off.'})
source=p/'tmp/STATIC_0917B/board_run_bounded.c'; dest=p/'tmp/WEEKEND_0918/board_run_bounded.c'
assert not dest.exists(); dest.write_bytes(source.read_bytes())
records.append({'source':str(source),'destination':str(dest),'sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'changes':'None'})
(out/'board-helper-provenance.json').write_text(json.dumps(records,indent=2))
print('Prepared only; no board contact, compilation, transfer or installation performed.')
