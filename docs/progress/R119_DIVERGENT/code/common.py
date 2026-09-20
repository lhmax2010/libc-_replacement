#!/usr/bin/env python3
import csv,datetime,gzip,hashlib,json,pathlib,subprocess,sys,time
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=ROOT/'docs/progress/R119_DIVERGENT'
OLD=ROOT/'docs/progress/R119_PROJECTION'
REC=ROOT/'docs/progress/API_0911/record.py'
LIMIT=ROOT/'docs/progress/R119_batch1/code/resume_limited.py'
def sha(p):return hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
def save(p,obj):p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(obj,ensure_ascii=False,indent=2)+'\n')
def tsv(p,rows):
 p.parent.mkdir(parents=True,exist_ok=True)
 with p.open('w',newline='') as f:
  w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
def record(tag,args,limited=True):
 p=OUT/'raw'/tag;i=1
 while pathlib.Path(str(p)+'.command.txt').exists():i+=1;p=OUT/'raw'/f'{tag}_attempt{i}'
 cmd=([sys.executable,str(LIMIT)] if limited else [])+list(map(str,args))
 subprocess.run([sys.executable,str(REC),str(p),*cmd],cwd=ROOT,check=False)
 rc=int(pathlib.Path(str(p)+'.exitcode').read_text());out=pathlib.Path(str(p)+'.stdout').read_text(errors='replace')
 return rc,out,str(p.relative_to(ROOT))
def gate(tag):
 rc,out,ref=record(tag,['bash','tools/resource_gate.sh','--level','light'])
 if rc:save(OUT/'RESOURCE_STOP.json',dict(exit=rc,record=ref));raise SystemExit(rc)
 return out
if __name__=='__main__':
 tag=sys.argv[1];args=sys.argv[2:];git=args[0]=='git';rc,out,ref=record(tag,args,not git);print(out if len(out)<12000 else out[:4000]+'\n[会话仅显示摘要；完整 stdout 已落盘 '+ref+'.stdout]\n'+out[-1500:]);print((ROOT/(ref+'.stderr')).read_text());raise SystemExit(rc)
