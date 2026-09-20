"""Read-only query in the exact ARM build root; never run build stages."""
import json,subprocess,sys
from pathlib import Path
p=Path.cwd()
cmd=json.loads((p/'progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json').read_text())
cmd=cmd[cmd.index('bwrap'):cmd.index('--')+1]
cmd=['--ro-bind' if v=='--bind' else v for v in cmd]
allowed={'rpm','rpmbuild','ls','stat','cat','readelf','sh'}
assert sys.argv[1] in allowed
cmd+=sys.argv[1:]
print('ACTUAL_COMMAND_JSON='+json.dumps(cmd),flush=True)
raise SystemExit(subprocess.run(cmd).returncode)
