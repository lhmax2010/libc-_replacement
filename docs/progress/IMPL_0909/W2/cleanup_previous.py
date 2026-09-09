#!/usr/bin/env python3
"""仅清理已释放实板的 IMPL_0908 精确残留；诊断先备份，摘要及引用检查先于删除。"""
import csv,hashlib,pathlib,re,shlex,subprocess,sys,zipfile
r=pathlib.Path.cwd();out=r/'docs/progress/IMPL_0909/W2';raw=out/'raw'
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
release=(raw/'release_poll_1.stdout').read_text()
assert '板子状态：**板子已释放**' in release
assert 'Raspberry Pi 4 Model B Rev 1.5' in (raw/'009_board_preflight.stdout').read_text()
roots=['/var/tmp/impl_0908','/opt/usr/impl_0908_lit']
canonical=['/opt/var/tmp/impl_0908','/opt/usr/impl_0908_lit']
uploads=['/opt/usr/home/owner/share/tmp/impl_0908_payload.tar',
 '/opt/usr/home/owner/share/tmp/impl_0908_old_timed',
 '/opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar',
 '/opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar']
zip_names=['t.tmp.exe_12031_20260908112709.zip','t.tmp.exe_16996_20260908131255.zip',
 't.tmp.exe_26713_20260908133235.zip','t.tmp.exe_26998_20260908133247.zip',
 't.tmp.exe_27220_20260908133255.zip','t.tmp.exe_28667_20260908114329.zip',
 't.tmp.exe_28747_20260908114332.zip','t.tmp.exe_31433_20260908133536.zip']
zips=['/opt/usr/share/crash/dump/'+x for x in zip_names]
backup=r/'tmp/IMPL_0909/W2/previous-crash-backup';backup.mkdir(parents=True,exist_ok=False)
counter=0
def command(label,cmd,remote=False):
 global counter
 counter+=1;stem=raw/f'previous_cleanup_{counter:02}_{label}'
 if remote:cmd=[*sdb,'shell',cmd+'; cleanup_rc=$?; printf "\n__CLEANUP_RC__=%d\n" "$cleanup_rc"']
 result=subprocess.run([sys.executable,str(r/'docs/progress/R114/code/record.py'),str(stem),'timeout','45',*cmd])
 text=pathlib.Path(str(stem)+'.stdout').read_text().replace('\r\n','\n')
 assert result.returncode==0,('carrier failure',stem)
 if remote:
  match=re.search(r'\n__CLEANUP_RC__=(\d+)\n?$',text);assert match and match[1]=='0',('remote failure',stem,text)
  text=text[:match.start()]
 return text
def owned(p):return any(p==q or p.startswith(q+'/') for q in roots+canonical)
assert owned('/opt/var/tmp/impl_0908/new/libc++.so.1') and not owned('/usr/lib/libc++.so.1')
text=command('references',
 'for p in /proc/[0-9]*/exe /proc/[0-9]*/cwd /proc/[0-9]*/fd/*; do x=$(readlink "$p") || continue; printf "%s\t%s\n" "$p" "$x"; done; true',True)
assert '/exe\t' in text and '/usr/bin/bash' in text
active=[x for x in text.splitlines() if '\t' in x and owned(x.split('\t',1)[1])]
assert not active,('active task reference: STOP',active)
maps=command('maps',
 'for p in /proc/[0-9]*/maps; do if test -r "$p"; then grep -H -F -e "/opt/var/tmp/impl_0908/" -e "/var/tmp/impl_0908/" -e "/opt/usr/impl_0908_lit/" "$p"; fi; done; printf "POSITIVE_MAP_CONTROL\n"; grep -m 1 "/usr/lib" /proc/self/maps',True)
assert 'POSITIVE_MAP_CONTROL' in maps and '/usr/lib' in maps
assert not any('/impl_0908/' in x or '/impl_0908_lit/' in x for x in maps.splitlines())
print('ACTIVE_REFS_AND_MAPS=0 POSITIVE_CONTROLS=PASS',flush=True)
inventory=command('inventory','stat -c "%F %s %n" '+' '.join(roots+uploads+zips)+
 ' && find /opt/usr/impl_0908_lit -maxdepth 3 -printf "%y %p\n" && cd /var/tmp/impl_0908 && sha256sum -c SHA256SUMS',True)
assert inventory.count(': OK')==16
paths=['/opt/usr/impl_0908_lit/runtime/libc++.so.1','/opt/usr/impl_0908_lit/runtime/libc++abi.so.1',
       '/opt/usr/impl_0908_lit/baseline/libc++.so.1','/opt/usr/impl_0908_lit/baseline/libc++abi.so.1',
       '/var/tmp/impl_0908/probes/old_timed',uploads[1]]
actual=command('runtime_hashes','sha256sum '+' '.join(paths),True)
expected={}
for variant,build in [('runtime','build-armv7l'),('baseline','build-baseline-armv7l')]:
 for name in ['libc++.so.1','libc++abi.so.1']:
  expected[f'/opt/usr/impl_0908_lit/{variant}/{name}']=hashlib.sha256((r/'tmp/IMPL_0908'/build/'lib'/name).read_bytes()).hexdigest()
digest=hashlib.sha256((r/'tmp/IMPL_0908/probes-armv7l/old_timed').read_bytes()).hexdigest()
expected[paths[-2]]=digest;expected[paths[-1]]=digest
observed={s.split()[1]:s.split()[0] for s in actual.splitlines() if re.match(r'^[0-9a-f]{64}\s',s)}
assert observed==expected
listing=command('upload_set',"find /opt/usr/home/owner/share/tmp -maxdepth 1 -name 'impl_0908_*' -print",True)
assert set(listing.splitlines())==set(uploads),('unexpected upload; STOP',listing)
rows=[]
for path in zips:
 cwd=command('zip_cwd','unzip -p '+shlex.quote(path)+" '*/proc/*/cwd'",True).strip('\x00 \r\n')
 assert cwd.startswith('/opt/usr/impl_0908_lit/runs/run-'),('ZIP not owned',path,cwd)
 local=backup/pathlib.Path(path).name
 command('zip_backup',[*sdb,'pull',path,str(local)])
 digest=hashlib.sha256(local.read_bytes()).hexdigest()
 with zipfile.ZipFile(local) as f:
  members=[x for x in f.infolist() if re.search(r'/proc/[0-9]+/cwd$',x.filename)]
  assert len(members)==1 and members[0].file_size<4096
  assert f.read(members[0]).decode().strip('\x00 \r\n')==cwd
 boardhash=command('zip_hash','sha256sum '+shlex.quote(path),True).split()[0]
 assert boardhash==digest
 rows.append([path,cwd,digest,str(local),'BACKUP_VERIFIED'])
with (out/'PREVIOUS_CRASH_BACKUPS.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['board_path','process_cwd','sha256','host_backup','status']);w.writerows(rows)
guard=' && '.join('test ! -L '+shlex.quote(p) for p in roots+uploads+zips)
remove=guard+' && rm -rf -- /var/tmp/impl_0908 /opt/usr/impl_0908_lit && rm -f -- '+' '.join(map(shlex.quote,uploads+zips))
command('remove_exact_owned_paths',remove,True)
verify=' && '.join('test ! -e '+shlex.quote(p)+' && test ! -L '+shlex.quote(p) for p in roots+uploads+zips)
verify+=' && test -d /opt/usr/share/crash/dump && test -d /opt/usr/home/owner/share/tmp && test -d /var/tmp && printf "EXACT_OWNED_PATHS_ABSENT=PASS PARENT_POSITIVE_CONTROL=PASS\n"'
command('verify_absence',verify,True)
print('PREVIOUS_CLEANUP=PASS removed_roots=2 uploads=4 diagnostic_zips=8',flush=True)
print('RECOVERABLE: diagnostic ZIP backups at '+str(backup)+'; host runtime and probe artifacts retained',flush=True)
