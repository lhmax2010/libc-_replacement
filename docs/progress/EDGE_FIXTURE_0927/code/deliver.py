"""只向运行时材料分支保存；Git add/commit/push 使用普通优先级。"""
import argparse,datetime,json,os,pathlib,resource,shlex,subprocess,time
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=ROOT/'docs/progress/EDGE_FIXTURE_0927';LOG=OUT/'delivery';LOG.mkdir(exist_ok=True)
mem=int(next(x.split()[1] for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
resource.setrlimit(resource.RLIMIT_AS,(mem*30//100,mem*30//100))
ap=argparse.ArgumentParser();ap.add_argument('label');ap.add_argument('args',nargs=argparse.REMAINDER);a=ap.parse_args()
assert a.args and a.args[0]=='git' and '--force' not in a.args and '-f' not in a.args
if 'push' in a.args:assert a.args[-2:]==['origin','codex/runtime-validation']
cmd=['git','-c','gc.auto=0','-c','pack.threads=1','-c','pack.windowMemory=32m']+a.args[1:]
# Only saving operations are normal priority. Read-only git checks stay light.
normal=any(x in ('add','commit','push') for x in a.args[1:])
if not normal:cmd=['nice','-n','19','ionice','-c','3']+cmd
base=LOG/(str(time.time_ns())+'_'+a.label)
base.with_suffix('.command.txt').write_text('cwd='+shlex.quote(str(ROOT))+'\n'+shlex.join(cmd)+'\n')
t=time.time()
with base.with_suffix('.stdout').open('wb') as o,base.with_suffix('.stderr').open('wb') as e:
 p=subprocess.run(cmd,cwd=ROOT,stdout=o,stderr=e)
base.with_suffix('.exitcode').write_text(str(p.returncode)+'\n')
base.with_suffix('.time.json').write_text(json.dumps(dict(start_utc=datetime.datetime.fromtimestamp(t,datetime.timezone.utc).isoformat(),elapsed=time.time()-t,normal_git_save_priority=normal,rlimit_as=mem*30//100),indent=2)+'\n')
print(base.with_suffix('.stdout').read_text(errors='replace'));print(base.with_suffix('.stderr').read_text(errors='replace'));raise SystemExit(p.returncode)
