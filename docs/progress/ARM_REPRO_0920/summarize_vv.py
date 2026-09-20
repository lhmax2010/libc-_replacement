import hashlib,json,pathlib,re,subprocess
b=pathlib.Path('progress/ARM_REPRO_0920');c=b/'cells/vv-only-repro';lines=(c/'build.log').read_text(errors='replace').splitlines()
events=[json.loads(x) for x in (c/'events.jsonl').read_text().splitlines()];start=next(e for e in events if e['event']=='start');end=next(e for e in reversed(events) if e['event']=='finished')
old=json.loads((b/'cell_commands/original-argv-repro.json').read_text());new=start['command'].copy();assert new.count('-vv')==1;new.remove('-vv');new[new.index('-o')+1]=old[old.index('-o')+1];assert new==old
wrote=[{'line':i,'name':pathlib.Path(s.removeprefix('Wrote: ')).name,'text':s} for i,s in enumerate(lines,1) if s.startswith('Wrote: ')]
errors=[{'line':i,'text':s} for i,s in enumerate(lines,1) if s.startswith('error:') or 'cpio: write failed' in s]
expected='error: create archive failed: cpio: write failed - Function not implemented'
if end['exitcode']==0:decision='STOP_VV_FAILURE_DISAPPEARED'
elif end['exitcode']==1 and any(e['text']==expected for e in errors) and len(wrote)==18 and wrote[-1]['name']=='clang-devel-debuginfo-22.1.8-1.armv7l.rpm':decision='VV_SAME_FAILURE_REPRODUCED'
else:decision='STOP_VV_OTHER_FAILURE_SHAPE'
inventory=[]
for p in sorted(pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l').glob('*.rpm')):
    cmd=['rpm','-K','--nosignature',str(p)];r=subprocess.run(cmd,capture_output=True,text=True);h=hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda:f.read(1024*1024),b''):h.update(chunk)
    inventory.append({'path':str(p),'sha256':h.hexdigest(),'bytes':p.stat().st_size,'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
o={'decision':decision,'start':start['time'],'finish':end['time'],'exitcode':end['exitcode'],'wrote':wrote,'errors':errors,'rpm_inventory':inventory,'one_build_variable_verified':True,'guard_sha256':start['script_sha256']}
(b/'VV_RESULT.json').write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:o[k] for k in ('decision','start','finish','exitcode')},ensure_ascii=False));print('Wrote',len(wrote),'RPM files',len(inventory),'digest failures',sum(x['exitcode']!=0 for x in inventory))
