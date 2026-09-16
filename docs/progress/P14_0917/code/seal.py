from common import *
import shutil,datetime
code=OUT/'code';code.mkdir(exist_ok=True)
snapshots={}
for p in sorted(TMP.iterdir()):
 if p.is_file() and p.suffix in ('.py','.cpp','.c','.h','.sh'):
  q=code/p.name;shutil.copy2(p,q);snapshots[str(q.relative_to(OUT))]=hashlib.sha256(q.read_bytes()).hexdigest()
(OUT/'SNAPSHOT_SHA256.json').write_text(json.dumps(snapshots,indent=2)+'\n')
before=json.loads((OUT/'W3/BEFORE_IDENTITY.json').read_text());changes=[]
for item in before:
 current=ROOT/item['path'];rel=current.relative_to(ROOT/'docs/progress/P13_0917');old=OUT/'W3/before_P13'/rel
 row=run('W3','diff_'+str(rel).replace('/','_'),['git','diff','--no-index',old,current],allow=(1,))
 changes.append({'path':item['path'],'before_sha256':item['sha256'],'after_sha256':hashlib.sha256(current.read_bytes()).hexdigest(),'diff_record':row['raw']})
save('W3','DOCUMENT_CHANGES.json',changes)
for rel in ['net/cookies/parsed_cookie.cc','net/cookies/parsed_cookie.h','tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc','build/config/compiler/BUILD.gn','build/config/BUILDCONFIG.gn']:
 p=TMP/'chromium-efl'/rel;q=OUT/'W2/source_evidence'/rel;q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,q)
def stamp(stage,file,field):return datetime.datetime.fromisoformat(json.loads((OUT/stage/'raw'/file).read_text())[field])
start=stamp('W1','001_gate.time.json','start');w2=stamp('W2','001_gate.time.json','start');w3=stamp('W3','001_gate.time.json','start');now=datetime.datetime.now(datetime.timezone.utc)
times={'actual_utc_start':str(start),'audit_utc':str(now),'W1_start_to_next_gate_seconds':(w2-start).total_seconds(),'W2_start_to_next_gate_seconds':(w3-w2).total_seconds(),'W3_until_audit_seconds':(now-w3).total_seconds(),'total_until_audit_seconds':(now-start).total_seconds()}
assert times['W1_start_to_next_gate_seconds']<7200 and times['W2_start_to_next_gate_seconds']<18000 and times['W3_until_audit_seconds']<7200 and times['total_until_audit_seconds']<32400
(OUT/'TIMINGS.json').write_text(json.dumps(times,indent=2)+'\n')
print(json.dumps(times))
