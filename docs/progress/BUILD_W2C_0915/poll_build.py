import datetime
import json
import shlex
import subprocess
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
with (p/'monitor_reads.jsonl').open('a') as record:
    for name in ('build-w2c-llvm-confirmed.build.log', 'build-w2c-llvm-confirmed.events.jsonl'):
        command = ['tail', '-n', '1', str(p/name)]
        r = subprocess.run(command, capture_output=True, text=True)
        row = dict(time=datetime.datetime.now().astimezone().isoformat(),
                   command=shlex.join(command), exitcode=r.returncode,
                   stdout=r.stdout, stderr=r.stderr)
        record.write(json.dumps(row, ensure_ascii=False)+'\n')
        if name.endswith('.events.jsonl') and r.returncode == 0:
            item = json.loads(r.stdout)
            if item['event'] == 'scope_processes':
                print('scope process snapshot recorded')
            else:
                print(r.stdout.strip())
        else:
            print(r.stdout.strip())
        if r.returncode:
            raise SystemExit(r.returncode)
