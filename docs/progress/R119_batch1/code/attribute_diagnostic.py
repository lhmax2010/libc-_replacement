from common import *
import zipfile,json
p=TMP/'callback_gnu_8998_20260916202320.zip'
expected='e574958dfe2242f5307c97829ad66f08986555533424be60c8c9345fdf0a67b8'
assert digest(p)==expected
with zipfile.ZipFile(p)as z:
    prefix='callback_gnu_8998_20260916202320/proc/8998/'
    cmd=z.read(prefix+'cmdline').decode().replace('\0',' ')
    cwd=z.read(prefix+'cwd').decode()
assert '/opt/usr/home/owner/share/tmp/r119' in cmd and '/r119' in cwd
evidence=dict(pid=8998,sha256=expected,command=cmd,cwd=cwd,local_backup=str(p),remote='/opt/usr/share/crash/dump/'+p.name,attribution='与 W0 原始输出 PID 8998、callback_gnu、唯一 r119 路径一致；不按日期单独推断归属')
save(OUT/'W0/DIAGNOSTIC_ATTRIBUTION.json',evidence);print(json.dumps(evidence,ensure_ascii=False,indent=2))
