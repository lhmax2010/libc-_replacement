"""串行、限制内存并保留命令原文与退出码；不在旧目录写文件。"""
import datetime, hashlib, json, os, pathlib, resource, shlex, subprocess, sys, time
OUT=pathlib.Path(__file__).resolve().parents[1]
ROOT=OUT.parents[2]
RAW=OUT/'raw'
RAW.mkdir(parents=True,exist_ok=True)
total=int(next(l.split()[1] for l in pathlib.Path('/proc/meminfo').read_text().splitlines() if l.startswith('MemTotal:')))*1024
LIMIT=total*30//100
resource.setrlimit(resource.RLIMIT_AS,(LIMIT,LIMIT))
def sha(p): return hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
def save(name,obj):
 p=OUT/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(obj,ensure_ascii=False,indent=2)+'\n')
def run(label,args,timeout=180,check=True,env=None,git=False):
 base=RAW/label
 n=1
 while base.with_suffix('.command.txt').exists():
  n+=1;base=RAW/(label+f'_attempt{n}')
 command=list(args) if git else ['nice','-n','19','ionice','-c','3']+list(args)
 base.with_suffix('.command.txt').write_text(shlex.join(command)+'\n')
 start=datetime.datetime.now(datetime.timezone.utc).isoformat();t=time.monotonic()
 with base.with_suffix('.stdout').open('wb') as out,base.with_suffix('.stderr').open('wb') as err:
  try:r=subprocess.run(command,cwd=ROOT,stdout=out,stderr=err,timeout=timeout,env=env);rc=r.returncode
  except subprocess.TimeoutExpired:rc=124
 base.with_suffix('.exitcode').write_text(str(rc)+'\n')
 base.with_suffix('.time.json').write_text(json.dumps(dict(start=start,elapsed_seconds=time.monotonic()-t,rlimit_as_bytes=LIMIT,parallelism=1))+'\n')
 text=base.with_suffix('.stdout').read_text(errors='replace')
 error=base.with_suffix('.stderr').read_text(errors='replace')
 print(f'{label}: exit={rc}; record={base.relative_to(ROOT)}',flush=True)
 if check and rc:raise RuntimeError(f'{label}: exit {rc}\n{error[-2500:]}\n{text[-1000:]}')
 return dict(exit=rc,stdout=text,stderr=error,record=str(base.relative_to(ROOT)))
def gate(label):
 r=run(label,['bash','tools/resource_gate.sh','--level','light'],check=False)
 print(r['stdout'],flush=True)
 if r['exit']:raise SystemExit(r['exit'])
if __name__=='__main__':
 if sys.argv[1]=='gate':gate('resource_gate_light')
 else:
  r=run(sys.argv[1],sys.argv[2:],check=False,git=sys.argv[2]=='git')
  print(r['stdout'][:18000]);print(r['stderr'][:4000],file=sys.stderr);sys.exit(r['exit'])
