import base64,csv,hashlib,json,os,pathlib,re,shutil,subprocess
root=pathlib.Path.cwd();src=root/'progress/ARM_RPM_DIAG';dst=root/'docs/progress/ARM_RPM_DIAG'
assert not dst.exists()
secrets=[]
for name in ['.quickbuild-credentials','password.md']:
    p=root/name
    if p.is_file():
        lines=p.read_bytes().splitlines()
        if name=='.quickbuild-credentials':lines=lines[1:]
        for value in lines:
            if len(value)>=4:secrets.extend([value,base64.b64encode(value)])
files=[(p,p.relative_to(src).as_posix()) for p in sorted(src.rglob('*')) if p.is_file()]
extra=root/'tmp/ARM_RPM_DIAG/guest-rpmbuild.strace'
assert extra.is_file();files.append((extra,'external/guest-rpmbuild.strace'))
manifest=[];issues=[]
patterns=[re.compile(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----'),re.compile(rb'gh[pousr]_[A-Za-z0-9]{30,}'),re.compile(rb'Authorization: (?:Basic|Bearer) [A-Za-z0-9+/=_-]{12,}',re.I)]
for p,rel in files:
    h=hashlib.sha256();tail=b''
    with p.open('rb') as f:
        while data:=f.read(1024*1024):
            h.update(data);window=tail+data
            if any(value in window for value in secrets) or any(r.search(window) for r in patterns):issues.append(rel);break
            tail=window[-4096:]
    manifest.append({'source':str(p.relative_to(root)),'logical_path':rel,'bytes':p.stat().st_size,'sha256':h.hexdigest()})
audit={'scanned_files':len(files),'known_credential_values_compared_in_memory':bool(secrets),'issues_paths_only':sorted(set(issues)),'scope':'Known local password values and encodings plus selected credential patterns; not a proof against every possible secret format.'}
(root/'progress/ALIGN_0920/security-before-delivery.json').write_text(json.dumps(audit,indent=2)+'\n')
assert not issues,'Potential secret detected; stop without printing value'
dst.mkdir(parents=True)
for (p,rel),entry in zip(files,manifest):
    if p.stat().st_size<=20*1024**2:
        target=dst/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
        entry['delivery']=[rel];entry['encoding']='identity';continue
    cmd=['xz','-T1','-1','-c',str(p)];parts=[];proc=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    index=0
    while chunk:=proc.stdout.read(32*1024**2):
        part=rel+'.xz.part-'+str(index).zfill(3);target=dst/part;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(chunk);parts.append(part);index+=1
    err=proc.stderr.read();rc=proc.wait()
    entry.update(delivery=parts,encoding='xz-parts-in-listed-order',compress_command=cmd,compress_exitcode=rc,compress_stderr=err.decode(errors='replace'))
    assert rc==0,(rel,rc)
(dst/'DELIVERY_MANIFEST.json').write_text(json.dumps(manifest,indent=2)+'\n')
(dst/'SECURITY_CHECK.json').write_text(json.dumps(audit,indent=2)+'\n')
print(json.dumps({'files':len(files),'original_bytes':sum(r['bytes'] for r in manifest),'compressed_files':[{k:r[k] for k in ('logical_path','bytes','delivery')} for r in manifest if r['encoding']!='identity'],'omitted_files':[]},indent=2))
