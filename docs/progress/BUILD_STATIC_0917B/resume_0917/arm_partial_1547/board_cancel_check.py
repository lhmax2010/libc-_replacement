"""Physical-board cancellation checks; explicit remote exit codes and cleanup.

Only task-owned probe files are installed. No RPM/system configuration changes.
On connection failure stop immediately and preserve cleanup requirements.
"""
import datetime
import hashlib
import json
import re
import shlex
import subprocess
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917/board_cancel'
out.mkdir(exist_ok=False)
local=p/'tmp/STATIC_0917B/board-probes'
remote='/var/tmp/build_static_0917b_cancel'
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
records=[]; results=[]; uploaded=[]
def command(args, seconds=40):
    n=len(records)
    record={'command':shlex.join(args),'started':datetime.datetime.now().astimezone().isoformat()}
    try:
        r=subprocess.run(args,capture_output=True,text=True,errors='replace',timeout=seconds)
    except subprocess.TimeoutExpired as e:
        record.update(exitcode='NOT_OBSERVED_TIMEOUT',timeout_seconds=seconds)
        records.append(record)
        (out/'commands.json').write_text(json.dumps(records,indent=2))
        raise RuntimeError('Connection/command timeout: stop; manual cleanup required') from e
    record.update(exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    records.append(record)
    (out/'commands.json').write_text(json.dumps(records,indent=2))
    print(n,shlex.join(args),'exitcode='+str(r.returncode),flush=True)
    assert r.returncode==0,record
    return r.stdout.replace('\r\n','\n')
def shell(script, expected=0):
    # sdb's transport exit code is not a substitute for the remote command code.
    text=command(sdb+['shell',script+'; task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"'])
    codes=re.findall(r'^TASK_REMOTE_RC=(\d+)$',text,re.M)
    assert len(codes)==1, 'Missing remote completion marker; stop without retry'
    code=int(codes[0])
    if expected is not None: assert code==expected,(code,text)
    return code,text

assert all((local/x).is_file() for x in ('run-bounded','cancel-shared-gcc','cancel-static-gcc'))
shell('test ! -e '+remote)
command(sdb+['root','on'])
_,identity=shell('id')
assert 'uid=0(' in identity
shell('mkdir '+remote)
for name in ('run-bounded','cancel-shared-gcc','cancel-static-gcc'):
    f=local/name
    sha=hashlib.sha256(f.read_bytes()).hexdigest()
    command(sdb+['push',str(f),remote+'/'+name],seconds=90)
    _,text=shell('sha256sum '+remote+'/'+name)
    assert re.search(r'^'+sha+r'\s+',text,re.M)
    uploaded.append({'name':name,'sha256':sha,'path':remote+'/'+name})
    (out/'transfer_sha256.json').write_text(json.dumps(uploaded,indent=2))
shell('chmod 755 '+ ' '.join(remote+'/'+x['name'] for x in uploaded))
shell('uname -a')
for link in ('shared-gcc','static-gcc'):
    for mode in ('wait','system_clock','steady_clock','custom_clock'):
        for repeat in range(1,6):
            code,text=shell('ulimit -c 0; '+remote+'/run-bounded 20 '+remote+'/cancel-'+link+' '+mode,expected=None)
            passed=(code==0 and 'event=wait_confirmed mode='+mode in text
                    and text.count('event=cleanup\n')==1
                    and 'event=joined cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in text)
            results.append({'platform':'物理板','link':link,'mode':mode,'repeat':repeat,
                            'remote_exitcode':code,'assertions_pass':passed,'stdout':text})
            (out/'results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2))
# Cleanup only the three files copied by this script, then the empty task dir.
shell('rm -- '+' '.join(shlex.quote(x['path']) for x in uploaded))
shell('rmdir '+remote)
shell('test ! -e '+remote)
command(sdb+['root','off'])
_,identity=shell('id')
assert 'uid=0(' not in identity
summary={'shared_gcc_pass':sum(x['assertions_pass'] for x in results if x['link']=='shared-gcc'),
         'static_gcc_pass':sum(x['assertions_pass'] for x in results if x['link']=='static-gcc'),
         'static_exitcodes':[x['remote_exitcode'] for x in results if x['link']=='static-gcc'],
         'cleanup':'VERIFIED_TASK_DIRECTORY_ABSENT_ROOT_OFF','platform':'物理板'}
(out/'summary.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2))
print(json.dumps(summary,ensure_ascii=False),flush=True)
assert summary['shared_gcc_pass']==20, 'Shared-unwinder cancellation gate failed; static control is observational'
