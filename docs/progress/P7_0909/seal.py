#!/usr/bin/env python3
"""封存第一阶段；核对时间、范围、压缩字节、数字与原有未提交改动。"""
import csv
import datetime
import gzip
import hashlib
import json
import pathlib
import re
import subprocess

ROOT=pathlib.Path.cwd();OUT=ROOT/'docs/progress/P7_0909'
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda:f.read(4*1024*1024),b''):h.update(chunk)
    return h.hexdigest()
expected_dirty={'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
assert subprocess.check_output(['git','branch','--show-current']).decode().strip()=='codex/runtime-validation'
assert subprocess.check_output(['git','rev-parse','HEAD']).decode().strip()=='4210a3d84fa67bde9b2e2f28f68ef3b76e453627'
assert set(subprocess.check_output(['git','diff','--name-only']).decode().splitlines())==set(expected_dirty)
for name,expected in expected_dirty.items():assert sha(ROOT/name)==expected,name
assert sha(ROOT/'HQ_files/api_compatibility_report.md')=='6b88ab84cc3b3d915d21f88891937d523de778fc6187c2a32ef9e7e2ddc15ad4'
allowed_nonzero={'004_input_layout.exitcode':'2','007_extracted_sample.exitcode':'2','023_representative_headers.exitcode':'1'}
for p in (OUT/'raw').rglob('*.exitcode'):
    assert p.read_text().strip()==allowed_nonzero.get(p.name,'0'),p
for name in ['012_scope_scan','032_reclassify','034_scope_wording_sync','035_validate_scope','041_validate_compressed','042_independent_counts']:
    assert (OUT/'raw'/f'{name}.exitcode').read_text().strip()=='0'
h=hashlib.sha256()
with gzip.open(OUT/'tables/headers.tsv.gz','rb') as f:
    for block in iter(lambda:f.read(4*1024*1024),b''):h.update(block)
assert h.hexdigest()==json.loads((OUT/'RECLASSIFICATION.json').read_text())['after_headers_sha256']
observed=dict(line.split('\t') for line in (OUT/'raw/042_independent_counts.stdout').read_text().splitlines())
assert observed=={'CPP_DECLARATION_SIGNAL':'70247','C_DECLARATION_SHAPE':'107772','NOT_AVAILABLE':'69','TOTAL':'229181','UNRESOLVED_OR_INCLUDE_MACRO_ONLY':'51093'}
for name in ['REPORT.md','METHOD.md','WEB_SOURCES.md']:
    p=OUT/name
    for link in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if link.startswith(('http://','https://')):continue
        assert (p.parent/link.split('#',1)[0]).resolve().exists(),(name,link)
start=datetime.datetime.fromisoformat(json.loads((OUT/'raw/001_resource.time.json').read_text())['start'])
now=datetime.datetime.now(datetime.timezone.utc);elapsed=(now-start).total_seconds();assert elapsed<7200
handoff=dict(stage1='COMPLETE_AWAITING_CONFIRMATION',stage2='NOT_STARTED',start_utc=start.isoformat(),sealed_utc=now.isoformat(),elapsed_minutes=round(elapsed/60,2),time_limit_minutes=120,recommendation_families=27,required_user_confirmation='第二阶段范围及产品/profile/SDK开放边界',existing_reports_unchanged=True,platform_code_config_mutations='NONE',gerrit_pushes='NONE',headers_uncompressed_sha256=h.hexdigest())
(OUT/'HANDOFF.json').write_text(json.dumps(handoff,ensure_ascii=False,indent=2)+'\n')
scripts=sorted(list(OUT.glob('*.py'))+list((OUT/'initial_classification').glob('*.py'))+[OUT/'snapshots/resource_gate.sh'])
(OUT/'SCRIPT_SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(OUT)}\n' for p in scripts))
files=sorted(p for p in OUT.rglob('*') if p.is_file() and 'delivery' not in p.relative_to(OUT).parts and p.name not in {'SHA256SUMS','EVIDENCE_INDEX.tsv'})
assert all(p.stat().st_size<90*1024*1024 for p in files)
with (OUT/'EVIDENCE_INDEX.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['path','bytes','sha256']);w.writerows((str(p.relative_to(OUT)),p.stat().st_size,sha(p)) for p in files)
files.append(OUT/'EVIDENCE_INDEX.tsv')
(OUT/'SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(OUT)}\n' for p in sorted(files)))
print(json.dumps(dict(result='PASS',files=len(files),scripts=len(scripts),**handoff),ensure_ascii=False))
