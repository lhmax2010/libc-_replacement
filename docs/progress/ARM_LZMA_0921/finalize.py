"""Preserve calibration payloads and verify the report's numeric assertions."""
import ast,csv,hashlib,json,pathlib,shutil
b=pathlib.Path('progress/ARM_LZMA_0921');w=pathlib.Path('tmp/ARM_LZMA_0921')
r=json.loads((b/'RESULT.json').read_text());p=json.loads((b/'POSTCHECK.json').read_text())
assert r['exitcode']==1 and len(r['wrote'])==18
assert r['integrity']=={'rows':209303,'sequences_exactly_1_to_N':True,'monotonic_nondecreasing':True}
assert r['return_counts']=={'0':209285,'1':18} and r['error_returns']==[]
assert len(p['archive_comparison'])==18
assert all(x['archive_size_equals_stream_total_in'] for x in p['archive_comparison'])
assert p['errno_pairs']=={'38/38':209303} and p['failed_host_write_counts']=={}
assert all(x['stream_end_in_same_order']['action']=='3' for x in p['archive_comparison'])
assert {pathlib.Path(x['path']).name for x in r['rpm_inventory'] if not x['wrote_in_this_run']}=={
 'lldb-devel-debuginfo-22.1.8-1.armv7l.rpm','compiler-rt-debuginfo-22.1.8-1.armv7l.rpm','libomp-debuginfo-22.1.8-1.armv7l.rpm'}
md=json.loads((b/'cells/preload-original/failure-file-metadata.json').read_text())
debug=[x for x in md if '/BUILDROOT/' in x['path']]
assert len(debug)==5 and sum(x['size'] for x in debug)==344668300
assert all(x['stable_size_mtime_ctime_during_read'] for x in debug)
for f in b.glob('*.py'):ast.parse(f.read_text(),filename=str(f))
cal=b/'calibration-evidence';cal.mkdir(exist_ok=True)
for f in sorted(w.glob('monotonic-*')):
 if f.is_file():
  dest=cal/f.name
  assert not dest.exists()
  shutil.copy2(f,dest)
src=b/'source-reference-snapshots';src.mkdir(exist_ok=True)
for name in ('upstream-rpmio.c','upstream-rpmfi.c','upstream-cpio.c','upstream-pack.c'):
 dest=src/name;assert not dest.exists();shutil.copy2(pathlib.Path('progress/ARM_RPM_DIAG')/name,dest)
manifest=[]
for f in sorted(b.rglob('*')):
 if not f.is_file() or f.name in ('FINAL_SHA256.json','SELF_CHECK.json') or f.name.startswith('093_'):continue
 h=hashlib.sha256()
 with f.open('rb') as source:
  for chunk in iter(lambda:source.read(1024*1024),b''):h.update(chunk)
 manifest.append({'path':str(f.relative_to(b)),'bytes':f.stat().st_size,'sha256':h.hexdigest()})
(b/'FINAL_SHA256.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n')
check={'result':'PASS','assertions':'numeric assertions, archive matching, old RPM distinction, captured file metadata, Python syntax','files_hashed':len(manifest),'calibration_files_preserved':len(list(cal.iterdir())),'manifest_excludes':'manifest/selfcheck and current logger raw/093 files; excludes no earlier completed raw evidence','no_new_build':True,'no_push':True}
(b/'SELF_CHECK.json').write_text(json.dumps(check,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(check,ensure_ascii=False,indent=2))
