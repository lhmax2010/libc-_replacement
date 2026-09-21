import base64,json,pathlib,re,subprocess,sys
allowed=sys.argv[1]
def git(*args):return subprocess.check_output(['git',*args])
paths=git('diff','--cached','--name-only','-z').split(b'\0');paths=[x.decode() for x in paths if x]
assert paths and all(x.startswith(allowed+'/') for x in paths),'Unexpected staged path; stop'
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
 p=pathlib.Path(name)
 if p.is_file():
  values=p.read_bytes().splitlines()
  if name=='.quickbuild-credentials':values=values[1:]
  for v in values:
   if len(v)>=4:secrets.extend((v,base64.b64encode(v)))
for path in paths:
 data=git('show',':'+path)
 assert not any(x in data for x in secrets),('Potential credential; stop',path)
 assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('Potential secret; stop',path)
print(json.dumps({'staged_files':len(paths),'allowed_prefix':allowed,'credential_scan':'PASS','other_paths_staged':False}))
