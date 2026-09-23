"""Read-only inventory of actual overlay roots. No RPM install or build."""
import json,shlex,subprocess
from pathlib import Path
P=Path.cwd(); E=P/'docs/progress/LLVM_W4_0923/inputs_R2'; E.mkdir(exist_ok=True)
sources={
 'bpf-armv7l':'docs/progress/BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json',
 'bpf-aarch64':'docs/progress/BPF_STATIC_0922/cell_commands/bpf-aarch64-libcxx.json',
 'llvm-armv7l':'docs/progress/ARM_W5_0921/cell_commands/w5-original.json',
 'llvm-aarch64':'docs/progress/BUILD_STATIC_0917B/native_rpm_serial_command.json'}
records=[]
for label,source in sources.items():
 cmd=json.loads((P/source).read_text()); start=cmd.index('bwrap'); stop=cmd.index('--',start)
 bw=['--ro-bind' if x=='--bind' else x for x in cmd[start:stop]]
 argv=bw+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SOURCERPM}\t%{VCS}\t%{SHA1HEADER}\n']
 r=subprocess.run(argv,capture_output=True,timeout=120)
 (E/(label+'-rpmdb.tsv')).write_bytes(r.stdout); (E/(label+'-rpmdb.stderr.txt')).write_bytes(r.stderr)
 records.append(dict(label=label,source=source,argv=argv,command=shlex.join(argv),exitcode=r.returncode,rows=len(r.stdout.splitlines()),limitation='Current retained root; historical snapshot identity is not inferred. BPF main and static share this root.'))
 (E/'commands_R2.json').write_text(json.dumps(records,indent=2)+'\n')
 print(label,r.returncode,len(r.stdout.splitlines()))
