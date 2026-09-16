import json
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B/llvm_full_rpmbuild_command.json')
cmd=json.loads(p.read_text())
for flag in ['--uid','--gid']:
    i=cmd.index(flag)+1
    assert cmd[i]=='0'
    cmd[i]='1000'
p.write_text(json.dumps(cmd,indent=2))
print('rpmbuild runs as namespace uid/gid 1000, not root; no archive owner restoration required.')
