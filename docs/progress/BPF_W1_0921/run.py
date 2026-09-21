import datetime,pathlib,shlex,subprocess,sys
base=pathlib.Path(__file__).resolve().parent/'raw';base.mkdir(exist_ok=True)
label,*cmd=sys.argv[1:]
def save(ext,value):
 p=base/(label+'.'+ext);assert not p.exists();p.write_text(value)
save('command.txt',shlex.join(cmd)+'\n');save('started.txt',datetime.datetime.now().astimezone().isoformat()+'\n')
with (base/(label+'.stdout.txt')).open('xb') as out,(base/(label+'.stderr.txt')).open('xb') as err:
 r=subprocess.run(cmd,stdout=out,stderr=err)
save('exitcode',str(r.returncode)+'\n');save('finished.txt',datetime.datetime.now().astimezone().isoformat()+'\n')
print(label,'exitcode',r.returncode)
for ext in ('stdout.txt','stderr.txt'):print((base/(label+'.'+ext)).read_text(errors='replace')[:12000])
sys.exit(r.returncode)
