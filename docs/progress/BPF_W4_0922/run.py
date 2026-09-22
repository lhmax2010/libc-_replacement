import datetime,json,pathlib,shlex,subprocess,sys
P=pathlib.Path(__file__).resolve().parent/'raw';P.mkdir(parents=True,exist_ok=True)
name,*cmd=sys.argv[1:];b=P/name
assert not pathlib.Path(str(b)+'.command.txt').exists()
for ext,value in [('command.txt',shlex.join(cmd)+'\n'),('argv.json',json.dumps(cmd,ensure_ascii=False,indent=2)),('started.txt',datetime.datetime.now().astimezone().isoformat())]:pathlib.Path(str(b)+'.'+ext).write_text(value)
with pathlib.Path(str(b)+'.stdout.txt').open('xb') as out,pathlib.Path(str(b)+'.stderr.txt').open('xb') as err:r=subprocess.run(cmd,stdout=out,stderr=err)
pathlib.Path(str(b)+'.exitcode').write_text(str(r.returncode)+'\n');pathlib.Path(str(b)+'.finished.txt').write_text(datetime.datetime.now().astimezone().isoformat())
print(name,'exitcode',r.returncode)
for ext in ('stdout.txt','stderr.txt'):print(pathlib.Path(str(b)+'.'+ext).read_text(errors='replace')[:10000])
sys.exit(r.returncode)
