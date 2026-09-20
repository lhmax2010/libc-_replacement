#!/usr/bin/env python3
import csv, datetime, gzip, hashlib, json, pathlib, subprocess, sys, time
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=ROOT/'docs/progress/R119_PROJECTION'
OLD=ROOT/'docs/progress/R119_RECLASSIFY'
REC=ROOT/'docs/progress/API_0911/record.py'
def sha(p):return hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
def save(p,obj):
 p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(obj,ensure_ascii=False,indent=2)+'\n')
def tsv(p,rows):
 p.parent.mkdir(parents=True,exist_ok=True)
 with p.open('w',newline='') as f:
  w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
def record(tag,args):
 p=OUT/'raw'/tag;i=1
 while pathlib.Path(str(p)+'.command.txt').exists():i+=1;p=OUT/'raw'/f'{tag}_attempt{i}'
 subprocess.run([sys.executable,str(REC),str(p),*map(str,args)],cwd=ROOT,check=False)
 rc=int(pathlib.Path(str(p)+'.exitcode').read_text())
 return rc,pathlib.Path(str(p)+'.stdout').read_text(errors='replace'),str(p.relative_to(ROOT))
def gate(tag):
 rc,_,ref=record(tag,['bash','tools/resource_gate.sh','--level','light'])
 if rc:
  save(OUT/'RESOURCE_STOP.json',dict(exit=rc,record=ref,time=datetime.datetime.now(datetime.timezone.utc).isoformat()))
  raise SystemExit(rc)
def candidates():
 rows=list(csv.DictReader((OLD/'RECLASSIFIED_001_200.tsv').open(),delimiter='\t'))
 result=[r for r in rows if r['subtype']=='COMPILE_TIME_ONLY'];assert len(result)==65
 return result
