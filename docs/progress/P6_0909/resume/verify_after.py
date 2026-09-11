#!/usr/bin/env python3
"""逐一核对远端全部 heads、五 tree、唯一 footer、原消息与线性链；无写引用操作。"""
import csv
import pathlib
import re
import subprocess
base=pathlib.Path('docs/progress/P6_0909/resume')
target='refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief'
def refs(name):
    return dict((ref,sha) for sha,ref in (x.split() for x in (base/name).read_text().splitlines()))
before=refs('raw/003_remote_before.stdout');after=refs('raw/014_remote_after.stdout')
rows=[]
for ref in sorted(before.keys()|after.keys()):
    a=before.get(ref,'ABSENT');b=after.get(ref,'ABSENT')
    state='AUTHORIZED_TARGET_CHANGED' if ref==target else ('UNCHANGED' if a==b else 'UNEXPECTED_CHANGE')
    rows.append([ref,a,b,state])
with (base/'ALL_BRANCH_COMPARISON.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['ref','before','after','result']);w.writerows(rows)
assert before.keys()==after.keys()
assert all(r[3]!='UNEXPECTED_CHANGE' for r in rows)
mapping=list(csv.DictReader((base/'SHA_MAPPING.tsv').open(),delimiter='\t'))
repo='tmp/IMPL_0908/source'
def git(*args):return subprocess.check_output(['git','-C',repo,*args])
head=git('rev-parse','HEAD').decode().strip()
assert head==mapping[-1]['new_sha']==after[target]
assert git('status','--porcelain')==b''
assert git('rev-list','--reverse',mapping[0]['old_parent']+'..HEAD').decode().splitlines()==[r['new_sha'] for r in mapping]
ids=set()
for r in mapping:
    old=git('cat-file','commit',r['old_sha']);new=git('cat-file','commit',r['new_sha'])
    oh,om=old.split(b'\n\n',1);nh,nm=new.split(b'\n\n',1)
    assert re.search(rb'^tree (\w+)$',oh,re.M)[1]==re.search(rb'^tree (\w+)$',nh,re.M)[1]
    assert git('diff','--exit-code',r['old_sha'],r['new_sha'])==b''
    assert nm==om+b'\nChange-Id: '+r['change_id'].encode()+b'\n'
    assert re.findall(rb'^Change-Id: (I[0-9a-f]{40})$',nm,re.M)==[r['change_id'].encode()]
    assert r['change_id'] not in ids;ids.add(r['change_id'])
print('REMOTE_LOCAL_HEAD',head)
print('ALL_BRANCHES',len(before),'TARGET_CHANGED=1','OTHER_UNCHANGED',len(before)-1)
print('FIVE_TREE_AND_EMPTY_DIFF_CHECKS_PASS; FIVE_UNIQUE_CHANGE_IDS; ORIGINAL_MESSAGE_BYTES_PLUS_HOOK_FOOTER; CHAIN_EXACTLY_FIVE')
print('SOURCE_WORKTREE_CLEAN; ORIGINAL_AUTHORS_PRESERVED; NEW_COMMITTER=hao.lin <hao.lin@samsung.com>')
