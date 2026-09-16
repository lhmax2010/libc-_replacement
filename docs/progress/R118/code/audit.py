#!/usr/bin/env python3
"""独立核对结果文件、次数与加载库名；不靠仓库路径中的 libc++ 字样判断。"""
import pathlib,json,re,datetime,collections
from run import ROOT,OUT,save,digest
summary={};issues=[]
for typ in ['string','vector','function']:
 cells=[];names=set();builds=0;failed=0;runs=0;maps_verified=0;no_maps=[];duration=0.;latest=None
 for file in sorted((OUT/typ).glob('*_matrix.json')):
  for row in json.loads(file.read_text()):
   key=(row['arch'],row['test'],row['library'])
   if key in names:issues.append('duplicate cell '+str(key))
   names.add(key);builds+=1
   measured=[row['build_raw']]+[r['raw']for r in row['runs']]
   for stem in measured:
    tm=json.loads(pathlib.Path(str(ROOT/stem)+'.time.json').read_text());a=datetime.datetime.fromisoformat(tm['start']);b=datetime.datetime.fromisoformat(tm['end']);duration+=(b-a).total_seconds();latest=b if latest is None else max(latest,b)
   if row['build_exit']!=0:failed+=1;continue
   if len(row['runs'])!=5:issues.append('repeat count '+str(key))
   for run in row['runs']:
    runs+=1;stem=ROOT/run['raw'];actual=int(pathlib.Path(str(stem)+'.exitcode').read_text())
    if row['arch']=='armv7l':
     txt=pathlib.Path(str(stem)+'.stdout').read_text();marker=re.findall(r'R118_TARGET_EXIT=(\d+)',txt)
     if not marker:issues.append('missing remote marker '+str(stem))
     else:actual=int(marker[-1])
    if actual!=run['exit'] or actual!=0:issues.append('run exit '+str(stem))
    raw=pathlib.Path(str(stem)+'.stdout').read_text()
    if 'LIB=' in raw:
     basenames=set(re.findall(r'/(lib(?:stdc\+\+|c\+\+(?:abi)?)\.so[^/\s]*)',raw))
     want='libstdc++.so' if row['library']=='gnu' else 'libc++.so'
     if not any(x.startswith(want) for x in basenames):issues.append('library basename missing '+str(stem))
     else:maps_verified+=1
    else:no_maps.append(run['raw'])
   if not row.get('repeat_results_identical',False):issues.append('repeat value differs '+str(key))
   cells.append(key)
 summary[typ]=dict(compile_cells=builds,compile_failures=failed,successful_cells=len(cells),run_count=runs,maps_basename_verified_runs=maps_verified,extension_runs_without_maps=no_maps,latest_unit_end=latest.isoformat(),unit_build_run_seconds=duration)
# 已有真实组件边界单独计数，不把未运行的链接失败或宿主调试算作运行格。
boundaries={}
for typ in ['string','vector']:
 rows=[]
 for file in (OUT/typ).glob('*_boundaries.json'):rows+=json.loads(file.read_text())
 boundaries[typ]=dict(build_cells=len(rows),link_failures=sum(x['build_exit']!=0 for x in rows),control_runs=sum(len(x['runs'])for x in rows),control_failures=sum(r['exit']!=0 for x in rows for r in x['runs']))
 for x in rows:
  for r in x['runs']:
   raw=pathlib.Path(str(ROOT/r['raw'])+'.stdout').read_text()
   if 'consumer_destroy_completed=1' not in raw or 'PROVIDER_MAP ' not in raw:issues.append('boundary incomplete '+r['raw'])
ewk=json.loads((OUT/'string/x86_64_ewk.json').read_text());boundaries['ewk']=dict(runs=len(ewk),gnu_pass=sum(x['consumer']=='gnu' and x['exit']==0 and x['exact']and x['destroyed'] for x in ewk),cxx_sigsegv=sum(x['consumer']=='cxx' and x['exit']==-11 for x in ewk),all_maps=all(x['maps']for x in ewk))
def timeval(stem,field):return datetime.datetime.fromisoformat(json.loads((OUT/'raw'/f'{stem}.time.json').read_text())[field])
times={}
for typ,start,end in [('string','001_resource_gate','033_string_commit'),('vector','037_vector_gate','052_vector_commit')]:
 a=timeval(start,'start');b=timeval(end,'end');times[typ]=dict(start=a.isoformat(),end=b.isoformat(),elapsed_seconds=(b-a).total_seconds(),complete_scope=False)
a=timeval('055_function_gate','start');b=datetime.datetime.now(datetime.timezone.utc);times['function']=dict(start=a.isoformat(),end=b.isoformat(),elapsed_seconds=(b-a).total_seconds(),complete_scope=False,end_is_precommit_audit=True)
save(OUT/'SELF_CHECK.json',dict(unit=summary,boundaries=boundaries,issues=issues));save(OUT/'TIMINGS.json',times)
print(json.dumps(dict(unit=summary,boundaries=boundaries,issues=issues,timings=times),ensure_ascii=False,indent=2))
if issues:raise SystemExit(1)
