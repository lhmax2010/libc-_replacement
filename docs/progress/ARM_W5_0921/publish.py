"""Deliver evidence, preserving and verifying large logs losslessly."""
import base64,datetime,gzip,hashlib,json,pathlib,re
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
src=root/'progress/ARM_W5_0921';dst=root/'docs/progress/ARM_W5_0921'
secrets=[]
for name in ('password.md','.quickbuild-credentials'):
    p=root/name
    if p.is_file():
        values=p.read_bytes().splitlines()
        if name=='.quickbuild-credentials':values=values[1:]
        for value in values:
            if len(value)>=4:secrets.extend((value,base64.b64encode(value)))
records=[]
for p in sorted(src.rglob('*')):
    if not p.is_file():continue
    assert not p.is_symlink(),str(p)
    before=p.stat();data=p.read_bytes()
    assert not any(s in data for s in secrets),('Possible credential',str(p))
    assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}',data),('Possible secret',str(p))
    rel=p.relative_to(src);q=dst/rel;q.parent.mkdir(parents=True,exist_ok=True)
    digest=hashlib.sha256(data).hexdigest()
    if len(data)>16000000:
        packed=gzip.compress(data,compresslevel=1,mtime=0);stored=[]
        for i,pos in enumerate(range(0,len(packed),32*1024*1024)):
            part=q.with_name(q.name+f'.gz.part-{i:03d}')
            part.write_bytes(packed[pos:pos+32*1024*1024]);stored.append(str(part.relative_to(dst)))
        restored=gzip.decompress(b''.join((dst/n).read_bytes() for n in stored))
        assert hashlib.sha256(restored).hexdigest()==digest
        mode='gzip-32MiB-parts'
    else:
        q.write_bytes(data);stored=[str(rel)];mode='verbatim'
        assert hashlib.sha256(q.read_bytes()).hexdigest()==digest
    after=p.stat();assert (before.st_size,before.st_mtime_ns)==(after.st_size,after.st_mtime_ns),str(p)
    records.append(dict(original=str(p.relative_to(root)),bytes=len(data),sha256=digest,stored=stored,mode=mode,restoration_verified=True))
assert any(r['original'].endswith('/FINAL_RESULT.md') for r in records)
(dst/'DELIVERY_MANIFEST.json').write_text(json.dumps(dict(completed_at=datetime.datetime.now().astimezone().isoformat(),credential_scan='PASS',files=len(records),records=records),ensure_ascii=False,indent=2)+'\n')
(dst/'README.md').write_text('''# armv7l LLVM w5.xzdio 整轮写包验证

先读 FINAL_RESULT.md；命令的唯一差异见 ARGV_DIFF.json，隔离宏实测见 PAYLOAD_PRECHECK.json。
完整写包记录在 cells/w5-original/；22 包校验见 RPM_RESULTS.json，归档身份对照见 FIVE_STATIC_IDENTITIES.tsv。
线程原始采样与汇总分别为 cells/w5-original/threads.jsonl 和 THREAD_SUMMARY.json。
DELIVERY_MANIFEST.json 列出全部材料 SHA256。大型日志按 stored 顺序连接 gzip 分片后解压；已回验无损。
RPM、提取的静态归档与旧失败树保留在本机 tmp/，不提交大型二进制；报告列出路径和身份。
''')
print(json.dumps(dict(files=len(records),compressed=[r['original'] for r in records if r['mode']!='verbatim'],credential_scan='PASS'),ensure_ascii=False))
