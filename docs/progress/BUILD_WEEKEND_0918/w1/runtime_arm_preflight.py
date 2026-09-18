"""Inspect exact runtime RPM macros before the ARM build; no compilation."""
import json,shlex,subprocess
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
cmd=json.loads((out/'cell_commands/runtime-arm-libcxx.json').read_text()); split=cmd.index('--')
base=cmd[:split]; args=cmd[split+2:]; args=[x for x in args if x not in ('--noclean','-bb')]; spec=args.pop()
commands=[]
def run(command):
    r=subprocess.run(command,capture_output=True,text=True)
    commands.append({'command':shlex.join(command),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'runtime-arm-preflight.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0
    return r.stdout
identity=run(base+['--','rpm',*args,'--eval','%{defined _toolchain}|%{?toolchain_is:%{toolchain_is clang}}|%{__cxx}'])
print(identity)
expanded=run(base+['--','rpmspec',*args,'-P',spec])
(out/'runtime-arm-expanded.spec').write_text(expanded)
assert '-DLIBCXX_ENABLE_STATIC=ON' in expanded and '-DLIBCXXABI_ENABLE_STATIC=ON' in expanded
assert 'armv7l-tizen-linux-gnueabi-clang++' in expanded
version=run(base+['--','armv7l-tizen-linux-gnueabi-clang++','--version'])
assert 'clang version 22.1.8' in version
print(version); print('STATIC_RUNTIME_MACRO_PREFLIGHT_PASS')
