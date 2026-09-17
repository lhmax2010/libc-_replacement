"""One-shot command recorder. Python compiles this file before starting a child."""
import datetime
import os
import shlex
import subprocess
import sys
from pathlib import Path

os.environ['PATH'] = '/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/home/toolchain/.local/bin:/home/toolchain/.vscode/extensions/openai.chatgpt-26.908.40401-linux-x64/bin/linux-x86_64'
root = Path('/home/toolchain/development/libc++_replacement/progress/BUILD_STATIC_0917B/resume_0917/raw')
label, *command = sys.argv[1:]
assert label and '/' not in label and command
prefix = root / label
assert not prefix.with_suffix('.command.txt').exists()
def write(suffix, value):
    with Path(str(prefix) + suffix).open('x') as f:
        f.write(value + '\n')
write('.command.txt', shlex.join(command))
write('.started.txt', datetime.datetime.now().astimezone().isoformat())
with Path(str(prefix) + '.stdout.txt').open('xb') as stdout, Path(str(prefix) + '.stderr.txt').open('xb') as stderr:
    rc = subprocess.run(command, stdout=stdout, stderr=stderr).returncode
write('.exitcode', str(rc))
write('.finished.txt', datetime.datetime.now().astimezone().isoformat())
print(label, 'exitcode=' + str(rc), flush=True)
sys.exit(rc if rc >= 0 else 128 - rc)
