import csv,hashlib,json,pathlib
base=pathlib.Path(__file__).resolve().parent;root=base.parent.parent
csv.field_size_limit(10000000)
rows=list(csv.DictReader((base/'BASE_73_LEDGER.tsv').open(),delimiter='\t'))
assert len(rows)==73 and len({r['源码包'] for r in rows})==73
assert not any(r['状态']=='NOT_EVALUATED' for r in rows)
assert all('R95' not in r['依据'] and '镜像' not in r['依据'] for r in rows)
assert all(r['状态']=='NO_CHANGE_NEEDED' for r in rows if r['源码包'] in ('ncurses','nghttp2','tiff','patchelf'))
for item in json.loads((base/'INPUT_IDENTITIES.json').read_text()):
 assert hashlib.sha256((root/item['path']).read_bytes()).hexdigest()==item['sha256']
scripts={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(base.glob('*.py'))}
(base/'SCRIPT_SHA256.json').write_text(json.dumps(scripts,indent=2)+'\n')
records=[]
for p in sorted(base.rglob('*')):
 if p.is_file() and p.name!='FILE_MANIFEST.json' and not (p.parent.name=='raw' and p.name.startswith('059_')):
  records.append(dict(path=str(p.relative_to(base)),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
(base/'FILE_MANIFEST.json').write_text(json.dumps(records,indent=2)+'\n')
print('PASS: 73 unique rows; four named packages NO_CHANGE_NEEDED; no image/R95 basis; original inputs unchanged; script and evidence hashes recorded.')
