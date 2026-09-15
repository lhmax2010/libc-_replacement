import json,shlex,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917')
old=shlex.split((p/'raw/192_consumer_source_search.command.txt').read_text())
directories=old[old.index('codes/R104/bcc-tools/packaging'):]
cmd=['rg','--files','-g','*.spec','-g','*.cmake','-g','*.sh','-g','*.md']+directories
r=subprocess.run(cmd,capture_output=True,text=True)
expected={s.split('\t')[0] for s in (p/'w4_searched_files.tsv').read_text().splitlines()[1:]}
seen=set(r.stdout.splitlines())
record=dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,expected_files=len(expected),enumerated_files=len(seen),missing=sorted(expected-seen),extra=sorted(seen-expected))
(p/'w4_search_scope_check.json').write_text(json.dumps(record,indent=2)); print(json.dumps(record,indent=2))
assert r.returncode==0 and seen==expected
