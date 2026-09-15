import datetime,json,subprocess,sys
from pathlib import Path
out=Path('progress/BUILD_NIGHT_0917'); cell,action=sys.argv[1:]
deadline=datetime.datetime.fromisoformat((out/'raw/043_w2_resource_gate.started.txt').read_text().strip()).timestamp()+5*3600
cmd=json.loads((out/'libclang_commands.json').read_text())[cell][action]
raise SystemExit(subprocess.call(['python3','-u',str(out/'build_monitor.py'),'--label','libclang-'+cell+'-'+action,'--deadline',str(deadline),'--',*cmd]))
