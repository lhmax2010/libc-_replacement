import hashlib,json
from pathlib import Path
p=Path('progress/BUILD_WEEKEND_0918/cell_commands')
cmd=json.loads((p/'bpf-aarch64-libcxx-user.json').read_text())
(p/'bpf-aarch64-libcxx-pathfixed.json').write_text(json.dumps(cmd,indent=2))
spec=Path('tmp/WEEKEND_0918/specs/bpftrace.spec')
snapshot=Path('progress/BUILD_WEEKEND_0918/bpftrace-pathfixed.spec')
snapshot.write_bytes(spec.read_bytes())
print('Spec SHA256',hashlib.sha256(spec.read_bytes()).hexdigest())
