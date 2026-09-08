#!/usr/bin/env python3
"""独立 owner 夹具的物理板 lit 执行器；每次保存原命令、双流及真实退出码。"""
import argparse,hashlib,io,os,pathlib,re,shlex,subprocess,sys,tarfile,tempfile,uuid
ROOT='/opt/usr/impl_0908_lit'
VARIANT=os.environ.get('IMPL_LIT_VARIANT','new')
assert VARIANT in ('old','new')
RUNTIME=ROOT+('/runtime' if VARIANT=='new' else '/baseline')
SDB=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
p=argparse.ArgumentParser();p.add_argument('--execdir',required=True);p.add_argument('--env',nargs='*',default=[]);p.add_argument('--prepend_env',nargs='*',default=[]);p.add_argument('command',nargs='+');args=p.parse_args()
run_id='run-'+uuid.uuid4().hex
remote=ROOT+'/runs/'+run_id;shared='/opt/usr/home/owner/share/tmp/impl_0908_'+run_id+'.tar'
raw=pathlib.Path(os.environ['IMPL_LIT_LOG']);raw.mkdir(parents=True,exist_ok=True)
log=(raw/(run_id+'.log')).open('xb');fault=raw/'CONNECTION_FAILURE'
def call(command,test=False):
 log.write(('COMMAND='+shlex.join(command)+'\n').encode());log.flush()
 try:done=subprocess.run(command,stdin=subprocess.DEVNULL,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=180)
 except subprocess.TimeoutExpired as e:
  log.write(b'STDOUT\n'+(e.stdout or b'')+b'\nSTDERR\n'+(e.stderr or b'')+b'\nEXITCODE=NOT_OBSERVED HOST_TIMEOUT=180\n');log.flush()
  if not test:fault.touch()
  raise
 log.write(b'STDOUT\n'+done.stdout+b'\nSTDERR\n'+done.stderr+('\nEXITCODE='+str(done.returncode)+'\n').encode());log.flush()
 if done.returncode: fault.touch();raise RuntimeError('SDB carrier failure')
 return done.stdout.decode(errors='replace').replace('\r\n','\n')
def shell(command,test=False):
 text=call(SDB+['shell',command+'; impl_rc=$?; printf "\n__IMPL_EXIT__=%d\n" "$impl_rc"'],test)
 m=re.search(r'\n__IMPL_EXIT__=(\d+)\n?$',text)
 if not m:fault.touch();raise RuntimeError('remote exit marker absent')
 return int(m[1]),text[:m.start()]
if fault.exists():raise SystemExit('CONNECTION_FAILURE already recorded; refusing new board work')
local=pathlib.Path(args.execdir).resolve();assert local.is_dir()
stdin=b'' if sys.stdin.isatty() else sys.stdin.buffer.read()
with tempfile.TemporaryDirectory(prefix='impl0908-lit-') as temp:
 archive=pathlib.Path(temp)/'payload.tar'
 with tarfile.open(archive,'w') as f:
  f.add(local,arcname='test');item=tarfile.TarInfo('test/.stdin');item.size=len(stdin);item.mode=0o644;f.addfile(item,io.BytesIO(stdin))
 try:
  call(SDB+['push',str(archive),shared])
  setup=' && '.join(['test -d '+ROOT,'mkdir '+remote,'tar -xf '+shared+' -C '+remote+' --strip-components 1','chown -R owner '+remote,'chmod u+rwx '+remote])
  rc,text=shell(setup)
  if rc:raise RuntimeError('setup failed: '+text)
  def mapped(s):return remote+s[len(str(local)):] if s.startswith(str(local)+'/') else s
  env={'LD_LIBRARY_PATH':RUNTIME,'TMPDIR':ROOT+'/fixtures','TMP':ROOT+'/fixtures','TEMP':ROOT+'/fixtures'}
  env.update(x.split('=',1) for x in args.env)
  for item in args.prepend_env:
   k,v=item.split('=',1);env[k]=v+(':'+env[k] if k in env else '')
  # The experimental runtime must be first even if the test specifies search paths.
  if not env['LD_LIBRARY_PATH'].startswith(RUNTIME):env['LD_LIBRARY_PATH']=RUNTIME+':'+env['LD_LIBRARY_PATH']
  owner='ulimit -c 0; cd '+remote+' && '+shlex.join(['env',*[k+'='+v for k,v in env.items()],'nice','-n','15','ionice','-c','3',*map(mapped,args.command)])+' < '+remote+'/.stdin'
  launch='printf "%d\\n" "$$" > '+remote+'/.pgid; exec su owner -c '+shlex.quote(owner)
  rc,text=shell('setsid sh -c '+shlex.quote(launch),test=True)
  sys.stdout.write(text)
  result=rc
 except subprocess.TimeoutExpired:
  stop='p=$(cat '+remote+'/.pgid); case "$p" in ""|*[!0-9]*) exit 92;; esac; test "$p" -gt 1 && pkill -KILL -g "$p"; true'
  rc,text=shell(stop)
  sys.stderr.write('TEST_TIMEOUT=180 remote process group stopped\n'+text);result=124
 except Exception as e:
  sys.stderr.write('EXECUTOR_ERROR: '+str(e)+'\n');result=90
 finally:
  if not fault.exists():
   rc,text=shell('rm -rf '+shlex.quote(remote)+' && rm -f '+shlex.quote(shared))
   if rc:sys.stderr.write('CLEANUP_ERROR '+text+'\n');result=91
log.close();raise SystemExit(result)
