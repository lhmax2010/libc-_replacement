"""Record command argv, output and exit status without shell interpolation."""
import datetime, json, pathlib, shlex, subprocess, sys
p=pathlib.Path(__file__).resolve().parent/'raw'; p.mkdir(exist_ok=True)
name,*cmd=sys.argv[1:]; base=p/name
assert not base.with_suffix('.command.txt').exists()
for ext,value in [('command.txt',shlex.join(cmd)+'\n'),('argv.json',json.dumps(cmd,ensure_ascii=False,indent=2)),('started.txt',datetime.datetime.now().astimezone().isoformat()+'\n')]:
    pathlib.Path(str(base)+'.'+ext).write_text(value)
with pathlib.Path(str(base)+'.stdout.txt').open('xb') as out, pathlib.Path(str(base)+'.stderr.txt').open('xb') as err:
    r=subprocess.run(cmd,stdout=out,stderr=err)
pathlib.Path(str(base)+'.exitcode').write_text(str(r.returncode)+'\n')
pathlib.Path(str(base)+'.finished.txt').write_text(datetime.datetime.now().astimezone().isoformat()+'\n')
print(name,'exitcode',r.returncode)
for ext in ('stdout.txt','stderr.txt'):
    print(pathlib.Path(str(base)+'.'+ext).read_text(errors='replace')[:10000])
sys.exit(r.returncode)
