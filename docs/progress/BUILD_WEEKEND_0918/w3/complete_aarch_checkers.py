"""Restore omitted original read-only packaging checkers; keep all checks enabled."""
import hashlib,json
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
source=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0'
app=source/'etc/app-rootstrap'; assert app.is_dir()
(out/'app-rootstrap-aarch64-input-identities.json').write_text(json.dumps([
 {'path':str(f),'sha256':hashlib.sha256(f.read_bytes()).hexdigest()}
 for f in sorted(app.rglob('*')) if f.is_file() and not f.is_symlink()],indent=2))
cmd=json.loads((out/'cell_commands/bpf-aarch64-libcxx-hal.json').read_text())
i=cmd.index('--'); cmd[i:i]=['--ro-bind',str(app),'/etc/app-rootstrap']
(out/'cell_commands/bpf-aarch64-libcxx-checkers.json').write_text(json.dumps(cmd,indent=2))
base=json.loads((out/'bpf-private-aarch64-hal-base.json').read_text())
base+=['--ro-bind',str(app),'/etc/app-rootstrap']
(out/'bpf-private-aarch64-checkers-base.json').write_text(json.dumps(base,indent=2))
print('Original app-rootstrap directory mounted read-only; no checker disabled; full -bb retained.')
