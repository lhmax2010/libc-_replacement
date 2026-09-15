import datetime,json,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); mode,action=sys.argv[1:]; assert mode in ['gcc','undefined']; assert action in ['configure','build','install']
cmd=json.loads((p/'bpftrace_fallback_commands.json').read_text())[mode][action]
start=datetime.datetime.fromisoformat((p/'raw/126_w3_resource_gate.started.txt').read_text().strip()).timestamp()
raise SystemExit(subprocess.call(['python3',str(p/'build_monitor.py'),'--label',f'bpftrace-aarch64-{mode}-{action}','--deadline',str(start+4*3600),'--']+cmd))
