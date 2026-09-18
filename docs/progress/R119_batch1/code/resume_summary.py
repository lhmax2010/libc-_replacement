#!/usr/bin/env python3
"""第一批统计单位、跨界形态与原十二项缺口的可复核汇总。"""
import collections
import csv
import json
import pathlib

base=pathlib.Path(__file__).resolve().parents[1]
with (base/'W1/REVIEW_001_100.tsv').open() as f:
    rows=list(csv.DictReader(f,delimiter='\t'))
notes={}
for p in sorted((base/'code').glob('review_notes_*.json')):
    notes.update(json.loads(p.read_text()))
shape={}
for n in ['unique_ptr','shared_ptr','set','function','tuple','unordered_map','array','initializer_list','hash','multimap','chrono::duration','equal_to','chrono::time_point']:
    shape['std::'+n]='传值（含外层值参数的子对象）'
for n in ['string','vector','map','pair','list','unordered_set','istream','basic_string','wstring','filesystem::path','allocator','less','string_view']:
    shape['std::'+n]='引用/借用元素（含外层引用的子对象）'
for n in ['stringstream','ostream','atomic','ios','ios_base','streambuf','deque','stack']:
    shape['std::'+n]='指针（含所指对象的子对象）'
cross=[r for r in rows if r['result']=='CROSS_PACKAGE']
assert {r['candidate'] for r in cross}==set(shape)
classified=[]
for r in cross:
    n=r['candidate']
    classified.append(dict(candidate=n,provider=r['provider_package'],consumer=r['consumer_package'],
                           primary_shape=shape[n],detail=r['shape'],ownership=r['ownership'],
                           explicit_ownership='独占转移' if n=='std::unique_ptr' else '共享所有权' if n=='std::shared_ptr' else 'NOT_OBSERVED_TRANSFER',
                           exception_crossing='NOT_OBSERVED',evidence=r['evidence']))
with (base/'W1/CROSS_PACKAGE_INPUT.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(classified[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(classified)
old_unknown={8,11,12,14,17,18,19,20,22,23,24,25}
unknown_lines=['# 原十二项未定：已查事实与缺少材料','','本表说明当前缺口，不以未知替代“不跨包”。','', '| 候选 | 已查事实 | 缺什么 | 依据 |','| --- | --- | --- | --- |']
for r in rows:
    if int(r['rank']) not in old_unknown:
        continue
    n=r['candidate'];note=notes[n]
    unknown_lines.append('| '+' | '.join([n,note['note'].replace('|','/'),note.get('missing','NOT_OBSERVED').replace('|','/'),f'`W1/reviewed/{int(r["rank"]):03d}.json`'])+' |')
(base/'W1/ORIGINAL_12_GAPS.md').write_text('\n'.join(unknown_lines)+'\n')
counts=collections.Counter(r['result'] for r in rows)
shapes=collections.Counter(r['primary_shape'] for r in classified)
runtime=sum(r['runtime_reference']=='RUNTIME_CALL' for r in rows)
lines=['# 第一批 100 个候选的阶段汇总','','状态：**PARTIAL**。100 项均有复核记录，但仅有证据闭合的项进入确定分类；不能将复核记录数称为完成定界数。', '', '| 互斥主分类 | 候选数 |','| --- | ---: |']
for k in ['INTERNAL_ONLY','PUBLIC_NO_CONSUMER','CROSS_PACKAGE','RUNTIME_CALL','NOT_OBSERVED','NOT_OBSERVED_INCLUDE_CHAIN']:
    lines.append(f'| {k} | {counts[k]} |')
lines += ['', '上述零值是“没有作出该分类”的记录数，不是“平台不存在此类接口”的零命中结论。',
          '',f'另有 {runtime} 个候选存在 RUNTIME_CALL 关系列；这与主分类可重叠。没有足够证据确认这些候选“只有”运行库调用，故不能将它们从未定项自动移出。',
          '', '## 已确认跨包项的形态', '', '| 主形态 | 候选数 |','| --- | ---: |']
for k,v in shapes.items():
    lines.append(f'| {k} | {v} |')
lines += ['', '其中显式独占转移 1 项（unique_ptr），共享所有权 1 项（shared_ptr）；这两项是主形态的子集，不能再次相加。其他值形参可能复制或移动，不从声明推断实际发生了转移。跨包异常传播本轮未闭合，不将“0 项已确认”写成“不抛异常”。',
          '', '计数单位是候选名字，不是独立函数数、包边数或底层类型数；string/basic_string 等别名分列，多个子对象可对应同一条外层包边。',
          '', '## 耗时与覆盖', '', '实际续跑墙钟时间与每个新增复核记录均时见 `W1/RESUME_CHECKPOINT_100.json`。这是复用既有证据的增量耗时，不是从零完成定界的均时；此前逐项人工时间未记录，第一批完整定界平均耗时仍 NOT_OBSERVED，不能用它推算后四批。',
          '', '本批限定为实际安装的 devel 头与现有快照；SDK 可用性另议。模板、宏、using 后无限定名、别名投影、返回类型不编码、异常边、dlopen 与外部产物是主要限制。正向符号/头证据可以证明“存在一条”，抽样不能证明“没有任何一条”。',
          '', '原十二项的逐项事实与缺口见 [缺口表](ORIGINAL_12_GAPS.md)。全部未定项的具体原因见 `W1/reviewed/`；它们不是统一贴一个无消费方标签。',
          '', '后续正在扩充 ELF 消费方覆盖；只有新证据闭合后才更新上述确定分类。']
(base/'W1/SUMMARY.md').write_text('\n'.join(lines)+'\n')
print(json.dumps(dict(counts=counts,shapes=shapes,runtime_relation_candidates=runtime),ensure_ascii=False))
