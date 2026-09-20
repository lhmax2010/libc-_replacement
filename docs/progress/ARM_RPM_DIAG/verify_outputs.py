"""Read-only RPM verification and file-property comparison; no repair."""
import collections,csv,hashlib,json,os,stat,subprocess
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG');task=Path('tmp/ARM_RPM_DIAG/rpm-arm-llvm')
newroot=task/'BUILDROOT/llvm-22.1.8-1.arm'
oldroot=Path('tmp/WEEKEND_0918/rpm-arm-llvm/BUILDROOT/llvm-22.1.8-1.arm')
log=(b/'cells/llvm-arm-repro/build.log').read_text()
order=[line.split('/')[-1] for line in log.splitlines() if line.startswith('Wrote: ')]
assert len(order)==len(set(order))==22
manifest=[];features=[];details=[]
fmt='[%{FILENAMES}\t%{FILEMODES}\t%{FILESIZES}\t%{FILELINKTOS}\t%{FILEINODES}\t%{FILEDEVICES}\t%{FILEFLAGS}\n]'
def digest(path):
    h=hashlib.sha256()
    with path.open('rb') as f:
        while chunk:=f.read(1024*1024):h.update(chunk)
    return h.hexdigest()
def call(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True)
    return {'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr}
for index,name in enumerate(order,1):
    p=task/'RPMS/armv7l'/name
    check=call(['rpm','-K','--nosignature',str(p)])
    query=call(['rpm','-qp','--nodigest','--nosignature','--qf',fmt,str(p)])
    assert check['exitcode']==query['exitcode']==0,(name,check,query)
    rows=list(csv.reader(query['stdout'].splitlines(),delimiter='\t'))
    (b/f'output-{index:02d}.json').write_text(json.dumps({'rpm':str(p),'check':check,'query':query},indent=2)+'\n')
    counts=collections.Counter();links=collections.Counter((r[5],r[4]) for r in rows if stat.S_ISREG(int(r[1])))
    for r in rows:
        mode=int(r[1]);kind='regular' if stat.S_ISREG(mode) else 'directory' if stat.S_ISDIR(mode) else 'symlink' if stat.S_ISLNK(mode) else 'fifo' if stat.S_ISFIFO(mode) else 'device' if stat.S_ISCHR(mode) or stat.S_ISBLK(mode) else 'other'
        counts[kind]+=1
    features.append({'order':index,'rpm':name,'entries':len(rows),**{k:counts[k] for k in ['regular','directory','symlink','fifo','device','other']},'rpm_hardlink_groups':sum(v>1 for v in links.values()),'special_permission_entries':sum(bool(int(r[1])&0o7000) for r in rows),'max_path_bytes':max(len(os.fsencode(r[0])) for r in rows),'max_file_bytes':max(int(r[2]) for r in rows)})
    manifest.append({'order':index,'rpm':str(p),'bytes':p.stat().st_size,'sha256':digest(p),'digest_check_exitcode':check['exitcode']})
    if index!=19:continue
    (b/'package-19-file-list.tsv').write_text('path\tmode\tsize\tlink_target\trpm_inode\trpm_device\tflags\n'+query['stdout'])
    for r in rows:
        f=newroot/r[0].lstrip('/');old=oldroot/r[0].lstrip('/');s=f.lstat();o=old.lstat()
        d={'path':r[0],'rpm_mode':oct(int(r[1])),'rpm_size':int(r[2]),'actual_mode':oct(s.st_mode),'uid':s.st_uid,'gid':s.st_gid,'nlink':s.st_nlink,'inode':s.st_ino,'size':s.st_size,'blocks_bytes':s.st_blocks*512,'sparse':stat.S_ISREG(s.st_mode) and s.st_blocks*512<s.st_size,'xattrs':os.listxattr(f,follow_symlinks=False),'old_mode':oct(o.st_mode),'old_nlink':o.st_nlink,'old_size':o.st_size}
        if stat.S_ISREG(s.st_mode):d.update(sha256=digest(f),old_sha256=digest(old));d['content_equal']=d['sha256']==d['old_sha256']
        details.append(d)
for name,rows in [('output-manifest',manifest),('package-feature-comparison',features)]:
    with (b/(name+'.tsv')).open('w') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
(b/'package-19-metadata.json').write_text(json.dumps(details,indent=2)+'\n')
print(json.dumps({'rpm_count':len(manifest),'all_digest_checks_pass':True,'package19_features':features[18],'package19_metadata':details},indent=2))
