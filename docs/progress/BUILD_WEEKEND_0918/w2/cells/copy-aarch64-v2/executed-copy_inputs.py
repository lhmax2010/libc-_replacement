"""Approved cp -a input copy, followed by per-file SHA256 verification."""
import hashlib, json, os, shlex, stat, subprocess, sys
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; task=p/'tmp/WEEKEND_0918'
arch=sys.argv[1]; assert arch in ('armv7l','aarch64')
source=p/f'tmp/GBS-ROOT/BUILD-W2C-bpftrace-{arch}-libcxx/local/BUILD-ROOTS/scratch.{arch}.0'
dest=task/f'input-{arch}'; dest.mkdir(parents=True,exist_ok=(arch=='aarch64'))
records=[]
def run(cmd,stream=False):
    idx=len(records); print('COMMAND '+shlex.join(cmd),flush=True)
    so=out/f'copy-v2-{arch}-{idx:02d}.stdout'; se=out/f'copy-v2-{arch}-{idx:02d}.stderr'
    with so.open('wb') as o,se.open('wb') as e:
        child=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=e)
        for line in child.stdout:
            o.write(line)
            if stream: print('COPY_ENTRY '+line.decode(errors='replace').rstrip(),flush=True)
        rc=child.wait()
    records.append({'command':shlex.join(cmd),'exitcode':rc,'stdout':str(so),'stderr':str(se)})
    (out/f'copy-v2-{arch}-commands.json').write_text(json.dumps(records,indent=2))
    assert rc==0,(cmd,rc,str(se))
    return so.read_text(errors='replace')
du=run(['du','-s','--block-size=1','--exclude=upgrade',str(source/'usr'),str(source/'var/lib/rpm')])
size=sum(int(x.split()[0]) for x in du.splitlines()); assert size<=3*1024**3
print('COPY_SIZE_GATE',size,flush=True)
for d in ('usr','usr/share','rpmdb','etc','root'): (dest/d).mkdir(parents=True,exist_ok=True)
os.chmod(dest/'usr/share',stat.S_IMODE((source/'usr/share').stat().st_mode))
(dest/'usr/sbin').mkdir(exist_ok=True)
cp=['cp','-av','--no-preserve=ownership','--']
main=sorted(x for x in (source/'usr').iterdir() if x.name not in ('share','sbin'))
run(cp+list(map(str,main))+[str(dest/'usr')+'/'],True)
share=sorted(x for x in (source/'usr/share').iterdir() if x.name!='upgrade')
run(cp+list(map(str,share))+[str(dest/'usr/share')+'/'],True)
run(cp+[str(source/'var/lib/rpm')+'/.',str(dest/'rpmdb')+'/'],True)
public=['ld.so.conf','ld.so.cache','ld.so.conf.d','passwd','group','nsswitch.conf','os-release','rpm','localtime']
public_present=[]
for name in public:
    f=source/'etc'/name
    if not os.path.lexists(f): continue
    run(cp+[str(f),str(dest/'etc')+'/'],True); public_present.append(name)
def entries(base,omit_upgrade=False):
    result={}
    for root,dirs,files in os.walk(base,followlinks=False):
        rel=Path(root).relative_to(base)
        if omit_upgrade and rel==Path('.') and 'sbin' in dirs: dirs.remove('sbin')
        if omit_upgrade and rel==Path('share') and 'upgrade' in dirs: dirs.remove('upgrade')
        for name in dirs+files:
            f=Path(root)/name; result[str(f.relative_to(base))]=f
    return result
def digest(f):
    with f.open('rb') as stream: return hashlib.file_digest(stream,'sha256').hexdigest()
manifest=out/f'copy-{arch}-sha256.jsonl'; count=0
with manifest.open('x') as stream:
    for label,old,new,omit in [('usr',source/'usr',dest/'usr',True),('rpmdb',source/'var/lib/rpm',dest/'rpmdb',False)]:
        a,b=entries(old,omit),entries(new,omit)
        assert set(a)==set(b),(label,sorted(set(a)^set(b))[:20])
        for rel,f in sorted(a.items()):
            g=b[rel]; sa,sd=f.lstat(),g.lstat()
            assert stat.S_IFMT(sa.st_mode)==stat.S_IFMT(sd.st_mode),(rel,'type')
            row={'group':label,'path':rel,'mode_source':stat.S_IMODE(sa.st_mode),'mode_destination':stat.S_IMODE(sd.st_mode)}
            assert row['mode_source']==row['mode_destination'],(rel,'mode')
            if f.is_symlink():
                row.update(link_source=os.readlink(f),link_destination=os.readlink(g))
                assert row['link_source']==row['link_destination']
            elif f.is_file():
                row.update(source_sha256=digest(f),destination_sha256=digest(g),bytes=sa.st_size)
                assert row['source_sha256']==row['destination_sha256'],rel
            else: assert f.is_dir(),(rel,'unexpected special file')
            stream.write(json.dumps(row)+'\n'); count+=1
            print('COPY_ENTRY verified '+label+'/'+rel,flush=True)
    for name in public_present:
        a,b=source/'etc'/name,dest/'etc'/name
        pairs=[('',a,b)] if not a.is_dir() else [(k,v,entries(b)[k]) for k,v in entries(a).items()]
        for rel,f,g in pairs:
            row={'group':'public_etc','path':name+'/'+rel}
            if f.is_symlink():
                row.update(link_source=os.readlink(f),link_destination=os.readlink(g)); assert row['link_source']==row['link_destination']
            elif f.is_file():
                row.update(source_sha256=digest(f),destination_sha256=digest(g)); assert row['source_sha256']==row['destination_sha256']
            else: assert f.is_dir() and g.is_dir()
            stream.write(json.dumps(row)+'\n'); count+=1
base=['bwrap','--unshare-user','--uid','0','--gid','0','--ro-bind',str(source),'/',
      '--bind',str(dest/'usr'),'/usr','--bind',str(dest/'rpmdb'),'/var/lib/rpm',
      '--ro-bind',str(source/'usr/sbin'),'/usr/sbin',
      '--bind',str(dest/'etc'),'/etc','--bind',str(dest/'root'),'/root','--dev','/dev','--proc','/proc','--tmpfs','/tmp',
      '--bind',str(task),'/tmp/task','--ro-bind',str(p/'tmp/NIGHT_0917'),'/tmp/night',
      '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','CMAKE_BUILD_PARALLEL_LEVEL','1',
      '--setenv','RPM_BUILD_NCPUS','1','--setenv','MAKEFLAGS','-j1','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C']
(out/f'bpf-private-{arch}-base.json').write_text(json.dumps(base,indent=2))
run(base+['--','id'])
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
(out/f'copy-{arch}-result.json').write_text(json.dumps({'status':'COPY_AND_PER_FILE_SHA256_PASS','arch':arch,
    'bytes_gate':size,'verified_entries':count,'manifest_sha256':digest(manifest),'excluded':'usr/share/upgrade; usr/sbin retained read-only from original root, NOT hash-verified; no arbitrary private etc/home files',
    'ownership':'destination owned by current user, no sudo','source':str(source),'destination':str(dest)},indent=2))
print('COPY_AND_PER_FILE_SHA256_PASS',arch,count,flush=True)
