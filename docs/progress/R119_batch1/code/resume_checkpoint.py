#!/usr/bin/env python3
"""只汇总已有逐项记录，不从未命中自动判定阴性。"""
import collections
import csv
import datetime
import hashlib
import json
import pathlib
import sys

base = pathlib.Path(__file__).resolve().parents[1]
end = int(sys.argv[1])
with (base / f'W1/REVIEW_001_{end:03d}.tsv').open() as f:
    rows = list(csv.DictReader(f, delimiter='\t'))
assert len(rows) == end
assert [int(r['rank']) for r in rows] == list(range(1, end+1))
counts = collections.Counter(r['result'] for r in rows)
now = datetime.datetime.now(datetime.timezone.utc)
start = datetime.datetime.fromisoformat(json.loads((base/'raw/resume0918/002_gate.time.json').read_text())['start'])
elapsed = (now-start).total_seconds()
report = dict(reviewed_through=end, classifications=counts, pending_first_pass=100-end,
              classification_complete=not any(r['result'].startswith('NOT_OBSERVED') for r in rows) and end==100,
              resume_elapsed_seconds=elapsed,
              resume_seconds_per_new_review_record=elapsed/(end-26) if end>26 else None,
              timing_limit='续跑墙钟时间含共享查证/提交及复用旧证据；不是从零完成定界的平均耗时。此前逐项人工时间未记录，总体完成定界均时仍 NOT_OBSERVED。',
              runtime_call_candidate_count=sum(r['runtime_reference']=='RUNTIME_CALL' for r in rows),
              runtime_limit='运行库引用是独立关系列，与 CROSS_PACKAGE 可重叠；不能把关系数量冒充互斥候选分类。',
              timestamp_utc=now.isoformat())
(base/f'W1/RESUME_CHECKPOINT_{end:03d}.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n')
hashes = {}
for p in sorted((base/'code').glob('*.py')):
    if p.name.startswith('resume_') or p.name=='render_review.py':
        hashes[str(p.relative_to(base))] = hashlib.sha256(p.read_bytes()).hexdigest()
for p in sorted((base/'code').glob('review_notes_*.json')):
    hashes[str(p.relative_to(base))] = hashlib.sha256(p.read_bytes()).hexdigest()
(base/'RESUME_SCRIPT_SHA256.json').write_text(json.dumps(hashes, indent=2)+'\n')
text = ['# 第一批续跑状态', '', f'已形成第 1–{end} 项逐项复核记录；尚有 {100-end} 项未完成本轮初次复核。',
        '', '**分类仍为 PARTIAL**：未定项不能强行分入四类；复核一项不等于该项证据闭合。', '', '| 状态 | 候选数 |', '| --- | ---: |']
for name in ['INTERNAL_ONLY','PUBLIC_NO_CONSUMER','CROSS_PACKAGE','RUNTIME_CALL','NOT_OBSERVED','NOT_OBSERVED_INCLUDE_CHAIN']:
    text.append(f'| {name} | {counts[name]} |')
text += ['', f'另有 {report["runtime_call_candidate_count"]} 个候选存在已索引运行库引用，作为并列关系列保留，不加入 CROSS_PACKAGE，也不视为“只有运行库调用”的阴性证明。',
         '', f'续跑至本检查点实耗 {elapsed:.1f} 秒；每个新增复核记录平均 {elapsed/(end-26):.1f} 秒（含共享工作，复用旧证据）。完整定界的总体平均耗时：NOT_OBSERVED，不据此估算后续从零定界工时。',
         '', '资源：light；单进程；nice 19 + ionice 3；RLIMIT_AS 9,921,875,558 字节；Git 普通 I/O。未使用开发板。',
         '', f'逐项结果：[TSV](W1/REVIEW_001_{end:03d}.tsv)。已查事实、所缺材料和依据见 `W1/reviewed/`；脚本摘要见 `RESUME_SCRIPT_SHA256.json`。',
         '', '成员/基类候选依据已核实的外层包边与源码成员链计数；同一包边可关联多个候选，不能相加当作不同边数。标准库源码链不等于另做了运行实测。',
         '', '方法限制：安装头和冻结 ELF 覆盖不包含全部外部 ELF、dlopen、生成头、宏配置、模板实例化及 using 后的无限定名。普通符号通常不记录返回类型或异常传播；不以抽样或零名字交集证明不存在。',
         '', '偏差披露：两次检索使用了未经核实的源码包版本目录，退出 2（raw/resume0918/025、030）；均已按已有证据中的实际目录纠正（027、032），错误检索不参与阴性结论。', '']
(base/'RESUME_STATUS.md').write_text('\n'.join(text))
print(json.dumps(report, ensure_ascii=False, indent=2))
