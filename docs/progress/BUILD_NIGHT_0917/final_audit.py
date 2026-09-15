import datetime,hashlib,json,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); rows=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True); rows.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)); assert r.returncode==0; return r.stdout
for repo,sha in [('tmp/BUILD_W1BC_0911/target-fetch','f203923a1508c9344f5fc6b17bd8822f011655c4'),('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push','f895f8c0373d224847fc7d3ecbeaac3bf926a1a1')]:
    assert run(['git','-C',repo,'rev-parse','HEAD']).strip()==sha
    assert not run(['git','-C',repo,'status','--porcelain']).strip()
for stage,cap in [('w1',3),('w2',5),('w3',4),('w4',1)]:
    d=json.loads((p/(stage+'_result.json')).read_text()); assert d['seconds']<cap*3600
assert json.loads((p/'w3_result.json').read_text())['status']=='PARTIAL'
for arch in ['x86_64','aarch64']:
    r=json.loads((p/f'bpftrace-{arch}-artifact.json').read_text())
    assert hashlib.sha256(Path(r['path']).read_bytes()).hexdigest()==r['sha256']
    assert r['abi_pass'] and r['needed_pass']
    assert json.loads((p/f'bpftrace-{arch}-link_map_summary.json').read_text())['stdcppfs_pulled']==0
pscmd=['ps','-eo','pid,ppid,ni,comm,args']
pr=subprocess.run(pscmd,capture_output=True,text=True); assert pr.returncode==0
text=pr.stdout
active=[l for l in text.splitlines() if ('build_monitor.py --label' in l or 'ninja -C /tmp/night/builds/' in l) and 'BUILD_NIGHT_0917' in l]
# The actual Ninja line may not include the host evidence path.
active += [l for l in text.splitlines() if '/tmp/night/builds/' in l and ('ninja -C ' in l or '/aarch64/ninja -C ' in l)]
rows.append(dict(command=pscmd,exitcode=pr.returncode,stdout='\n'.join(active),stdout_scope='only matching task build processes; unrelated process arguments omitted',stderr=pr.stderr))
assert not active,active
(p/'final_audit_commands.json').write_text(json.dumps(rows,indent=2))
(p/'final_audit.json').write_text(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),source_worktrees_clean=True,candidates_hash_unchanged=True,no_owned_builds_running=True,all_stage_reported_durations_within_caps=True,overall='PARTIAL'),indent=2))
print('PASS source/candidate/resource-scope audit; overall status remains PARTIAL')
