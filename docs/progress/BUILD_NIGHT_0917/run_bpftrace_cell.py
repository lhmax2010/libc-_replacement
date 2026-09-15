import datetime,json,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); arch,action=sys.argv[1:]; assert action in ['configure','build']
cmd=json.loads((p/f'bpftrace-{arch}-commands.json').read_text())[action]
start=datetime.datetime.fromisoformat((p/'raw/126_w3_resource_gate.started.txt').read_text().strip()).timestamp()
raise SystemExit(subprocess.call(['python3',str(p/'build_monitor.py'),'--label',f'bpftrace-{arch}-static-{action}','--deadline',str(start+4*3600),'--']+cmd))
