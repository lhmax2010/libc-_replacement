"""Copy the authorized verified recipe byte-for-byte; create a local-only commit."""
import hashlib,json,subprocess,shlex,re,sys
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';R=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'
B='f895f8c0373d224847fc7d3ecbeaac3bf926a1a1';resume=sys.argv[1:]==['--resume-after-comparator-check'];records=json.loads((E/'LOCAL_COMMIT_COMMANDS.json').read_text()) if resume else []
def run(argv):
 r=subprocess.run(argv,capture_output=True)
 records.append(dict(command=shlex.join(argv),argv=argv,exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')))
 (E/'LOCAL_COMMIT_COMMANDS.json').write_text(json.dumps(records,indent=2))
 assert r.returncode==0,('command failed',argv,r.returncode)
 return r.stdout
def git(*a):return run(['git','-C',str(R),*a])
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
assert git('branch','--show-current').strip()==b'sandbox/lhmax2025/libcxx-migration'
assert git('rev-parse','HEAD').decode().strip()==B
assert git('rev-parse','FETCH_HEAD').decode().strip()==B
status=git('status','--porcelain')
assert status==(b' M packaging/bpftrace.spec\n?? packaging/static-link-libbpf.patch\n' if resume else b'')
assets={str(f.relative_to(R)):sha(f) for f in (R/'packaging').iterdir() if f.is_file() and f.name not in ('bpftrace.spec','static-link-libbpf.patch')}
recipe=P/'docs/progress/BPF_STATIC_0922/recipe';identities=[]
for name in ('bpftrace.spec','static-link-libbpf.patch'):
 src=recipe/name;dst=R/'packaging'/name
 if name.endswith('.patch'):
  if not resume:assert not dst.exists()
  assert sha(src)=='abe6d2fa379195c83c2317d368df306ade03c9cc7a1c8dc3f58ad25e65ae5b9e'
 if not resume:run(['cp','--',str(src),str(dst)])
 assert src.read_bytes()==dst.read_bytes()
 identities.append(dict(file=name,source=str(src),destination=str(dst),sha256=sha(dst),byte_identical=True))
expected=(E/'BASELINE_TO_VERIFIED_RECIPE.diff').read_bytes()
actual=git('diff',B,'--','packaging/bpftrace.spec')
def hunks(data):
 # Git appends optional function-context text to @@ headers. It is not file content.
 return re.sub(rb'^(@@ -[0-9,]+ \+[0-9,]+ @@)[^\n]*',rb'\1',data[data.index(b'@@'):],flags=re.M)
assert hunks(expected)==hunks(actual),'spec hunks differ: STOP'
for name,h in assets.items():assert sha(R/name)==h,('unchanged asset changed',name)
git('add','--','packaging/bpftrace.spec','packaging/static-link-libbpf.patch')
assert set(git('diff','--cached','--name-only').decode().splitlines())=={'packaging/bpftrace.spec','packaging/static-link-libbpf.patch'}
git('diff','--cached','--check')
git('commit','-F',str(E/'COMMIT_MESSAGE.txt'))
head=git('rev-parse','HEAD').decode().strip()
assert git('status','--porcelain')==b''
assert git('rev-list','--left-right','--count','FETCH_HEAD...HEAD').split()==[b'0',b'1']
assert hunks(git('diff',B+'..HEAD','--','packaging/bpftrace.spec'))==hunks(expected)
assert git('show','HEAD:packaging/bpftrace.spec')==(recipe/'bpftrace.spec').read_bytes()
assert git('show','HEAD:packaging/static-link-libbpf.patch')==(recipe/'static-link-libbpf.patch').read_bytes()
(E/'SANDBOX_COMMIT.diff').write_bytes(git('show','--format=fuller','--no-ext-diff','HEAD'))
(E/'0001-build-static-subpackage-from-source.patch').write_bytes(git('format-patch','-1','--stdout','HEAD'))
result=dict(status='PASS',base=B,commit=head,identities=identities,spec_hunks_identical=True,added_patch_byte_identical=True,comparison_note='BASELINE_TO_VERIFIED_RECIPE.diff contains spec hunks only; added patch checked separately byte-for-byte',unchanged_packaging_files=assets,ahead=1,behind=0,clean=True,pushed=False)
(E/'LOCAL_COMMIT_RESULT.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
