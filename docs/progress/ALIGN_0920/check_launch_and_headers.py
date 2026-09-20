import hashlib,json,pathlib,subprocess
p=pathlib.Path('progress/ALIGN_0920')
out=[]
for label,base,cell in [('old','BUILD_WEEKEND_0918','llvm-arm-whole-retry'),('new','ARM_RPM_DIAG','llvm-arm-repro')]:
    root=pathlib.Path('progress')/base
    events=[json.loads(s) for s in (root/'cells'/cell/'events.jsonl').read_text().splitlines()]
    start=next(e for e in events if e['event']=='start')
    cmd=json.loads((root/'cell_commands'/f'{cell}.json').read_text())
    digest=hashlib.sha256((root/'guard.py').read_bytes()).hexdigest()
    assert start['command']==cmd and start['script_sha256']==digest
    out.append({'label':label,'event_source':str(root/'cells'/cell/'events.jsonl')+':5','argv_matches_executed_event':True,'guard_hash_matches_executed_event':True,'time':start['time']})
for item in json.loads((p/'static-identities/summary.json').read_text()):
    cmd=['rpm','-qp','--qf','%{NAME}\t%{PAYLOADCOMPRESSOR}\t%{PAYLOADFLAGS}\t%{VCS}\n',item['rpm']]
    r=subprocess.run(cmd,capture_output=True,text=True)
    out.append({'label':item['label'],'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
(p/'launch-and-payload-verification.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(out,ensure_ascii=False,indent=2))
