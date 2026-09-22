"""Read-only current compiler dispatch/version trace; not a historical build or compile."""
import hashlib,json,subprocess,shlex
from pathlib import Path
P=Path.cwd();O=P/'progress/BPF_W3R_0922/compiler-read';O.mkdir(exist_ok=False);rows=[]
for arch,triple in [('armv7l','armv7l-tizen-linux-gnueabi'),('aarch64','aarch64-tizen-linux-gnu')]:
 a=json.loads((P/f'progress/BPF_STATIC_0922/cell_commands/bpf-{arch}-libcxx.json').read_text());a=a[:a.index('--')];a=['--ro-bind' if x=='--bind' else x for x in a]
 argv=['strace','-f','-e','trace=execve','-s','200','-o',str(O/(arch+'.trace'))]+a+['--','/usr/bin/'+triple+'-clang++','--version']
 r=subprocess.run(argv,capture_output=True);(O/(arch+'.out')).write_bytes(r.stdout);(O/(arch+'.err')).write_bytes(r.stderr);rows.append(dict(command=shlex.join(argv),argv=argv,exitcode=r.returncode))
(O/'commands.json').write_text(json.dumps(rows,indent=2));print('VERSION_ONLY_READS_COMPLETE')
