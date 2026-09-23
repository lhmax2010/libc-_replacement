"""Read existing W1 RPM databases and macro files; bind every existing input read-only."""
import json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/QB_PRECHECK_0922/roots';E.mkdir(parents=True,exist_ok=False);records=[]
def run(label,argv):
 r=subprocess.run(argv,stdin=subprocess.DEVNULL,capture_output=True,timeout=90)
 (E/(label+'.out')).write_bytes(r.stdout);(E/(label+'.err')).write_bytes(r.stderr)
 records.append(dict(label=label,command=shlex.join(argv),argv=argv,exitcode=r.returncode));(E/'commands.json').write_text(json.dumps(records,indent=2))
 print(label,r.returncode,r.stdout.decode(errors='replace')[:6000])
for arch in ('armv7l','aarch64'):
 cmd=json.loads((P/f'docs/progress/BPF_W1_0921/cell_commands/bpf-{arch}-libcxx.json').read_text());i=cmd.index('--');bw=cmd[:i]
 bw=['--ro-bind' if x=='--bind' else x for x in bw]+['--']
 args=cmd[i+2:];rpmargs=[]
 for opt in ('--rcfile','--macros','--target'):
  j=args.index(opt);rpmargs+=args[j:j+2]
 run(arch+'-static-packages',bw+['rpm','-q','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SOURCERPM}\t%{VENDOR}\t%{DISTRIBUTION}\t%{BUILDTIME:date}\n','bcc-tools-static','libbpf-static','liblzma-static','zlib-devel-static'])
 run(arch+'-payload-effective',bw+['rpm',*rpmargs,'--eval','%{_binary_payload}'])
 run(arch+'-payload-files',bw+['/bin/sh','-c',r'''for f in /usr/lib/rpm/macros /etc/rpm/macros* /usr/lib/rpm/tizen/macros /home/abuild/.rpmmacros; do
 if test -f "$f"; then
  printf 'FILE %s\n' "$f"
  sha256sum "$f"
  grep -n '_binary_payload' "$f"
  printf 'GREP_EXIT=%s\n' "$?"
 else printf 'NOT_REGULAR_OR_ABSENT %s\n' "$f"; fi
done'''])
