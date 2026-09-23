import json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';R=P/'tmp/BUILD_W1BC_0911/target-fetch';records=[]
def git(*args):
 cmd=['git','-C',str(R),*args];r=subprocess.run(cmd,capture_output=True,text=True);records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr));(E/'commit-commands.json').write_text(json.dumps(records,indent=2));assert r.returncode==0;return r.stdout
sha=git('rev-parse','HEAD').strip();status=git('status','--porcelain');counts=git('rev-list','--left-right','--count','FETCH_HEAD...HEAD').strip()
assert not status and counts.split()==['0','1']
diff=git('diff','f203923a1508c9344f5fc6b17bd8822f011655c4..HEAD','--','packaging')
assert len([x for x in diff.splitlines() if x.startswith('+') and not x.startswith('+++')])==1
(E/'SANDBOX_COMMIT.diff').write_text(git('show','--format=fuller','--no-ext-diff','HEAD'))
(E/'0001-Build-static-libclang.patch').write_text(git('format-patch','-1','--stdout','HEAD'))
remote=git('ls-remote','origin','refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport');assert remote.split()[0]=='f203923a1508c9344f5fc6b17bd8822f011655c4'
(E/'LOCAL_COMMIT.json').write_text(json.dumps(dict(sha=sha,status='clean',ahead=1,behind=0,remote=remote.strip(),pushed=False),indent=2));print(sha)
