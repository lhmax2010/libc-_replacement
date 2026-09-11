#!/usr/bin/env python3
import pathlib,sys,hashlib,json,shlex,re
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,record
o=r/'docs/progress/ADAPTOR_0912/W1';t=r/'tmp/ADAPTOR_0912/W1'
remote='/opt/usr/home/owner/share/tmp/adaptor_0912_w1';sdb=['sdb','-s','192.168.108.26:26101']
hal='--hal' in sys.argv
prefix='hal_' if hal else ''
def rec(tag,cmd):return record('W1',prefix+tag,cmd)
def run(tag,cmd):
 rc,out=rec(tag,['timeout','45',*sdb,'shell','{ '+cmd+'; }; task_rc=$?; printf "\\nTARGET_EXIT=%s\\n" "$task_rc"; exit "$task_rc"'])
 m=re.findall(r'TARGET_EXIT=(\d+)',out)
 if rc or not m:raise RuntimeError('连接中断或目标结束标记缺失；停止板上工作，不重试：'+tag)
 return int(m[-1]),out
assert run('017_board_create','test ! -e '+remote+' && mkdir '+remote)[0]==0
files=[(t/'offscreen_probe','offscreen_probe'),(r/'tmp/IMPL_0908/build-armv7l/lib/libc++.so.1.0','libc++.so.1'),(r/'tmp/IMPL_0908/build-armv7l/lib/libc++abi.so.1.0','libc++abi.so.1')]
manifest=[]
for p,n in files:
 manifest.append(dict(local=str(p),remote=remote+'/'+n,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
 assert rec('upload_'+n,[*sdb,'push',p,remote+'/'+n])[0]==0
(o/(prefix+'board_uploads.json')).write_text(json.dumps(manifest,indent=2)+'\n')
rc,out=run('018_board_hashes','sha256sum '+' '.join(x['remote'] for x in manifest));assert rc==0
for x in manifest:assert x['sha256']+'  '+x['remote'] in out
assert run('019_board_mode','chmod 755 '+remote+'/offscreen_probe')[0]==0
rows=[]
for i in range(1,6):
 rc,out=run('offscreen_physical_'+str(i),'cd '+remote+' && XDG_RUNTIME_DIR=/run WAYLAND_DISPLAY=wayland-0 LD_LIBRARY_PATH='+remote+(':/hal/lib/driver:/hal/lib' if hal else '')+' nice -n 15 ionice -c 3 ./offscreen_probe /usr/lib/libdali2-csharp-binder.so.2.0.0')
 rows.append(dict(repeat=i,architecture='armv7l',environment='物理板',target_exit=rc,started='OFFSCREEN_START_BEGIN' in out,finished='OFFSCREEN_TERMINATE_RETURNED' in out,view='BINDER_VIEW_NONNULL=1' in out))
(o/(prefix+'offscreen_results.json')).write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
assert run('020_board_cleanup','rm -- '+' '.join(x['remote'] for x in manifest)+' && rmdir '+remote+' && test ! -e '+remote)[0]==0
