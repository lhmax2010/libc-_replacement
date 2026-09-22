import datetime,difflib,hashlib,json,subprocess
from pathlib import Path
P=Path.cwd();O=P/'progress/BPF_W3R_0922/history';O.mkdir(exist_ok=False)
result=[]
for arch in ('armv7l','aarch64'):
 for name in ('BPF_W1_0921','BPF_STATIC_0922'):
  f=P/'progress'/name/'cells'/f'bpf-{arch}-libcxx'/'events.jsonl';ev=[json.loads(x) for x in f.read_text().splitlines()]
  start=next(x for x in ev if x['event']=='start');end=next(x for x in ev if x['event']=='finished');seconds=(datetime.datetime.fromisoformat(end['time'])-datetime.datetime.fromisoformat(start['time'])).total_seconds()
  commands=[x for x in ev if x['event'] in ('scheduler_arguments','build_scheduler_verified')]
  (O/(name+'-'+arch+'-scheduler.json')).write_text(json.dumps(commands,indent=2))
  result.append(dict(arch=arch,round=name,start=start['time'],end=end['time'],seconds=seconds,announcements=end['completed'],entry_examples=[x.get('command') for x in commands[:2]],io=[x for x in ev if x['event']=='io_probe'],pauses=[x for x in ev if 'pause' in x['event']]))
 a=(P/'progress/BPF_W3R_0922/build-audit'/('BPF_W1_0921-'+arch+'-targets.txt')).read_text().splitlines(True);b=(P/'progress/BPF_W3R_0922/build-audit'/('BPF_STATIC_0922-'+arch+'-targets.txt')).read_text().splitlines(True)
 (O/(arch+'-targets.diff')).write_text(''.join(difflib.unified_diff(a,b)))
(O/'TIMINGS.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
# Read-only execution of version queries, never compile or mutate caches.
commands=[]
for arch in ('armv7l','aarch64'):
 a=json.loads((P/f'progress/BPF_STATIC_0922/cell_commands/bpf-{arch}-libcxx.json').read_text());a=a[:a.index('--')];a=['--ro-bind' if x=='--bind' else x for x in a]
 script='''ls -l /usr/bin/qemu-arm-static /usr/bin/qemu-aarch64-static; file /usr/bin/qemu-arm /usr/bin/qemu-aarch64; /usr/bin/qemu-arm --version; /usr/bin/qemu-aarch64 --version; for f in /emul/usr/bin/clang /emul/usr/bin/clang-22 /emul/usr/bin/clang++; do ls -l "$f"; file -L "$f"; sha256sum "$f"; done; /emul/usr/bin/clang --version'''
 argv=a+['--','/bin/sh','-c',script];r=subprocess.run(argv,capture_output=True);(O/(arch+'-emul.out')).write_bytes(r.stdout);(O/(arch+'-emul.err')).write_bytes(r.stderr);commands.append(dict(argv=argv,exitcode=r.returncode))
(O/'commands.json').write_text(json.dumps(commands,indent=2))
