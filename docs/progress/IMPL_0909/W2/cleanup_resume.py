#!/usr/bin/env python3
"""续测后只清理本轮认领路径，诊断先备份；从不清理共享目录本身。"""
import csv,hashlib,pathlib,re,shlex,subprocess,sys,time,zipfile
r=pathlib.Path.cwd();out=r/'docs/progress/IMPL_0909/W2';raw=out/'raw';sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101'];n=0
for label in ['lit_resume','lit_resume2','lit_resume3']:assert (raw/label/'exitcode').exists(),('TESTS_RUNNING',label)
def run(label,cmd,remote=True):
    global n
    n+=1;stem=raw/f'resume_cleanup_{n:03}_{label}'
    if remote:cmd=sdb+['shell',cmd+'; cleanup_rc=$?; printf "\n__FINAL_CLEANUP__=%d\n" "$cleanup_rc"']
    rc=subprocess.call([sys.executable,str(r/'docs/progress/R114/code/record.py'),str(stem),'timeout','60',*cmd]);assert rc==0,(stem,rc)
    text=pathlib.Path(str(stem)+'.stdout').read_text().replace('\r\n','\n')
    if remote:
        m=re.search(r'\n__FINAL_CLEANUP__=0\n?$',text);assert m,('REMOTE_NOT_CONFIRMED',stem,text);text=text[:m.start()]
    return text
refs=run('references','for p in /proc/[0-9]*/exe /proc/[0-9]*/cwd /proc/[0-9]*/fd/*; do x=$(readlink "$p") || continue; printf "%s\t%s\n" "$p" "$x"; done; true')
assert '/exe\t/usr/bin/bash' in refs and not any('/opt/usr/impl_0908_lit' in x.split('\t',1)[-1] for x in refs.splitlines()),'ACTIVE_TASK_REFERENCE_STOP'
maps=run('maps','for p in /proc/[0-9]*/maps; do if test -r "$p"; then grep -H -F "/opt/usr/impl_0908_lit/" "$p"; fi; done; printf "POSITIVE_MAP_CONTROL\n"; grep -m 1 "/usr/lib" /proc/self/maps')
assert '/usr/lib' in maps and '/opt/usr/impl_0908_lit/' not in maps
hashes=run('runtime_hash','sha256sum /opt/usr/impl_0908_lit/runtime/libc++.so.1 /opt/usr/impl_0908_lit/runtime/libc++abi.so.1')
for row in csv.DictReader((out/'RESUME_RUNTIME_IDENTITIES.tsv').open(),delimiter='\t'):
    if '/runtime/' not in row['board']:continue
    assert row['sha256']+'  '+row['board'] in hashes
    assert hashlib.sha256(pathlib.Path(row['local']).read_bytes()).hexdigest()==row['sha256']
initial={x for x in (raw/'resume_deploy_00_initial_diagnostics.stdout').read_text().splitlines() if x.endswith('.zip')}
now=set(run('diagnostic_inventory','find /opt/usr/share/crash/dump -maxdepth 1 -name "*.zip" -printf "%f\n"').splitlines())
assert initial and initial<=now,'PREVIOUS_NON_TASK_DIAGNOSTIC_MISSING'
rows=[];owned=[];backup=r/'tmp/IMPL_0909/W2/resume-crash-backup';backup.mkdir(exist_ok=False)
run_ids=set()
for folder in [raw/'config_executor',raw/'lit_resume/executor',raw/'lit_resume2/executor',raw/'lit_resume3/executor']:
    run_ids.update(x.stem for x in folder.glob('run-*.log'))
for name in sorted(now-initial):
    assert re.fullmatch(r'[A-Za-z0-9_.+-]+\.zip',name),('UNEXPECTED_FILENAME',name)
    path='/opt/usr/share/crash/dump/'+name
    cwd=run('diagnostic_cwd','unzip -p '+shlex.quote(path)+" '*/proc/*/cwd'").strip('\x00 \r\n')
    if not cwd.startswith('/opt/usr/impl_0908_lit/runs/') or cwd[len('/opt/usr/impl_0908_lit/runs/'):].split('/',1)[0] not in run_ids:
        rows.append([path,cwd,'NOT_OWNED_PRESERVED','','']);continue
    local=backup/name;run('backup',sdb+['pull',path,str(local)],False)
    digest=hashlib.sha256(local.read_bytes()).hexdigest()
    with zipfile.ZipFile(local) as z:
        members=[x for x in z.infolist() if re.search(r'/proc/[0-9]+/cwd$',x.filename)]
        assert len(members)==1 and members[0].file_size<4096
        assert z.read(members[0]).decode().strip('\x00 \r\n')==cwd
    assert run('backup_sha','sha256sum '+shlex.quote(path)).split()[0]==digest
    rows.append([path,cwd,'OWNED_BACKED_UP',digest,str(local)]);owned.append(path)
with (out/'RESUME_DIAGNOSTIC_CLEANUP.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['board_path','cwd','classification','sha256','host_backup']);w.writerows(rows)
uploads=run('upload_inventory',"find /opt/usr/home/owner/share/tmp -maxdepth 1 \\( -name 'impl_0908_run-*.tar' -o -name 'impl_0909_libc*.so.1' \\) -print").splitlines()
for p in uploads:
    name=pathlib.Path(p).name
    assert name in ['impl_0909_libc++.so.1','impl_0909_libc++abi.so.1'] or name.removeprefix('impl_0908_').removesuffix('.tar') in run_ids,('UNKNOWN_UPLOAD_STOP',p)
run('directory_inventory','find /opt/usr/impl_0908_lit -maxdepth 3 -printf "%y %p\n"')
run('remove_task_root','test -d /opt/usr/impl_0908_lit && test ! -L /opt/usr/impl_0908_lit && rm -rf -- /opt/usr/impl_0908_lit')
for path in uploads+owned:run('remove_owned_file','test ! -L '+shlex.quote(path)+' && rm -f -- '+shlex.quote(path))
verification=[]
for path in ['/opt/usr/impl_0908_lit',*uploads,*owned]:
    run('absence','test ! -e '+shlex.quote(path)+' && test ! -L '+shlex.quote(path)+' && printf "ABSENT '+path+'\n"')
    verification.append([path,'ABSENT'])
control=sorted(initial)[0]
run('parent_control','test -d /opt/usr/home/owner/share/tmp && test -d /opt/usr/share/crash/dump && test -f '+shlex.quote('/opt/usr/share/crash/dump/'+control)+' && printf "NON_TASK_POSITIVE_CONTROL=PASS\n"')
time.sleep(10) # bounded wait for late diagnostic delivery; do not silently miss a new file
final=set(run('final_diagnostic_inventory','find /opt/usr/share/crash/dump -maxdepth 1 -name "*.zip" -printf "%f\n"').splitlines())
expected=now-{pathlib.Path(x).name for x in owned}
assert final==expected,('DIAGNOSTIC_SET_CHANGED_REQUIRES_RECHECK',sorted(final-expected),sorted(expected-final))
with (out/'RESUME_CLEANUP_VERIFICATION.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['path','state']);w.writerows(verification)
print('BOARD_CLEANED_AND_RELEASED=PASS owned_root=1 uploads='+str(len(uploads))+' diagnostics='+str(len(owned)))
print('Diagnostic backups retained on host; runtime/test binaries retained on host; not all board temporary files archived')
