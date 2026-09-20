import hashlib,json,pathlib,stat,subprocess,time
base=pathlib.Path('progress/ARM_REPRO_0920');src=pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm');dst=pathlib.Path('tmp/ARM_REPRO_0920/previous-weekend-rpm-arm-llvm')
assert src.is_dir() and not dst.exists();dst.parent.mkdir(parents=True,exist_ok=True)
cmd=['cp','-a','--',str(src),str(dst)];t=time.monotonic();r=subprocess.run(cmd,capture_output=True,text=True)
(base/'preserve-command.json').write_text(json.dumps({'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'seconds':time.monotonic()-t},indent=2)+'\n')
assert r.returncode==0,'Copy failed; do not run build'
rows=[]
def digest(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
for p in sorted(src.rglob('*')):
    rel=p.relative_to(src);q=dst/rel;a=p.lstat();b=q.lstat();assert a.st_mode==b.st_mode and a.st_uid==b.st_uid and a.st_gid==b.st_gid,(str(p),'metadata')
    row={'path':str(rel),'mode':oct(a.st_mode),'size':a.st_size,'mtime_ns':a.st_mtime_ns}
    if stat.S_ISREG(a.st_mode):
        x=digest(p);y=digest(q);assert x==y,(str(p),'sha256');row['sha256']=x
    elif stat.S_ISLNK(a.st_mode):assert p.readlink()==q.readlink();row['link']=str(p.readlink())
    rows.append(row)
(base/'preserved-input-manifest.json').write_text(json.dumps({'source':str(src),'backup':str(dst),'all_verified':True,'entries':rows},indent=2)+'\n')
print(json.dumps({'all_verified':True,'entries':len(rows),'seconds':time.monotonic()-t,'original_not_removed':True}))
