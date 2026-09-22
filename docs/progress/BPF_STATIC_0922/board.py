"""Authorized physical-board checks of the newly built RPM; no bypass or retry."""
import datetime,hashlib,json,re,shlex,subprocess,sys
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'; O=E/'board'; O.mkdir(exist_ok=False)
REMOTE='/var/tmp/bpf_static_0922'; SDB=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
records=[]; transfers=[]; owned=[]; root_changed=False; connected=True
def sha(f):
    with f.open('rb') as s:return hashlib.file_digest(s,'sha256').hexdigest()
def dump(name,value): (O/name).write_text(json.dumps(value,ensure_ascii=False,indent=2))
def transport(args,seconds=90):
    global connected
    n=len(records); row=dict(command=shlex.join(args),time=datetime.datetime.now().astimezone().isoformat())
    try:r=subprocess.run(args,capture_output=True,timeout=seconds)
    except subprocess.TimeoutExpired as ex:
        connected=False;row.update(exitcode='NOT_OBSERVED_TIMEOUT');records.append(row);dump('commands.json',records)
        raise RuntimeError('Transport/remote timeout; preserve board state, no retry') from ex
    (O/f'{n:03d}.transport.out').write_bytes(r.stdout);(O/f'{n:03d}.transport.err').write_bytes(r.stderr)
    row.update(exitcode=r.returncode,stdout=f'{n:03d}.transport.out',stderr=f'{n:03d}.transport.err');records.append(row);dump('commands.json',records)
    if r.returncode:connected=False;raise RuntimeError('Transport nonzero; preserve board state, no retry')
    return r.stdout.decode(errors='replace').replace('\r\n','\n')
def shell(script,required=True):
    s=transport(SDB+['shell','('+script+'); task_rc=$?; printf "\\nTASK_REMOTE_RC=%s\\n" "$task_rc"'])
    codes=re.findall(r'^TASK_REMOTE_RC=(\d+)$',s,re.M)
    if len(codes)!=1:raise RuntimeError('No unique remote completion marker')
    rc=int(codes[0]);records[-1]['remote_exitcode']=rc;dump('commands.json',records)
    if required and rc:raise RuntimeError('Remote command failed; preserve state; see commands.json')
    return rc,re.sub(r'\nTASK_REMOTE_RC=\d+\n?','',s)
def upload(f,name,expected=None):
    h=sha(f);assert expected is None or h==expected
    target=REMOTE+'/'+name;transport(SDB+['push',str(f),target],180);owned.append(target)
    _,s=shell('sha256sum '+shlex.quote(target));assert re.search(r'^'+h+r'\s',s,re.M)
    transfers.append(dict(source=str(f),destination=target,sha256_before=h,sha256_after=h,bytes=f.stat().st_size));dump('transfers.json',transfers)
    return target
def test(label,command,seconds=40):
    assert re.fullmatch('[a-z0-9_-]+',label)
    files=[REMOTE+'/'+label+x for x in ('.out','.err')];owned.extend(files)
    script='ulimit -c 0; '+REMOTE+'/run-bounded '+str(seconds)+' '+command
    rc,_=shell('('+script+') >'+shlex.quote(files[0])+' 2>'+shlex.quote(files[1]),required=False)
    for suffix,remote in zip(('.out','.err'),files):transport(SDB+['pull',remote,str(O/(label+suffix))],120)
    return dict(label=label,command=command,exitcode=rc,stdout=(O/(label+'.out')).read_text(errors='replace').replace('\r\n','\n'),stderr=(O/(label+'.err')).read_text(errors='replace').replace('\r\n','\n'))
def state(path):
    q=shlex.quote(path)
    _,s=shell('if test -L '+q+'; then printf "LINK\\n"; readlink '+q+'; stat -c "%a %u %g" '+q+'; elif test -f '+q+'; then printf "FILE\\n"; sha256sum '+q+'; stat -c "%a %u %g" '+q+'; elif test -d '+q+'; then printf "DIR\\n"; stat -c "%a %u %g" '+q+'; elif test -e '+q+'; then printf "SPECIAL\\n"; else printf "ABSENT\\n"; fi')
    return s.strip()

gate=json.loads((E/'verify-armv7l/result.json').read_text())
assert gate['status']=='PASS' and (E/'cells/bpf-armv7l-libcxx/exitcode').read_text().strip()=='0'
rpms={x['identity'].split('\t')[0]:x for x in gate['rpms']};assert len(rpms)==3
for row in rpms.values():assert sha(Path(row['path']))==row['sha256']
refs=json.loads((P/'docs/progress/BPF_W1_0921/W3_REFERENCE_INPUTS.json').read_text())['rows']
for row in refs:assert sha(Path(row['path']))==row['sha256']
dump('PINNED_INPUTS.json',dict(rpms=gate['rpms'],reference_inputs=refs))
devices=transport([SDB[0],'devices'])
if not re.search(r'^192\.168\.108\.26:26101\s+device\b',devices,re.M):
    transport([SDB[0],'connect','192.168.108.26:26101'])
    devices=transport([SDB[0],'devices'])
assert re.search(r'^192\.168\.108\.26:26101\s+device\b',devices,re.M),'Board endpoint unavailable; no retry, no board mutation'
_,identity=shell('id');initial_root='uid=0(' in identity
shell('test ! -e '+REMOTE)
_,packages=shell("rpm -qa --qf '%{NAME}\\n'")
existing=[x for x in packages.splitlines() if x in ('bpftrace','bpftrace-common','bpftrace-static')]
dump('existing_packages.json',existing)
assert not existing,'Existing bpftrace packages: need an explicit restoration plan; no install performed'
_,ps=shell('ps -eo pid,ppid,comm,args');dump('process_preflight.json',dict(stdout=ps))
assert not any(re.search(r'\b(?:bpftrace|cancel-shared-gcc|cancel-static-gcc)\b',x) for x in ps.splitlines()),'Existing related process; no mutation'
if not initial_root:transport(SDB+['root','on']);root_changed=True
assert 'uid=0(' in shell('id')[1]
dump('ROOT_STATE.json',dict(initial_root=initial_root,root_changed=root_changed))
shell('uname -a');shell('cat /proc/version')
rc,config=shell('if test -r /proc/config.gz; then zcat /proc/config.gz; else found=0; for f in /boot/config-*; do if test -r "$f"; then cat "$f"; found=1; fi; done; test "$found" = 1; fi',required=False)
dump('kernel_config.json',dict(exitcode=rc,status='OBSERVED' if rc==0 and re.search(r'CONFIG_',config) else 'NOT_OBSERVED',bpf_lines=[x for x in config.splitlines() if re.search(r'CONFIG_.*(?:BPF|KPROBE|FTRACE|TRACEPOINT)',x)],full=config))
_,disk=shell('df -Pk /var/tmp /usr');dump('disk.json',dict(stdout=disk))
free=[int(x.split()[3])*1024 for x in disk.splitlines()[1:] if len(x.split())>=6 and x.split()[3].isdigit()]
required=sum(x['bytes'] for x in rpms.values())+gate['observations']['static']['bytes']*2+100000000+67108864
assert free and min(free)>required,'Board free space insufficient for RPM, backup and side-by-side inputs'
shell('mkdir '+REMOTE)
# File-level snapshot includes all installed static payload paths plus post-script target.
paths=set((E/'verify-armv7l/bpftrace-static.files.txt').read_text().splitlines())|{'/usr/bin/bpftrace','/usr/bin/bpftrace-static','/usr/bin/bpftrace-aotrt'}
_,listed=shell('find /usr/bin -maxdepth 1 -name "bpftrace*" -print')
paths.update(x for x in listed.splitlines() if x.startswith('/usr/bin/bpftrace'))
before={x:state(x) for x in sorted(paths)};dump('before.json',before)
assert all(not x.startswith('SPECIAL') for x in before.values()),'Special file would be affected; preserve state'
assert not any(before[x].startswith('DIR') for x in ('/usr/bin/bpftrace','/usr/bin/bpftrace-static')),'Unexpected directory at post-script target; preserve without installing'
backups={}
for i,(path,metadata) in enumerate(before.items()):
    if metadata.startswith(('FILE','LINK')):
        dst=REMOTE+'/backup-'+str(i);shell('cp -a '+shlex.quote(path)+' '+dst);backups[path]=dst;owned.append(dst)
dump('backups.json',backups)
for name,row in rpms.items():upload(Path(row['path']),Path(row['path']).name,row['sha256'])
runner=P/'tmp/WEEKEND_0918/board-probes/run-bounded';upload(runner,'run-bounded');shell('chmod 755 '+REMOTE+'/run-bounded')
orig=next(x for x in refs if x['kind']=='original_arm_asset');original=upload(Path(orig['path']),'original',orig['sha256'])
mainargs=' '.join(shlex.quote(REMOTE+'/'+Path(rpms[n]['path']).name) for n in ('bpftrace','bpftrace-common'))
main_rc,main_text=shell('rpm -ivh --test '+mainargs,required=False)
staticrpm=REMOTE+'/'+Path(rpms['bpftrace-static']['path']).name
static_rc,static_text=shell('rpm -ivh --test '+shlex.quote(staticrpm),required=False)
dump('INSTALL_PREFLIGHT.json',dict(main=dict(exitcode=main_rc,output=main_text),static=dict(exitcode=static_rc,output=static_text),dependency_bypass=False))
assert static_rc==0,'Normal static install preflight failed; preserve state, no --nodeps'
shell('rpm -ivh '+shlex.quote(staticrpm))
expected=gate['observations']['static']['sha256'];assert re.search('^'+expected+r'\s',shell('sha256sum /usr/bin/bpftrace')[1],re.M)
candidate=REMOTE+'/candidate';shell('cp -a /usr/bin/bpftrace '+candidate);owned.append(candidate)
assert re.search('^'+expected+r'\s',shell('sha256sum '+candidate)[1],re.M)
shell('chmod 755 '+original+' '+candidate)
dump('INSTALL_RESULT.json',dict(status='PASS',post_static_sha256=expected,candidate_from_new_rpm=True,platform='物理板'))
pairs=[];listings={}
def paired(name,args):
    pair=dict(name=name,platform='物理板')
    for label,binary in [('candidate',candidate),('original',original)]:pair[label]=test(name+'-'+label,shlex.quote(binary)+' '+args)
    a,b=pair['candidate'],pair['original']
    pair['classification']=('CANDIDATE_ONLY_FAILURE' if a['exitcode']!=0 and b['exitcode']==0 else 'BOTH_NONZERO_REVIEW_ENVIRONMENT' if a['exitcode'] and b['exitcode'] else 'BOTH_ZERO' if a['exitcode']==b['exitcode']==0 else 'DIFFERENT_EXIT_STATUS')
    pair['stdout_equal']=a['stdout']==b['stdout'];pair['stderr_equal']=a['stderr']==b['stderr'];pairs.append(pair);dump('pairs.json',pairs)
    return pair
paired('version','--version');paired('info','--info');listing=paired('list','-l')
for label in ('candidate','original'):
    (O/('list-'+label+'-first50.txt')).write_text('\n'.join(listing[label]['stdout'].splitlines()[:50])+'\n')
    listings[label]=set(x.strip() for x in listing[label]['stdout'].splitlines() if re.fullmatch(r'(?:kprobe|tracepoint):[A-Za-z0-9_:]+',x.strip()))
common=listings['candidate']&listings['original']
selected=next((x for x in ('tracepoint:sched:sched_switch','tracepoint:raw_syscalls:sys_enter','kprobe:do_sys_openat2','kprobe:vfs_read') if x in common),None)
if selected is None:selected=next(iter(sorted(x for x in common if x.startswith('tracepoint:'))),None)
scripts=[('begin','BEGIN { printf("ok\\n"); exit(); }'),('interval','interval:s:1 { exit(); }')]
if selected:scripts.insert(1,('kernel',selected+' { @hits = count(); } interval:s:1 { exit(); }'))
dump('PROBE_SELECTION.json',dict(selected=selected or 'NOT_OBSERVED',reason='Exact intersection of actual -l output; no inferred probe names',scripts=scripts))
for name,script in scripts:
    for repeat in range(1,4):paired(name+'-'+str(repeat),'-e '+shlex.quote(script))
# Existing binaries, unchanged source/ABI/arguments, four waits times five repeats.
cancel=[]
for link in ('shared-gcc','static-gcc'):
    ref=next(x for x in refs if x['kind']==link);binary=upload(Path(ref['path']),'cancel-'+link,ref['sha256']);shell('chmod 755 '+binary)
    for mode in ('wait','system_clock','steady_clock','custom_clock'):
        for repeat in range(1,6):
            row=test('cancel-'+link+'-'+mode.replace('_','-')+'-'+str(repeat),binary+' '+mode,20)
            row.update(link=link,mode=mode,repeat=repeat,platform='物理板')
            row['assertions_pass']=row['exitcode']==0 and 'event=wait_confirmed mode='+mode in row['stdout'] and row['stdout'].count('event=cleanup\n')==1 and 'event=joined cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in row['stdout']
            cancel.append(row);dump('cancel.json',cancel)
dump('cancel_summary.json',{link:dict(passed=sum(x['assertions_pass'] for x in cancel if x['link']==link),total=20,exitcodes=[x['exitcode'] for x in cancel if x['link']==link]) for link in ('shared-gcc','static-gcc')})
shell('rpm -e --test bpftrace-static');shell('rpm -e bpftrace-static')
for path,backup in backups.items():shell('cp -a '+shlex.quote(backup)+' '+shlex.quote(path))
after={x:state(x) for x in sorted(paths)};dump('after.json',after)
assert before==after,'Board affected file identity/ownership differs after uninstall; preserve backup and state'
_,ps=shell('ps -eo pid,ppid,comm,args');assert not any(REMOTE+'/' in x for x in ps.splitlines()),'Task processes remain; preserve state'
_,pkg=shell("rpm -qa --qf '%{NAME}\\n'");assert not any(x in ('bpftrace','bpftrace-common','bpftrace-static') for x in pkg.splitlines())
# Delete only explicitly enumerated task-owned files, after all evidence pulled.
assert len(set(owned))==len(owned)
for f in owned:shell('rm -- '+shlex.quote(f))
shell('rmdir '+REMOTE);shell('test ! -e '+REMOTE)
if root_changed:transport(SDB+['root','off'])
assert ('uid=0(' in shell('id')[1])==initial_root
dump('CLEANUP.json',dict(status='PASS',files_restored=True,no_related_packages=True,task_directory_absent=True,root_state_restored=True))
dump('SUMMARY.json',dict(platform='物理板',static_install='PASS',main_precheck_exitcode=main_rc,paired_tests=len(pairs),candidate_only_failures=sum(x['classification']=='CANDIDATE_ONLY_FAILURE' for x in pairs),kernel_probe=selected or 'NOT_OBSERVED',cancel=json.loads((O/'cancel_summary.json').read_text()),cleanup='PASS'))
print('BOARD_SEQUENCE_COMPLETED; review pair differences before functional-equivalence verdict',flush=True)
