"""Read package remote refs without changing codes/ or any remote branch."""
import csv,datetime,json,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918'); rows=[]; commands=[]
packages=list(csv.DictReader(Path('docs/progress/BUILD_NIGHT_0916/PACKAGE_STATUS.tsv').open(),delimiter='\t'))
for package in packages:
    name=package['package']
    repo=Path('tmp/BUILD_W1BC_0911/target-fetch') if name=='llvm' else Path('codes/R104')/name
    ref='refs/heads/'+package['branch']
    cmd=['git','-C',str(repo),'ls-remote','--exit-code','origin',ref]
    record={'command':shlex.join(cmd),'started':datetime.datetime.now().astimezone().isoformat()}
    try:
        r=subprocess.run(cmd,capture_output=True,text=True,timeout=60)
        record.update(exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
        matches=[line.split('\t')[0] for line in r.stdout.splitlines() if line.endswith('\t'+ref)]
        head=matches[0] if r.returncode==0 and len(matches)==1 else 'NOT_OBSERVED'
    except subprocess.TimeoutExpired:
        record.update(exitcode='NOT_OBSERVED_TIMEOUT'); head='NOT_OBSERVED'
    commands.append(record)
    rows.append({'package':name,'branch':package['branch'],'audited_pin':package['adaptation_commit'],
                 'remote_head':head,'relation':'MATCH' if head==package['adaptation_commit'] else ('NOT_OBSERVED' if head=='NOT_OBSERVED' else 'DRIFT_REQUIRES_SEPARATE_AUDIT')})
    (out/'package-head-commands.json').write_text(json.dumps(commands,indent=2))
    (out/'package-head-results.json').write_text(json.dumps(rows,indent=2))
    print(name,rows[-1]['relation'],head,flush=True)
assert all(x['relation']=='MATCH' for x in rows),'Do not claim the pinned audit covers changed/unobserved remote heads'
