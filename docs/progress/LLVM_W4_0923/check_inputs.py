import difflib,hashlib,json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923'; records=[]
def run(argv):
 r=subprocess.run(argv,capture_output=True,text=True);records.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr));(E/'input-commands.json').write_text(json.dumps(records,indent=2));assert r.returncode==0;return r.stdout
specs=[P/'tmp/WEEKEND_0918/specs/llvm.spec',P/'tmp/STATIC_0917B/rpm-aarch64/SPECS/llvm.spec']
assert specs[0].read_bytes()==specs[1].read_bytes()
sha=hashlib.sha256(specs[0].read_bytes()).hexdigest();assert sha=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
baseline=(E/'raw/008_baseline_spec.stdout.txt').read_text();candidate=specs[0].read_text()
diff=''.join(difflib.unified_diff(baseline.splitlines(True),candidate.splitlines(True),fromfile='f203923a/packaging/llvm.spec',tofile='verified/packaging/llvm.spec'))
(E/'BASELINE_TO_VERIFIED_LLVM.diff').write_text(diff)
assert candidate.replace('    -DLIBCLANG_BUILD_STATIC=ON \\\n','')==baseline
(E/'SPEC_IDENTITY.json').write_text(json.dumps({'paths':[str(s.relative_to(P)) for s in specs],'sha256':sha,'same_bytes':True,'functional_added_lines':['    -DLIBCLANG_BUILD_STATIC=ON \\'],'nonfunctional_differences':[]},indent=2))
roots={'armv7l':P/'tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l','aarch64':P/'tmp/STATIC_0917B/rpm-aarch64/RPMS/aarch64'}
result=[]
for arch,root in roots.items():
 files=sorted(root.glob('*.rpm'));assert len(files)==(22 if arch=='armv7l' else 12)
 for f in files:
  listing=run(['rpm','-qlp','--nosignature',str(f)])
  paths=[x for x in listing.splitlines() if x.endswith('/libclang.a')]
  if paths:
   name=run(['rpm','-qp','--nosignature','--qf','%{NAME} %{VERSION}-%{RELEASE} %{ARCH}\n',str(f)]).strip()
   h=hashlib.sha256()
   with f.open('rb') as stream:
    for b in iter(lambda:stream.read(1048576),b''):h.update(b)
   result.append(dict(arch=arch,rpm=str(f.relative_to(P)),identity=name,paths=paths,sha256=h.hexdigest(),rpm_count=len(files)))
(E/'LIBCLANG_PACKAGE.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2));assert len(result)==2
