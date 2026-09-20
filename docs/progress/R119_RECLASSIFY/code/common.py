#!/usr/bin/env python3
import csv, datetime, hashlib, json, pathlib, subprocess, sys, time
ROOT = pathlib.Path(__file__).resolve().parents[4]
OUT = ROOT / 'docs/progress/R119_RECLASSIFY'
REC = ROOT / 'docs/progress/API_0911/record.py'
LIMIT = ROOT / 'docs/progress/R119_batch1/code/resume_limited.py'
def save(path, obj):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(obj, ensure_ascii=False, indent=2)+'\n')
def sha(path): return hashlib.sha256(pathlib.Path(path).read_bytes()).hexdigest()
def record(tag, args):
    stem = OUT/'raw'/tag
    subprocess.run([sys.executable, str(REC), str(stem), *map(str,args)], cwd=ROOT, check=False)
    rc=int(pathlib.Path(str(stem)+'.exitcode').read_text())
    return rc, pathlib.Path(str(stem)+'.stdout').read_text(errors='replace')
def gate(tag):
    rc, _ = record(tag, ['bash', 'tools/resource_gate.sh', '--level', 'light'])
    if rc:
        save(OUT/'RESOURCE_STOP.json',dict(exit=rc,tag=tag,time=datetime.datetime.now(datetime.timezone.utc).isoformat()))
        raise SystemExit(rc)
def inputs():
    result=[]
    for batch, name in [(1,'REVIEW_001_100.tsv'), (2,'REVIEW_101_200.tsv')]:
        base=ROOT/f'docs/progress/R119_batch{batch}'
        for row in csv.DictReader((base/'W1'/name).open(),delimiter='\t'):
            rank=int(row['rank']); path=base/'W1/reviewed'/f'{rank:03}.json'
            result.append(dict(row=row,rank=rank,base=base,dossier_path=path,dossier=json.loads(path.read_text())))
    return result
def tsv(path, rows):
    path.parent.mkdir(parents=True,exist_ok=True)
    with path.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
