"""Keep full reads in the journal, print a concise progress line for verbose builds."""
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
    for suffix, count in [('.build.log',20),('.events.jsonl',1)]:
        command = ['tail','-n',str(count),str(p/(unit+suffix))]
        result = subprocess.run(command, capture_output=True, text=True)
        record.write(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),
            command=shlex.join(command), exitcode=result.returncode,
            stdout=result.stdout, stderr=result.stderr), ensure_ascii=False)+'\n')
        result.check_returncode()
        if suffix == '.events.jsonl':
            event = json.loads(result.stdout)
            print('scope process snapshot recorded' if event['event']=='scope_processes' else result.stdout.strip())
        else:
            lines = result.stdout.splitlines()
            targets = [line for line in lines if re.search(r'\[\s*\d+%\]', line)]
            line = targets[-1] if targets else (lines[-1] if lines else '(empty log)')
            print(line if len(line)<=300 else line[:300]+' [display shortened; full line retained in log]')
