#!/usr/bin/env python3
"""封存脚本摘要、最终身份与文档引用，保护源分支不变。"""
import hashlib,json,pathlib,re,subprocess
p=pathlib.Path('docs/progress/IMPL_0909/W2');rows=[]
for file in sorted(p.glob('*.py')):
    compile(file.read_text(),str(file),'exec');rows.append(hashlib.sha256(file.read_bytes()).hexdigest()+'  '+str(file))
(p/'RESUME_SCRIPT_SHA256SUMS').write_text('\n'.join(rows)+'\n')
for name in ['REPORT.md','RESUME_PREFLIGHT.md']:
    file=p/name
    for link in re.findall(r'\]\(([^)]+)\)',file.read_text()):
        if link.startswith(('http://','https://')):continue
        assert (file.parent/link.split('#',1)[0]).exists(),(file,link)
assert 'BOARD_CLEANED_AND_RELEASED=PASS owned_root=1 uploads=0 diagnostics=74' in (p/'raw/059_cleanup_resume.stdout').read_text()
s=json.loads((p/'ARM_SUMMARY.json').read_text());assert s['valid_total']==11402 and s['new_valid']==3554 and s['missing']==0
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/runtime-validation'
assert subprocess.check_output(['git','-C','tmp/IMPL_0908/source','rev-parse','HEAD'],text=True).strip()=='f3c1af692b579add991861e1f7c4950f6af39932'
assert not subprocess.check_output(['git','-C','tmp/IMPL_0908/source','status','--porcelain'],text=True).strip()
for file in [pathlib.Path('tmp/IMPL_0908/build-armv7l/lib/libc++.so.1'),pathlib.Path('tmp/IMPL_0908/build-armv7l/lib/libc++abi.so.1'),p/'OFFICIAL_RESUME_EVIDENCE.tgz',pathlib.Path('docs/progress/R115/W3/raw/033_angle_files.stderr'),pathlib.Path('docs/progress/R115/W3/raw/033_angle_files.stdout')]:
    print('FINAL_SHA256',hashlib.sha256(file.read_bytes()).hexdigest(),file)
print('SCRIPT_SNAPSHOTS',len(rows),'SYNTAX_AND_LINKS=PASS SOURCE_CLEAN=PASS BOARD_RELEASED=PASS')
