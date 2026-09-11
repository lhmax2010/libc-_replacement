#!/usr/bin/env python3
"""封存已完成阶段的提交/普通推送原始回执；不包含正在进行的自身交付。"""
import pathlib,shutil,hashlib,json
root=pathlib.Path('docs/progress/API_0911');dest=root/'W4/prior_delivery';dest.mkdir(exist_ok=True)
rows=[]
for p in sorted((root/'delivery').glob('*')):
    if not p.is_file() or not p.name.startswith(('W1_','W2_','W3_')):continue
    target=dest/p.name;shutil.copyfile(p,target)
    rows.append(dict(path=str(target),sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
(root/'W4/prior_delivery_index.json').write_text(json.dumps(rows,indent=2)+'\n')
print('RECEIPTS_COPIED',len(rows))
