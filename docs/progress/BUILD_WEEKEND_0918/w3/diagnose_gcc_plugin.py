import json,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
cmd=json.loads((out/'cell_commands/bpf-aarch64-gcc.json').read_text())
base=cmd[:cmd.index('--')]
root=Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-gcc/local/BUILD-ROOTS/scratch.aarch64.0')
commands=[]
def run(args):
    r=subprocess.run(args,capture_output=True,text=True)
    commands.append({'command':shlex.join(args),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    print(r.stdout,r.stderr)
for rel in ('emul/usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/liblto_plugin.so',
            'usr/aarch64-tizen-linux-gnu/bin/ld','emul/usr/aarch64-tizen-linux-gnu/bin/ld'):
    run(['file',str(root/rel)])
    run(['readelf','-hWd',str(root/rel)])
run(base+['--','sh','-c','ls -l /usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/liblto_plugin.so /emul/usr/lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/liblto_plugin.so; /usr/aarch64-tizen-linux-gnu/bin/ld --version'])
(out/'gcc-plugin-diagnostic.json').write_text(json.dumps(commands,indent=2))
