import hashlib
import json
import re
import shlex
import subprocess
from pathlib import Path
p=Path('progress/BUILD_W2C_0915/static_origin')
j=(p/'final_commands.jsonl').open('w',buffering=1)
def run(args,stdin=None):
    r=subprocess.run(args,input=stdin,capture_output=True,text=True)
    j.write(json.dumps(dict(command=shlex.join(args),stdin=stdin,exitcode=r.returncode,
        stdout=r.stdout,stderr=r.stderr),ensure_ascii=False)+'\n')
    r.check_returncode();return r.stdout
fixture='_ZNSt3__112basic_string\n_ZNSt7__cxx1112basic_string\nClang 21.1.1\nLLVM version 21.1.1\n'
actual=run(['strings','-a'],fixture)
assert all(token in actual for token in ['_ZNSt3__1','_ZNSt7__cxx11','Clang 21.1.1','LLVM version 21.1.1'])
s=json.loads((p/'summary.json').read_text())
assert len(s['images'])==6 and sum(x['entries'] for x in s['images'])==4399
assert all(x['bpftrace_static']==x['bpftrace']==x['qemu_accel_family']==0 for x in s['images'])
assert s['ref_tip_hits']==[] and s['unique_ref_tips']==97
payloads={}
for label in ['011_qemu_installed_payload_correct_db','016_qemu_aarch64_payload']:
    lines=(p/f'raw/{label}.stdout.txt').read_text().splitlines()
    assert not any(re.search('bpftrace|libbpf|bcc',line,re.I) for line in lines)
    payloads[label]=len(lines)
assert run(['git','-C','codes/qemu-accel','rev-parse','HEAD']).strip()==s['qemu_revision']
assert not run(['git','-C','codes/qemu-accel','status','--porcelain']).strip()
assert run(['git','-C','codes/R104/bpftrace','rev-parse','HEAD']).strip()=='30e51cd665360f85b665308dc13ba27df0c5f739'
assert run(['git','-C','codes/R104/bpftrace','diff','--name-only']).strip()=='packaging/bpftrace.spec'
assert Path('codes/R104/bpftrace/packaging/bpftrace.spec').read_bytes()==Path('progress/BUILD_W2C_0915/bpftrace_candidate.spec').read_bytes()
assert not run(['git','-C','codes/R104/bpftrace','diff','--cached','--name-only']).strip()
hashes=[]
for path in sorted(p.glob('*.py'))+sorted(p.glob('*.sh')):
    hashes.append(dict(file=path.name,sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
result=dict(status='PASS',strings_positive_control='4/4',image_positive_control='4 cases PASS',
    six_manifest_entries=4399,qemu_payload_records=payloads,bpftrace_candidate_unchanged=True,
    qemu_source_clean=True,package_push_performed=False,scripts=hashes)
(p/'final_check.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False))
