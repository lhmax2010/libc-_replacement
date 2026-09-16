import datetime
import json
import subprocess
import sys
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
start=datetime.datetime.fromisoformat((p/'raw/037_sudo_copy.started.txt').read_text().strip()).timestamp()
cmd=json.loads((p/'llvm_full_rpmbuild_command.json').read_text())
raise SystemExit(subprocess.call([sys.executable,str(p/'build_monitor.py'),'--label','llvm-aarch64-full-rpm-retry','--deadline',str(start+4*3600),'--',*cmd]))
