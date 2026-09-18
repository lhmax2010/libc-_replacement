"""Match the existing GBS GCC plugin and linker, without changing the original root."""
import hashlib,json,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
cmd=json.loads((out/'cell_commands/bpf-armv7l-gcc.json').read_text()); pos=cmd.index('--')
root=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-gcc/local/BUILD-ROOTS/scratch.armv7l.0'
linker=root/'emul/usr/bin/ld'; plugin=root/'emul/usr/lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/liblto_plugin.so'
assert hashlib.sha256(linker.read_bytes()).hexdigest()=='e080425ae98cd5d4f663a9f875988485f8d20192f93d0d9ccf80f192846b350b'
mount=['--ro-bind',str(linker),'/usr/armv7l-tizen-linux-gnueabi/bin/ld']; base=cmd[:pos]+mount; records=[]
def run(args):
    r=subprocess.run(args,capture_output=True,text=True)
    records.append({'command':shlex.join(args),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'gcc-arm-linker-repair-test.json').write_text(json.dumps(records,indent=2))
    print(r.stdout,r.stderr,flush=True); assert r.returncode==0
    return r.stdout
run(['file',str(plugin)])
run(base+['--','/usr/armv7l-tizen-linux-gnueabi/bin/ld','-V'])
run(base+['--','armv7l-tizen-linux-gnueabi-gcc','--version'])
run(base+['--','armv7l-tizen-linux-gnueabi-gcc','-march=armv7-a','-mfpu=neon','-mfloat-abi=softfp','-mthumb',
          '/tmp/task/gcc-toolchain-probe.c','-o','/tmp/task/gcc-toolchain-probe.armv7l'])
header=run(['readelf','-hW',str(p/'tmp/WEEKEND_0918/gcc-toolchain-probe.armv7l')]); assert 'ARM' in header and 'ELF32' in header
run(['readelf','-AW',str(p/'tmp/WEEKEND_0918/gcc-toolchain-probe.armv7l')])
for mode in ('gcc','undefined'):
    command=json.loads((out/'cell_commands'/f'bpf-armv7l-{mode}.json').read_text()); split=command.index('--')
    command[split:split]=mount
    (out/'cell_commands'/f'bpf-armv7l-{mode}-linkerfixed.json').write_text(json.dumps(command,indent=2))
(out/'gcc-arm-linker-input.json').write_text(json.dumps({'native_linker_sha256':hashlib.sha256(linker.read_bytes()).hexdigest(),
    'plugin_sha256':hashlib.sha256(plugin.read_bytes()).hexdigest(),
    'change':'Read-only invocation-local binding of the existing GBS GNU linker matching its native plugin. Original root unchanged; no Clang toolchain switch.',
    'test':'Original GCC driver compiled and linked ARM ELF32 with armv7-a/softfp/thumb options.'},indent=2))
