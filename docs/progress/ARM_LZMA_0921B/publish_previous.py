"""Publish the prior task unchanged; large files get verified lossless storage."""
import base64,hashlib,json,lzma,pathlib,re,shutil
src=pathlib.Path('progress/ARM_LZMA_0921');dst=pathlib.Path('docs/progress/ARM_LZMA_0921')
assert not dst.exists(), 'Destination already exists; inspect, do not overwrite'
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
 p=pathlib.Path(name)
 if p.is_file():
  values=p.read_bytes().splitlines()
  if name=='.quickbuild-credentials':values=values[1:]
  for v in values:
   if len(v)>=4:secrets.extend((v,base64.b64encode(v)))
files=sorted(p for p in src.rglob('*') if p.is_file())
records=[]
for p in files:
 rel=p.relative_to(src);data=p.read_bytes()
 assert not any(s in data for s in secrets),('Potential credential; stop',str(rel))
 assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('Potential credential; stop',str(rel))
 h=hashlib.sha256(data).hexdigest();q=dst/rel;q.parent.mkdir(parents=True,exist_ok=True)
 if len(data)>16000000:
  packed=lzma.compress(data,preset=1);stored=[]
  for i,start in enumerate(range(0,len(packed),32*1024*1024)):
   part=q.with_name(q.name+f'.xz.part-{i:03d}')
   part.write_bytes(packed[start:start+32*1024*1024]);stored.append(str(part.relative_to(dst)))
  restored=lzma.decompress(b''.join((dst/n).read_bytes() for n in stored))
  assert hashlib.sha256(restored).hexdigest()==h
  mode='xz-32MiB-parts'
 else:
  shutil.copy2(p,q);assert hashlib.sha256(q.read_bytes()).hexdigest()==h
  mode='verbatim';stored=[str(rel)]
 assert hashlib.sha256(p.read_bytes()).hexdigest()==h,'Source changed while archiving'
 records.append({'original':str(p),'bytes':len(data),'sha256':h,'stored':stored,'mode':mode,'restoration_verified':True})
assert files==sorted(p for p in src.rglob('*') if p.is_file())
manifest={'source_unchanged':True,'files':len(records),'credential_scan_passed':True,'known_credentials_checked_in_memory':bool(secrets),'records':records}
(dst/'DELIVERY_MANIFEST.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n')
(dst/'ARCHIVE_STORAGE_README.md').write_text('''# 原样交付的存储说明

正文、原始记录和脚本未修改。超过 16000000 字节的文件仅做无损 xz 压缩，必要时切为 32 MiB 分片；原始文件仍保留在 progress/ARM_LZMA_0921/。
DELIVERY_MANIFEST.json 列出全部原文件、SHA256、分片和解压校验结果。按 stored 数组的顺序连接分片，经 xz 解压即可还原原文件；还原 SHA256 必须等于清单。
原报告中的“未推送”是当时结案状态，按本轮“推送前不改内容”的要求原样保留；本次推送回执另记在 ARM_LZMA_0921B。
''')
print(json.dumps({'files':len(records),'source_unchanged':True,'credential_scan_passed':True,'compressed':[r for r in records if r['mode']!='verbatim']},ensure_ascii=False,indent=2))
