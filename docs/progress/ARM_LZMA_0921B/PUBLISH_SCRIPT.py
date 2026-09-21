import base64,datetime,gzip,hashlib,json,pathlib,re,resource,subprocess,sys
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
src=root/'progress/ARM_LZMA_0921B';dst=root/'docs/progress/ARM_LZMA_0921B'
mem=int(next(x.split()[1] for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
resource.setrlimit(resource.RLIMIT_AS,(mem*30//100,mem*30//100))
assert not dst.exists(),'Destination exists; do not overwrite'
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
 p=root/name
 if p.is_file():
  vals=p.read_bytes().splitlines()
  if name=='.quickbuild-credentials':vals=vals[1:]
  for v in vals:
   if len(v)>=4:secrets.extend((v,base64.b64encode(v)))
files=sorted(p for p in src.rglob('*') if p.is_file())
records=[];start=datetime.datetime.now(datetime.timezone.utc).isoformat()
for p in files:
 assert not p.is_symlink(),str(p)
 before=p.stat();data=p.read_bytes();after=p.stat()
 assert not any(s in data for s in secrets),('Potential credential; stopped',str(p.relative_to(src)))
 assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('Potential credential; stopped',str(p.relative_to(src)))
 rel=p.relative_to(src);q=dst/rel;q.parent.mkdir(parents=True,exist_ok=True)
 h=hashlib.sha256(data).hexdigest()
 if len(data)>16000000:
  packed=gzip.compress(data,compresslevel=1,mtime=0);stored=[]
  for i,pos in enumerate(range(0,len(packed),32*1024*1024)):
   part=q.with_name(q.name+f'.gz.part-{i:03d}');part.write_bytes(packed[pos:pos+32*1024*1024]);stored.append(str(part.relative_to(dst)))
  restored=gzip.decompress(b''.join((dst/n).read_bytes() for n in stored))
  assert hashlib.sha256(restored).hexdigest()==h
  mode='gzip-32MiB-parts'
 else:
  q.write_bytes(data);assert hashlib.sha256(q.read_bytes()).hexdigest()==h
  stored=[str(rel)];mode='verbatim'
 records.append(dict(original=str(p.relative_to(root)),bytes=len(data),sha256=h,stored=stored,mode=mode,restoration_verified=True,changed_during_read=(before.st_size,before.st_mtime_ns)!=(after.st_size,after.st_mtime_ns)))
assert any(r['original'].endswith('/STAGE1_RESULT.md') for r in records)
manifest=dict(snapshot_start_utc=start,snapshot_end_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),files=len(records),source_written=False,gdb_interrupted=False,credential_scan_passed=True,records=records)
(dst/'DELIVERY_MANIFEST.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n')
(dst/'ARCHIVE_STORAGE_README.md').write_text('''# 本次交付快照

ARM_LZMA_0921B 的正文、脚本与记录按读取时内容原样保存，含 STAGE1_RESULT.md。未修改原文件、未干预 GDB。

超过 16000000 字节的记录无损 gzip 压缩为最多 32 MiB 分片。DELIVERY_MANIFEST.json 的 stored 数组给出顺序；连接后 gzip 解压，所得 SHA256 必须等于 sha256。所有分片均已解压回验。

第二节仍可运行；本快照不宣称多文件同时刻一致，也不包含快照完成后新产生的记录。清单记录逐文件读取期间有无变化。原文中的历史状态不改写。
''')
(dst/'PUBLISH_SCRIPT.py').write_bytes(pathlib.Path(__file__).read_bytes())
print(json.dumps(dict(files=len(records),changed_during_read=[r['original'] for r in records if r['changed_during_read']],compressed=[r['original'] for r in records if r['mode']!='verbatim'],destination=str(dst)),ensure_ascii=False))
