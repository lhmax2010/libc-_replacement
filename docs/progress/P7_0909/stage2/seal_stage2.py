#!/usr/bin/env python3
"""封存本批静态分类：范围、数量、链接、证据链、原有改动与脚本摘要。"""
import csv,datetime,hashlib,json,pathlib,re,subprocess
R=pathlib.Path.cwd();P=R/'docs/progress/P7_0909';O=P/'stage2'
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
    return h.hexdigest()
def rows(name):return list(csv.DictReader((O/'tables'/name).open(),delimiter='\t'))
assert subprocess.check_output(['git','branch','--show-current']).decode().strip()=='codex/runtime-validation'
assert subprocess.check_output(['git','rev-parse','HEAD']).decode().strip()=='31a92b1af1ca702fd24e3875b16d48a8dbff69ae'
protected={'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
assert set(subprocess.check_output(['git','diff','--name-only']).decode().splitlines())==set(protected)
assert not subprocess.check_output(['git','diff','--cached','--name-only'])
for name,value in protected.items():assert sha(R/name)==value
assert sha(R/'HQ_files/api_compatibility_report.md')=='6b88ab84cc3b3d915d21f88891937d523de778fc6187c2a32ef9e7e2ddc15ad4'
H=rows('headers.tsv');scope=[r for r in H if r['group'] in {'DALI_NUI','WEB'}]
assert len(scope)==1690 and {r['arch'] for r in scope}=={'x86_64'}
N=rows('native_summary.tsv');assert len(N)==12
totals={k:sum(int(r[k]) for r in N) for k in ['public_recipe_headers','internal_named_headers','public_name_candidates','c_shape','cpp_signal','unresolved']}
assert totals==dict(public_recipe_headers=287,internal_named_headers=67,public_name_candidates=220,c_shape=100,cpp_signal=105,unresolved=15)
assert sum(int(r['cpp_signal'])>0 for r in N)==11
assert [r['family'] for r in N if r['cpp_signal']=='0']==['capi-ui-inputmethod']
api=rows('API_CLASSIFICATION.tsv');assert len(api)==22 and len({r['id'] for r in api})==22
families={r['family'] for r in api};assert len(families)==11
expected={r['source_name'] for r in csv.DictReader((P/'tables/STAGE2_RECOMMENDED.tsv').open(),delimiter='\t') if r['group'] in {'DALI_NUI','WEB'}}
assert families==expected
for r in api:
    for p in r['evidence'].split(';'):assert (O/p).exists(),p
for p in O.glob('*.md'):
    for target in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if target.startswith(('https://','http://')):continue
        assert (p.parent/target.split('#',1)[0]).resolve().exists(),(p,target)
sources=rows('source_identity.tsv');assert {r['status'] for r in sources}=={'MATCH'}
for r in rows('evidence_files.tsv'):assert sha(R/r['snapshot'])==r['sha256']
for r in rows('native_public_scan.tsv'):assert sha(O/r['snapshot'])==r['file_sha256']
assert len(rows('elf_identity.tsv'))+len(rows('elf_identity_additional.tsv'))==17
for r in rows('elf_identity.tsv')+rows('elf_identity_additional.tsv'):
    for k in ['symbol_record','dynamic_record']:assert (O/r[k]).exists()
controls=json.loads((O/'tables/scan_controls.json').read_text());assert len(controls)==4 and all(r['expected']==r['observed'] for r in controls)
allowed={'005_corpus_index.exitcode','007_inventory.exitcode','021_binder_exception_macro.exitcode','028_webapi_message_transport.exitcode','047_scl_public.exitcode','083_csapi_optional_libraries.exitcode','119_diff_snapshot.exitcode'}
for p in (O/'raw').rglob('*.exitcode'):
    assert p.read_text().strip()==('1' if p.name in allowed else '0'),p
assert not list(O.rglob('*.pyc'))
S=O/'snapshots/scripts';S.mkdir(parents=True,exist_ok=True)
for p in [P/'record.py',P/'scan_scope.py',R/'tools/resource_gate.sh']:(S/p.name).write_bytes(p.read_bytes())
scripts=sorted(list(O.glob('*.py'))+list(S.iterdir()))
(O/'SCRIPT_SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(O)}\n' for p in scripts))
start=json.loads((O/'raw/001_resource.time.json').read_text())['start'];now=datetime.datetime.now(datetime.timezone.utc)
validation=dict(result='PASS',stage='第二阶段首批静态分类',scope_families=11,api_rows=22,native_families=12,native_totals=totals,elf_static_files=17,source_read_events=len(sources),runtime_tests='NOT_OBSERVED',sdk_final_allowlist='NOT_AVAILABLE',start_utc=start,sealed_utc=now.isoformat(),elapsed_minutes=round((now-datetime.datetime.fromisoformat(start)).total_seconds()/60,2),unrelated_changes_preserved=True,next_action='STOP_FOR_HUMAN_REVIEW',remaining_four_families='DESCRIBED_NOT_ANALYZED')
(O/'VALIDATION.json').write_text(json.dumps(validation,ensure_ascii=False,indent=2)+'\n')
files=sorted(p for p in O.rglob('*') if p.is_file() and 'delivery' not in p.relative_to(O).parts and p.name not in {'SHA256SUMS','EVIDENCE_INDEX.tsv'})
assert all(p.stat().st_size<90*1024*1024 for p in files)
with (O/'EVIDENCE_INDEX.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['path','bytes','sha256']);w.writerows((str(p.relative_to(O)),p.stat().st_size,sha(p)) for p in files)
files.append(O/'EVIDENCE_INDEX.tsv')
(O/'SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(O)}\n' for p in sorted(files)))
print(json.dumps(validation,ensure_ascii=False))
