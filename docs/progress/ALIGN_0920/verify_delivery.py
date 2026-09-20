import hashlib,json,lzma,pathlib
b=pathlib.Path('docs/progress/ARM_RPM_DIAG');rows=json.loads((b/'DELIVERY_MANIFEST.json').read_text());checked=[]
for r in rows:
    h=hashlib.sha256();size=0;dec=lzma.LZMADecompressor() if r['encoding']!='identity' else None
    for part in r['delivery']:
        with (b/part).open('rb') as f:
            while chunk:=f.read(1024*1024):
                if dec:
                    data=dec.decompress(chunk,max_length=1024*1024);h.update(data);size+=len(data)
                    while not dec.needs_input and not dec.eof:
                        data=dec.decompress(b'',max_length=1024*1024);h.update(data);size+=len(data)
                else:h.update(chunk);size+=len(chunk)
    assert not dec or dec.eof,r['logical_path']
    assert size==r['bytes'] and h.hexdigest()==r['sha256'],r['logical_path']
    checked.append(r['logical_path'])
result={'verified_files':len(checked),'original_byte_size_and_sha256_all_equal':True,'omitted':[]}
(b/'DELIVERY_VERIFICATION.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
