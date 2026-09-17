import json
import subprocess
from pathlib import Path
p=Path('progress/BUILD_STATIC_0917B')
cmd=json.loads((p/'native_rpm_serial_command.json').read_text())
base=cmd[:cmd.index('--')]
script='cd /tmp/task/ninja-wrapper-test; /bin/ninja --version; /bin/ninja -t list; /bin/ninja -t restat; /bin/ninja -j1; set +e; /bin/ninja -j2; rc=$?; printf "forbidden_jobs_exitcode=%s\\n" "$rc"; test "$rc" = 64'
r=subprocess.run(base+['--','/bin/sh','-ec',script],text=True,capture_output=True)
(p/'ninja_wrapper_test.json').write_text(json.dumps(dict(command=base+['--','/bin/sh','-ec',script],exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2))
print(r.stdout,end=''); print(r.stderr,end='')
raise SystemExit(r.returncode)
