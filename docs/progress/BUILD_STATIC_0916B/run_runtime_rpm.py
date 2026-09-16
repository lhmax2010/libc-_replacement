import datetime,json,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_STATIC_0916B')
start=datetime.datetime.fromisoformat((p/'raw/023_w2_gate.started.txt').read_text().strip()).timestamp()
cmd=json.loads((p/'runtime_rpmbuild_command.json').read_text())
cmd[-1:-1]=['--define','_tmppath /tmp/night/finish_validation/rpm']
raise SystemExit(subprocess.call([sys.executable,str(p/'build_monitor.py'),'--label','runtime-aarch64-rpm','--deadline',str(start+4*3600),'--',*cmd]))
