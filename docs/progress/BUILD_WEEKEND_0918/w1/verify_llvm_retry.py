"""Verify the retry's full command result separately from its RPM payloads."""
import hashlib,json,re,shlex,subprocess,sys
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
cell=out/'cells/llvm-arm-whole-retry'
events=[json.loads(x) for x in (cell/'events.jsonl').read_text().splitlines()]
finished=[x for x in events if x.get('event')=='finished']
assert len(finished)==1 and finished[0]['exitcode']==0,'Whole rpmbuild did not finish successfully'
old=p/'progress/BUILD_STATIC_0917B/resume_0917/verify_llvm_rpms.executed.py'
data=old.read_text().replace("progress/BUILD_STATIC_0917B/resume_0917", "progress/BUILD_WEEKEND_0918/llvm_retry")
data=data.replace("tmp/STATIC_0917B", "tmp/WEEKEND_0918")
dest=out/'llvm_retry'; dest.mkdir(exist_ok=False)
snapshot=dest/'payload_verifier.py'; snapshot.write_text(data)
cmd=['/usr/bin/python3',str(snapshot),'armv7l']
print('COMMAND '+shlex.join(cmd),flush=True)
r=subprocess.run(cmd)
(dest/'verification_command.json').write_text(json.dumps({'command':shlex.join(cmd),'exitcode':r.returncode,
    'source_sha256':hashlib.sha256(old.read_bytes()).hexdigest(),
    'executed_sha256':hashlib.sha256(snapshot.read_bytes()).hexdigest()},indent=2))
assert r.returncode==0
result=json.loads((dest/'verify_armv7l/result.json').read_text()); commands=[]
for row in result['rpms']:
    cmd=['rpm','-qp','--qf','%{PAYLOADCOMPRESSOR}|%{PAYLOADFLAGS}',row['path']]
    r=subprocess.run(cmd,capture_output=True,text=True)
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (dest/'compression_commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0 and r.stdout=='xz|5T1'
written=re.findall(r'^Wrote: (\S+\.rpm)$',(cell/'build.log').read_text(errors='replace'),re.M)
assert len(written)==len(result['rpms'])
assert {Path(x).name for x in written}=={Path(x['path']).name for x in result['rpms']}
prior=json.loads((p/'progress/BUILD_STATIC_0917B/resume_0917/verify_armv7l/result.json').read_text())
result.update(status='COMPLETE_RPM_BUILD_AND_PAYLOAD_VERIFIED',full_command_exitcode=0,
              wrote_records=len(written),previous_input_status='USABLE_BUT_BUILD_INCOMPLETE',
              libclang_archive_same_as_previous_verified_input=result['libclang_archive_sha256']==prior['libclang_archive_sha256'],
              scope='Incremental complete rpmbuild -bb using the approved prepared tree; not a clean-room rebuild; no board validation implied.')
(dest/'result.json').write_text(json.dumps(result,indent=2))
print(result['status'],len(result['rpms']),flush=True)
