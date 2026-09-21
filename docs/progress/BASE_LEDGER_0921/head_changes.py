import csv,json,pathlib,shlex,subprocess
base=pathlib.Path(__file__).resolve().parent;root=base.parent.parent
rows=json.loads((base/'history_results.json').read_text())
impl={r['源码包']:r for r in csv.DictReader((root/'docs/progress/R101/tables/implementation_10_packages.tsv').open(),delimiter='\t')}
for row in rows:
 git=['git','-C',str(root/row['repository'])];commands=[]
 def run(args):
  p=subprocess.run(git+args,capture_output=True,text=True,timeout=60)
  commands.append(dict(command=shlex.join(git+args),argv=git+args,exitcode=p.returncode,stdout=p.stdout,stderr=p.stderr));return p
 head=row['head'];parent=row['head_metadata'][1].split()
 assert len(parent)==1
 row['head_parent']=parent[0]
 r=run(['rev-list','--count',parent[0]+'..'+head]);row['head_commit_count']=int(r.stdout) if r.returncode==0 else 'NOT_AVAILABLE'
 r=run(['diff-tree','--no-commit-id','--name-status','-r',head]);row['head_changed_files']=r.stdout.splitlines()
 r=run(['show','--format=fuller','--no-ext-diff','--stat',head]);row['head_show_stat']=r.stdout
 r=run(['diff','--no-ext-diff',parent[0],head,'--','packaging/*.spec','packaging/*.patch','tensorflow/lite/kernels/elementwise.cc']);row['head_patch']=r.stdout
 if row['package'] in impl or row['package']=='libcxx-runtimes':
  chosen=impl['llvm' if row['package']=='libcxx-runtimes' else row['package']]
  baseline=chosen['2026-09-03观测到的tizen_base_HEAD'];row['observed_implementation_baseline']=baseline
  r=run(['merge-base','--is-ancestor',baseline,head])
  if r.returncode==0:
   r=run(['rev-list','--count',baseline+'..'+head]);row['commits_since_implementation_baseline']=int(r.stdout)
  else:row['commits_since_implementation_baseline']='NOT_AVAILABLE_SHALLOW_OR_NONANCESTRAL'
 else:row['commits_since_implementation_baseline']=row.get('commits_since_r101','NOT_AVAILABLE')
 row['head_commands']=commands
 (base/'history'/(row['package']+'.json')).write_text(json.dumps(row,ensure_ascii=False,indent=2)+'\n')
 print(row['package'],row['commits_since_implementation_baseline'],row['head_changed_files'],flush=True)
(base/'history_results.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
