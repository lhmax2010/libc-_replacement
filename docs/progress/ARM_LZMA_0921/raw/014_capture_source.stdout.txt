import hashlib,json,pathlib,stat,subprocess,sys,time
out=pathlib.Path(sys.argv[1]);root=pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm')
rows=[]
paths=list(root.glob('RPMS/armv7l/*'))
paths+=list(root.glob('BUILDROOT/*/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/bin/*'))
paths+=list(root.glob('BUILDROOT/*/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/lib/*'))
paths+=list(root.glob('BUILDROOT/*/usr/lib/debug/usr/bin/lldb-mcp.debug'))
for p in sorted(paths):
    s=p.lstat();r={'path':str(p),'capture_ns':time.time_ns(),'size':s.st_size,'mtime_ns':s.st_mtime_ns,'ctime_ns':s.st_ctime_ns,'mode':oct(s.st_mode),'uid':s.st_uid,'gid':s.st_gid,'inode':s.st_ino}
    rows.append(r)
# Capture sizes and timestamps for all paths before hashing any large file.
(out/'failure-file-metadata-immediate.json').write_text(json.dumps(rows,indent=2)+'\n')
for r in rows:
    p=pathlib.Path(r['path'])
    if stat.S_ISREG(int(r['mode'],8)):
        h=hashlib.sha256()
        with p.open('rb') as f:
            for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
        r['sha256']=h.hexdigest()
        after=p.stat();r['stable_size_mtime_ctime_during_read']=(after.st_size,after.st_mtime_ns,after.st_ctime_ns)==(r['size'],r['mtime_ns'],r['ctime_ns'])
(out/'failure-file-metadata.json').write_text(json.dumps(rows,indent=2)+'\n')
cmd=['ps','-eo','pid,ppid,ni,comm,args'];r=subprocess.run(cmd,capture_output=True,text=True)
(out/'failure-processes.json').write_text(json.dumps({'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr},indent=2)+'\n')
print(json.dumps({'files':len(rows),'no_files_removed':True}))
