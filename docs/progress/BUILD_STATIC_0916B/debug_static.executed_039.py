import json,subprocess,sys
from pathlib import Path
p=Path('progress/BUILD_STATIC_0916B')
c=json.loads(Path('progress/BUILD_NIGHT_0917/bpftrace-x86_64-commands.json').read_text())['build']; base=c[:c.index('--')]
cmd=base+['--ro-bind','/usr','/tmp/night/hostusr','--ro-bind','/lib/x86_64-linux-gnu','/tmp/night/hostlibs',
 '--ro-bind','/lib64/ld-linux-x86-64.so.2','/tmp/night/hostld','--',
 '/tmp/night/hostld','--library-path','/tmp/night/hostlibs','/tmp/night/hostusr/bin/gdb',
 '--data-directory=/tmp/night/hostusr/share/gdb','--nx','--batch',
 '-ex','set pagination off','-ex','set debuginfod enabled off','-ex','set startup-with-shell off',
 '-ex','set disable-randomization off','-ex','run','-ex','thread apply all bt','-ex','info sharedlibrary',
 '--args','/tmp/night/finish_validation/cancel-static','wait']
suffix=''
if '--abort' in sys.argv:
    i=cmd.index('run')-1
    cmd[i:i]=['-ex','handle SIG32 nostop noprint pass']
    suffix='_abort'
if '--registers' in sys.argv:
    i=cmd.index('run')-1
    cmd[i:i]=['-ex','handle SIG32 nostop noprint pass','-ex','break _Unwind_SetGR']
    i=cmd.index('run')+1
    cmd[i:i]=['-ex','info registers rdi rsi rdx','-ex','x/17ub &dwarf_reg_size_table','-ex','disassemble _Unwind_SetGR']
    suffix='_registers'
r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=120)
(p/('static_gdb'+suffix+'.json')).write_text(json.dumps(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2))
print(r.stdout); print(r.stderr); raise SystemExit(r.returncode)

