#!/usr/bin/env python3
"""删除后的独立只读短命令核验；不重复删除，避免 SDB 服务字符串长度限制。"""
import csv,hashlib,pathlib,re,shlex,subprocess,sys
r=pathlib.Path.cwd();out=r/'docs/progress/IMPL_0909/W2';raw=out/'raw'
assert '__CLEANUP_RC__=0' in (raw/'previous_cleanup_30_remove_exact_owned_paths.stdout').read_text()
rows=list(csv.DictReader((out/'PREVIOUS_CRASH_BACKUPS.tsv').open(),delimiter='\t'));assert len(rows)==8
for x in rows:
 assert re.fullmatch(r'/opt/usr/share/crash/dump/t\.tmp\.exe_[0-9]+_[0-9]{14}\.zip',x['board_path'])
 assert hashlib.sha256(pathlib.Path(x['host_backup']).read_bytes()).hexdigest()==x['sha256']
paths=['/var/tmp/impl_0908','/opt/usr/impl_0908_lit',
 '/opt/usr/home/owner/share/tmp/impl_0908_payload.tar','/opt/usr/home/owner/share/tmp/impl_0908_old_timed',
 '/opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar',
 '/opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar',*[x['board_path'] for x in rows]]
results=[]
for i in range(0,len(paths),4):
 batch=paths[i:i+4]
 cmd='bad=0; for p in '+' '.join(map(shlex.quote,batch))+'; do if test -e "$p" || test -L "$p"; then printf "PRESENT %s\n" "$p"; bad=1; else printf "ABSENT %s\n" "$p"; fi; done; test "$bad" = 0 && test -d /opt/usr/share/crash/dump && test -d /opt/usr/home/owner/share/tmp && test -d /var/tmp && printf "PARENT_POSITIVE_CONTROL=PASS\n"'
 cmd+='; verify_rc=$?; printf "\n__VERIFY_RC__=%s\n" "$verify_rc"'
 assert len(cmd)<1600
 stem=raw/f'cleanup_absence_batch_{i//4+1}'
 rc=subprocess.run([sys.executable,str(r/'docs/progress/R114/code/record.py'),str(stem),'timeout','30',
   '/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101','shell',cmd]).returncode
 text=pathlib.Path(str(stem)+'.stdout').read_text().replace('\r\n','\n')
 assert rc==0 and re.search(r'__VERIFY_RC__=0\s*$',text) and 'PARENT_POSITIVE_CONTROL=PASS' in text
 assert [s[7:] for s in text.splitlines() if s.startswith('ABSENT ')]==batch
 results.extend([[p,'ABSENT',str(stem.relative_to(out))+'.stdout'] for p in batch])
with (out/'PREVIOUS_CLEANUP_VERIFICATION.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','observed_state','evidence']);w.writerows(results)
print('PREVIOUS_CLEANUP=PASS verified_absent=14 backup_hashes=8/8 parent_controls=PASS')
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
