"""Preserve the fully written but nonconforming RPM set; no acceptance override."""
import hashlib,json,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; top=p/'tmp/WEEKEND_0918/rpm-bpf-aarch64-libcxx'
rows=[]; commands=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'aarch64-first-rpm-set-commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0
    return r.stdout
for f in sorted((top/'RPMS').rglob('*.rpm')):
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
    run(['rpm','--checksig','--nosignature',str(f)])
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    rows.append({'path':str(f),'identity':identity,'sha256':sha,'bytes':f.stat().st_size})
binary=p/'tmp/WEEKEND_0918/payload-bpf-aarch64-libcxx/bpftrace-static/usr/bin/bpftrace-static'
dynamic=run(['readelf','-dW',str(binary)])
needed=[line.split('[')[1].split(']')[0] for line in dynamic.splitlines() if '(NEEDED)' in line]
assert 'libbpf.so.1' in needed
result={'arch':'aarch64','mode':'libcxx','rpmbuild_exitcode':0,'rpm_set':rows,'static_needed':needed,
        'status':'BUILD_COMPLETE_PAYLOAD_GATE_FAIL_EXTRA_DYNAMIC_LIBBPF',
        'candidate_spec_sha256':'36269f8316976ba9dffae1696294c3698a8d367a97eac8ee1355e08733b8a550',
        'failure_record':'cells/check-aarch64-libcxx/build.log','configuration_audit':'audit-bpf-aarch64-libcxx/result.json',
        'checker_scope':'Both original checker scripts returned zero against actual spec; app-rootstrap output explicitly skipped its two rule files. This does not prove those skipped rules were enforced.',
        'proposed_correction':'Awaiting human confirmation; static CMake branch bare bpf -> ${LIBBPF_LIBRARIES}; no C++ change.'}
(out/'aarch64-first-rpm-set-result.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result,indent=2))
