"""Verify exactly the eleven registered sandbox refs using their prior observed URLs."""
import csv,datetime,json,os,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/QB_PRECHECK_0922';records=[]
table=P/'docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.md'
(E/'PACKAGE_STATUS.before.md').write_bytes(table.read_bytes())
for line in table.read_text().splitlines():
 if not line.startswith('| ') or not 'sandbox/' in line:continue
 cols=[x.strip() for x in line.strip('|').split('|')];name,branch,expected=cols[:3]
 if name=='bpftrace':expected='7831fb34c6881b5cff0b8bdd0dac27a1829290fa'
 old=json.loads((P/f'docs/progress/BASE_LEDGER_0921/remote/{name}.json').read_text());url=old['url'];ref='refs/heads/'+branch
 argv=['git','ls-remote','--exit-code',url,ref];env=os.environ.copy();env.update(GIT_TERMINAL_PROMPT='0',GIT_SSH_COMMAND='ssh -o BatchMode=yes -o ConnectTimeout=20')
 start=datetime.datetime.now().astimezone().isoformat();r=subprocess.run(argv,stdin=subprocess.DEVNULL,capture_output=True,text=True,env=env,timeout=45)
 head=r.stdout.split()[0] if r.returncode==0 and r.stdout.strip() else 'NOT_AVAILABLE'
 records.append(dict(package=name,url=url,branch=branch,expected=expected,remote=head,status='MATCH' if head==expected else 'MISMATCH' if r.returncode==0 else 'NOT_AVAILABLE',command=shlex.join(argv),argv=argv,env_overrides={k:env[k] for k in ('GIT_TERMINAL_PROMPT','GIT_SSH_COMMAND')},start=start,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
 (E/'REMOTE_RESULTS.json').write_text(json.dumps(records,ensure_ascii=False,indent=2));print(name,records[-1]['status'],head,flush=True)
assert len(records)==11
with (E/'REMOTE_BRANCHES.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=['package','url','branch','expected','remote','status'],delimiter='\t',extrasaction='ignore');w.writeheader();w.writerows(records)
