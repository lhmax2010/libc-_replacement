import json,subprocess
from pathlib import Path
p=Path('progress/BUILD_STATIC_0917')
def refs(file):
    return {line.split()[1]:line.split()[0] for line in file.read_text().splitlines()}
before=refs(p/'raw/043_remote_before.stdout.txt')
after=refs(p/'raw/054_remote_after.stdout.txt')
target='refs/heads/codex/r5-ehabi-diagnosis'
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
checks=dict(same_branch_names=before.keys()==after.keys(),other_branches_unchanged=all(before[k]==after[k] for k in before if k!=target),target_matches_local=after[target]==head,push_exit_zero=(p/'raw/053_push.exitcode').read_text().strip()=='0')
r=subprocess.run(['git','merge-base','--is-ancestor',before[target],head],capture_output=True,text=True)
checks['fast_forward']=r.returncode==0
paths=subprocess.check_output(['git','diff-tree','--no-commit-id','--name-only','-r',head],text=True).splitlines()
checks['only_task_docs']=bool(paths) and all(x.startswith('docs/progress/BUILD_STATIC_0917/') for x in paths)
record=dict(before=before,after=after,commit=head,checks=checks,ancestor_command=['git','merge-base','--is-ancestor',before[target],head],ancestor_exitcode=r.returncode)
(p/'push_verification.json').write_text(json.dumps(record,indent=2))
print(json.dumps(record,indent=2)); raise SystemExit(0 if all(checks.values()) else 1)
