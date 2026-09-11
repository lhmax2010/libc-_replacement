#!/usr/bin/env python3
"""物理板，真实已安装 Chromium provider；确切清单上传、核验与清理。"""
import pathlib,sys,json,re,hashlib
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,record
o=r/'docs/progress/ADAPTOR_0912/W2';t=r/'tmp/ADAPTOR_0912/W2/v2/armv7l'
remote='/opt/usr/home/owner/share/tmp/adaptor_0912_w2';sdb=['sdb','-s','192.168.108.26:26101'];connected=True
def run(tag,cmd):
 global connected
 rc,out=record('W2',tag,['timeout','45',*sdb,'shell','{ '+cmd+'; }; task_rc=$?; printf "\\nTARGET_EXIT=%s\\n" "$task_rc"; exit "$task_rc"'])
 m=re.findall(r'TARGET_EXIT=(\d+)',out)
 if rc or not m:
  connected=False
  raise RuntimeError('连接或远端结束标记异常；本脚本停止，不重试，不再删除：'+tag)
 return int(m[-1]),out
assert run('010_board_precheck','uname -m; test ! -e /opt/usr/home/owner/share/tmp/adaptor_0912_w1 && test ! -e /opt/usr/home/owner/share/tmp/api_w1_0911_1050 && test ! -e '+remote)[0]==0
rc,before=run('011_diagnostics_before','ls -1 /opt/usr/share/crash/dump');assert rc==0
assert run('012_board_create','mkdir '+remote)[0]==0
b=r/'tmp/IMPL_0908/build-armv7l/lib'
files=[(b/'libc++.so.1.0','libc++.so.1'),(b/'libc++abi.so.1.0','libc++abi.so.1')]+[(t/x,x) for x in ['probe_gnu','probe_cxx','state.so','drop.so']]
uploads=[];rows=[]
try:
 for p,n in files:
  x=dict(local=str(p),remote=remote+'/'+n,sha256=hashlib.sha256(p.read_bytes()).hexdigest());uploads.append(x)
  assert record('W2','board_upload_'+n,[*sdb,'push',p,x['remote']])[0]==0
 rc,out=run('013_board_hashes','sha256sum '+' '.join(x['remote'] for x in uploads));assert rc==0
 for x in uploads:assert x['sha256']+'  '+x['remote'] in out
 provider='/usr/share/chromium-efl/lib/libchromium-impl.so'
 rc,out=run('014_provider_identity','sha256sum '+provider+' /usr/lib/libstdc++.so.6; rpm -q chromium-efl elementary-tizen');assert rc==0
 assert 'c135dbad06ad82dbb7ded9311552ec25b9975d2b94e47753032a2104065683fa' in out
 assert run('015_board_mode','chmod 755 '+remote+'/probe_gnu '+remote+'/probe_cxx')[0]==0
 for kind,mode in [('gnu','direct'),('cxx','state'),('cxx','drop')]:
  for i in range(1,6):
   tag=f'physical_{mode}_{i}';entry=provider if mode=='direct' else remote+'/'+mode+'.so'
   rc,out=run(tag,'cd '+remote+' && LD_LIBRARY_PATH='+remote+':/usr/share/chromium-efl/lib nice -n 15 ionice -c 3 ./probe_'+kind+' '+entry+' '+provider)
   rows.append(dict(arch='armv7l',environment='物理板',path=mode,repeat=i,exit=rc,maps='MAP_ASSERT cxx=1 gnu=1 entry=1 provider=1' in out,summary=re.findall(r'SUMMARY EXACT=(\d+)/12 DESTROYED=(\d+)/12',out),cases=[dict(case=int(k),valid=int(v),exact=int(x)) for k,v,x in re.findall(r'CASE=(\d+) VALID=(\d+) EXACT=(\d+)',out)],raw=tag))
finally:
 (o/'board_uploads.json').write_text(json.dumps(uploads,indent=2)+'\n');(o/'board_matrix.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
 if connected:
  rc,out=run('016_board_cleanup','rm -- '+' '.join(x['remote'] for x in uploads)+' && rmdir '+remote+' && test ! -e '+remote);assert rc==0
  rc,after=run('017_diagnostics_after','ls -1 /opt/usr/share/crash/dump');assert rc==0
  clean=lambda text:{x.strip() for x in text.splitlines() if x.strip() and not x.startswith('TARGET_EXIT=')}
  delta=sorted(clean(after)-clean(before))
  (o/'board_cleanup.json').write_text(json.dumps(dict(directory=remote,removed_uploads=len(uploads),directory_absent=True,new_diagnostic_names=delta,board_released=True),ensure_ascii=False,indent=2)+'\n')
  if delta:raise RuntimeError('有新增诊断文件，不能宣称清理完成，需按名称归属核查')
