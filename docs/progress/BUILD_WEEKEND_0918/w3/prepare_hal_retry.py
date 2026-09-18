import hashlib,json,subprocess,shlex
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
root=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0'
source=root/'etc/hal'; assert source.is_dir()
script=source/'rootstrap/hal-rootstrap-checker.sh'; assert script.is_file()
records=[]
for f in sorted(source.rglob('*')):
    if f.is_symlink(): records.append({'path':str(f),'link':str(f.readlink())})
    elif f.is_file(): records.append({'path':str(f),'sha256':hashlib.sha256(f.read_bytes()).hexdigest()})
(out/'hal-input-identities.json').write_text(json.dumps(records,indent=2))
base=json.loads((out/'bpf-private-aarch64-base.json').read_text())
base+=['--ro-bind',str(source),'/etc/hal']
# Preserve the prior base rather than replace executed input records.
(out/'bpf-private-aarch64-hal-base.json').write_text(json.dumps(base,indent=2))
cmd=json.loads((out/'cell_commands/bpf-aarch64-libcxx-user.json').read_text())
pos=cmd.index('--'); cmd[pos:pos]=['--ro-bind',str(source),'/etc/hal']
(out/'cell_commands/bpf-aarch64-libcxx-hal.json').write_text(json.dumps(cmd,indent=2))
print('Added read-only HAL input; files',len(records),'no script bypass')
