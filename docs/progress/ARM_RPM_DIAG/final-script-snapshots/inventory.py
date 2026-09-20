"""Metadata only: preserve file types, hardlinks, extents and attributes."""
import collections,csv,json,os,stat,subprocess
from pathlib import Path
p=Path.cwd();out=p/'progress/ARM_RPM_DIAG'
root=p/'tmp/WEEKEND_0918/rpm-arm-llvm/BUILDROOT/llvm-22.1.8-1.arm'
rows=[]
for parent,dirs,files in os.walk(root):
    for name in sorted(dirs+files):
        f=Path(parent)/name;s=f.lstat(); rel='/'+str(f.relative_to(root))
        kind='file' if stat.S_ISREG(s.st_mode) else 'directory' if stat.S_ISDIR(s.st_mode) else 'symlink' if stat.S_ISLNK(s.st_mode) else 'fifo' if stat.S_ISFIFO(s.st_mode) else 'char' if stat.S_ISCHR(s.st_mode) else 'block' if stat.S_ISBLK(s.st_mode) else 'socket' if stat.S_ISSOCK(s.st_mode) else 'other'
        try: attrs=json.dumps(os.listxattr(f,follow_symlinks=False))
        except OSError as e: attrs='NOT_OBSERVED: '+str(e)
        rows.append(dict(path=rel,type=kind,mode=oct(stat.S_IMODE(s.st_mode)),uid=s.st_uid,gid=s.st_gid,nlink=s.st_nlink,inode=s.st_ino,device=s.st_dev,size=s.st_size,blocks512=s.st_blocks,sparse=kind=='file' and s.st_blocks*512<s.st_size,path_bytes=len(os.fsencode(rel)),max_component_bytes=max(map(lambda v:len(os.fsencode(v)),rel.split('/'))),xattrs=attrs,target=os.readlink(f) if kind=='symlink' else ''))
with (out/'historical-buildroot-file-metadata.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=rows[0].keys(),delimiter='\t');w.writeheader();w.writerows(rows)
with (out/'first-18-rpm-files.tsv').open('w') as f:
    f.write('rpm\tpackage\tpath\tmode\tfile_size\tlink_target\tinode\n')
    for rpm in sorted((p/'tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l').glob('*.rpm')):
        cmd=['rpm','-qp','--qf','[%{=NAME}\t%{FILENAMES}\t%{FILEMODES:perms}\t%{FILESIZES}\t%{FILELINKTOS}\t%{FILEINODES}\n]',str(rpm)]
        r=subprocess.run(cmd,capture_output=True,text=True)
        (out/('rpm-query-'+rpm.stem+'.json')).write_text(json.dumps({'command':cmd,'exitcode':r.returncode,'stderr':r.stderr},indent=2)+'\n')
        assert r.returncode==0,r.stderr
        for line in r.stdout.splitlines(): f.write(rpm.name+'\t'+line+'\n')
summary={'count':len(rows),'types':dict(collections.Counter(r['type'] for r in rows)),'max_path_bytes':max(r['path_bytes'] for r in rows),'max_file_bytes':max(r['size'] for r in rows if r['type']=='file'),'sparse_files':sum(r['sparse'] for r in rows),'special_modes':sum(bool(int(r['mode'],8)&0o7000) for r in rows),'xattr_entries':sum(r['xattrs']!='[]' for r in rows),'scope':'historical BUILDROOT, not new reproduction'}
(out/'historical-metadata-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary))
