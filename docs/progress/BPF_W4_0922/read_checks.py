"""Read-only historical launch scripts/current root configuration and recipe gap."""
import difflib,hashlib,json,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';O=E/'read-checks';O.mkdir(exist_ok=False);commands=[]
def run(label,argv):
 r=subprocess.run(argv,capture_output=True);(O/(label+'.out')).write_bytes(r.stdout);(O/(label+'.err')).write_bytes(r.stderr);commands.append(dict(label=label,argv=argv,command=shlex.join(argv),exitcode=r.returncode));(O/'commands.json').write_text(json.dumps(commands,indent=2));return r
files=[];snippets=[]
for roundname in ('BPF_W1_0921','BPF_STATIC_0922'):
 base=P/'docs/progress'/roundname
 for rel in ['run.py','guard.py','cells/bpf-armv7l-libcxx/executed-guard.py','cell_commands/bpf-armv7l-libcxx.json']:
  p=base/rel;data=p.read_bytes();files.append(dict(path=str(p),sha256=hashlib.sha256(data).hexdigest()))
  text=data.decode();lines=text.splitlines();interesting=set()
  for i,line in enumerate(lines):
   if any(x in line for x in ('qemu','subprocess','cmd =','cmd=','command=',"'--'",'"bwrap"','"rpmbuild"')):interesting.update(range(max(0,i-2),min(len(lines),i+3)))
  snippets.append('\nFILE '+str(p)+'\n'+'\n'.join(f'{i+1}: {lines[i]}' for i in sorted(interesting)))
 for arch in ('armv7l','aarch64'):
  p=base/'cell_commands'/f'bpf-{arch}-libcxx.json';a=json.loads(p.read_text());a=a[:a.index('--')];a=['--ro-bind' if x=='--bind' else x for x in a]
  script='''for f in /usr/bin/qemu-arm-static /usr/bin/qemu-arm /usr/bin/qemu-aarch64-static /usr/bin/qemu-aarch64; do printf '\\nPATH=%s\\n' "$f"; ls -l "$f"; readlink -f "$f"; file -L "$f"; sha256sum "$f"; rpm -qf "$f"; done; ls -ld /emul /emul/usr/bin; for f in /emul/usr/bin/clang /emul/usr/bin/make /emul/usr/bin/cmake; do ls -l "$f"; file -L "$f"; sha256sum "$f"; done; for f in /proc/sys/fs/binfmt_misc/*arm*; do if test -f "$f"; then printf '\\nENTRY %s\\n' "$f"; cat "$f"; fi; done; rpm -qa --qf '%{NAME} %{VERSION}-%{RELEASE}\\n' | grep -E '^(qemu|clang-accel|gcc-accel)' '''
  run(roundname+'-'+arch+'-root',a+['--','/bin/sh','-c',script])
  log=base/'cells'/f'bpf-{arch}-libcxx'/'build.log'
  run(roundname+'-'+arch+'-compiler-evidence',['rg','-n','/emul|x86_64|x86-64|clang version|CXX compiler identification|C compiler identification|CXX compiler:|C compiler:',str(log)])
(O/'SCRIPT_IDENTITIES.json').write_text(json.dumps(files,indent=2));(O/'LAUNCH_EXCERPTS.txt').write_text('\n'.join(snippets))
for f in Path('/proc/sys/fs/binfmt_misc').iterdir():
 if 'arm' in f.name:run('host-'+f.name,['cat',str(f)])
repo=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'
r=run('baseline-spec',['git','-C',str(repo),'show','f895f8c0373d224847fc7d3ecbeaac3bf926a1a1:packaging/bpftrace.spec']);assert r.returncode==0
old=r.stdout.decode();new=(P/'docs/progress/BPF_STATIC_0922/recipe/bpftrace.spec').read_text()
(E/'BASELINE_TO_VERIFIED_RECIPE.diff').write_text(''.join(difflib.unified_diff(old.splitlines(True),new.splitlines(True),fromfile='f895f8c/packaging/bpftrace.spec',tofile='verified/packaging/bpftrace.spec')))
patch=P/'docs/progress/BPF_STATIC_0922/recipe/static-link-libbpf.patch';ref=P/'docs/progress/BPF_LIBBPF_0922/armv7l-static-libbpf.patch';assert patch.read_bytes()==ref.read_bytes()
(E/'PATCH_IDENTITY.json').write_text(json.dumps(dict(verified=str(patch),reference=str(ref),sha256=hashlib.sha256(patch.read_bytes()).hexdigest(),identical=True),indent=2))
(E/'BUILD_AUDIT_FIX.diff').write_text(''.join(difflib.unified_diff((P/'docs/progress/BPF_W3R_0922/build_audit.py').read_text().splitlines(True),(E/'build_audit.py').read_text().splitlines(True),fromfile='before/build_audit.py',tofile='after/build_audit.py')))
print('READ_CHECKS_COMPLETE; SPEC_GAP_REQUIRES_HUMAN_DECISION')
