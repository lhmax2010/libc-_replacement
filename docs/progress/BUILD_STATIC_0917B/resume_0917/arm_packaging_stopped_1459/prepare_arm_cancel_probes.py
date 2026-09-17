"""Compile the unchanged cancellation probe against the installed static RPMs."""
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

p=Path.cwd(); out=p/'progress/BUILD_STATIC_0917B/resume_0917'
task=p/'tmp/STATIC_0917B'; dest=task/'board-probes'
dest.mkdir(exist_ok=False)
base=json.loads((out/'bpf-private-armv7l-base.json').read_text())
assert json.loads((out/'bpf-install-armv7l-result.json').read_text())['status']=='STATIC_INPUT_RPMS_INSTALLED'
source=p/'docs/progress/BUILD_STATIC_0917/inputs/condition_cancel_probe.cpp'
base+=['--ro-bind',str(source),'/tmp/task/board-probes/condition_cancel_probe.cpp']
commands=[]; results=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'arm_cancel_compile_commands.json').write_text(json.dumps(commands,indent=2))
    print(shlex.join(cmd),'exitcode='+str(r.returncode),flush=True)
    assert r.returncode==0,r.stderr
    return r.stdout
version=run(base+['--','armv7l-tizen-linux-gnueabi-clang++','--version'])
assert 'clang version 22.1.8' in version
for mode in ('shared-gcc','static-gcc'):
    exe='/tmp/task/board-probes/cancel-'+mode
    args=['armv7l-tizen-linux-gnueabi-clang++','-std=c++20','-O0','-g','-fno-inline',
          '-march=armv7-a','-mtune=cortex-a8','-mfpu=neon','-mfloat-abi=softfp','-mthumb',
          '-stdlib=libc++','-static-libstdc++','-shared-libgcc' if mode=='shared-gcc' else '-static-libgcc',
          '/tmp/task/board-probes/condition_cancel_probe.cpp','-pthread','-latomic',
          '-Wl,--start-group','/usr/lib/libc++.a','/usr/lib/libc++abi.a','-Wl,--end-group',
          '-Wl,-Map,'+exe+'.map','-o',exe]
    run(base+['--',*args])
    f=dest/('cancel-'+mode)
    dynamic=run(['readelf','-dW',str(f)])
    needed=[x.split('[')[1].split(']')[0] for x in dynamic.splitlines() if '(NEEDED)' in x]
    assert not any(x.startswith(('libc++.so','libc++abi.so','libstdc++')) for x in needed)
    assert ('libgcc_s.so.1' in needed)==(mode=='shared-gcc')
    text=(dest/('cancel-'+mode+'.map')).read_text(errors='replace')
    hits=[x for x in text.splitlines() if any(k in x for k in ('libc++.a','libc++abi.a','libgcc_eh','libgcc_s','libunwind','cxa_personality'))]
    (out/f'arm_cancel_{mode}_map_excerpt.txt').write_text('\n'.join(hits)+'\n')
    results.append({'mode':mode,'binary':str(f),'sha256':hashlib.sha256(f.read_bytes()).hexdigest(),
                    'needed':needed,'status':'COMPILED_NOT_RUN'})
(out/'arm_cancel_probe_inputs.json').write_text(json.dumps({'source':str(source),
    'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'compiler_version':version,'probes':results},indent=2))
