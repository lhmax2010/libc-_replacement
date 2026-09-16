import sys
from probes import base, run, inside

mode = sys.argv[1]
cmd = base+['--ro-bind','/usr',inside+'/hostusr','--ro-bind','/lib/x86_64-linux-gnu',inside+'/hostlibs',
 '--ro-bind','/lib64/ld-linux-x86-64.so.2',inside+'/hostld','--',
 inside+'/hostld','--library-path',inside+'/hostlibs',inside+'/hostusr/bin/gdb',
 '--data-directory='+inside+'/hostusr/share/gdb','--nx','--batch']
ex = ['set pagination off','set debuginfod enabled off','set startup-with-shell off',
 'set disable-randomization off','handle SIG32 nostop noprint pass']
if mode == 'registers': ex += ['break _Unwind_SetGR']
ex += ['run']
if mode == 'registers': ex += ['info registers rdi rsi rdx','x/17ub &dwarf_reg_size_table','x/30i $pc']
ex += ['thread apply all bt','info sharedlibrary']
for x in ex: cmd += ['-ex',x]
cmd += ['--args',inside+'/gnu-static','wait']
r=run('gnu-gdb-'+mode,cmd,120)
print(r['stdout']); print(r['stderr'])
