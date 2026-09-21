import concurrent.futures,datetime,json,os,pathlib,shlex,subprocess
base=pathlib.Path(__file__).resolve().parent
rows=json.loads((base/'inventory.json').read_text());out=base/'remote';out.mkdir(exist_ok=True)
def query(row):
 name=row['package'];ref='refs/heads/'+row['branch']
 cmd=['git','ls-remote','--exit-code',row['url'],ref]
 env=os.environ.copy();env['GIT_SSH_COMMAND']='ssh -o BatchMode=yes -o ConnectTimeout=20';env['GIT_TERMINAL_PROMPT']='0'
 record=dict(package=name,url=row['url'],ref=ref,argv=cmd,command=shlex.join(cmd),environment_overrides={k:env[k] for k in ('GIT_SSH_COMMAND','GIT_TERMINAL_PROMPT')},started=datetime.datetime.now().astimezone().isoformat())
 try:
  p=subprocess.run(cmd,capture_output=True,text=True,env=env,timeout=60)
  record.update(exitcode=p.returncode,stdout=p.stdout,stderr=p.stderr)
  refs=[s.split('\t')[0] for s in p.stdout.splitlines() if s.endswith('\t'+ref)]
  if p.returncode==0 and len(refs)==1:record.update(state='EXISTS',head=refs[0])
  elif p.returncode==2 and not p.stdout.strip():record.update(state='ABSENT',head=None)
  else:record.update(state='NOT_AVAILABLE',head=None)
 except subprocess.TimeoutExpired as e:
  record.update(exitcode='TIMEOUT',state='NOT_AVAILABLE',head=None,stdout=(e.stdout or b'').decode(errors='replace') if isinstance(e.stdout,bytes) else (e.stdout or ''),stderr=(e.stderr or b'').decode(errors='replace') if isinstance(e.stderr,bytes) else (e.stderr or ''))
 record['finished']=datetime.datetime.now().astimezone().isoformat()
 (out/(name+'.json')).write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
 print(name,record['state'],record['head'] or '',flush=True)
 return record
with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
 result=list(pool.map(query,rows))
(base/'remote_results.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
