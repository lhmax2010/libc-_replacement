#!/usr/bin/env python3
"""仅增添本批旧定时探针；上传前检查目标不存在，上传后比对摘要。"""
import hashlib,pathlib,re,subprocess,sys
r=pathlib.Path.cwd();raw=r/'docs/progress/IMPL_0908/W3/raw'
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
shared='/opt/usr/home/owner/share/tmp/impl_0908_old_timed'
target='/var/tmp/impl_0908/probes/old_timed'
local=r/'tmp/IMPL_0908/probes-armv7l/old_timed'
expected=hashlib.sha256(local.read_bytes()).hexdigest()
def run(label,cmd,remote=False):
 stem=raw/('timed_deploy_'+label)
 if remote:cmd=[*sdb,'shell',cmd+'; impl_rc=$?; printf "\\nREMOTE_EXIT=%d\\n" "$impl_rc"']
 result=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'timeout','45',*cmd])
 text=pathlib.Path(str(stem)+'.stdout').read_text()
 assert result.returncode==0,('carrier failed; STOP',label)
 if remote:assert re.search(r'REMOTE_EXIT=0\s*$',text),('remote failure; STOP',label)
 return text
run('01_preflight','test -d /var/tmp/impl_0908/probes && test ! -e '+target+' && test ! -e '+shared,True)
run('02_push',[*sdb,'push',str(local),shared])
text=run('03_install','test -f '+shared+' && cp '+shared+' '+target+' && chmod 755 '+target+' && sha256sum '+shared+' '+target,True)
lines=[line for line in text.splitlines() if re.match(r'^[0-9a-f]{64}\s',line)]
assert len(lines)==2 and all(line.split()[0]==expected for line in lines)
print('TIMED_DEPLOY=PASS SHA256='+expected)
