#!/usr/bin/env python3
"""删除前最后只读闸门；不执行删除，全部断言通过才写确认。"""
import csv
import hashlib
import json
import pathlib
import subprocess
import sys

out = pathlib.Path('docs/progress/P6B_0909')
src = pathlib.Path('tmp/IMPL_0908/source')
head = 'c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13'
old = 'refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief'
new = 'refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport'
def refs(stem):
    assert (out/'raw'/f'{stem}.exitcode').read_text().strip() == '0'
    return {r:s for s,r in (line.split() for line in (out/'raw'/f'{stem}.stdout').read_text().splitlines())}
before = refs('010_heads_after_merge')
current = refs('018_heads_before_delete')
assert before == current
assert current[old] == current[new] == head
for stem in ['011_post_merge','015_delivery_check','016_documents_check','017_material_check']:
    assert (out/'raw'/f'{stem}.exitcode').read_text().strip() == '0'
check = json.loads((out/'MATERIAL_CHECK_pre_delete.json').read_text())
assert check['current_old_branch_dependencies'] == 0
assert check['raw_history_unchanged'] and check['sha_refs_reachable']
files = list(csv.DictReader((out/'CHANGED_FILES.tsv').open(),delimiter='\t'))
for row in files:
    assert hashlib.sha256(pathlib.Path(row['path']).read_bytes()).hexdigest() == row['after_sha256']
seq = 0
def git(*args):
    global seq
    seq += 1
    stem = out/'raw'/'deletion_gate'/f'{seq:03d}'
    subprocess.run([sys.executable,str(out/'record.py'),str(stem),'git','-C',str(src),*args],check=True)
    return pathlib.Path(str(stem)+'.stdout').read_bytes()
assert git('rev-parse',new).decode().strip() == head
assert git('branch','--show-current').decode().strip() == new.removeprefix('refs/heads/')
assert not git('status','--porcelain').strip()
five = list(csv.DictReader((out/'FIVE_COMMITS_BEFORE.tsv').open(),delimiter='\t'))
for i,row in enumerate(five):
    resolved = git('rev-parse',new+'~'+str(4-i)).decode().strip()
    assert resolved == row['commit']
    assert git('rev-parse',resolved+'^{tree}').decode().strip() == row['tree']
    assert git('diff','--exit-code',row['tree'],resolved+'^{tree}') == b''
    assert git('cat-file','commit',resolved) == (out/'commit_objects'/f'{resolved}.commit').read_bytes()
assert (out/'REPORT.md').exists()
text = f'''# 删除前最终确认

状态：`PRE_DELETE_VERIFIED`。本文件在删除命令执行前写入。

1. 远端目标分支与旧分支当前 HEAD 都是 `{head}`；本地目标分支与其一致。
2. 远端完整 heads 共 {len(current)} 个，与合并后快照完全一致；新旧分支没有在核验后被推进。
3. 五个提交在新分支 HEAD 的五个连续祖先位置逐条解析一致；tree ID、提交原字节相同；五次 diff 退出 0。
4. 合并后完整祖先清单已逐项确认保留旧 565,564 个提交、只增加五个；原始核验 raw/011 已成功。
5. {len(files)} 个现行材料文件与刚完成的核验摘要一致；有效旧分支依赖为 0，有正向对照。
6. 89 个历史文件摘要不变；两个历史操作段落保留原文并附当前映射；这些均不依赖旧远端分支存在。
7. 五提交的现行 SHA 引用全部仍可从新分支取得；补丁和中英文 92 个本地引用核验通过。

授权删除的唯一对象：`{old}`。本次不使用 force，不删除其他分支、本地备份或提交对象。
上述核验已写入本报告及 [执行报告](REPORT.md)，第一步与第二步已全部完成。
删除后须对照 raw/018_heads_before_delete.stdout 的完整列表确认只有这一条消失。
'''
(out/'PRE_DELETE_CONFIRMATION.md').write_text(text)
print(text)
