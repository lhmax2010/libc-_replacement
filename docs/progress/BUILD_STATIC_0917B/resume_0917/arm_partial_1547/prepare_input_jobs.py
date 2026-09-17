import hashlib
import json
from pathlib import Path

out=Path('progress/BUILD_STATIC_0917B/resume_0917')
for arch in ('armv7l','aarch64'):
    for label,script in [('copy-inputs','prepare_bpf_private.py'),('install-inputs','install_bpf_inputs.py')]:
        cmd=['python3',str(out/script),arch]
        f=out/'cell_commands'/f'{label}-{arch}.json'
        assert not f.exists()
        f.write_text(json.dumps(cmd,indent=2))
snapshot=out/'cell_guard.executed_v1.py'
assert not snapshot.exists()
snapshot.write_bytes((out/'cell_guard.py').read_bytes())
print('cell_guard_snapshot_sha256',hashlib.sha256(snapshot.read_bytes()).hexdigest())
