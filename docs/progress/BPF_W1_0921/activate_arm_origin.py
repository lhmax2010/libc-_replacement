"""Bind only the successfully installed fresh private tree into the existing ARM command."""
import json,hashlib
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; task=p/'tmp/BPF_W1_0921'
prepared=json.loads((out/'prepare-armv7l-result.json').read_text())
assert prepared['status']=='PASS' and not prepared['replacefiles_used']
new=Path(prepared['input_root']); old=task/'input-armv7l-v3'
assert new==task/'input-armv7l-origin'
f=out/'cell_commands/bpf-armv7l-libcxx.json'; old_bytes=f.read_bytes(); argv=json.loads(old_bytes)
assert not (out/'cells/bpf-armv7l-libcxx').exists()
changed=[]
for part,guest in (('usr','/usr'),('rpmdb','/var/lib/rpm'),('etc','/etc'),('root','/root')):
    matches=[i for i in range(len(argv)-2) if argv[i:i+3]==['--bind',str(old/part),guest]]
    assert len(matches)==1,(part,matches)
    i=matches[0]+1; before=argv[i]; argv[i]=str(new/part)
    changed.append(dict(index=i,before=before,after=argv[i]))
backup=out/'cell_commands/bpf-armv7l-libcxx-before-origin.json'
assert not backup.exists(); backup.write_bytes(old_bytes)
f.write_text(json.dumps(argv,indent=2))
(out/'origin-command-diff.json').write_text(json.dumps(dict(changes=changed,other_arguments_identical=True,before_sha256=hashlib.sha256(old_bytes).hexdigest(),after_sha256=hashlib.sha256(f.read_bytes()).hexdigest()),indent=2))
pre=out/'payload-precheck-armv7l.json'
if pre.exists():
    backup=out/'payload-precheck-armv7l-before-origin.json'; assert not backup.exists(); backup.write_bytes(pre.read_bytes())
print('Only four private input bindings changed; spec and payload arguments unchanged.')
