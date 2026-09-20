import json,subprocess,sys
from pathlib import Path
cmd=json.loads(Path('progress/BUILD_STATIC_0917B/native_rpm_resume_command.json').read_text())
cmd=cmd[:cmd.index('--')+1]
cmd=['--ro-bind' if s=='--bind' else s for s in cmd]
assert sys.argv[1]=='rpm'
cmd+=sys.argv[1:]
print('ACTUAL_COMMAND_JSON='+json.dumps(cmd),flush=True)
raise SystemExit(subprocess.run(cmd).returncode)
