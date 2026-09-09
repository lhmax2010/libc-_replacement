#!/usr/bin/env python3
"""只部署此前同摘要的两份运行库；所有远端命令验证真实退出码。"""
import csv,pathlib,re,subprocess,sys
r=pathlib.Path.cwd(); out=r/'docs/progress/IMPL_0909/W2';raw=out/'raw'
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
def run(label,cmd,shell=False):
    stem=raw/('resume_deploy_'+label)
    if shell:cmd=sdb+['shell',cmd+'; rc=$?; printf "\n__RESUME_RC__=%d\n" "$rc"']
    rc=subprocess.call([sys.executable,str(r/'docs/progress/R114/code/record.py'),str(stem),'timeout','60',*cmd]);assert rc==0,(label,rc)
    text=pathlib.Path(str(stem)+'.stdout').read_text().replace('\r\n','\n')
    if shell:assert re.search(r'\n__RESUME_RC__=0\n?$',text),text
    return text
assert 'BOARD_PREDEPLOY=PASS' in (raw/'025_resume_preflight.stdout').read_text()
run('00_initial_diagnostics','find /opt/usr/share/crash/dump -maxdepth 1 -name "*.zip" -printf "%f\n"',True)
run('01_prepare','test ! -e /opt/usr/impl_0908_lit && test ! -L /opt/usr/impl_0908_lit && mkdir -p /opt/usr/impl_0908_lit/runtime /opt/usr/impl_0908_lit/runs /opt/usr/impl_0908_lit/fixtures && chown owner /opt/usr/impl_0908_lit/fixtures',True)
for i,row in enumerate(csv.DictReader((out/'RESUME_RUNTIME_IDENTITIES.tsv').open(),delimiter='\t')):
    if '/runtime/' not in row['board']:continue
    upload='/opt/usr/home/owner/share/tmp/impl_0909_'+pathlib.Path(row['board']).name
    run(f'{i}_upload_absent','test ! -e '+upload+' && test ! -L '+upload,True)
    run(f'{i}_push',sdb+['push',row['local'],upload])
    text=run(f'{i}_install','cp '+upload+' '+row['board']+' && sha256sum '+row['board']+' && rm -f -- '+upload,True)
    assert row['sha256']+'  '+row['board'] in text,('BOARD_RUNTIME_MISMATCH_STOP',row)
print('BOARD_LOCAL_PREVIOUS_SHA256=PASS libc++ and libc++abi; no runtime build',flush=True)
