"""Copy the completed ledger unchanged, with input/output hashes and a private secret check."""
import base64,hashlib,json,pathlib,re,shutil
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
src=root/'progress/BASE_LEDGER_0921';dst=root/'docs/progress/BASE_LEDGER_0921'
assert not dst.exists()
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
 p=root/name
 if p.is_file():
  vals=p.read_bytes().splitlines()
  if name=='.quickbuild-credentials':vals=vals[1:]
  for v in vals:
   if len(v)>=4:secrets.extend((v,base64.b64encode(v)))
records=[]
for p in sorted(src.rglob('*')):
 if not p.is_file():continue
 assert not p.is_symlink()
 data=p.read_bytes()
 assert not any(v in data for v in secrets),('Potential secret',str(p))
 assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('Potential secret',str(p))
 assert len(data)<50000000,('Large file requires disclosure',str(p),len(data))
 records.append(dict(path=str(p.relative_to(src)),bytes=len(data),sha256=hashlib.sha256(data).hexdigest()))
shutil.copytree(src,dst,copy_function=shutil.copy2)
for row in records:
 assert hashlib.sha256((src/row['path']).read_bytes()).hexdigest()==row['sha256']
 assert hashlib.sha256((dst/row['path']).read_bytes()).hexdigest()==row['sha256']
out=root/'progress/BPF_W1_0921/ledger_copy_verified.json'
out.write_text(json.dumps(dict(source_unchanged=True,verbatim_copy=True,credential_scan='PASS',files=records),indent=2)+'\n')
print(json.dumps(dict(files=len(records),bytes=sum(r['bytes'] for r in records),verbatim_copy=True,credential_scan='PASS')))
