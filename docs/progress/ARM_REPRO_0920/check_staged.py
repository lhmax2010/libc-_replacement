import base64,hashlib,json,pathlib,re,subprocess
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
    p=pathlib.Path(name)
    if p.is_file():
        values=p.read_bytes().splitlines()
        if name=='.quickbuild-credentials':values=values[1:]
        for s in values:
            if len(s)>=4:secrets.extend((s,base64.b64encode(s)))
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/r5-ehabi-diagnosis'
rows=[];issues=[]
for raw in subprocess.check_output(['git','diff','--cached','--name-only','-z']).split(b'\0'):
    if not raw:continue
    name=raw.decode();assert name.startswith('docs/progress/ARM_REPRO_0920/'),name
    data=subprocess.check_output(['git','show',':'+name])
    if any(s in data for s in secrets) or re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data):issues.append(name)
    rows.append({'path':name,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
out=pathlib.Path('progress/ARM_REPRO_0920')/f'staged-security-{len(rows)}.json'
out.write_text(json.dumps({'files':len(rows),'issues_paths_only':issues,'records':rows},indent=2)+'\n')
assert not issues,'Potential credential; stop'
print(json.dumps({'files':len(rows),'issues':0,'report':str(out)}))
