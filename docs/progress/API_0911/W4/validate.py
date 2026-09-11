#!/usr/bin/env python3
"""复核数字、原始断言、相对引用、英文残留与既有脏文件身份。"""
import pathlib,json,re,hashlib,subprocess
root=pathlib.Path('docs/progress/API_0911'); out=root/'W4'
(out/'VALIDATION.json').write_text('{"status":"IN_PROGRESS"}\n')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
matrix=json.loads((root/'W2/matrix.json').read_text())
assert len(matrix)==20 and sum(x['exact_payloads'] for x in matrix)==100
cells={}
for x in matrix:
    cells.setdefault((x['cell'],x['mode']),[]).append(x['repeat'])
    p=root/'W2/raw'/x['raw']; text=p.with_suffix('.stdout').read_text()
    assert p.with_suffix('.exitcode').read_text().strip()=='0'
    assert 'ASSERT values=1 maps_cxx=1 maps_gnu=1 maps_provider=1' in text
    vals=re.findall(r'VALUE case=(\d+) bytes=(\d+) expected=(\d+) exact=(\d) hex=([0-9a-f]*)',text)
    assert len(vals)==5
    assert [int(v[1]) for v in vals]==[0,9,11,3,257]
    assert all(v[1]==v[2] and v[3]=='1' and len(bytes.fromhex(v[4]))==int(v[1]) for v in vals)
    assert text.count('CALLER_OUTPUT_DESTROYED case=')==5
    if x['mode']=='shim': assert 'PROVIDER_SCOPES_COMPLETED=5' in text and text.count('REAL_CALLEE=')==5
assert len(cells)==4 and all(sorted(v)==[1,2,3,4,5] for v in cells.values())
native=json.loads((root/'W1/native_final_results.json').read_text())
assert len(native)==8 and all(x['link_exit']==0 and x['positive_cpp'] and x['execution']=='NOT_OBSERVED' for x in native)
links=[];errors=[]
for p in [*root.glob('W*/*.md'),pathlib.Path('docs/progress/API_STATUS_0911.md')]:
    for href in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if href.startswith(('https://','http://','#')):continue
        target=(p.parent/href.split('#')[0]).resolve()
        links.append(dict(file=str(p),href=href,exists=target.exists()))
        if not target.exists():errors.append((str(p),href))
assert not errors,errors
en=(out/'DECISION_EN.md').read_text();assert not re.search(r'[\u4e00-\u9fff]',en)
checksums={}
for stage in ['W1','W2','W3']:
    failures=[];count=0
    for line in (root/stage/'SHA256SUMS').read_text().splitlines():
        digest,name=line.split('  ',1);count+=1
        if sha(root/stage/name)!=digest:failures.append(name)
    assert not failures,(stage,failures)
    checksums[stage]=count
protected={'docs/progress/R115/W3/raw/033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','docs/progress/R115/W3/raw/033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
for p,h in protected.items():assert sha(pathlib.Path(p))==h,p
result=dict(status='PASS',behavior_matrix_cells=4,runs=20,exact_content_comparisons=100,native_linked_elfs=8,target_behavior_status='PARTIAL; NOT_OBSERVED',relative_links_checked=len(links),english_cjk=0,stage_manifest_counts=checksums,protected_dirty_files='UNCHANGED',scope='本轮检查不证明目标接口兼容性')
(out/'VALIDATION.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
(out/'link_check.json').write_text(json.dumps(links,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False,indent=2))
