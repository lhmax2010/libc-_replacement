"""Use the existing GBS native linker with its existing native LTO plugin."""
import hashlib,json,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
cmd=json.loads((out/'cell_commands/bpf-aarch64-gcc.json').read_text()); pos=cmd.index('--')
root=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-gcc/local/BUILD-ROOTS/scratch.aarch64.0'
linker=root/'emul/usr/bin/ld'
mount=['--ro-bind',str(linker),'/usr/aarch64-tizen-linux-gnu/bin/ld']
base=cmd[:pos]+mount; records=[]
def run(args):
    r=subprocess.run(args,capture_output=True,text=True)
    records.append({'command':shlex.join(args),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'gcc-linker-repair-test.json').write_text(json.dumps(records,indent=2))
    print(r.stdout,r.stderr,flush=True); assert r.returncode==0
    return r.stdout
run(base+['--','/usr/aarch64-tizen-linux-gnu/bin/ld','--version'])
run(base+['--','aarch64-tizen-linux-gnu-gcc','/tmp/task/gcc-toolchain-probe.c','-o','/tmp/task/gcc-toolchain-probe.aarch64'])
header=run(['readelf','-hW',str(p/'tmp/WEEKEND_0918/gcc-toolchain-probe.aarch64')]); assert 'AArch64' in header
(out/'cell_commands/bpf-aarch64-gcc-linkerfixed.json').write_text(json.dumps(base+cmd[pos:],indent=2))
(out/'gcc-linker-input.json').write_text(json.dumps({'native_linker_sha256':hashlib.sha256(linker.read_bytes()).hexdigest(),
    'change':'read-only mount existing GBS emul linker over target linker only inside invocation; original root unchanged',
    'test':'original GCC driver compiled and linked AArch64 ELF successfully'},indent=2))
