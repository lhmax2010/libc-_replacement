import base64,hashlib,json,pathlib,re,subprocess
root=pathlib.Path.cwd();secrets=[]
for name in ['password.md','.quickbuild-credentials']:
    p=root/name
    if p.is_file():
        values=p.read_bytes().splitlines()
        if name=='.quickbuild-credentials':values=values[1:]
        for value in values:
            if len(value)>=4:secrets.extend([value,base64.b64encode(value)])
paths=subprocess.check_output(['git','diff','--cached','--name-only','-z']).split(b'\0');records=[];issues=[]
for raw in paths:
    if not raw:continue
    path=raw.decode();assert path.startswith(('docs/progress/ARM_RPM_DIAG/','docs/progress/ALIGN_0920/')),path
    data=subprocess.check_output(['git','show',':'+path]);records.append({'path':path,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
    if any(s in data for s in secrets) or re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data):issues.append(path)
result={'files':len(records),'issues_paths_only':issues,'known_local_credentials_checked_in_memory':bool(secrets),'records':records}
out=root/'progress/ALIGN_0920'/('staged-security-'+str(len(records))+'.json');out.write_text(json.dumps(result,indent=2)+'\n')
assert not issues,'Potential credential: stop'
print(json.dumps({'files':len(records),'issues':len(issues),'report':str(out)}))
