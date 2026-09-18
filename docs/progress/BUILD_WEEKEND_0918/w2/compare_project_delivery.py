"""Verify an ordinary project push changed only the authorized branch."""
import datetime,json,shlex,subprocess,sys
from pathlib import Path
label,before_label,after_label,head_label=sys.argv[1:]
out=Path('progress/BUILD_WEEKEND_0918'); raw=out/'raw'
def read(name):
    assert (raw/(name+'.exitcode')).read_text().strip()=='0'
    return (raw/(name+'.stdout.txt')).read_text()
before={ref:sha for sha,ref in (x.split() for x in read(before_label).splitlines())}
after={ref:sha for sha,ref in (x.split() for x in read(after_label).splitlines())}
head=read(head_label).strip(); target='refs/heads/codex/r5-ehabi-diagnosis'
assert set(before)==set(after),'Unexpected branch addition/removal'
assert after[target]==head
unchanged=[ref for ref in before if ref!=target]
assert all(before[x]==after[x] for x in unchanged),'Other project branch changed; report before continuing'
cmd=['git','merge-base','--is-ancestor',before[target],head]
r=subprocess.run(cmd,capture_output=True,text=True)
record={'checked_at':datetime.datetime.now().astimezone().isoformat(),'before':before,'after':after,
        'target':target,'commit':head,'other_branches_unchanged':unchanged,
        'ancestry_command':shlex.join(cmd),'ancestry_exitcode':r.returncode,
        'ancestry_stdout':r.stdout,'ancestry_stderr':r.stderr,'force_used':False,
        'before_record':before_label,'after_record':after_label,'head_record':head_label}
(out/(label+'-project-delivery.json')).write_text(json.dumps(record,indent=2))
assert r.returncode==0
print('PASS',head,'other branches unchanged',len(unchanged),'prior head remains ancestor')
