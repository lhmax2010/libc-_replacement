"""Export current local commit without pretending a second commit exists."""
import hashlib,json,shlex,subprocess
from pathlib import Path
P=Path.cwd(); E=P/'docs/progress/LLVM_W4_0923'; repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
records=[]
def run(label,args):
 cmd=['git','-C',str(repo),*args];r=subprocess.run(cmd,capture_output=True)
 records.append(dict(label=label,argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout.decode(),stderr=r.stderr.decode()))
 assert r.returncode==0
 return r.stdout
head=run('head',['rev-parse','HEAD']).decode().strip()
diff=run('show',['show','--format=fuller','--no-ext-diff','HEAD'])
(E/'SANDBOX_COMMITS_R2.diff').write_bytes(diff)
(E/'0001-Build-static-libclang_R2.patch').write_bytes(run('format-patch',['format-patch','-1','--stdout']))
status=run('status',['status','--porcelain']).decode();counts=run('ahead-behind',['rev-list','--left-right','--count','FETCH_HEAD...HEAD']).decode().strip()
assert not status;assert counts=='0\t1'
tree=run('old-tree',['rev-parse','2cba97e5^{tree}']).decode().strip()
assert tree==run('new-tree',['rev-parse','HEAD^{tree}']).decode().strip()
(E/'LOCAL_COMMITS_R2.json').write_text(json.dumps(dict(commits=[head],second_commit='NOT_CREATED_PENDING_GATE_CLARIFICATION',ahead=1,behind=0,clean=True,tree_unchanged_after_amend=True,remote_base='f203923a1508c9344f5fc6b17bd8822f011655c4',package_push=False),indent=2)+'\n')
(E/'export_commands_R2.json').write_text(json.dumps(records,indent=2)+'\n')
print(head,counts,'clean',not status)
