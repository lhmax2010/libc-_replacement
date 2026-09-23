"""Export the two local commits; never push or sign the package repository."""
import hashlib,json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';repo=P/'tmp/BUILD_W1BC_0911/target-fetch';records=[]
def run(label,args):
 cmd=['git','-C',str(repo),*args];r=subprocess.run(cmd,capture_output=True);records.append(dict(label=label,argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout.decode(),stderr=r.stderr.decode()));assert r.returncode==0;return r.stdout
remote=run('fetch-head',['rev-parse','FETCH_HEAD']).decode().strip();assert remote=='f203923a1508c9344f5fc6b17bd8822f011655c4'
commits=run('commits',['rev-list','--reverse',remote+'..HEAD']).decode().splitlines();assert len(commits)==2
(E/'SANDBOX_COMMITS_R3.diff').write_bytes(b'\n'.join(run('show-'+c,['show','--format=fuller','--no-ext-diff',c]) for c in commits))
for i,c in enumerate(commits,1):(E/f'{i:04d}-local-recipe_R3.patch').write_bytes(run('patch-'+c,['format-patch','-1',c,'--stdout']))
clean=run('status',['status','--porcelain'])==b'';counts=run('ahead-behind',['rev-list','--left-right','--count',remote+'...HEAD']).decode().strip();assert clean and counts=='0\t2'
assert hashlib.sha256((repo/'packaging/libcxx-runtimes.spec').read_bytes()).hexdigest()=='c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe'
for c in commits:
 assert b'Signed-off-by:' not in run('message-'+c,['show','-s','--format=%B',c])
 assert run('author-'+c,['show','-s','--format=%an <%ae>',c]).decode().strip()=='Hao Lin <hao.lin@samsung.com>'
(E/'LOCAL_COMMITS_R3.json').write_text(json.dumps(dict(commits=commits,base=remote,ahead=2,behind=0,clean=True,package_push=False,signoff=False),indent=2)+'\n')
(E/'export_commands_R3.json').write_text(json.dumps(records,indent=2)+'\n');print(commits,'ahead2/behind0','clean',clean)
