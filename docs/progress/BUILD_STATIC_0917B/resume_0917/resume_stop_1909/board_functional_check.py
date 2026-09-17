"""Record bounded tests of an already installed, SHA-pinned board RPM payload.

This script does not install packages or change configuration. Missing probes,
permissions and unsupported kernel features remain observations, not PASS.
"""
import hashlib
import json
import re
import shlex
import subprocess
import sys
from pathlib import Path

label,expected=sys.argv[1:]
assert label in ('candidate','original') and re.fullmatch('[0-9a-f]{64}',expected)
p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917'/('board-functional-'+label)
out.mkdir(exist_ok=False)
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
runner='/var/tmp/build_static_0917b_function/run-bounded'
records=[]
def run(name,script,limit=40):
    cmd=sdb+['shell',script+'; task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"']
    r=subprocess.run(cmd,capture_output=True,timeout=limit)
    (out/(name+'.stdout')).write_bytes(r.stdout)
    (out/(name+'.stderr')).write_bytes(r.stderr)
    text=r.stdout.decode(errors='replace').replace('\r\n','\n')
    match=re.findall(r'^TASK_REMOTE_RC=(\d+)$',text,re.M)
    records.append({'name':name,'command':shlex.join(cmd),'transport_exitcode':r.returncode,
                    'remote_exitcode':int(match[0]) if len(match)==1 else 'NOT_OBSERVED',
                    'stdout_sha256':hashlib.sha256(r.stdout).hexdigest()})
    (out/'commands.json').write_text(json.dumps(records,indent=2))
    assert r.returncode==0 and len(match)==1,'Connection/completion failure: stop, do not retry automatically'
    return int(match[0]),text
code,text=run('identity','id; sha256sum /usr/bin/bpftrace; test -x '+runner)
assert code==0 and 'uid=0(' in text and re.search('^'+expected+r'\s',text,re.M)
for name,args in [('version','--version'),('info','--info'),('probes','-l'),('help','--help')]:
    run(name,'ulimit -c 0; '+runner+' 25 /usr/bin/bpftrace '+args)
_,functions=run('available_filter_functions','cat /sys/kernel/debug/tracing/available_filter_functions')
_,events=run('available_events','cat /sys/kernel/debug/tracing/available_events')
function=next((x for x in ('do_sys_openat2','do_filp_open','vfs_read')
               if re.search(r'^'+re.escape(x)+r'(?:\s|$)',functions,re.M)),None)
event='sched:sched_switch' if re.search(r'^sched:sched_switch$',events,re.M) else None
tests=[('begin','BEGIN { printf("hello\\n"); exit(); }')]
if function:
    tests.append(('kprobe','kprobe:'+function+' { @hits = count(); } interval:s:2 { exit(); }'))
if event:
    tests.append(('tracepoint','tracepoint:'+event+' { @hits = count(); } interval:s:2 { exit(); }'))
results=[]
for name,script in tests:
    code,text=run('script_'+name,'ulimit -c 0; '+runner+' 20 /usr/bin/bpftrace -e '+shlex.quote(script))
    hit=re.search(r'@hits:\s*(\d+)',text)
    passed=code==0 and (bool(re.search(r'^hello$',text,re.M)) if name=='begin' else bool(hit and int(hit[1])>0))
    results.append({'name':name,'script':script,'remote_exitcode':code,'assertions_pass':passed,
                    'hits':int(hit[1]) if hit else 'NOT_OBSERVED'})
summary={'platform':'物理板','installed_binary_sha256':expected,'tests':results,
         'kprobe_function':function or 'NOT_OBSERVED','tracepoint':event or 'NOT_OBSERVED',
         'limitation':'Successful listing is not proof that a probe program loaded or fired; only explicit script assertions count.'}
(out/'summary.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2))
print(json.dumps(summary,ensure_ascii=False),flush=True)
