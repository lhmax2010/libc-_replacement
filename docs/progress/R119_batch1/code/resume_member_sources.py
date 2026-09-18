#!/usr/bin/env python3
"""为既有真实包边保存成员/基类链的源码证据；不是新运行实验。"""
import hashlib
import json
import pathlib

root = pathlib.Path(__file__).resolve().parents[4]
out = root / 'docs/progress/R119_batch1/W1/member_sources.json'
files = {
    'bits/unordered_map.h': [(42, 53), (98, 114)],
    'bits/hashtable.h': [(180, 215)],
    'bits/hashtable_policy.h': [(1230, 1271), (1298, 1310), (1682, 1708)],
    'bits/stl_vector.h': [(82, 93), (131, 141), (370, 390), (423, 430)],
    'bits/stl_tree.h': [(133, 143), (650, 668), (695, 705)],
    'bits/stl_map.h': [(149, 158)],
    'bits/basic_ios.h': [(62, 70)],
    'std/istream': [(56, 67), (975, 984)],
    'std/iosfwd': [(130, 147)],
    'std/ostream': [(62, 72)],
    'std/sstream': [(71, 80), (980, 1040)],
    'bits/chrono.h': [(902, 930), (994, 1003)],
}
rows = []
for name, ranges in files.items():
    path = root / 'codes/gcc/libstdc++-v3/include' / name
    data = path.read_bytes()
    lines = data.decode().splitlines()
    snippets = ['\n'.join(f'{i}: {lines[i-1]}' for i in range(a, b+1)) for a, b in ranges]
    row = dict(path=str(path.relative_to(root)), sha256=hashlib.sha256(data).hexdigest(), ranges=ranges, snippets=snippets)
    rows.append(row)
    print(row['path'], row['sha256'], *snippets, sep='\n')
out.write_text(json.dumps(rows, ensure_ascii=False, indent=2)+'\n')
