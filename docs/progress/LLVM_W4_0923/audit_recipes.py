"""Compare preserved exported recipes from accepted build logs to observed remote heads."""
import csv,difflib,hashlib,json,os,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';A=E/'recipe-audit';A.mkdir(exist_ok=True)
rows=list(csv.DictReader((P/'docs/progress/QB_PRECHECK_0922/REMOTE_BRANCHES.tsv').open(),delimiter='\t'))
records=[];results=[]
def run(cmd):
 r=subprocess.run(cmd,capture_output=True,timeout=90,env={**os.environ,'GIT_SSH_COMMAND':'ssh -o BatchMode=yes -o ConnectTimeout=20','GIT_TERMINAL_PROMPT':'0'})
 records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stderr=r.stderr.decode(errors='replace')));(A/'commands.json').write_text(json.dumps(records,indent=2));assert r.returncode==0,(cmd,r.returncode);return r.stdout
def hashfile(f):
 h=hashlib.sha256()
 with f.open('rb') as s:
  for b in iter(lambda:s.read(1048576),b''):h.update(b)
 return h.hexdigest()
for row in rows:
 name=row['package'];out=A/name;out.mkdir(exist_ok=True)
 remote=run(['git','ls-remote','--exit-code',row['url'],'refs/heads/'+row['branch']]).decode();(out/'remote.txt').write_text(remote);sha=remote.split()[0];assert sha==row['remote']
 repo=P/'codes/R104'/name
 if name=='llvm':repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
 if name=='bpftrace':repo=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'
 if name in ('llvm','bpftrace'):
  if name=='llvm':spec=P/'tmp/WEEKEND_0918/specs/llvm.spec';src=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/SOURCES';evidence='docs/progress/ARM_W5_0921/cell_commands/w5-original.json'
  else:spec=P/'tmp/BPF_STATIC_0922/specs/bpftrace.spec';src=P/'tmp/BPF_STATIC_0922/sources-armv7l';evidence='docs/progress/BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json'
 else:
  if name=='bcc-tools':log=P/'docs/progress/BUILD_W2C_0915/build-w2c-bcc-tools-armv7l-libcxx.build.log'
  else:
   round='R105' if name in ('boost','tensorflow2') else 'R104'
   label={'abseil-cpp':'abseil','libsigc++':'libsigc'}.get(name,name)
   logs=sorted((P/f'docs/progress/{round}/raw').glob(f'*build_{label}_x86_64_libcxx*.stdout*'))
   if not logs:raise RuntimeError(('missing log',name))
   log=logs[-1]
  text=log.read_text(errors='replace');m=re.search(r'processing recipe (\S+\.spec)',text);assert m,name
  spec=Path(m.group(1));src=spec.parent;evidence=str(log.relative_to(P))+':'+str(text[:m.start()].count('\n')+1)
 base=run(['git','-C',str(repo),'show',sha+':packaging/'+spec.name]);actual=spec.read_bytes()
 (out/'sandbox.spec').write_bytes(base);(out/'validated.spec').write_bytes(actual)
 delta=''.join(difflib.unified_diff(base.decode().splitlines(True),actual.decode().splitlines(True),fromfile='sandbox/packaging/'+spec.name,tofile='validated/'+spec.name));(out/'SPEC.diff').write_text(delta)
 changes=[l for l in delta.splitlines() if l[:1] in ('+','-') and not l.startswith(('+++','---'))]
 metadata=all(re.match(r'^[+-](?:VCS:|Release:)',l) for l in changes)
 classification='一致' if not changes else ('仅非功能差异' if metadata else '有功能差异未推送')
 paths=run(['git','-C',str(repo),'ls-tree','-r','--name-only',sha,'--','packaging']).decode().splitlines()
 details=[]
 for path in paths:
  rel=path[len('packaging/'):]
  if rel==spec.name:continue
  tested=src/rel
  # Other specs and non-exported packaging documentation are recorded, not invented.
  if not tested.is_file():details.append(dict(path=path,state='NOT_OBSERVED',reason='not in this preserved exported source directory'));continue
  data=run(['git','-C',str(repo),'show',sha+':'+path]);h=hashlib.sha256(data).hexdigest();t=hashfile(tested)
  state='MATCH' if h==t else 'DIFFERENT'
  details.append(dict(path=path,source=str(tested.relative_to(P)),remote_sha256=h,validated_sha256=t,state=state))
  if state=='DIFFERENT' and (path.endswith(('.patch','.diff'))):
   (out/(Path(path).name+'.diff')).write_text(''.join(difflib.unified_diff(data.decode(errors='replace').splitlines(True),tested.read_text(errors='replace').splitlines(True),fromfile='sandbox/'+path,tofile='validated/'+rel)))
   classification='有功能差异未推送'
 extra=[str(x.relative_to(src)) for x in src.rglob('*') if x.is_file() and x.suffix in ('.patch','.diff') and 'packaging/'+str(x.relative_to(src)) not in paths]
 if extra:classification='有功能差异未推送'
 result=dict(package=name,remote_sha=sha,branch=row['branch'],evidence=evidence,validated_spec=str(spec.relative_to(P)),validated_sha256=hashlib.sha256(actual).hexdigest(),sandbox_sha256=hashlib.sha256(base).hexdigest(),classification=classification,changed_spec_lines=changes,packaging_files=details,extra_patches=extra)
 (out/'result.json').write_text(json.dumps(result,ensure_ascii=False,indent=2));results.append(result)
 (A/'RESULTS.json').write_text(json.dumps(results,ensure_ascii=False,indent=2));print(name,classification,changes,flush=True)
