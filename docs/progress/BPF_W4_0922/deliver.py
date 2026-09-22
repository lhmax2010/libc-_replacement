"""Publish W4 evidence and check the exact staged scope without printing secrets."""
import base64,hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';D=P/'docs/progress/BPF_W4_0922';secrets=[]
for name in ('.quickbuild-credentials','password.md'):
 f=P/name
 if f.exists():
  values=f.read_bytes().splitlines()
  if name=='.quickbuild-credentials':values=values[1:]
  for value in values:
   if len(value)>=4:secrets.extend((value,base64.b64encode(value)))
def safe(data,name):
 assert not any(x in data for x in secrets),('potential credential STOP',name)
 assert not re.search(rb'-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('potential secret STOP',name)
def git(*args):
 r=subprocess.run(['git',*args],capture_output=True);assert r.returncode==0;return r.stdout
if sys.argv[1]=='publish':
 manifest=[];excluded=[]
 for f in sorted(E.rglob('*')):
  if not f.is_file() or f.name=='PUBLICATION_MANIFEST.json':continue
  rel=f.relative_to(E);assert not f.is_symlink()
  data=f.read_bytes();h=hashlib.sha256(data).hexdigest()
  if '__pycache__' in rel.parts:
   excluded.append(dict(path=str(rel),bytes=len(data),sha256=h,reason='Python generated cache; scripts retained'));continue
  safe(data,str(rel));assert len(data)<50000000
  assert not data.startswith(b'\x7fELF'),('unexpected ELF in evidence',str(rel))
  d=D/rel;d.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(f,d);assert hashlib.sha256(d.read_bytes()).hexdigest()==h
  manifest.append(dict(path=str(rel),bytes=len(data),sha256=h))
 result=dict(credential_scan='PASS',files=manifest,excluded=excluded,binary_inputs='RPMs, extracted ELFs and cpio payloads retained under tmp; identities in RESTORE_RECORDS.json')
 (E/'PUBLICATION_MANIFEST.json').write_text(json.dumps(result,indent=2));shutil.copy2(E/'PUBLICATION_MANIFEST.json',D/'PUBLICATION_MANIFEST.json')
 print('PUBLISHED',len(manifest),'files',sum(x['bytes'] for x in manifest),'bytes','EXCLUDED',len(excluded),'CREDENTIAL_SCAN_PASS')
elif sys.argv[1]=='check':
 assert git('branch','--show-current').strip()==b'codex/r5-ehabi-diagnosis'
 files=[x.decode() for x in git('diff','--cached','--name-only','-z').split(b'\0') if x];assert files
 assert all(x=='docs/LINE_STATUS.md' or x.startswith('docs/progress/BPF_W4_0922/') for x in files)
 rows=[]
 for name in files:
  data=git('show',':'+name);safe(data,name);rows.append(dict(path=name,sha256=hashlib.sha256(data).hexdigest()))
 (E/'STAGED_CHECK.json').write_text(json.dumps(dict(status='PASS',files=rows,scope='task docs and LINE_STATUS only'),indent=2));print('STAGED_SCOPE_AND_CREDENTIAL_SCAN_PASS',len(rows))
else:raise ValueError('publish/check only')
