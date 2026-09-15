import datetime
import json
import re
import shlex
import subprocess
import sys
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
unit = sys.argv[1]
assert re.fullmatch(r'build-w2c-(bcc-tools|bpftrace)-(armv7l|aarch64)-(libcxx|gcc)', unit)
with (p/'cell_monitor_reads.jsonl').open('a') as record:
    for suffix in ('.build.log', '.events.jsonl'):
        command = ['tail', '-n', '1', str(p/(unit+suffix))]
        result = subprocess.run(command, capture_output=True, text=True)
        record.write(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),
            command=shlex.join(command), exitcode=result.returncode,
            stdout=result.stdout, stderr=result.stderr), ensure_ascii=False)+'\n')
        result.check_returncode()
        if suffix == '.events.jsonl' and json.loads(result.stdout)['event'] == 'scope_processes':
            print('scope process snapshot recorded')
        else:
            print(result.stdout.strip())
