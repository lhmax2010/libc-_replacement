"""Archive the completed previous run unchanged; verify lossless storage."""
import base64, datetime, gzip, hashlib, json, pathlib, re
root = pathlib.Path('/home/toolchain/development/libc++_replacement')
src = root/'progress/ARM_LZMA_MODE_0921'
dst = root/'docs/progress/ARM_LZMA_0921B/mode-entry'
secrets = []
for name in ('password.md', '.quickbuild-credentials'):
    p = root/name
    if p.is_file():
        values = p.read_bytes().splitlines()
        if name == '.quickbuild-credentials': values = values[1:]
        for v in values:
            if len(v) >= 4: secrets.extend((v, base64.b64encode(v)))
records = []
for p in sorted(src.rglob('*')):
    if not p.is_file(): continue
    assert not p.is_symlink(), str(p)
    before = p.stat()
    data = p.read_bytes()
    assert not any(s in data for s in secrets), ('Possible credential', str(p))
    assert not re.search(rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}', data), ('Possible secret', str(p))
    rel = p.relative_to(src)
    q = dst/rel
    q.parent.mkdir(parents=True, exist_ok=True)
    digest = hashlib.sha256(data).hexdigest()
    if len(data) > 16000000:
        packed = gzip.compress(data, compresslevel=1, mtime=0)
        stored = []
        for i, pos in enumerate(range(0, len(packed), 32*1024*1024)):
            part = q.with_name(q.name+f'.final.gz.part-{i:03d}')
            part.write_bytes(packed[pos:pos+32*1024*1024])
            stored.append(str(part.relative_to(dst)))
        restored = gzip.decompress(b''.join((dst/n).read_bytes() for n in stored))
        assert hashlib.sha256(restored).hexdigest() == digest
        mode = 'gzip-32MiB-parts'
    else:
        q.write_bytes(data)
        stored = [str(rel)]
        assert hashlib.sha256(q.read_bytes()).hexdigest() == digest
        mode = 'verbatim'
    after = p.stat()
    assert (before.st_size, before.st_mtime_ns) == (after.st_size, after.st_mtime_ns), str(p)
    records.append(dict(original=str(p.relative_to(root)), bytes=len(data), sha256=digest, stored=stored, mode=mode, restoration_verified=True))
assert any(r['original'].endswith('/FINAL_RESULT.md') for r in records)
(dst/'FINAL_DELIVERY_MANIFEST.json').write_text(json.dumps(dict(completed_at=datetime.datetime.now().astimezone().isoformat(), credential_scan='PASS', files=len(records), records=records), ensure_ascii=False, indent=2)+'\n')
(dst/'FINAL_ARCHIVE_README.md').write_text('''# 入口 mode 观测交付

本目录原样归档 progress/ARM_LZMA_MODE_0921 的完整记录。FINAL_RESULT.md 为结论。
FINAL_DELIVERY_MANIFEST.json 列出全部输入及 SHA256。大型 trace 无损 gzip 分片，按 stored 数组连接并解压后校验 sha256；已全部回验。
上一轮最终报告先行推送的 SHA 为 85e8238b72a2fab9505a9fc306b57ecf2ff69134。
''')
(dst/'FINAL_PUBLISH_SCRIPT.py').write_bytes(pathlib.Path(__file__).read_bytes())
print(json.dumps(dict(files=len(records), compressed=[r['original'] for r in records if r['mode']!='verbatim'], credential_scan='PASS'), ensure_ascii=False))
