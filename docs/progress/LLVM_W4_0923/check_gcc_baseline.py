"""Compare the exact requested GCC override against the unchanged remote spec."""
import json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';C=E/'prep-check'
record=next(x for x in json.loads((C/'commands.json').read_text()) if x['label']=='gcc')
cmd=record['argv'];i=cmd.index('--');cmd=cmd[:i]+['--ro-bind',str(E/'raw/008_baseline_spec.stdout.txt'),'/tmp/baseline.spec']+cmd[i:];cmd[-1]='/tmp/baseline.spec'
r=subprocess.run(cmd,capture_output=True,text=True)
(C/'gcc-baseline.stdout.txt').write_text(r.stdout);(C/'gcc-baseline.stderr.txt').write_text(r.stderr)
(C/'gcc-baseline-command.json').write_text(json.dumps(dict(argv=cmd,command=shlex.join(cmd),exitcode=r.returncode),indent=2))
result=dict(exitcode=r.returncode,static_option_count=r.stdout.count('-DLIBCLANG_BUILD_STATIC=ON'),libcxx_option_count=r.stdout.count('-DLLVM_ENABLE_LIBCXX=ON'))
(C/'gcc-baseline-result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result));assert r.returncode==0
