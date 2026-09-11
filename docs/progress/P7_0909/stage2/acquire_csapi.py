#!/usr/bin/env python3
"""从既有、摘要相符的源码 RPM 仅读取 NUI 相关源码到任务临时区，不构建。"""
import pathlib,hashlib,subprocess,tarfile,csv,json
O=pathlib.Path('docs/progress/P7_0909/stage2');T=pathlib.Path('tmp/P7_0909/csapi');T.mkdir(parents=True,exist_ok=True)
rpm=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/srpms/unified_source/csapi-tizenfx-15.0.0.19383-1.src.rpm')
h=hashlib.sha256()
with rpm.open('rb') as f:
    for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
assert h.hexdigest()=='472490abfd8201b5dc01e412779a0cbebbd7499616e5f09f18a33bac67de9015'
archive='csapi-tizenfx-15.0.0.19383.tar.gz'
a=subprocess.Popen(['rpm2cpio',str(rpm)],stdout=subprocess.PIPE)
b=subprocess.run(['cpio','-idm','--quiet',archive],stdin=a.stdout,cwd=T)
a.stdout.close();rc=a.wait();print('rpm2cpio exit',rc,'cpio exit',b.returncode,flush=True);assert rc==b.returncode==0
rows=[];listing=[]
with tarfile.open(T/archive,'r|gz') as tf:
    for m in tf:
        listing.append(m.name)
        if not m.isfile() or not ('Tizen.NUI' in m.name or m.name.endswith(('csapi-tizenfx.spec','TizenFX.sln'))):continue
        path=pathlib.PurePosixPath(m.name);assert not path.is_absolute() and '..' not in path.parts
        content=tf.extractfile(m).read();dest=T/path;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(content)
        rows.append(dict(member=m.name,bytes=len(content),sha256=hashlib.sha256(content).hexdigest(),local=str(dest)))
with (O/'tables/csapi_selected_members.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
(O/'tables/csapi_archive_files.txt').write_text('\n'.join(listing)+'\n')
print(json.dumps(dict(rpm=str(rpm),rpm_sha256=h.hexdigest(),archive_files=len(listing),selected_files=len(rows),temporary_root=str(T))))
