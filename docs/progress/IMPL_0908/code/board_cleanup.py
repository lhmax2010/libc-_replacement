#!/usr/bin/env python3
"""仅在全部实板测试结束后清理本批的明确目录；先检查活跃引用及摘要。"""
import hashlib,pathlib,re,subprocess,sys
r=pathlib.Path.cwd();raw=r/'docs/progress/IMPL_0908/W3/raw'
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
for d in raw.glob('lit_armv7l_*'):
 assert (d/'exitcode').exists(),('test still running',d)
 if (d/'executor/CONNECTION_FAILURE').exists():
  checked={'lit_armv7l_new_full':'088_board_recovery.stdout','lit_armv7l_new_resume1':'099_board_recovery2.stdout'}
  assert d.name in checked and 'RECOVERY_CHECK=PASS' in (raw/checked[d.name]).read_text(),('connection failure requires review',d)
def shell(label,cmd):
 stem=raw/('cleanup_'+label)
 command=[*sdb,'shell',cmd+'; impl_rc=$?; printf "\\nREMOTE_EXIT=%d\\n" "$impl_rc"']
 done=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'timeout','45',*command])
 text=pathlib.Path(str(stem)+'.stdout').read_text()
 assert done.returncode==0 and re.search(r'REMOTE_EXIT=0\s*$',text),('remote failure; stop',label)
 return text
roots=['/var/tmp/impl_0908','/opt/usr/impl_0908_lit']
uploads=['/opt/usr/home/owner/share/tmp/impl_0908_payload.tar',
         '/opt/usr/home/owner/share/tmp/impl_0908_old_timed',
         '/opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar',
         '/opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar']
text=shell('01_processes','for impl_proc in /proc/[0-9]*/exe /proc/[0-9]*/cwd; do impl_path=$(readlink "$impl_proc") || continue; printf "%s\\t%s\\n" "$impl_proc" "$impl_path"; done; true')
def owned(path):return any(path==p or path.startswith(p+'/') for p in roots)
assert owned('/opt/usr/impl_0908_lit/runs/positive/t.tmp.exe')
assert owned('/var/tmp/impl_0908/probes/wait')
assert not owned('/usr/bin/other')
assert any('/exe\t' in s for s in text.splitlines()),'process scan needs positive observation'
active=[s for s in text.splitlines() if '\t' in s and owned(s.split('\t',1)[1])]
assert not active,('active task processes: refuse cleanup',active)
print('ACTIVE_TASK_REFERENCES=0 POSITIVE_CONTROL=PASS',flush=True)
shell('02_inventory','ls -ld '+' '.join(roots+uploads)+' && cd /var/tmp/impl_0908 && sha256sum -c SHA256SUMS')
upload_listing=shell('02b_upload_inventory',"find /opt/usr/home/owner/share/tmp -maxdepth 1 -name 'impl_0908_*' -print")
found={s for s in upload_listing.splitlines() if s.startswith('/opt/usr/home/owner/share/tmp/')}
assert found==set(uploads),('unexpected task upload: preserve for inspection',found)
paths=['/opt/usr/impl_0908_lit/runtime/libc++.so.1','/opt/usr/impl_0908_lit/runtime/libc++abi.so.1',
       '/opt/usr/impl_0908_lit/baseline/libc++.so.1','/opt/usr/impl_0908_lit/baseline/libc++abi.so.1',
       '/var/tmp/impl_0908/probes/old_timed',uploads[1]]
actual=shell('03_final_hashes','sha256sum '+' '.join(paths))
expected={}
for variant,build in [('runtime','build-armv7l'),('baseline','build-baseline-armv7l')]:
 for name in ['libc++.so.1','libc++abi.so.1']:
  expected[f'/opt/usr/impl_0908_lit/{variant}/{name}']=hashlib.sha256((r/'tmp/IMPL_0908'/build/'lib'/name).read_bytes()).hexdigest()
old_hash=hashlib.sha256((r/'tmp/IMPL_0908/probes-armv7l/old_timed').read_bytes()).hexdigest()
expected[paths[-2]]=old_hash;expected[paths[-1]]=old_hash
observed={line.split()[1]:line.split()[0] for line in actual.splitlines() if re.match(r'^[0-9a-f]{64}\s',line)}
assert observed==expected,(observed,expected)
print('FINAL_DEPLOYED_HASHES=PASS',flush=True)
guards=' && '.join('test ! -L '+p for p in roots+uploads)
shell('04_remove',guards+' && rm -rf /var/tmp/impl_0908 /opt/usr/impl_0908_lit && rm -f /opt/usr/home/owner/share/tmp/impl_0908_payload.tar /opt/usr/home/owner/share/tmp/impl_0908_old_timed /opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar /opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar')
shell('05_verify',' && '.join('test ! -e '+p for p in roots+uploads)+' && test -d /opt/usr/home/owner/share/tmp && test -d /var/tmp && printf "OWNED_PATHS_ABSENT=PASS PARENT_POSITIVE_CONTROL=PASS\\n"')
print('BOARD_CLEANUP=PASS; only listed task directories/uploads removed; host artifacts retained')
