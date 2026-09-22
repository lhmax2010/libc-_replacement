import json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';out=E/'gbs-read';out.mkdir(exist_ok=False);rows=[]
for arch in ('armv7l','aarch64'):
 a=json.loads((P/f'docs/progress/BPF_STATIC_0922/cell_commands/bpf-{arch}-libcxx.json').read_text());root=a[a.index('--ro-bind')+1]
 script='''for f in /usr/bin/qemu-arm-static /usr/bin/qemu-arm /usr/bin/qemu-arm-binfmt /usr/bin/qemu-binfmt; do ls -l "$f"; readlink -f "$f"; file -L "$f"; sha256sum "$f"; done; ls -ld /emul /emul/usr/bin; for f in /etc/binfmt.d/*.conf /usr/lib/binfmt.d/*.conf /etc/build.conf /etc/build.data; do if test -f "$f"; then printf '\\nFILE=%s\\n' "$f"; cat "$f"; fi; done; rpm -qa --qf '%{NAME} %{VERSION}-%{RELEASE}\\n' | grep -E '^(qemu|clang-accel)' '''
 argv=['bwrap','--unshare-user','--uid','1000','--gid','1000','--ro-bind',root,'/','--dev','/dev','--proc','/proc','--tmpfs','/tmp','--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--','/bin/sh','-c',script]
 r=subprocess.run(argv,capture_output=True);(out/(arch+'.out')).write_bytes(r.stdout);(out/(arch+'.err')).write_bytes(r.stderr);rows.append(dict(arch=arch,root=root,command=shlex.join(argv),exitcode=r.returncode))
(out/'commands.json').write_text(json.dumps(rows,indent=2));print('GBS_ROOT_READS_COMPLETE')
