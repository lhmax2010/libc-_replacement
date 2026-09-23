"""Task-scoped evidence and secret checks; public Git identities are permitted."""
import base64,hashlib,json,pathlib,re,subprocess
P=pathlib.Path.cwd();E=P/'docs/progress/LLVM_W4_0923'
secrets=[];blobs=[]
for name in ('.quickbuild-credentials','password.md'):
 f=P/name
 if f.is_file():
  lines=f.read_bytes().splitlines();secrets += [x for x in (lines[1:] if name=='.quickbuild-credentials' else lines) if len(x)>3]
auth=[pathlib.Path.home()/'.oscrc',pathlib.Path.home()/'.netrc'];ssh=pathlib.Path.home()/'.ssh'
if ssh.is_dir():auth += [x for x in ssh.iterdir() if x.is_file()]
for f in auth:
 if not f.is_file():continue
 data=f.read_bytes()
 if len(data)>32:blobs.append(data)
 if b'PRIVATE KEY' in data:secrets += [x.strip() for x in data.splitlines() if len(x.strip())>40 and not x.startswith(b'-----')]
 if f.name in ('.oscrc','.netrc'):secrets += [m.group(1) for m in re.finditer(rb'(?im)(?:password|passwd|token|pass)\s*(?:=\s*|\s+)([^\s]+)',data) if len(m.group(1))>3]
secrets=[v for s in secrets for v in (s,base64.b64encode(s),s.hex().encode())]
files=[f for f in E.rglob('*') if f.is_file()]+[P/'docs/LINE_STATUS.md']
for f in files:
 data=f.read_bytes()
 assert not any(s in data for s in secrets+blobs),'STOP: secret content match'
 for pattern in (rb'-----BEGIN (?:OPENSSH |RSA |EC |DSA )?PRIVATE KEY-----',rb'gh[pousr]_[A-Za-z0-9]{30,}',rb'github_pat_[A-Za-z0-9_]{40,}',rb'(?im)^(?:Set-Cookie|Cookie|Authorization):\s*\S+'):
  assert not re.search(pattern,data),'STOP: authentication pattern'
 assert len(data)<5*1024*1024
rows=json.loads((E/'recipe-audit/RESULTS.json').read_text());assert len(rows)==11 and len({r['package'] for r in rows})==11
spec=P/'tmp/BUILD_W1BC_0911/target-fetch/packaging/llvm.spec'
assert hashlib.sha256(spec.read_bytes()).hexdigest()=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
result=dict(secret_scan='PASS',files_scanned=len(files),eleven_packages=True,local_spec_matches_verified=True,gcc_override_gate='FAIL_CRITERION',package_push=False,prep_only=True)
(E/'SELFCHECK.json').write_text(json.dumps(result,indent=2)+'\n')
(E/'SCRIPT_SHA256.json').write_text(json.dumps({x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in sorted(E.glob('*.py'))},indent=2)+'\n')
print(json.dumps(result))
