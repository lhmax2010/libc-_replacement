import json
from pathlib import Path
p=Path('progress/BUILD_WEEKEND_0918/cell_commands')
cmd=json.loads((p/'bpf-aarch64-libcxx.json').read_text())
cmd[cmd.index('--uid')+1]='1000'; cmd[cmd.index('--gid')+1]='1000'
(p/'bpf-aarch64-libcxx-user.json').write_text(json.dumps(cmd,indent=2))
