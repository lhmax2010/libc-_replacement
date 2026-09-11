#!/usr/bin/env python3
"""五提交快照、说明结构、patch-id 与旧补丁保护的只读核对。"""
import csv,hashlib,pathlib,re,subprocess
root=pathlib.Path.cwd();src=root/'tmp/IMPL_0908/source';out=root/'docs/progress/IMPL_0909/W4'
base='c3f8578a4db871d9d6de96d751f4c2ea7b1638fa';head='c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13'
def git(*args):return subprocess.check_output(['git','-C',str(src),*args])
commits=git('rev-list','--reverse',base+'..'+head).decode().splitlines()
assert len(commits)==5
patches=sorted((out/'patches').glob('*.patch'));assert len(patches)==5
rows=[];prev=base
for commit,patch in zip(commits,patches):
    parent=git('rev-parse',commit+'^').decode().strip();assert parent==prev;prev=commit
    body=git('show','-s','--format=%B',commit).decode()
    subject=body.splitlines()[0]
    assert subject.startswith('libcxx:') and '中文说明' in body and 'Validation:' in body and 'Evidence:' in body
    change_id=bool(re.search(r'^Change-Id: I[0-9a-f]{40}$',body,re.M))
    assert patch.read_text().startswith('From '+commit+' ')
    def patch_id(data):return subprocess.check_output(['git','patch-id','--stable'],input=data).decode().split()[0]
    assert patch_id(patch.read_bytes())==patch_id(git('show','--format=fuller',commit))
    digest=hashlib.sha256(patch.read_bytes()).hexdigest()
    rows.append([commit,subject,str(patch.relative_to(out)),digest,'PRESENT' if change_id else 'ABSENT','MATCH'])
    print(commit,'Change-Id='+str(change_id),'PATCH_ID_MATCH=PASS',digest)
positive=git('show','-s','--format=%B',base).decode()
assert re.search(r'^Change-Id: I[0-9a-f]{40}$',positive,re.M)
print('CHANGE_ID_DETECTOR_POSITIVE_CONTROL=PASS baseline='+base)
with (out/'COMMIT_INDEX.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['commit','subject','patch','sha256','change_id','patch_id']);w.writerows(rows)
assert not git('status','--porcelain').strip()
assert git('rev-parse','HEAD').decode().strip()==head
assert subprocess.run(['git','-C',str(src),'diff','--check',base,head]).returncode==0
print('SOURCE_CLEAN=PASS FIVE_LINEAR_COMMITS=PASS DIFF_CHECK=PASS')
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
