"""Record exact command, output, return code, and timestamps for this task."""
import datetime
import json
import pathlib
import shlex
import subprocess
import sys

root = pathlib.Path(__file__).resolve().parent / 'raw'
root.mkdir(exist_ok=True)
label, *argv = sys.argv[1:]
prefix = root / label
assert not pathlib.Path(str(prefix) + '.command.txt').exists()
def save(ext, value):
    pathlib.Path(str(prefix) + '.' + ext).write_text(value)
save('command.txt', shlex.join(argv) + '\n')
save('argv.json', json.dumps(argv, ensure_ascii=False, indent=2) + '\n')
save('started.txt', datetime.datetime.now(datetime.timezone.utc).isoformat() + '\n')
with pathlib.Path(str(prefix) + '.stdout.txt').open('wb') as out, pathlib.Path(str(prefix) + '.stderr.txt').open('wb') as err:
    result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=out, stderr=err)
save('exitcode', str(result.returncode) + '\n')
save('finished.txt', datetime.datetime.now(datetime.timezone.utc).isoformat() + '\n')
print(label, 'exitcode', result.returncode)
for ext in ('stdout.txt', 'stderr.txt'):
    print(pathlib.Path(str(prefix) + '.' + ext).read_text(errors='replace')[:12000])
sys.exit(result.returncode)
