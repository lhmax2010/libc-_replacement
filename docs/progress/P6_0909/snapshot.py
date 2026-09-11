#!/usr/bin/env python3
"""只读记录实施提交、原消息、树和远端分支；不生成提交或改写引用。"""
import csv
import hashlib
import pathlib
import re
import subprocess

out=pathlib.Path('docs/progress/P6_0909')
repo='tmp/IMPL_0908/source'
base='c3f8578a4db871d9d6de96d751f4c2ea7b1638fa'
def git(*args):
    return subprocess.check_output(['git','-C',repo,*args])
def write_table(name,header,rows):
    with (out/name).open('w') as stream:
        w=csv.writer(stream,delimiter='\t',lineterminator='\n');w.writerow(header);w.writerows(rows)
head=git('rev-parse','HEAD').decode().strip()
commits=git('rev-list','--reverse',base+'..'+head).decode().splitlines()
assert len(commits)==5
assert git('status','--porcelain')==b''
refs=[line.split() for line in (out/'raw/005_remote_heads.stdout').read_text().splitlines()]
assert all(len(r)==2 for r in refs)
remote=dict((ref,sha) for sha,ref in refs)
target='refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief'
assert remote[target]==head=='f3c1af692b579add991861e1f7c4950f6af39932'
write_table('SANDBOX_BRANCHES_BEFORE.tsv',['sha','ref'],[r for r in refs if r[1].startswith('refs/heads/sandbox/')])
write_table('NAMESPACE_BRANCHES_BEFORE.tsv',['sha','ref'],[r for r in refs if r[1].startswith('refs/heads/sandbox/lhmax2025/')])
directory=out/'original_commits';directory.mkdir(exist_ok=True)
result=[];previous=base
for i,sha in enumerate(commits,1):
    raw=git('cat-file','commit',sha)
    headers,message=raw.split(b'\n\n',1)
    tree=re.search(rb'^tree (\w+)$',headers,re.M)[1].decode()
    parents=re.findall(rb'^parent (\w+)$',headers,re.M)
    assert parents==[previous.encode()]
    change_ids=re.findall(rb'^Change-Id:\s*(\S+)',message,re.M)
    assert not change_ids
    (directory/(sha+'.commit')).write_bytes(raw)
    (directory/(sha+'.message')).write_bytes(message)
    result.append([i,sha,previous,tree,'ABSENT','NOT_GENERATED',message.decode().splitlines()[0]])
    previous=sha
write_table('COMMITS_BEFORE.tsv',['order','old_sha','old_parent','old_tree','change_id','new_sha','subject'],result)
print('LOCAL_REMOTE_HEAD_MATCH',head)
print('LINEAR_COMMITS',len(commits),'NO_CHANGE_ID=5; NO_REWRITE_PERFORMED')
print('TARGET_NAMESPACE_BRANCHES',sum(r[1].startswith('refs/heads/sandbox/lhmax2025/') for r in refs))
print('ALL_HEADS_CAPTURED',len(refs))
print('DECISION_REQUIRED: replacing these already-published commits with a rewritten five-commit chain is not a fast-forward')
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
