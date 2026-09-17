import hashlib
import json
import shutil
from pathlib import Path

src=Path('progress/BUILD_STATIC_0917B/resume_0917')
dst=Path('docs/progress/BUILD_STATIC_0917B/resume_0917')
stage=dst/'resume_stop_1909'
assert not Path('password.md').exists()
manifest=json.loads((stage/'MANIFEST.json').read_text())
for row in manifest['files']:
    f=Path(row['delivery'])
    assert hashlib.sha256(f.read_bytes()).hexdigest()==row['sha256'],str(f)
copied=[]
for f in sorted((src/'raw').iterdir()):
    label=f.name.split('.')[0]
    if not (f.parent/(label+'.finished.txt')).exists(): continue
    target=dst/'raw'/f.name
    if target.exists():
        assert target.read_bytes()==f.read_bytes(),str(target)
    else:
        shutil.copyfile(f,target)
        copied.append({'path':str(target),'sha256':hashlib.sha256(target.read_bytes()).hexdigest()})
for f in ('FINAL_RESULT.md','STATUS.md','ARM_RPM_PARTIAL.md','copy_read_diagnostic.json',
          'copy_rsync.strace','copy_cp.strace'):
    assert (stage/f).is_file()
assert (stage/'interrupted_raw/269_copy_aarch_inputs.command.txt').is_file()
assert not (stage/'interrupted_raw/269_copy_aarch_inputs.exitcode').exists()
shutil.copyfile(__file__,stage/'final_delivery_check.py')
result={'status':'DELIVERY_FILES_VERIFIED_NOT_BUILD_GATE_PASS',
        'manifest_files_sha256_verified':len(manifest['files']),
        'interrupted_copy_exitcode':'NOT_OBSERVED; no fabricated completion',
        'additional_completed_records':copied,
        'password_file_absent':True,
        'exact_password_string_scan':'NOT_OBSERVED; credential not retained or read',
        'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
(stage/'FINAL_DELIVERY_CHECK.json').write_text(json.dumps(result,indent=2))
print(json.dumps({k:v for k,v in result.items() if k!='additional_completed_records'},indent=2))
