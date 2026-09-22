"""Read-only immediate preservation of a failed cell's filesystem metadata."""
import hashlib,json,os,stat,subprocess,sys,time
from pathlib import Path
p=Path.cwd(); label=sys.argv[1]; assert label.startswith('bpf-')
top=p/'tmp/BPF_STATIC_0922'/f'rpm-{label}'; out=p/'progress/BPF_STATIC_0922/cells'/label/'failure'; out.mkdir(exist_ok=False)
records=[]
for d in (top/'BUILDROOT',top/'RPMS'):
    cmd=['ls','-laR',str(d)]; r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    records.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (out/'commands.json').write_text(json.dumps(records,indent=2))
    rows=[]
    for f in sorted(d.rglob('*')):
        s=f.lstat(); row=dict(path=str(f),bytes=s.st_size,mode=stat.filemode(s.st_mode),uid=s.st_uid,gid=s.st_gid,mtime_ns=s.st_mtime_ns,ctime_ns=s.st_ctime_ns)
        if stat.S_ISLNK(s.st_mode): row['target']=os.readlink(f)
        elif stat.S_ISREG(s.st_mode):
            h=hashlib.sha256()
            with f.open('rb') as stream:
                for b in iter(lambda:stream.read(1048576),b''): h.update(b)
            row['sha256']=h.hexdigest()
        rows.append(row)
    (out/(d.name+'.json')).write_text(json.dumps(rows,indent=2))
print('Failure filesystem preserved; no cleanup or retries performed.',time.time())

