#!/usr/bin/env python3
"""隔离重建五提交。每个消息仅由实际 Gerrit hook 增加 footer，复用原 tree；不推送。"""
import csv
import datetime
import hashlib
import pathlib
import re
import subprocess
import sys

root=pathlib.Path.cwd()
out=root/'docs/progress/P6_0909/resume'
repo=root/'tmp/P6_0909.7w4ZI8/repo'
recorder=root/'docs/progress/P6_0909/record.py'
base='c3f8578a4db871d9d6de96d751f4c2ea7b1638fa'
old_head='f3c1af692b579add991861e1f7c4950f6af39932'
counter=0
def run(args,label,env=None):
    global counter
    counter+=1
    stem=out/f'raw/rebuild/{counter:03d}_{label}'
    cmd=list(args)
    if env:cmd=['env',*[k+'='+v for k,v in env.items()],*cmd]
    rc=subprocess.run([sys.executable,str(recorder),str(stem),*cmd]).returncode
    assert rc==0,(label,rc)
    return pathlib.Path(str(stem)+'.stdout').read_bytes()
def git(*args,label='git',env=None):
    return run(['git','-C',str(repo),*args],label,env)
assert git('rev-parse','HEAD',label='old_head').decode().strip()==old_head
commits=git('rev-list','--reverse',base+'..'+old_head,label='old_chain').decode().splitlines()
assert len(commits)==5
before=dict((ref,sha) for sha,ref in (line.split() for line in (out/'raw/003_remote_before.stdout').read_text().splitlines()))
assert before['refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief']==old_head
git('update-ref','HEAD',base,old_head,label='isolate_start_at_base')
time=datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%S+0000')
parent=base; prior=base; results=[]; ids=set()
objects=out/'commit_objects';objects.mkdir(exist_ok=True)
for index,old in enumerate(commits,1):
    raw=git('cat-file','commit',old,label=f'old_{index}')
    headers,message=raw.split(b'\n\n',1)
    tree=re.search(rb'^tree (\w+)$',headers,re.M)[1].decode()
    author=re.search(rb'^author (.*?) <(.*?)> (\d+ [+-]\d+)$',headers,re.M)
    assert re.findall(rb'^parent (\w+)$',headers,re.M)==[prior.encode()]
    assert not re.search(rb'^Change-Id:',message,re.M)
    (objects/(old+'.old.commit')).write_bytes(raw)
    path=objects/f'{index:02d}.message'
    path.write_bytes(message)
    env={'GIT_DIR':str(repo/'.git'),'GIT_COMMITTER_NAME':'hao.lin',
         'GIT_COMMITTER_EMAIL':'hao.lin@samsung.com','GIT_COMMITTER_DATE':time,
         'GIT_AUTHOR_NAME':author[1].decode(),'GIT_AUTHOR_EMAIL':author[2].decode(),
         'GIT_AUTHOR_DATE':'@'+author[3].decode()}
    run([str(repo/'.git/hooks/commit-msg'),str(path)],f'hook_{index}',env)
    new_message=path.read_bytes()
    matches=re.findall(rb'^Change-Id: (I[0-9a-f]{40})$',new_message,re.M)
    assert len(matches)==1 and matches[0] not in ids
    ids.add(matches[0])
    # 标准 hook 为 footer 加一个分隔空行；此前完整消息必须逐字节保留。
    assert new_message==message+b'\nChange-Id: '+matches[0]+b'\n'
    new=git('commit-tree',tree,'-p',parent,'-F',str(path),label=f'new_{index}',env=env).decode().strip()
    new_raw=git('cat-file','commit',new,label=f'new_object_{index}')
    nh,nm=new_raw.split(b'\n\n',1)
    assert nm==new_message
    assert re.search(rb'^tree (\w+)$',nh,re.M)[1].decode()==tree
    assert re.search(rb'^author (.*)$',nh,re.M)[1]==re.search(rb'^author (.*)$',headers,re.M)[1]
    assert re.findall(rb'^parent (\w+)$',nh,re.M)==[parent.encode()]
    assert b'committer hao.lin <hao.lin@samsung.com> ' in nh
    assert git('diff','--exit-code',old,new,label=f'tree_diff_{index}')==b''
    (objects/(new+'.new.commit')).write_bytes(new_raw)
    git('update-ref','HEAD',new,parent,label=f'isolate_advance_{index}')
    results.append([index,old,new,tree,tree,matches[0].decode(),prior,parent,'PASS','PASS_ORIGINAL_PLUS_HOOK_FOOTER'])
    prior=old;parent=new
new_chain=git('rev-list','--reverse',base+'..HEAD',label='new_chain').decode().splitlines()
assert new_chain==[r[2] for r in results] and len(ids)==5
with (out/'SHA_MAPPING.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n')
    w.writerow(['order','old_sha','new_sha','old_tree','new_tree','change_id','old_parent','new_parent','tree_diff','message_check']);w.writerows(results)
(out/'NEW_HEAD.txt').write_text(parent+'\n')
print('FIVE_TREES_IDENTICAL; FIVE_UNIQUE_CHANGE_IDS; ORIGINAL_MESSAGE_BYTES_PRESERVED_PLUS_STANDARD_FOOTER')
print('NEW_HEAD',parent)
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
