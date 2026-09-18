"""Preflight the test harness without attempting unbuilt RPM file lists."""
import hashlib,json,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918'); task=Path('tmp/WEEKEND_0918')
wrapper=task/'specs/bpftrace-undefined-harness.spec'
assert wrapper.read_bytes()==(out/'bpftrace-undefined-harness.spec').read_bytes()
records=[]
for arch,old in [('armv7l','bpf-armv7l-undefined-linkerfixed'),('aarch64','bpf-aarch64-undefined')]:
    cmd=json.loads((out/'cell_commands'/(old+'.json')).read_text())
    assert cmd[-1]=='/tmp/task/specs/bpftrace.spec'
    cmd[-1]='/tmp/task/specs/bpftrace-undefined-harness.spec'
    label='bpf-'+arch+'-undefined-harness'
    (out/'cell_commands'/(label+'.json')).write_text(json.dumps(cmd,indent=2))
    probe=[('-bp' if x=='-bb' else x) for x in cmd[:-1]]+['--nobuild',cmd[-1]]
    r=subprocess.run(probe,capture_output=True,text=True)
    (out/(label+'-nobuild-prep.stdout')).write_text(r.stdout)
    (out/(label+'-nobuild-prep.stderr')).write_text(r.stderr)
    record={'arch':arch,'command':shlex.join(probe),'exitcode':r.returncode,
            'harness_sha256':hashlib.sha256(wrapper.read_bytes()).hexdigest(),
            'candidate_sha256':hashlib.sha256((task/'specs/bpftrace.spec').read_bytes()).hexdigest()}
    records.append(record); (out/'undefined-harness-preflight-v2.json').write_text(json.dumps(records,indent=2))
    print(arch,r.returncode,r.stdout,r.stderr,flush=True)
    assert r.returncode==0 and 'HARNESS_AFTER_UNDEFINE=0|' in r.stdout
    assert 'HARNESS_AFTER_INCLUDE=0||' in r.stdout
    check=['python3',str(out/'check_bpf_cell.py'),arch,'undefined',label]
    (out/'cell_commands'/('check-'+arch+'-undefined.json')).write_text(json.dumps(check,indent=2))
