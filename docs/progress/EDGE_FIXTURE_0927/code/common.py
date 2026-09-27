"""轻量串行任务日志；闸门失败交还调度，不在后台抢资源。"""
import datetime,hashlib,json,os,pathlib,resource,shlex,subprocess,sys,time,signal
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=ROOT/'docs/progress/EDGE_FIXTURE_0927'; TMP=ROOT/'tmp/EDGE_FIXTURE_0927'
RAW=OUT/'raw';RAW.mkdir(parents=True,exist_ok=True);TMP.mkdir(parents=True,exist_ok=True)
# Default is this task's immutable cutoff. A separately authorized future replay
# must explicitly provide its own cutoff; this task never sets the override.
DEADLINE=datetime.datetime.fromisoformat(os.environ.get('EDGE_FIXTURE_DEADLINE_UTC','2026-09-28T00:30:00+00:00')).timestamp()
MEM=int(next(x.split()[1] for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
resource.setrlimit(resource.RLIMIT_AS,(MEM*30//100,MEM*30//100))
def sha(p):
 h=hashlib.sha256()
 with open(p,'rb') as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
def save(name,obj):
 p=OUT/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(obj,ensure_ascii=False,indent=2)+'\n')
def run(label,args,timeout=120,check=True,cwd=None,env=None):
 if time.time()>DEADLINE-300:raise SystemExit('DEADLINE: reserve last five minutes for handoff')
 base=RAW/(str(time.time_ns())+'_'+label)
 cmd=['nice','-n','19','ionice','-c','3']+list(map(str,args))
 base.with_suffix('.command.txt').write_text('cwd='+shlex.quote(str(cwd or ROOT))+'\n'+shlex.join(cmd)+'\n')
 t=time.time()
 with base.with_suffix('.stdout').open('wb') as o,base.with_suffix('.stderr').open('wb') as e:
  process=subprocess.Popen(cmd,cwd=cwd or ROOT,env=env,stdout=o,stderr=e,start_new_session=True)
  try:rc=process.wait(timeout=min(timeout,max(1,DEADLINE-300-t)))
  except subprocess.TimeoutExpired:
   os.killpg(process.pid,signal.SIGTERM)
   try:process.wait(timeout=5)
   except subprocess.TimeoutExpired:os.killpg(process.pid,signal.SIGKILL);process.wait()
   rc=124
 base.with_suffix('.exitcode').write_text(str(rc)+'\n')
 save(str(base.relative_to(OUT))+'.time.json',dict(start_utc=datetime.datetime.fromtimestamp(t,datetime.timezone.utc).isoformat(),elapsed=time.time()-t,rlimit_as=MEM*30//100,parallelism=1))
 result=dict(exit=rc,record=str(base.relative_to(ROOT)),stdout=base.with_suffix('.stdout').read_text(errors='replace'),stderr=base.with_suffix('.stderr').read_text(errors='replace'))
 print(label,rc,flush=True)
 if check and rc:raise RuntimeError(json.dumps(result,ensure_ascii=False)[-6000:])
 return result
def gate():
 state=OUT/'GATE_STATE.json';old=json.loads(state.read_text()) if state.exists() else {'failures':0,'next_retry':0}
 if time.time()<old['next_retry']:raise SystemExit('GATE_WAIT_UNTIL '+str(old['next_retry']))
 r=run('resource_gate',['bash','tools/resource_gate.sh','--level','light'],check=False)
 print(r['stdout'],flush=True)
 if r['exit']:
  old['failures']+=1;old['next_retry']=time.time()+600;save('GATE_STATE.json',old)
  raise SystemExit('GATE_STOP_SIX_FAILURES' if old['failures']>=6 else 'GATE_WAIT_10_MINUTES')
 save('GATE_STATE.json',dict(failures=0,next_retry=0))
if __name__=='__main__':
 if sys.argv[1]=='gate':gate()
 else:
  r=run(sys.argv[1],sys.argv[2:],check=False);print(r['stdout'][:18000]);print(r['stderr'][:5000]);sys.exit(r['exit'])
