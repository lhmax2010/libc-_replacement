#!/usr/bin/env python3
"""复用原编译配置及执行器，仅运行精确缺项；预留清理时间。"""
import datetime,json,os,pathlib,shlex,signal,subprocess,time
r=pathlib.Path.cwd();out=r/'docs/progress/IMPL_0909/W2';raw=out/'raw/lit_resume3';raw.mkdir(exist_ok=False)
assert 'CURRENT_VS_PREVIOUS_COMPILER_STANDARD_OPTIMIZATION_LINK_FLAGS=EXACT_MATCH' in (out/'raw/029_check_current_configuration.stdout').read_text()
start=datetime.datetime.now(datetime.timezone.utc)
resume_start=datetime.datetime.fromisoformat(json.loads((out/'raw/015_resume_resource.time.json').read_text())['start']).timestamp()
deadline=resume_start+(6*3600-40*60) # account for approximately 40 minutes already used
env=os.environ.copy();env['IMPL_LIT_LOG']=str(raw/'executor');env['IMPL_LIT_VARIANT']='new'
cmd=['nice','-n','15','ionice','-c','3','python3',str(r/'tmp/IMPL_0908/source/llvm/utils/lit/lit.py'),'-j','1','-D','std=c++26','--timeout=240','--order=lexical','--show-unsupported','--show-skipped','--time-tests','-v','-o',str(raw/'result.json'),'-D','compiler=/home/toolchain/development/libc++_replacement/progress/R68/tools/armv7l_lit_host_clangxx.sh','-D','executor=python3 '+str(r/'docs/progress/IMPL_0909/W2/lit_executor.py'),'@'+str(out/'resume3.args'),str(r/'tmp/IMPL_0908/build-armv7l/libcxx/test'),str(r/'tmp/IMPL_0908/build-armv7l/libcxxabi/test')]
(raw/'command.txt').write_text('cwd: '+str(r)+'\ncommand: '+shlex.join(cmd)+'\nIMPL_LIT_LOG='+env['IMPL_LIT_LOG']+'\nIMPL_LIT_VARIANT=new\n')
reason='COMPLETE'
with (raw/'stdout').open('xb') as stdout,(raw/'stderr').open('xb') as stderr:
    p=subprocess.Popen(cmd,stdout=stdout,stderr=stderr,env=env,start_new_session=True)
    while True:
        try:rc=p.wait(timeout=30);break
        except subprocess.TimeoutExpired:
            mem=int(next(x for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemAvailable:')).split()[1])
            with (raw/'resources.tsv').open('a') as f:f.write(f'{time.time()}\t{mem}\t{os.getloadavg()}\n')
            print('HEARTBEAT',datetime.datetime.now(datetime.timezone.utc).isoformat(),'available_kib',mem,flush=True)
            if (raw/'executor/CONNECTION_FAILURE').exists():reason='CONNECTION_FAILURE'
            elif time.time()>=deadline-300:reason='TASK_TIME_LIMIT'
            elif mem<4*1024*1024:reason='RESOURCE_STOP'
            else:continue
            os.killpg(p.pid,signal.SIGINT)
            try:rc=p.wait(timeout=10)
            except subprocess.TimeoutExpired:
                os.killpg(p.pid,signal.SIGTERM)
                try:rc=p.wait(timeout=10)
                except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);rc=p.wait()
            break
(raw/'exitcode').write_text(str(rc)+'\n')
(raw/'time.json').write_text(json.dumps({'start':start.isoformat(),'end':datetime.datetime.now(datetime.timezone.utc).isoformat(),'reason':reason,'deadline_utc':datetime.datetime.fromtimestamp(deadline,datetime.timezone.utc).isoformat(),'prior_used_seconds':2400},indent=2)+'\n')
print('RESUME_FINISHED',rc,reason,'RESULT_PRESENT',(raw/'result.json').exists(),flush=True)
raise SystemExit(rc if rc>=0 else 128-rc)
