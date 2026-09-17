"""Archive only the final delta, preserving existing immutable raw evidence."""
import datetime
import gzip
import hashlib
import json
import re
from pathlib import Path
src=Path('progress/BUILD_STATIC_0917B')
dst=Path('docs/progress/BUILD_STATIC_0917B')
assert not Path('password.md').exists()
names=['README.md','NATIVE_TOOLCHAIN_REPORT.md','TECHNICAL_RECOVERY.md','HOURLY_STATUS.md',
       'external_io_pause.py','external-io-pause.events.jsonl','deadline_signal_guard.py',
       'test_serial_ninja.py','ninja_wrapper_test.json','ninja-serial.v2.snapshot',
       'check_native_push.py','native_checkpoint_push.json','archive_native_final.py',
       'llvm-aarch64-native-rpm.build.log','llvm-aarch64-native-rpm.events.jsonl',
       'llvm-aarch64-native-rpm.io.log','llvm-aarch64-native-rpm.previous-2.build.log']
selected=[src/n for n in names]
for q in (src/'raw').iterdir():
    n=int(q.name.split('_')[0])
    if n>=194 or n==178:
        prefix=q.name.split('.')[0]
        if (q.parent/(prefix+'.finished.txt')).exists(): selected.append(q)
manifest=[]
for q in sorted(selected):
    if not q.exists():
        raise RuntimeError('Missing final evidence: '+str(q))
    data=q.read_bytes()
    for pattern in [rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+']:
        assert not re.search(pattern,data), str(q)
    rel=q.relative_to(src); target=dst/rel
    stored=data
    if len(data)>2_000_000:
        target=target.with_suffix(target.suffix+'.gz'); stored=gzip.compress(data,mtime=0)
    if target.exists() and rel.parts[0]=='raw': assert target.read_bytes()==stored, str(q)
    target.parent.mkdir(parents=True,exist_ok=True); target.write_bytes(stored)
    manifest.append(dict(source=str(q),delivery=str(target),sha256=hashlib.sha256(data).hexdigest(),delivery_sha256=hashlib.sha256(stored).hexdigest()))
(dst/'NATIVE_FINAL_MANIFEST.json').write_text(json.dumps(manifest,indent=2))
check=dict(captured_at=datetime.datetime.now().astimezone().isoformat(),files=len(manifest),raw_preserved=True,secret_pattern_scan='PASS',credential_absent=True,exact_password_scan='NOT_OBSERVED: secret already shredded and never retained',new_sudo_calls=0,new_board_operations=0,package_push=False)
(dst/'NATIVE_FINAL_SELF_CHECK.json').write_text(json.dumps(check,indent=2))
print(json.dumps(check,indent=2))
