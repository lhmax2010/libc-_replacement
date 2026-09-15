"""Enumerate literal spec option contexts; this is not an RPM macro evaluator."""
import csv
import hashlib
import json
import re
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
def scan(text):
    stack, options, definitions = [], [], []
    for number, raw in enumerate(text.splitlines(),1):
        line = raw.strip()
        if line.startswith('#'):
            continue
        if re.match(r'%if(?:arch|narch|os|nos)?\s',line):
            stack.append([line, True])
        elif line.startswith('%else'):
            assert stack
            stack[-1][1] = False
        elif line.startswith('%elif '):
            assert stack
            stack[-1] = [line, True]
        elif line.startswith('%endif'):
            assert stack
            stack.pop()
        context = [condition+(' [else]' if not positive else '') for condition,positive in stack]
        if re.match(r'%define\s+build_with_libcxx\s+1\s*$',line):
            defined = any(positive and condition=='%if %{defined _toolchain}' for condition,positive in stack)
            clang = any(positive and condition=='%if %{toolchain_is clang}' for condition,positive in stack)
            definitions.append(dict(line=number, text=raw, context=context, valid=defined and clang))
        if re.search(r'-stdlib=libc\+\+|-lc\+\+abi|LLVM_ENABLE_LIBCXX=ON|^(?:BuildRequires|Requires):.*libc\+\+',line):
            guarded = any(positive and condition=='%if 0%{?build_with_libcxx}' for condition,positive in stack)
            options.append(dict(line=number, text=raw, context=context, guarded=guarded))
    assert not stack, 'Unbalanced spec conditionals'
    return definitions,options

valid = '%if %{defined _toolchain}\n%if %{toolchain_is clang}\n%define build_with_libcxx 1\n%endif\n%endif\n%if 0%{?build_with_libcxx}\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n'
d,o=scan(valid)
assert d[0]['valid'] and o[0]['guarded']
assert not scan('export CXXFLAGS="-stdlib=libc++"\n')[1][0]['guarded']
assert not scan('%if 0%{?build_with_libcxx}\n%else\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n')[1][0]['guarded']
assert not scan('%if %{defined _toolchain}\n%define build_with_libcxx 1\n%endif\n')[0][0]['valid']

rows = list(csv.DictReader((p/'spec_audit_before/inputs.tsv').open(), delimiter='\t'))
records=[]
for row in rows:
    name=row['package']
    path = Path(f'codes/R104/{name}/packaging/{name}.spec') if name in ['bcc-tools','bpftrace'] else p/f'spec_audit_before/{name}.spec'
    data=path.read_bytes()
    definitions, options=scan(data.decode())
    assert len(definitions)==1 and definitions[0]['valid'], name
    assert options and all(item['guarded'] for item in options), name
    records.append(dict(package=name, path=str(path), sha256=hashlib.sha256(data).hexdigest(),
                        definitions=definitions, options=options))
result=dict(status='LITERAL_OPTION_CONTEXTS_PASS', calibration_cases=4, packages=len(records),
    scope='Literal newly added standard-library option/dependency contexts only; not RPM macro evaluation or proof against arbitrary external flag injection.',
    records=records)
(p/'guard_context_audit.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(f'LITERAL_OPTION_CONTEXTS_PASS packages={len(records)} calibration_cases=4')
