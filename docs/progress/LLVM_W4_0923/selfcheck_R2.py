"""Check only the task-scoped staged delivery; never print credentials."""
import base64,hashlib,json,re,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923'
names=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0')
names=[n for n in names if n]
assert names and all(n.startswith('docs/progress/LLVM_W4_0923/') or n=='docs/LINE_STATUS.md' for n in names)
secret=[]; blobs=[]
for filename in ['.quickbuild-credentials','password.md']:
 f=P/filename
 if f.is_file():
  lines=f.read_bytes().splitlines();secret += [v for v in (lines[1:] if filename=='.quickbuild-credentials' else lines) if len(v)>3]
for f in [Path.home()/'.oscrc',Path.home()/'.netrc']:
 if f.is_file():
  data=f.read_bytes();blobs.append(data)
  secret += [m.group(1) for m in re.finditer(rb'(?im)(?:password|passwd|token|pass)\s*(?:=\s*|\s+)([^\s]+)',data) if len(m.group(1))>3]
ssh=Path.home()/'.ssh'
if ssh.is_dir():
 for f in ssh.iterdir():
  if f.is_file():
   data=f.read_bytes()
   if len(data)>32:blobs.append(data)
   if b'PRIVATE KEY' in data:secret += [s for s in data.splitlines() if len(s)>40 and not s.startswith(b'-----')]
needles=[v for s in secret for v in (s,base64.b64encode(s),s.hex().encode())]+[b for b in blobs if len(b)>32]
for name in names:
 data=subprocess.check_output(['git','show',':'+name])
 assert not any(s in data for s in needles),'STOP: credential match'
 assert not re.search(rb'-----BEGIN (?:OPENSSH |RSA |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{40,}|(?im:^Authorization:\s*\S+)|(?im:^Cookie:\s*\S+)',data),'STOP: sensitive pattern'
 assert len(data)<5*1024*1024,'STOP: large artifact'
r=dict(secret_scan='PASS',staged_files=len(names),scope='only LLVM_W4_0923 and LINE_STATUS',package_push=False,local_commits=1,second_commit='NOT_CREATED',runtime_gate='PENDING_BLANK_LINE_CLARIFICATION',input_provenance='PARTIAL')
(E/'SELFCHECK_R2.json').write_text(json.dumps(r,indent=2)+'\n')
(E/'SCRIPT_SHA256_R2.json').write_text(json.dumps({f.name:hashlib.sha256(f.read_bytes()).hexdigest() for f in sorted(E.glob('*_R2.py'))},indent=2)+'\n')
print(json.dumps(r))
