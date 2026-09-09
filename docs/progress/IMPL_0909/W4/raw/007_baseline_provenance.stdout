#!/usr/bin/env python3
"""固定既有官方对照的每项状态；ARM 使用已证明与四补丁侧一致的 owner 表。"""
import collections,csv,hashlib,json,pathlib
r=pathlib.Path.cwd();external=pathlib.Path('/home/toolchain/development/libc++_replacement')
inputs=[('x86_64','libcxx',external/'progress/R68/raw/full_run/results/patched_x86_64/libcxx/result.json'),
 ('x86_64','libcxxabi',external/'progress/R68/raw/full_run/results/patched_x86_64/libcxxabi/result.json'),
 ('armv7l','libcxx',r/'docs/progress/R81/raw/phase2/results/baseline_armv7l/libcxx/aggregate_result_owner_normalized.json'),
 ('armv7l','libcxxabi',r/'docs/progress/R81/raw/phase2/results/baseline_armv7l/libcxxabi/batch-001/result.json')]
summary=json.loads((r/'docs/progress/R81/tables/final_summary.json').read_text())
assert summary['libcxx']['owner_normalized_differences']==0 and summary['libcxxabi']['differences']==0
rows=[]
for arch,suite,p in inputs:
 data=json.loads(p.read_text());counts=collections.Counter(x['code'] for x in data['tests'])
 print(arch,suite,'SHA256',hashlib.sha256(p.read_bytes()).hexdigest(),'SOURCE',p,'COUNTS',dict(counts))
 for entry in data['tests']:rows.append([arch,suite,entry['name'].split(' :: ',1)[-1],entry['code']])
with (r/'docs/progress/IMPL_0908/W3/historical_baseline.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['arch','suite','test','historical_code']);w.writerows(rows)
print('ARM_OWNER_EQUIVALENCE=R81_PAIRWISE_CONFIRMED')
