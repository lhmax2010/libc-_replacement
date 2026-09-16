import json
from pathlib import Path
p=Path('progress/BUILD_STATIC_0917B')
cmd=json.loads((p/'native_rpm_resume_command.json').read_text())
root=Path('tmp/STATIC_0917B/llvm-root-minimal').resolve()
wrapper=Path('tmp/STATIC_0917B/ninja-serial').resolve()
wrapper.chmod(0o755)
assert (root/'usr/bin/ninja').is_file()
assert not (p/'native_rpm_serial_command.json').exists()
sep=cmd.index('--')
cmd[sep:sep]=['--ro-bind',str(root/'usr/bin/ninja'),'/usr/bin/ninja.real','--ro-bind',str(wrapper),'/usr/bin/ninja']
(p/'native_rpm_serial_command.json').write_text(json.dumps(cmd,indent=2))
print('Private mount only: all Ninja invocations have trailing -j1; source files and specs unchanged.')
