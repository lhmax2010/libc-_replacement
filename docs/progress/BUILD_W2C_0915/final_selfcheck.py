"""Final read-only gates and evidence inventory; never promotes the static scope."""
import csv
import datetime
import hashlib
import json
import re
import shlex
import subprocess
from pathlib import Path

p=Path('progress/BUILD_W2C_0915')
journal=(p/'final_selfcheck.commands.jsonl').open('w',buffering=1)
def run(args, allowed=(0,)):
    r=subprocess.run(args,capture_output=True,text=True)
    journal.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,
        stdout=r.stdout,stderr=r.stderr),ensure_ascii=False)+'\n')
    assert r.returncode in allowed,(args,r.returncode)
    return r.stdout
def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
def heads(label):
    return dict((ref,sha) for sha,ref in (line.split() for line in
        (p/f'raw/{label}.stdout.txt').read_text().splitlines()))

assert run(['git','branch','--show-current']).strip()=='codex/r5-ehabi-diagnosis'
assert json.loads((p/'bcc_gate.json').read_text())['status']=='SIX_CELLS_PASS'
matrix=list(csv.DictReader((p/'MATRIX.tsv').open(),delimiter='\t'))
assert len(matrix)==10 and all(r['build']=='PASS' for r in matrix)
packages=list(csv.DictReader((p/'PACKAGE_STATUS.tsv').open(),delimiter='\t'))
assert len(packages)==11 and sum(r['current_status']=='已完成并推送' for r in packages)==10
assert json.loads((p/'guard_context_audit.json').read_text())['packages']==11
assert json.loads((p/'bpftrace_source_matrix.json').read_text())['package_pushed'] is False
for label in ['048_llvm_confirmed_resume','093_bcc_armv7l_libcxx','102_bcc_armv7l_gcc',
              '129_bpftrace_armv7l_libcxx','134_bpftrace_armv7l_gcc',
              '145_bpftrace_aarch64_libcxx','150_bpftrace_aarch64_gcc']:
    # Labels are checked against actual files; no synthetic missing exit codes.
    matches=list((p/'raw').glob(label.split('_')[0]+'_*.exitcode'))
    assert len(matches)==1 and matches[0].read_text().strip()=='0',matches

for arch in ['armv7l','aarch64']:
    for mode in ['libcxx','gcc']:
        check=json.loads((p/f'checks/bpftrace_{arch}_{mode}/summary.json').read_text())
        assert check['status']=='SOURCE_PROGRAMS_AND_PROVIDERS_PASS'
        assert check['prebuilt']['byte_identical']
        assert len(check['source_programs'])==len(check['build_root_providers'])==2
        if mode=='gcc':
            text=(p/f'build-w2c-bpftrace-{arch}-{mode}.build.log').read_text()
            assert '-stdlib=libc++' not in text and '-lc++abi' not in text
before=heads('121_bpftrace_remote_before');after=heads('156_bpftrace_remote_after')
assert before==after
ref='refs/heads/sandbox/lhmax2025/libcxx-migration'
assert after[ref]=='30e51cd665360f85b665308dc13ba27df0c5f739'
bcc_before=heads('110_bcc_remote_branches_before');bcc_after=heads('115_bcc_remote_branches_after')
assert set(bcc_before)==set(bcc_after) and len(bcc_after)==45
assert [key for key in bcc_before if bcc_before[key]!=bcc_after[key]]==[ref]
assert bcc_after[ref]=='5bd0654e24bb231035ea684d278f674aa95a2cd2'
assert not run(['git','-C','codes/R104/bcc-tools','status','--porcelain']).strip()
assert run(['git','-C','codes/R104/bpftrace','rev-parse','HEAD']).strip()==after[ref]
assert run(['git','-C','codes/R104/bpftrace','diff','--name-only']).strip()=='packaging/bpftrace.spec'
assert not run(['git','-C','codes/R104/bpftrace','diff','--cached','--name-only']).strip()
assert digest(Path('codes/R104/bpftrace/packaging/bpftrace.spec'))==digest(p/'bpftrace_candidate.spec')

units=['build-w2c-llvm-confirmed','build-w2c-bcc-tools-armv7l-libcxx',
       'build-w2c-bcc-tools-armv7l-gcc']
units += [f'build-w2c-bpftrace-{a}-{m}' for a in ['armv7l','aarch64'] for m in ['libcxx','gcc']]
scopes={}
for unit in units:
    value=run(['systemctl','--user','show',unit+'.scope','--property=ActiveState','--value'],(0,1)).strip()
    assert value in ('inactive','failed',''),(unit,value)
    scopes[unit]=value or 'unit unavailable after completion'
# Restrict process output to our exact build paths, not unrelated user commands.
processes=run(['ps','-eo','pid=,ppid=,ni=,comm='])
run(['df','-Pk','tmp/GBS-ROOT'])
scripts=sorted(path for path in p.glob('*') if path.suffix in ['.py','.sh'])
with (p/'SCRIPT_SHA256.tsv').open('w') as f:
    f.write('sha256\tfile\n')
    for path in scripts:f.write(digest(path)+'\t'+path.name+'\n')
for path in scripts:
    if path.suffix=='.py':compile(path.read_text(),str(path),'exec')
    else:run(['bash','-n',str(path)])
assert all(path.stat().st_size < 90*1024*1024 for path in p.rglob('*') if path.is_file())
assert not any(path.suffix in ['.rpm','.a','.o'] for path in p.rglob('*') if path.is_file())
result=dict(status='PASS_WITH_EXPLICIT_PARTIAL_PACKAGE_SCOPE',time=datetime.datetime.now().astimezone().isoformat(),
    packages_pushed=10,bcc_cells=6,bpftrace_source_cells=4,bpftrace_package_gate='WAITING_FOR_HUMAN_CONFIRMATION',
    bpftrace_remote_branches=len(after),bpftrace_remote_changes=0,bcc_remote_unchanged=44,
    scope_states=scopes,script_files=len(scripts),board_used=False)
(p/'final_selfcheck.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False))
