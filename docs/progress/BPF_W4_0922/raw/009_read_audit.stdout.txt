"""Read-only toolchain/cache/log and already-extracted ELF comparison."""
import hashlib,json,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();O=P/'progress/BPF_W3R_0922/build-audit';O.mkdir(exist_ok=False);rows=[]
def run(label,argv):
 r=subprocess.run(argv,capture_output=True);(O/(label+'.out')).write_bytes(r.stdout);(O/(label+'.err')).write_bytes(r.stderr)
 rows.append(dict(label=label,argv=argv,command=shlex.join(argv),exitcode=r.returncode));(O/'commands.json').write_text(json.dumps(rows,indent=2));return r
for arch in ('armv7l','aarch64'):
 a=json.loads((P/f'progress/BPF_STATIC_0922/cell_commands/bpf-{arch}-libcxx.json').read_text());a=a[:a.index('--')];a=['--ro-bind' if x=='--bind' else x for x in a]
 script='''printf 'HOME=%s\\n' "$HOME"; for t in /usr/bin/ARCH-tizen-linux-gnueabi-clang /usr/bin/ARCH-tizen-linux-gnueabi-clang++ /bin/ARCH-tizen-linux-gnueabi-clang /bin/ARCH-tizen-linux-gnueabi-clang++; do ls -l "$t"; readlink -f "$t"; file -L "$t"; done; for d in /home/abuild/.ccache /root/.ccache /var/tmp/ccache "$HOME/.cache/ccache"; do if test -d "$d"; then du -sh "$d"; else printf 'ABSENT %s\\n' "$d"; fi; done; command -v ccache; if command -v ccache >/dev/null; then ccache --version; ccache -s; fi'''.replace('ARCH',arch)
 if arch=='aarch64':script=script.replace('aarch64-tizen-linux-gnueabi','aarch64-tizen-linux-gnu')
 run('root-tools-'+arch,a+['--','/bin/sh','-c',script])
comparison=[]
for arch in ('armv7l','aarch64'):
 for roundname,verify in [('BPF_W1_0921',f'verify-bpf-{arch}-libcxx'),('BPF_STATIC_0922',f'verify-{arch}')]:
  base=P/'progress'/roundname
  result=json.loads((base/verify/'result.json').read_text());f=Path(result['observations']['main']['path'])
  data=f.read_bytes();out=O/(roundname+'-'+arch+'.text')
  rr=run(roundname+'-'+arch+'-text',['/usr/bin/llvm-objcopy','--dump-section','.text='+str(out),str(f)])
  row=dict(round=roundname,arch=arch,path=str(f),sha256=hashlib.sha256(data).hexdigest(),text_sha256=hashlib.sha256(out.read_bytes()).hexdigest() if rr.returncode==0 else 'NOT_OBSERVED')
  log=base/'cells'/f'bpf-{arch}-libcxx'/'build.log';lines=log.read_text(errors='replace').splitlines()
  targets=[x for x in lines if re.match(r'^\[\s*\d+%\]',x) and ('Building ' in x or 'Linking ' in x)]
  (O/(roundname+'-'+arch+'-targets.txt')).write_text('\n'.join(targets)+'\n');row['target_lines']=len(targets)
  ev=[json.loads(x) for x in (base/'cells'/f'bpf-{arch}-libcxx'/'events.jsonl').read_text().splitlines()]
  row['events_start_end']=[x for x in ev if x.get('event') in ('start','exit','finished','complete')]
  comparison.append(row)
  caches=list((P/'tmp'/roundname/f'rpm-bpf-{arch}-libcxx'/'BUILD').glob('*/build*/CMakeCache.txt'))
  for i,c in enumerate(caches):
   run(roundname+'-'+arch+'-cache-'+str(i),['rg','-n','CMAKE_.*COMPILER|ccache',str(c)])
 (O/'ELF_COMPARISON.json').write_text(json.dumps(comparison,indent=2))
print(json.dumps(comparison,indent=2))
