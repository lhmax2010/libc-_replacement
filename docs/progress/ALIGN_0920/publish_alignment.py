import hashlib,json,pathlib,shutil,sys
src=pathlib.Path('progress/ALIGN_0920');dst=pathlib.Path('docs/progress/ALIGN_0920')
records=[]
for path in sorted(src.rglob('*')):
    if not path.is_file():continue
    # The logger that launches this copy is still open; archive that receipt later.
    if path.parent.name=='raw' and path.name.startswith(sys.argv[1]+'_'):continue
    rel=path.relative_to(src);target=dst/rel;target.parent.mkdir(parents=True,exist_ok=True)
    data=path.read_bytes();digest=hashlib.sha256(data).hexdigest()
    if target.exists():assert target.read_bytes()==data,('Would overwrite different evidence',str(target))
    else:shutil.copy2(path,target)
    assert hashlib.sha256(target.read_bytes()).hexdigest()==digest
    records.append({'source':str(path),'destination':str(target),'bytes':len(data),'sha256':digest,'verified':True})
(dst/'DELIVERY_MANIFEST.json').write_text(json.dumps({'mode':'all files copied verbatim; active copy receipt deferred','files':len(records),'bytes':sum(r['bytes'] for r in records),'records':records},ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'files':len(records),'bytes':sum(r['bytes'] for r in records),'all_verified':True}))
