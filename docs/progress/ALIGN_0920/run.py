import datetime, pathlib, shlex, subprocess, sys
base=pathlib.Path('/home/toolchain/development/libc++_replacement/progress/ALIGN_0920/raw');base.mkdir(parents=True,exist_ok=True)
label,*cmd=sys.argv[1:]
assert label and '/' not in label and cmd
for suffix,value in [('.command.txt',shlex.join(cmd)),('.started.txt',datetime.datetime.now().astimezone().isoformat())]:
    with (base/(label+suffix)).open('x') as f:f.write(value+'\n')
with (base/(label+'.stdout.txt')).open('xb') as out,(base/(label+'.stderr.txt')).open('xb') as err:r=subprocess.run(cmd,stdout=out,stderr=err)
for suffix,value in [('.exitcode',str(r.returncode)),('.finished.txt',datetime.datetime.now().astimezone().isoformat())]:
    with (base/(label+suffix)).open('x') as f:f.write(value+'\n')
print(label,'exitcode',r.returncode)
if '--quiet-view' not in sys.argv:
    for suffix in ('.stdout.txt','.stderr.txt'):
        data=(base/(label+suffix)).read_bytes()
        print(data[:18000].decode(errors='replace'))
sys.exit(r.returncode)
