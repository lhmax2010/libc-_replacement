import hashlib,json,pathlib,stat,subprocess,time
base=pathlib.Path('progress/ARM_LZMA_MODE_0921')
src=pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm')
dst=pathlib.Path('tmp/ARM_LZMA_0921B/previous-gdb-failure')
assert src.is_dir() and not dst.exists()
cmd=['cp','-a','--',str(src),str(dst)];t=time.monotonic()
r=subprocess.run(cmd,capture_output=True,text=True)
(base/'previous-copy-command.json').write_text(json.dumps({'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'seconds':time.monotonic()-t},indent=2)+'\n')
assert r.returncode==0
def digest(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for data in iter(lambda:f.read(1024*1024),b''):h.update(data)
    return h.hexdigest()
rows=[]
for p in sorted(src.rglob('*')):
    rel=p.relative_to(src);q=dst/rel;a=p.lstat();b=q.lstat()
    assert (a.st_mode,a.st_uid,a.st_gid)==(b.st_mode,b.st_uid,b.st_gid)
    row={'path':str(rel),'mode':oct(a.st_mode),'size':a.st_size,'mtime_ns':a.st_mtime_ns,'ctime_ns_original':a.st_ctime_ns}
    if stat.S_ISREG(a.st_mode):
        sha=digest(p);assert sha==digest(q),str(p);row['sha256']=sha
    elif stat.S_ISLNK(a.st_mode):assert p.readlink()==q.readlink();row['link']=str(p.readlink())
    rows.append(row)
(base/'previous-preserved-manifest.json').write_text(json.dumps({'source':str(src),'backup':str(dst),'all_verified':True,'entries':rows},indent=2)+'\n')
print(json.dumps({'all_verified':True,'entries':len(rows),'seconds':time.monotonic()-t,'original_not_removed':True,'backup':str(dst)}))
