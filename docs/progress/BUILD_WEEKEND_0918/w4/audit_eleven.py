"""Recheck eleven published spec revisions; no package tree changes."""
import ast,csv,hashlib,json,re,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918'); dest=out/'spec_audit_v2'; dest.mkdir(exist_ok=False)
source=Path('progress/BUILD_W2C_0915/audit_guard_context.py')
(dest/'guard_scanner_source.py').write_bytes(source.read_bytes())
node=next(x for x in ast.parse(source.read_text()).body if isinstance(x,ast.FunctionDef) and x.name=='scan')
namespace={'re':re}; exec(compile(ast.Module(body=[node],type_ignores=[]),str(source),'exec'),namespace)
scan=namespace['scan']
good='%if %{defined _toolchain}\n%if %{toolchain_is clang}\n%define build_with_libcxx 1\n%endif\n%endif\n%if 0%{?build_with_libcxx}\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n'
assert scan(good)[0][0]['valid'] and scan(good)[1][0]['guarded']
assert not scan('export CXXFLAGS="-stdlib=libc++"\n')[1][0]['guarded']
assert not scan('%if 0%{?build_with_libcxx}\n%else\nexport CXXFLAGS="-stdlib=libc++"\n%endif\n')[1][0]['guarded']
assert not scan('%if %{defined _toolchain}\n%define build_with_libcxx 1\n%endif\n')[0][0]['valid']
packages=list(csv.DictReader(Path('docs/progress/BUILD_NIGHT_0916/PACKAGE_STATUS.tsv').open(),delimiter='\t'))
inputs={r['package']:r for r in csv.DictReader(Path('progress/BUILD_W2C_0915/spec_audit_before/inputs.tsv').open(),delimiter='\t')}
assert len(packages)==11
commands=[]; rows=[]
for package in packages:
    name=package['package']; revision=package['adaptation_commit']
    repo=(Path('tmp/BUILD_W1BC_0911/target-fetch') if name=='llvm' else
          Path('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push') if name=='bpftrace' else Path('codes/R104')/name)
    command=['git','-C',str(repo),'show',revision+':'+inputs[name]['spec']]
    r=subprocess.run(command,capture_output=True)
    commands.append({'command':shlex.join(command),'exitcode':r.returncode,'stderr':r.stderr.decode()})
    (out/'spec-audit-commands-v2.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0
    data=r.stdout; (dest/(name+'.spec')).write_bytes(data)
    definitions,options=scan(data.decode())
    valid=len(definitions)==1 and definitions[0]['valid'] and bool(options) and all(x['guarded'] for x in options)
    rows.append({'package':name,'branch':package['branch'],'revision':revision,'sha256':hashlib.sha256(data).hexdigest(),
                 'definitions':definitions,'options':options,'literal_gate':'PASS' if valid else 'FAIL',
                 'prior_validation':package['validation'],'status':package['current_status']})
    if not valid:
        (out/'spec-audit-failure-v2.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2))
        raise RuntimeError('Published spec guard check failed: '+name)
(out/'spec-audit-result-v2.json').write_text(json.dumps({'status':'PASS','packages':rows,'calibration_cases':4,
    'scanner_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
    'scope':'Literal standard-library option/dependency guards in eleven pinned published specs; not arbitrary external macro injection; historical full builds reused, no claim of eleven new builds.'},ensure_ascii=False,indent=2))
print('PASS eleven published specs; four calibration cases')
