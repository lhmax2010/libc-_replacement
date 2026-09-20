import hashlib,json,pathlib,re,subprocess
b=pathlib.Path('progress/ARM_REPRO_0920');cell=b/'cells/original-argv-repro';log=(cell/'build.log').read_text(errors='replace');lines=log.splitlines()
events=[json.loads(x) for x in (cell/'events.jsonl').read_text().splitlines()]
started=next(e for e in events if e['event']=='start');finished=next(e for e in reversed(events) if e['event']=='finished')
old=json.loads((b/'original-argv.json').read_text());new=started['command'];diffs=[{'index':i,'old':x,'new':y} for i,(x,y) in enumerate(zip(old,new)) if x!=y]
assert len(old)==len(new) and len(diffs)==1 and old[diffs[0]['index']-1]=='-o'
wrote=[{'line':i,'text':s,'name':pathlib.Path(s.removeprefix('Wrote: ')).name} for i,s in enumerate(lines,1) if s.startswith('Wrote: ')]
errors=[{'line':i,'text':s} for i,s in enumerate(lines,1) if s.startswith('error:') or 'cpio: write failed' in s]
code=finished['exitcode'];expected='error: create archive failed: cpio: write failed - Function not implemented'
if code==0:decision='STOP_STEP_1_SUCCESS'
elif code==1 and any(e['line']==7560 and e['text']==expected for e in errors):decision='STEP_1_EXACT_FAILURE_REPRODUCED'
else:decision='STOP_OTHER_FAILURE_SHAPE'
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for part in iter(lambda:f.read(1024*1024),b''):h.update(part)
    return h.hexdigest()
inventory=[]
for p in sorted(pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l').glob('*.rpm')):
    cmd=['rpm','-K','--nosignature',str(p)];r=subprocess.run(cmd,capture_output=True,text=True)
    inventory.append({'path':str(p),'name':p.name,'bytes':p.stat().st_size,'sha256':sha(p),'digest_command':cmd,'digest_exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
result={'decision':decision,'start':started['time'],'finish':finished['time'],'exitcode':code,'argv_differences':diffs,'wrote':wrote,'errors':errors,'rpm_inventory':inventory,'same_rpm_name_set_as_wrote':set(x['name'] for x in wrote)==set(x['name'] for x in inventory),'guard_sha256':started['script_sha256'],'log_sha256':sha(cell/'build.log'),'archive_failure_values':'NOT_OBSERVED unless separate failure instrumentation evidence exists'}
(b/'STEP1_RESULT.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['decision','start','finish','exitcode','same_rpm_name_set_as_wrote']},ensure_ascii=False));print('Wrote',len(wrote),'RPM files',len(inventory),'digest failures',sum(x['digest_exitcode']!=0 for x in inventory))
