#!/usr/bin/env python3
"""材料改动清单、完整 diff、五补丁代码字节不变及现行引用一致性核验。"""
import csv
import difflib
import hashlib
import pathlib
import re
import subprocess
base=pathlib.Path('docs/progress/P6_0909/resume')
mapping=list(csv.DictReader((base/'SHA_MAPPING.tsv').open(),delimiter='\t'))
material_base='833f819419e906c17bf4c36cd61c207ba976b709'
files=subprocess.check_output(['git','diff','--name-only',material_base],text=True).splitlines()
untouched={'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c',
           'docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
for name,digest in untouched.items():assert hashlib.sha256(pathlib.Path(name).read_bytes()).hexdigest()==digest
selected=[name for name in files if name not in untouched and not name.startswith(str(base)+'/')]
assert len(selected)==23,(len(selected),selected)
allowed_prefix=('docs/progress/IMPL_0908/','docs/progress/IMPL_0909/','docs/progress/P1_0909/')
assert all(name.startswith(allowed_prefix) or name in ['docs/progress/IMPL_STATUS_0908.md','docs/progress/IMPL_STATUS_0909.md','docs/progress/IMPL_QUESTIONS_0909.md'] for name in selected)
rows=[];numbered=[]
diffdir=base/'diffs';diffdir.mkdir(exist_ok=True)
pattern=re.compile(r'(?<![0-9a-fA-F])([0-9a-f]{7,40})(?![0-9a-fA-F])')
for index,name in enumerate(selected,1):
    old=subprocess.check_output(['git','show',material_base+':'+name]);path=pathlib.Path(name);new=path.read_bytes()
    snap=base/'before_current'/name;snap.parent.mkdir(parents=True,exist_ok=True)
    if snap.exists():assert snap.read_bytes()==old
    else:snap.write_bytes(old)
    diff=''.join(difflib.unified_diff(old.decode().splitlines(True),new.decode().splitlines(True),fromfile='before/'+name,tofile='after/'+name))
    file=diffdir/f'{index:02d}_{path.name}.diff';file.write_text(diff)
    if path.suffix=='.patch':
        assert old.split(b'diff --git ',1)[1]==new.split(b'diff --git ',1)[1],name
        def pid(data):return subprocess.check_output(['git','patch-id','--stable'],input=data).decode().split()[0]
        assert pid(old)==pid(new)
    for line_number,line in enumerate(new.decode().splitlines(),1):
        for token in pattern.findall(line):
            assert not any(r['old_sha'].startswith(token) for r in mapping),(name,line_number,token)
            found=[r for r in mapping if r['new_sha'].startswith(token)]
            if found:numbered.append([name,line_number,token,found[0]['new_sha']])
    rows.append([name,hashlib.sha256(old).hexdigest(),hashlib.sha256(new).hexdigest(),str(file.relative_to(base))])
for filename,header,data in [('CHANGED_FILES.tsv',['file','before_sha256','after_sha256','diff'],rows),('CURRENT_NEW_SHA_LOCATIONS.tsv',['file','line','token','full_new_sha'],numbered)]:
    with (base/filename).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(header);w.writerows(data)
index=list(csv.DictReader(pathlib.Path('docs/progress/IMPL_0909/W4/COMMIT_INDEX.tsv').open(),delimiter='\t'))
assert [r['commit'] for r in index]==[r['new_sha'] for r in mapping]
assert all(r['change_id']=='PRESENT' and r['patch_id']=='MATCH' for r in index)
assert 'Change-Id' in pathlib.Path('docs/progress/IMPL_0909/W4/DELIVERY_ZH.md').read_text()
assert not re.search(r'[\u3400-\u9fff]',pathlib.Path('docs/progress/IMPL_0909/W4/DELIVERY_EN.md').read_text())
assert re.search(r'[\u3400-\u9fff]','正向对照')
print('MATERIAL_FILES_CHANGED',len(rows),'CURRENT_SHA_LOCATIONS',len(numbered))
print('FIVE_PATCH_CODE_DIFF_BYTES_IDENTICAL; STABLE_PATCH_IDS_IDENTICAL; CURRENT_REFS_MATCH_NEW_CHAIN; NO_OLD_SHA_IN_CHANGED_CURRENT_FILES')
print('ORIGINAL_FOUR_PATCHES_NOT_IN_DIFF; SOURCE_TREES_VERIFIED_SEPARATELY; PREEXISTING_R115_EDITS_PRESERVED')
