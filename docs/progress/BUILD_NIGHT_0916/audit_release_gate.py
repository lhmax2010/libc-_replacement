"""Re-read eleven pinned specs and the already completed bpftrace four-cell evidence."""
import ast
import csv
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess

out = Path('progress/BUILD_NIGHT_0916')
prior = Path('progress/BUILD_W2C_0915')
(out/'spec_audit').mkdir(exist_ok=True)
log = (out/'audit_commands.jsonl').open('w')
def run(args):
    r = subprocess.run(args, capture_output=True)
    log.write(json.dumps(dict(command=shlex.join(args), exitcode=r.returncode,
                             stdout=r.stdout.decode(), stderr=r.stderr.decode()))+'\n')
    assert r.returncode == 0, (args, r.returncode)
    return r.stdout

# Reuse just the inspected pure scan function, not the old script's output-writing main.
scanner_source = (prior/'audit_guard_context.py').read_text()
function = next(n for n in ast.parse(scanner_source).body if isinstance(n, ast.FunctionDef) and n.name == 'scan')
namespace = {'re': re}
exec(compile(ast.Module(body=[function], type_ignores=[]), str(prior/'audit_guard_context.py'), 'exec'), namespace)
scan = namespace['scan']
good = '%if %{defined _toolchain}\n%if %{toolchain_is clang}\n%define build_with_libcxx 1\n%endif\n%endif\n%if 0%{?build_with_libcxx}\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n'
assert scan(good)[0][0]['valid'] and scan(good)[1][0]['guarded']
assert not scan('export CXXFLAGS="-stdlib=libc++"\n')[1][0]['guarded']
assert not scan('%if 0%{?build_with_libcxx}\n%else\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n')[1][0]['guarded']
assert not scan('%if %{defined _toolchain}\n%define build_with_libcxx 1\n%endif\n')[0][0]['valid']
status = list(csv.DictReader((prior/'PACKAGE_STATUS.tsv').open(), delimiter='\t'))
inputs = {r['package']: r for r in csv.DictReader((prior/'spec_audit_before/inputs.tsv').open(), delimiter='\t')}
records = []
for package in status:
    name = package['package']
    entry = inputs[name]
    if name == 'bpftrace':
        data = Path('codes/R104/bpftrace/packaging/bpftrace.spec').read_bytes()
        revision = '30e51cd665360f85b665308dc13ba27df0c5f739 + previously tested 15-line candidate'
        expected = 'f471f98b0d48dd769de286fbeaa8d7292230fbf316c95b06e03e2b631a86cdd3'
        assert data == (prior/'bpftrace_candidate.spec').read_bytes()
    else:
        repo = Path('tmp/BUILD_W1BC_0911/target-fetch') if name == 'llvm' else Path('codes/R104')/name
        revision = package['adaptation_commit']
        data = run(['git', '-C', str(repo), 'show', revision+':'+entry['spec']])
        expected = entry['sha256']
    digest = hashlib.sha256(data).hexdigest()
    assert digest == expected, (name, digest, expected)
    definitions, options = scan(data.decode())
    assert len(definitions) == 1 and definitions[0]['valid'], name
    assert options and all(x['guarded'] for x in options), name
    (out/'spec_audit'/f'{name}.spec').write_bytes(data)
    records.append(dict(package=name, revision=revision, sha256=digest,
                        definitions=definitions, options=options,
                        status='LITERAL_GUARDS_PASS', gcc_evidence=package['validation']))

matrix_data = (prior/'bpftrace_source_matrix.json').read_bytes()
matrix = json.loads(matrix_data)
assert matrix['source_cells'] == 4 and matrix['source_cells_status'] == 'PASS'
assert {(c['arch'],c['mode']) for c in matrix['cells']} == {(a,m) for a in ('armv7l','aarch64') for m in ('libcxx','gcc')}
for cell in matrix['cells']:
    assert cell['status'] == 'SOURCE_PROGRAMS_AND_PROVIDERS_PASS'
    for program in cell['source_programs']:
        required = 'libc++.so.1' if cell['mode'] == 'libcxx' else 'libstdc++.so.6'
        forbidden = 'libstdc++.so.6' if cell['mode'] == 'libcxx' else 'libc++.so.1'
        assert required in program['needed'] and forbidden not in program['needed']
    if cell['mode'] == 'gcc':
        raw = (prior/f"build-w2c-bpftrace-{cell['arch']}-gcc.build.log").read_text(errors='replace')
        for token in ('-stdlib=libc++','-lc++abi'):
            assert token not in raw, (cell['arch'],token)
    assert cell['prebuilt']['byte_identical']
result = dict(status='PASS', packages=records, calibration_cases=4,
              source_matrix_path=str(prior/'bpftrace_source_matrix.json'),
              source_matrix_sha256=hashlib.sha256(matrix_data).hexdigest(),
              source_cells=4, new_builds=0, static_scope='separately accepted by user for push; ABI still NOT_OBSERVED',
              scope='Pinned spec literal guard checks plus prior real GCC build evidence; not arbitrary external flag overrides.')
(out/'release_gate.json').write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
print('PASS: 11 pinned specs, 4 scanner calibration cases, 4 prior bpftrace cells; both GCC logs contain zero forbidden flags.')
