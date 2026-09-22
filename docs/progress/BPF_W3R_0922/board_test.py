"""Extract the pinned RPM and run paired tests only in the approved writable directory."""
import difflib,hashlib,json,re,shlex,subprocess,time
from pathlib import Path
P=Path.cwd();O=P/'progress/BPF_W3R_0922/board-test';O.mkdir(exist_ok=False)
S=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101'];W='/var/tmp/bpf_static_0922';rows=[];transfers=[];pairs=[];cancel=[]
def dump(n,x):(O/n).write_text(json.dumps(x,ensure_ascii=False,indent=2))
def transport(label,argv,limit=100):
 t=time.monotonic();r=subprocess.run(argv,capture_output=True,timeout=limit)
 (O/(label+'.transport.out')).write_bytes(r.stdout);(O/(label+'.transport.err')).write_bytes(r.stderr)
 rows.append(dict(label=label,command=shlex.join(argv),exitcode=r.returncode,seconds=time.monotonic()-t));dump('commands.json',rows);assert r.returncode==0,('transport error',label)
 return r.stdout.decode(errors='replace').replace('\r\n','\n')
def shell(label,cmd,required=True):
 s=transport(label,S+['shell','('+cmd+'); task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"']);m=re.findall(r'^TASK_REMOTE_RC=(\d+)$',s,re.M);assert len(m)==1
 rc=int(m[0]);rows[-1]['remote_exitcode']=rc;dump('commands.json',rows);s=re.sub(r'\nTASK_REMOTE_RC=\d+\n?','',s)
 if required:assert rc==0,(label,rc)
 return rc,s
def sha(p):
 with Path(p).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def push(p,name,expected):
 assert sha(p)==expected;dest=W+'/'+name;shell('absent-'+name,'test ! -e '+dest)
 transport('push-'+name,S+['push',str(p),dest]);s=shell('sha-'+name,'sha256sum '+dest)[1];assert s.split()[0]==expected
 transfers.append(dict(source=str(p),destination=dest,sha256=expected));dump('transfers.json',transfers)
def test(label,binary,args,limit=20):
 cmd=shlex.quote(binary)+' '+args
 prefix='cd '+W+' && ulimit -c 0 && export TMPDIR='+W+' HOME='+W+' XDG_CACHE_HOME='+W+'/.cache && '
 t=time.monotonic();rc,_=shell('test-'+label,prefix+W+'/run-bounded '+str(limit)+' '+cmd+' >'+W+'/'+label+'.out 2>'+W+'/'+label+'.err',False);elapsed=time.monotonic()-t
 for ext in ('out','err'):transport('pull-'+label+'-'+ext,S+['pull',W+'/'+label+'.'+ext,str(O/(label+'.'+ext))])
 return dict(command=cmd,exitcode=rc,seconds=elapsed,stdout_file=label+'.out',stderr_file=label+'.err',stdout=(O/(label+'.out')).read_text(errors='replace'),stderr=(O/(label+'.err')).read_text(errors='replace'))
def paired(name,args):
 a=test(name+'-candidate',W+'/extract/usr/bin/bpftrace-static',args);b=test(name+'-original',W+'/original',args)
 row=dict(name=name,candidate=a,original=b,classification='BOTH_ZERO' if a['exitcode']==b['exitcode']==0 else 'CANDIDATE_ONLY_FAILURE' if a['exitcode']!=0 and b['exitcode']==0 else 'BOTH_NONZERO_REVIEW_ENVIRONMENT' if a['exitcode'] and b['exitcode'] else 'DIFFERENT_EXIT_STATUS')
 pairs.append(row);dump('pairs.json',pairs);print('PAIR',name,a['exitcode'],b['exitcode'],flush=True);return row
assert 'rpi4' in shell('kernel','uname -r')[1]
assert shell('machine','uname -m')[1].strip()=='armv7l'
assert re.search(r'^ID=["\x27]?tizen["\x27]?$',shell('os','cat /etc/os-release')[1],re.M|re.I)
assert 'uid=0(' not in shell('id-before','id')[1]
transport('root-on',S+['root','on']);completed=False
try:
 assert 'uid=0(' in shell('id-root','id')[1]
 mounts_before=shell('mounts-before','cat /proc/mounts')[1];(O/'mounts-before.txt').write_text(mounts_before)
 resolved=shell('resolve-work','readlink -f '+W)[1].strip();assert resolved=='/opt/var/tmp/bpf_static_0922'
 mounts=[x.split() for x in mounts_before.splitlines()];m=max((x for x in mounts if resolved==x[1] or resolved.startswith(x[1].rstrip('/')+'/')),key=lambda x:len(x[1]))
 assert m[1]=='/opt' and 'rw' in m[3].split(',')
 disk=shell('df','df -Pk '+W)[1];free=int(disk.splitlines()[-1].split()[3])*1024;assert free>=500*1024*1024
 dump('WORK_DIRECTORY.json',dict(path=W,resolved=resolved,mount=m,free_bytes=free,minimum_bytes=500*1024*1024))
 ps=shell('processes-before','ps -eo pid,ppid,comm,args')[1]
 assert not any(re.search(r'\b(?:bpftrace|cancel-shared-gcc|cancel-static-gcc)\b',x) for x in ps.splitlines())
 assert not shell('packages-before',"rpm -qa --qf '%{NAME}\\n' | grep bpftrace",False)[1].strip()
 assert shell('system-before','ls -l /usr/bin/bpftrace*',False)[0]!=0
 rpm=W+'/bpftrace-static-0.24.2-1.armv7l.rpm'
 assert shell('rpm-sha','sha256sum '+rpm)[1].split()[0]=='672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756'
 assert shell('original-sha','sha256sum '+W+'/original')[1].split()[0]=='ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b'
 # Paths were vetted against the RPM manifest; extract only below a fresh directory.
 listing=shell('rpm-list','rpm -qpl --nosignature '+rpm)[1].splitlines()
 assert set(listing)=={'/usr/bin/bpftrace-static','/usr/share/licenses/bpftrace-static','/usr/share/licenses/bpftrace-static/LICENSE'}
 cpio=P/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-libcxx/local/BUILD-ROOTS/scratch.armv7l.0/usr/bin/cpio'
 push(cpio,'cpio','b8e9cb4a96dade226d2659381043d279e227c734374914f8325d79d6946301b7')
 shell('cpio-mode','chmod 755 '+W+'/cpio');shell('cpio-version',W+'/cpio --version')
 shell('extract','set -o pipefail; test ! -e '+W+'/extract && mkdir '+W+'/extract && cd '+W+'/extract && rpm2cpio '+rpm+' | '+W+'/cpio -idmv')
 candidate=W+'/extract/usr/bin/bpftrace-static'
 assert shell('candidate-sha','sha256sum '+candidate)[1].split()[0]=='6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba'
 shell('executable','chmod +x '+candidate+' '+W+'/original')
 dump('EXTRACTION.json',dict(status='PASS',candidate=candidate,sha256='6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba',rpm_install=False))
 paired('version','--version');info=paired('info','--info');listing=paired('list','-l')
 (O/'info.diff').write_text(''.join(difflib.unified_diff(info['original']['stdout'].splitlines(True),info['candidate']['stdout'].splitlines(True),fromfile='original',tofile='candidate')))
 for label in ('candidate','original'):(O/('list-'+label+'-first50.txt')).write_text('\n'.join(listing[label]['stdout'].splitlines()[:50])+'\n')
 probes=sorted(x.strip() for x in listing['candidate']['stdout'].splitlines() if re.fullmatch(r'(?:kprobe|tracepoint):[A-Za-z0-9_:]+',x.strip()))
 selected=next((x for x in ('tracepoint:sched:sched_switch','tracepoint:raw_syscalls:sys_enter','kprobe:vfs_read') if x in probes),probes[0] if probes else None)
 dump('PROBE_SELECTION.json',dict(selected=selected or 'NOT_OBSERVED',source='candidate actual -l output'))
 scripts=[('begin','BEGIN { printf("ok\\n"); exit(); }')]
 if selected:scripts.append(('kernel',selected+' { exit(); }'))
 scripts.append(('interval','interval:s:1 { exit(); }'))
 for name,script in scripts:
  for i in range(1,4):paired(name+'-'+str(i),'-e '+shlex.quote(script))
 refs=json.loads((P/'docs/progress/BPF_W1_0921/W3_REFERENCE_INPUTS.json').read_text())['rows']
 for link in ('shared-gcc','static-gcc'):
  r=next(x for x in refs if x['kind']==link);push(Path(r['path']),'cancel-'+link,r['sha256']);shell('mode-'+link,'chmod 755 '+W+'/cancel-'+link)
  for mode in ('wait','system_clock','steady_clock','custom_clock'):
   for i in range(1,6):
    x=test('cancel-'+link+'-'+mode+'-'+str(i),W+'/cancel-'+link,mode)
    x.update(link=link,mode=mode,repeat=i);x['assertions_pass']=x['exitcode']==0 and 'event=wait_confirmed mode='+mode in x['stdout'] and x['stdout'].count('event=cleanup\n')==1 and 'event=joined cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in x['stdout']
    cancel.append(x);dump('cancel.json',cancel)
  print('CANCEL',link,sum(x['assertions_pass'] for x in cancel if x['link']==link),'/20',flush=True)
 summary={link:dict(passed=sum(x['assertions_pass'] for x in cancel if x['link']==link),executed=sum(x['link']==link for x in cancel),exitcodes=[x['exitcode'] for x in cancel if x['link']==link]) for link in ('shared-gcc','static-gcc')};dump('cancel_summary.json',summary)
 completed=True
finally:
 # Read-only final state and restore only root mode opened by this script.
 try:
  rc,pkg=shell('packages-after',"rpm -qa --qf '%{NAME}\\n' | grep bpftrace",False)
  rc2,files=shell('system-after','ls -l /usr/bin/bpftrace*',False)
  after=shell('mounts-after','cat /proc/mounts')[1];(O/'mounts-after.txt').write_text(after)
  same=after==mounts_before;dump('MOUNTS_COMPARISON.json',dict(byte_equal=same))
  shell('preserved-shas','find '+W+' -type f -exec sha256sum {} \\;')
  allpass=completed and selected is not None and all(x['candidate']['exitcode']==x['original']['exitcode']==0 for x in pairs) and all(x['assertions_pass'] for x in cancel) and same and not pkg.strip() and rc2!=0
  # Any nonzero test retains the scene, including the static-libgcc control.
  if allpass:
   shell('delete-files','find '+W+' -type f -delete')
   shell('delete-dirs','find '+W+' -depth -type d -empty -delete')
  dump('CLEANUP.json',dict(test_sequence_completed=completed,all_pass=allpass,scene='DELETED_AFTER_ALL_PASS' if allpass else 'PRESERVED_FAILURE_SCENE',no_installed_packages=not pkg.strip(),system_binary_absent=rc2!=0,mounts_unchanged=same))
 finally:
  transport('root-off',S+['root','off']);assert 'uid=0(' not in shell('id-after','id')[1]
  dump('ROOT_RESTORED.json',dict(initial_root=False,final_root=False,status='PASS'))
