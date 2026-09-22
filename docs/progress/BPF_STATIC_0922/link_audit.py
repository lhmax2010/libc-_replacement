"""Trace a separate diagnostic relink; never replace the RPM or build ELF."""
import hashlib,json,shlex,subprocess,sys
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'; T=P/'tmp/BPF_STATIC_0922'; arch=sys.argv[1]
label=f'bpf-{arch}-libcxx'; assert (E/'cells'/label/'exitcode').read_text().strip()=='0'
out=E/f'link-audit-{arch}';out.mkdir(exist_ok=False)
d=T/'link-audit'/arch;d.mkdir(parents=True,exist_ok=False)
link=T/f'rpm-{label}/BUILD/bpftrace-0.24.2/build-static/src/CMakeFiles/bpftrace.dir/link.txt'
cmd=shlex.split(link.read_text());assert '-lstdc++fs' in cmd
(out/'recipe-link.txt').write_text(link.read_text())
cmd[cmd.index('-o')+1]=f'/tmp/task/link-audit/{arch}/bpftrace'
# Keep the original dependency file intact as well as the original ELF.
cmd=[x.replace('--dependency-file=CMakeFiles/bpftrace.dir/link.d',f'--dependency-file=/tmp/task/link-audit/{arch}/link.d') for x in cmd]
cmd.append('-Wl,--trace')
old=json.loads((E/'cell_commands'/f'{label}.json').read_text());guest=old[:old.index('--')+1]
guest+=['/bin/sh','-ec',f'cd /tmp/task/rpm-{label}/BUILD/bpftrace-0.24.2/build-static/src; '+shlex.join(cmd)]
(out/'command.json').write_text(json.dumps(guest,indent=2)); (out/'command.txt').write_text(shlex.join(guest)+'\n')
with (out/'stdout.txt').open('wb') as o,(out/'stderr.txt').open('wb') as err:r=subprocess.run(guest,stdout=o,stderr=err)
(out/'exitcode').write_text(str(r.returncode)+'\n');assert r.returncode==0
lines=[x for x in (out/'stdout.txt').read_text(errors='replace').splitlines() if 'stdc++fs' in x]
(out/'resolution.txt').write_text('\n'.join(lines)+'\n')
print('LINK_DIAGNOSTIC_EXIT',r.returncode,'stdc++fs trace lines',json.dumps(lines))
