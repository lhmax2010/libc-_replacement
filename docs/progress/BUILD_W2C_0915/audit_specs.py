import csv
import hashlib
import json
import subprocess
from pathlib import Path

base = Path('/home/toolchain/development/libc++_replacement')
out = base/'progress/BUILD_W2C_0915/spec_audit_before'
out.mkdir(exist_ok=False)
with (base/'docs/progress/BUILD_W2B_0915/spec_audit/inputs.tsv').open() as f:
    rows = list(csv.DictReader(f, delimiter='\t'))
records = []
for row in rows:
    name = row['package']
    repo = base/('tmp/BUILD_W1BC_0911/target-fetch' if name == 'llvm' else f'codes/R104/{name}')
    if name == 'bcc-tools':
        content = (repo/row['spec']).read_bytes()
    else:
        command = ['git', '-C', str(repo), 'show', row['revision']+':'+row['spec']]
        r = subprocess.run(command, capture_output=True)
        (out/(name+'.command.json')).write_text(json.dumps(command)+'\n')
        (out/(name+'.exitcode')).write_text(str(r.returncode)+'\n')
        (out/(name+'.stderr.txt')).write_bytes(r.stderr)
        if r.returncode:
            print(name, 'NOT_AVAILABLE', flush=True)
            raise SystemExit(r.returncode)
        content = r.stdout
    (out/(name+'.spec')).write_bytes(content)
    digest = hashlib.sha256(content).hexdigest()
    assert digest == row['sha256'], (name, digest, row['sha256'])
    lines = content.decode().splitlines()
    markers = [(n, line) for n, line in enumerate(lines, 1)
               if any(token in line for token in ('_toolchain', 'build_with_libcxx', '-stdlib=libc++', '-lc++abi', 'LLVM_ENABLE_LIBCXX', 'USE_CLANG'))]
    records.append(dict(package=name, revision=row['revision'], sha256=digest,
                        matches_previous_snapshot=True, relevant_lines=markers))
    print(name, 'IDENTICAL_TO_PREVIOUS_AUDITED_SPEC', digest, flush=True)
(out/'comparison.json').write_text(json.dumps(records, ensure_ascii=False, indent=2)+'\n')
with (out/'inputs.tsv').open('w') as f:
    w = csv.DictWriter(f, delimiter='\t', fieldnames=rows[0].keys())
    w.writeheader()
    w.writerows(rows)
