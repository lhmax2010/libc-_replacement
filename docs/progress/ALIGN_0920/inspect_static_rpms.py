import hashlib,json,pathlib,subprocess
b=pathlib.Path('progress/ALIGN_0920');out=b/'static-identities';out.mkdir(exist_ok=True)
paths=[('approved_old','tmp/STATIC_0917B/rpm-arm-llvm/RPMS/armv7l'),('weekend_w2','tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l'),('diagnostic_success','tmp/ARM_RPM_DIAG/rpm-arm-llvm/RPMS/armv7l'),('stopped_parallel_not_approved','tmp/STATIC_0917B/rpm-arm-llvm/RPMS_stopped_parallel_1459/armv7l')]
rows=[]
def file_sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        while c:=f.read(1024*1024):h.update(c)
    return h.hexdigest()
for label,d in paths:
    rpm=pathlib.Path(d)/'llvm-static-devel-22.1.8-1.armv7l.rpm'
    q=['rpm','-qp','--nodigest','--nosignature','--qf','[%{FILENAMES}\t%{FILESIZES}\t%{FILEDIGESTS}\n]',str(rpm)]
    r=subprocess.run(q,capture_output=True,text=True);assert r.returncode==0
    (out/(label+'-header-files.tsv')).write_text('path\tbytes\tdigest\n'+r.stdout)
    check=['rpm','-K','--nosignature',str(rpm)];c=subprocess.run(check,capture_output=True,text=True)
    assert c.returncode==0,(label,c.stderr)
    producer=['rpm2cpio',str(rpm)];consumer=['cpio','-i','--to-stdout','./usr/lib/libclang.a']
    with (out/(label+'-rpm2cpio.stderr')).open('wb') as pe,(out/(label+'-cpio.stderr')).open('wb') as ce:
        p=subprocess.Popen(producer,stdout=subprocess.PIPE,stderr=pe)
        x=subprocess.Popen(consumer,stdin=p.stdout,stdout=subprocess.PIPE,stderr=ce);p.stdout.close()
        h=hashlib.sha256();size=0
        while data:=x.stdout.read(1024*1024):h.update(data);size+=len(data)
        cx=x.wait();cp=p.wait()
    assert cx==cp==0 and size>0,(label,cx,cp,size)
    row={'label':label,'rpm':str(rpm),'rpm_bytes':rpm.stat().st_size,'rpm_sha256':file_sha(rpm),'query_command':q,'query_exitcode':r.returncode,'digest_command':check,'digest_exitcode':c.returncode,'digest_stdout':c.stdout,'extract_commands':[producer,consumer],'extract_exitcodes':[cp,cx],'member':'./usr/lib/libclang.a','archive_bytes':size,'archive_sha256':h.hexdigest()}
    (out/(label+'.json')).write_text(json.dumps(row,indent=2)+'\n');rows.append(row)
(out/'summary.json').write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps([{k:r[k] for k in ('label','rpm','rpm_sha256','archive_bytes','archive_sha256')} for r in rows],indent=2))
