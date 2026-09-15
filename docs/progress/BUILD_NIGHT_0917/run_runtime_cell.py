import datetime,json,subprocess,sys
from pathlib import Path
out=Path('progress/BUILD_NIGHT_0917'); cell=sys.argv[1]
deadline=datetime.datetime.fromisoformat((out/'raw/001_resource_gate.started.txt').read_text().strip()).timestamp()+3*3600
cmd=json.loads((out/'runtime_commands.json').read_text())[cell]
raise SystemExit(subprocess.call(['python3','-u',str(out/'build_monitor.py'),'--label','runtime-'+cell,'--deadline',str(deadline),'--',*cmd]))
