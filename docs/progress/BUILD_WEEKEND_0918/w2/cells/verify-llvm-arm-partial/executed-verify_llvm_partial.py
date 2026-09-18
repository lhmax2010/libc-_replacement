"""Check surviving RPM payloads without upgrading the failed whole build."""
import hashlib,json,re,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
events=[json.loads(x) for x in (out/'cells/llvm-arm-whole-retry/events.jsonl').read_text().splitlines()]
finished=[x for x in events if x.get('event')=='finished']
assert len(finished)==1 and finished[0]['exitcode']==1
source=p/'progress/BUILD_STATIC_0917B/resume_0917/verify_llvm_rpms.executed.py'
dest=out/'llvm_retry_partial'; dest.mkdir(exist_ok=False)
data=source.read_text().replace('progress/BUILD_STATIC_0917B/resume_0917','progress/BUILD_WEEKEND_0918/llvm_retry_partial').replace('tmp/STATIC_0917B','tmp/WEEKEND_0918')
snapshot=dest/'payload_verifier.py'; snapshot.write_text(data)
command=['python3',str(snapshot),'armv7l']
r=subprocess.run(command)
(dest/'verification_command.json').write_text(json.dumps({'command':shlex.join(command),'exitcode':r.returncode,'original_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'executed_sha256':hashlib.sha256(snapshot.read_bytes()).hexdigest()},indent=2))
assert r.returncode==0
result=json.loads((dest/'verify_armv7l/result.json').read_text())
written=re.findall(r'^Wrote: (\S+\.rpm)$',(out/'cells/llvm-arm-whole-retry/build.log').read_text(errors='replace'),re.M)
assert len(written)==len(result['rpms'])==18
assert {Path(x).name for x in written}=={Path(x['path']).name for x in result['rpms']}
prior=json.loads((p/'progress/BUILD_STATIC_0917B/resume_0917/verify_armv7l/result.json').read_text())
result.update(status='RPM_PAYLOADS_VERIFIED_BUT_BUILD_INCOMPLETE',whole_build_exitcode=1,wrote_records=len(written),
    libclang_archive_same_as_previous_verified_input=result['libclang_archive_sha256']==prior['libclang_archive_sha256'],
    scope='New retry outputs only. Failed complete build remains failed; no functional or board validation implied; original approved eighteen input RPMs unchanged.')
(dest/'result.json').write_text(json.dumps(result,indent=2))
print(result['status'],len(result['rpms']),flush=True)
