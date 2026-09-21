import datetime,pathlib,shlex,subprocess,sys
p=pathlib.Path(__file__).resolve().parent/'raw';p.mkdir(parents=True,exist_ok=True)
label,*cmd=sys.argv[1:]
for suffix,value in [('command.txt',shlex.join(cmd)),('started.txt',datetime.datetime.now().astimezone().isoformat())]:
    dest=p/f'{label}.{suffix}';assert not dest.exists();dest.write_text(value+'\n')
with (p/f'{label}.stdout.txt').open('xb') as out,(p/f'{label}.stderr.txt').open('xb') as err:
    r=subprocess.run(cmd,stdout=out,stderr=err)
(p/f'{label}.exitcode').write_text(str(r.returncode)+'\n')
(p/f'{label}.finished.txt').write_text(datetime.datetime.now().astimezone().isoformat()+'\n')
print(label,'exitcode',r.returncode)
for suffix in ['stdout.txt','stderr.txt']:print((p/f'{label}.{suffix}').read_text(errors='replace')[:16000])
sys.exit(r.returncode)
