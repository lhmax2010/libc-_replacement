import datetime,json,pathlib,re,shlex,subprocess
base=pathlib.Path(__file__).resolve().parent;root=base.parent.parent
rows=json.loads((base/'inventory.json').read_text());remotes={r['package']:r for r in json.loads((base/'remote_results.json').read_text())}
out=base/'history';out.mkdir(exist_ok=True)
def command(record,argv):
 p=subprocess.run(argv,capture_output=True,text=True,timeout=90)
 record['commands'].append(dict(argv=argv,command=shlex.join(argv),exitcode=p.returncode,stdout=p.stdout,stderr=p.stderr))
 return p
results=[]
for row in rows:
 remote=remotes[row['package']]
 if remote['state']!='EXISTS':continue
 name=row['package'];head=remote['head'];record=dict(package=name,head=head,commands=[])
 match=re.search(re.escape(row['repo_path'])+r'#([a-f0-9]{40})',row['r101']['证据'])
 baseline=match[1] if match else None
 record['r101_baseline']=baseline
 candidates=[root/'codes/R104'/name]
 if name=='bpftrace':candidates.append(root/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push')
 if name in ('llvm','libcxx-runtimes'):
  candidates=[root/'tmp/BUILD_W1BC_0911/target-fetch',root/'codes/R104/llvm']
 if name=='libcxx-runtimes':candidates.insert(0,root/'codes/llvm')
 found=None
 for path in candidates:
  if not path.is_dir():continue
  p=command(record,['git','-C',str(path),'cat-file','-t',head])
  if p.returncode==0:found=path;break
 if found is None:
  record['state']='NOT_AVAILABLE_LOCAL_OBJECT';results.append(record)
  (out/(name+'.json')).write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n');print(name,record['state'],flush=True);continue
 record['repository']=str(found.relative_to(root));git=['git','-C',str(found)]
 p=command(record,git+['show','-s','--format=%H%n%P%n%aI%n%cI%n%s',head]);record['head_metadata']=p.stdout.splitlines()
 p=command(record,git+['rev-parse','--is-shallow-repository']);record['shallow']=p.stdout.strip()
 p=command(record,git+['rev-list','--count',head]);record['visible_history_count']=int(p.stdout) if p.returncode==0 else 'NOT_AVAILABLE'
 record['complete_history_count']=record['visible_history_count'] if record['shallow']=='false' else 'NOT_AVAILABLE_SHALLOW'
 if baseline:
  p=command(record,git+['merge-base','--is-ancestor',baseline,head]);record['baseline_is_ancestor']=p.returncode==0;record['baseline_ancestry_exitcode']=p.returncode
  if p.returncode==0:
   rev=baseline+'..'+head
   p=command(record,git+['rev-list','--count',rev]);record['commits_since_r101']=int(p.stdout) if p.returncode==0 else 'NOT_AVAILABLE'
   p=command(record,git+['log','--format=%H%x09%cI%x09%s',rev]);record['commits']=p.stdout.splitlines()
   p=command(record,git+['diff','--name-status',baseline,head]);record['changed_files']=p.stdout.splitlines()
   p=command(record,git+['diff','--stat',baseline,head]);record['diffstat']=p.stdout
   p=command(record,git+['diff','--no-ext-diff',baseline,head,'--','packaging/*.spec','packaging/*.patch']);record['packaging_diff']=p.stdout
 record['state']='OBSERVED';results.append(record)
 (out/(name+'.json')).write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
 print(name,record['state'],'delta',record.get('commits_since_r101'),'total',record['complete_history_count'],flush=True)
(base/'history_results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
