"""Read-only failed-preflight capture, then restore the initial SDB root mode.
No RPM transaction retry, no installation, no policy modification, no deletion.
"""
import datetime,json,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_STATIC_0922';O=E/'board/failure-state';O.mkdir(exist_ok=False)
S=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101'];records=[]
def run(args):
    r=subprocess.run(args,capture_output=True,text=True,errors='replace',timeout=60)
    row=dict(command=shlex.join(args),time=datetime.datetime.now().astimezone().isoformat(),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr);records.append(row)
    (O/'commands.json').write_text(json.dumps(records,indent=2));assert r.returncode==0,'Transport failure; do not retry'
    return r.stdout.replace('\r\n','\n')
def shell(cmd):
    s=run(S+['shell','('+cmd+'); task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"'])
    rc=re.findall(r'^TASK_REMOTE_RC=(\d+)$',s,re.M);assert len(rc)==1
    records[-1]['remote_exitcode']=int(rc[0]);(O/'commands.json').write_text(json.dumps(records,indent=2))
    return int(rc[0]),re.sub(r'\nTASK_REMOTE_RC=\d+\n?','',s)
before=json.loads((E/'board/before.json').read_text());after={}
for path in before:
    q=shlex.quote(path)
    rc,s=shell('if test -L '+q+'; then printf "LINK\\n"; readlink '+q+'; stat -c "%a %u %g" '+q+'; elif test -f '+q+'; then printf "FILE\\n"; sha256sum '+q+'; stat -c "%a %u %g" '+q+'; elif test -d '+q+'; then printf "DIR\\n"; stat -c "%a %u %g" '+q+'; else printf "ABSENT\\n"; fi')
    assert rc==0;after[path]=s.strip()
(E/'board/after.json').write_text(json.dumps(after,indent=2))
rc,packages=shell("rpm -qa --qf '%{NAME}\\n'");assert rc==0
related=[x for x in packages.splitlines() if x in ('bpftrace','bpftrace-common','bpftrace-static')]
shell('df -Pk / /etc /usr/bin /var/tmp')
shell('ls -ld /etc /etc/device-sec-policy')
shell('ls -la /var/tmp/bpf_static_0922')
transfers=json.loads((E/'board/transfers.json').read_text());retained=[]
for x in transfers:
    rc,s=shell('sha256sum '+shlex.quote(x['destination']));assert rc==0
    retained.append(dict(path=x['destination'],sha256=x['sha256_after'],match=bool(re.search('^'+x['sha256_after']+r'\s',s,re.M))))
state=json.loads((E/'board/ROOT_STATE.json').read_text())
if state['root_changed']:run(S+['root','off'])
rc,identity=shell('id');assert rc==0
restored=('uid=0(' in identity)==state['initial_root'];assert restored
result=dict(status='PRESERVED_FAILURE_SCENE',files_restored=before==after,no_related_packages=not related,related_packages=related,task_directory_absent=False,retained_files=retained,root_state_restored=restored,installation_attempted=False,only_normal_test_executed=True,reason='User requires failed scene preservation; no retry/bypass/repair. Uploaded files retained; no test package was installed.')
(E/'board/CLEANUP.json').write_text(json.dumps(result,indent=2));print(json.dumps(result),flush=True)
