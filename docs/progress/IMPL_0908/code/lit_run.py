#!/usr/bin/env python3
"""单架构官方测试，30 秒资源/连接/截止点检查；输出 JSON 由 lit 生成。"""
import datetime,json,os,pathlib,shlex,signal,subprocess,sys,time
r=pathlib.Path.cwd();arch,variant,label=sys.argv[1:4];pattern=sys.argv[4] if len(sys.argv)>4 else None
base=r/'docs/progress/IMPL_0908/W3';raw=base/'raw'/('lit_'+arch+'_'+variant+'_'+label);raw.mkdir(parents=True,exist_ok=True)
suffix='native' if arch=='x86_64' else 'armv7l'
build=r/'tmp/IMPL_0908'/('build-'+('baseline-' if variant=='old' else '')+suffix)
src=r/'tmp/IMPL_0908'/('source' if variant=='new' else 'baseline-source')
executor=r/'docs/progress/IMPL_0908/code/lit_executor.py'
jobs=os.environ.get('IMPL_LIT_JOBS','2');assert jobs in ('1','2')
cmd=['nice','-n','15','ionice','-c','3','python3',str(src/'llvm/utils/lit/lit.py'),'-j',jobs,'-D','std=c++26','--timeout=240','--order=lexical','--show-unsupported','--show-skipped','--time-tests','-v','-o',str(raw/'result.json')]
if arch=='armv7l':cmd+=['-D','compiler=/home/toolchain/development/libc++_replacement/progress/R68/tools/armv7l_lit_host_clangxx.sh','-D','executor=python3 '+str(executor)]
if pattern:cmd+=['--filter',pattern]
if os.environ.get('IMPL_LIT_ARGS_FILE'):
 args_file=pathlib.Path(os.environ['IMPL_LIT_ARGS_FILE']).resolve();assert args_file.is_file()
 cmd+=['@'+str(args_file)]
cmd+=[str(build/'libcxx/test'),str(build/'libcxxabi/test')]
deadline=datetime.datetime.fromisoformat(json.loads((base/'raw/001_resource.time.json').read_text())['start']).timestamp()+6*3600
env=os.environ.copy();env['IMPL_LIT_LOG']=str(raw/'executor');env['IMPL_LIT_VARIANT']=variant
(raw/'command.txt').write_text('cwd: '+str(r)+'\ncommand: '+shlex.join(cmd)+'\nIMPL_LIT_LOG='+env['IMPL_LIT_LOG']+'\nIMPL_LIT_VARIANT='+variant+'\n')
start=datetime.datetime.now(datetime.timezone.utc).isoformat();reason='COMPLETE'
with (raw/'stdout').open('xb') as out,(raw/'stderr').open('xb') as err:
 proc=subprocess.Popen(cmd,stdout=out,stderr=err,env=env,start_new_session=True)
 while True:
  try:rc=proc.wait(timeout=30);break
  except subprocess.TimeoutExpired:
   mem=int(next(s for s in pathlib.Path('/proc/meminfo').read_text().splitlines() if s.startswith('MemAvailable:')).split()[1])
   with (raw/'resources.tsv').open('a') as f:f.write(f'{time.time()}\t{mem}\t{os.getloadavg()}\n')
   if (raw/'executor/CONNECTION_FAILURE').exists():reason='CONNECTION_FAILURE'
   elif time.time()>=deadline-180:reason='TASK_TIME_LIMIT' # reserve bounded stop/cleanup
   elif mem<4*1024*1024:reason='RESOURCE_STOP'
   else:continue
   os.killpg(proc.pid,signal.SIGINT)
   try:rc=proc.wait(timeout=10)
   except subprocess.TimeoutExpired:
    os.killpg(proc.pid,signal.SIGTERM)
    try:rc=proc.wait(timeout=10)
    except subprocess.TimeoutExpired:os.killpg(proc.pid,signal.SIGKILL);rc=proc.wait()
   break
(raw/'exitcode').write_text(str(rc)+'\n')
(raw/'time.json').write_text(json.dumps({'start':start,'end':datetime.datetime.now(datetime.timezone.utc).isoformat(),'reason':reason},indent=2)+'\n')
print('LIT_RUN',arch,variant,label,'exit',rc,'reason',reason,'json_present',(raw/'result.json').exists())
raise SystemExit(rc if rc>=0 else 128-rc)
