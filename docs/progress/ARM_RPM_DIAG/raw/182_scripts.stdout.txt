"""Analyze retained bytes of failed outputs; never regard them as valid RPMs."""
import csv,json,lzma,os,stat,struct,subprocess
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG');d=b/'observed-header-prefixes'
assert (b/'raw/045_reproduce.exitcode').exists(),'Run only after the single reproduction exits'
failed=list(d.glob('*.deleted.header-prefix'))
print('Deleted output prefixes:',[p.name for p in failed])
for p in failed:
    ident=p.name.removesuffix('.deleted.header-prefix');out=b/('failed-'+ident);out.mkdir(exist_ok=False)
    fmt='[%{FILENAMES}\t%{FILEMODES}\t%{FILESIZES}\t%{FILELINKTOS}\t%{FILEINODES}\t%{FILEDEVICES}\t%{FILEFLAGS}\n]'
    cmd=['rpm','-qp','--nodigest','--nosignature','--qf',fmt,str(p)]
    r=subprocess.run(cmd,capture_output=True,text=True)
    (out/'query.json').write_text(json.dumps({'command':cmd,'exitcode':r.returncode,'stderr':r.stderr,'header_query_only_not_integrity_verification':True},indent=2)+'\n')
    (out/'header-files.tsv').write_text('path\tmode\tsize\tlink_target\trpm_inode\trpm_device\tflags\n'+r.stdout)
    assert r.returncode==0,r.stderr
    rows=[];root=Path('tmp/ARM_RPM_DIAG/rpm-arm-llvm/BUILDROOT/llvm-22.1.8-1.arm')
    for row in csv.DictReader((out/'header-files.tsv').open(),delimiter='\t'):
        f=root/row['path'].lstrip('/')
        try:
            s=f.lstat();attrs=os.listxattr(f,follow_symlinks=False)
            row.update(actual_mode=oct(s.st_mode),uid=s.st_uid,gid=s.st_gid,nlink=s.st_nlink,inode=s.st_ino,bytes=s.st_size,allocated_bytes=s.st_blocks*512,sparse=stat.S_ISREG(s.st_mode) and s.st_blocks*512<s.st_size,xattrs=json.dumps(attrs),path_bytes=len(os.fsencode(row['path'])),actual_link=os.readlink(f) if stat.S_ISLNK(s.st_mode) else '')
        except OSError as e:row.update(error=str(e))
        rows.append(row)
    (out/'file-metadata.json').write_text(json.dumps(rows,indent=2)+'\n')
    data=p.read_bytes();assert data[:4]==b'\xed\xab\xee\xdb'
    def header_end(offset):
        assert data[offset:offset+3]==b'\x8e\xad\xe8'
        n,size=struct.unpack_from('>II',data,offset+8)
        return offset+16+n*16+size
    main=(header_end(96)+7)&~7;payload=header_end(main)
    dec=lzma.LZMADecompressor();body=dec.decompress(data[payload:])
    (out/'observed-payload-prefix.cpio').write_bytes(body)
    entries=[];offset=0
    while offset+110<=len(body):
        magic=body[offset:offset+6];assert magic in (b'070701',b'070702'),(offset,magic)
        fields=[int(body[offset+6+i*8:offset+14+i*8],16) for i in range(13)]
        n=fields[11];size=fields[6];name=body[offset+110:offset+110+n].rstrip(b'\0').decode(errors='replace')
        content=(offset+110+n+3)&~3
        entries.append({'header_offset':offset,'name':name,'mode':oct(fields[1]),'nlink':fields[4],'inode':fields[0],'declared_size':size,'body_offset':content,'body_end':content+size,'complete_content_in_saved_prefix':content+size<=len(body)})
        offset=(content+size+3)&~3
        if name=='TRAILER!!!':break
    result={'retained_prefix':str(p),'saved_bytes':len(data),'payload_offset':payload,'xz_eof':dec.eof,'decompressed_bytes':len(body),'next_expected_offset':offset,'entries':entries}
    (out/'cpio-prefix.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
