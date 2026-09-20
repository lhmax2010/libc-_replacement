#!/usr/bin/env python3
from common import *
from collections import Counter
gate('finish_gate')
audit=json.loads((OUT/'FINAL_AUDIT.json').read_text());rows=list(csv.DictReader((OUT/'CANDIDATE_RESULTS.tsv').open(),delimiter='\t'))
assert len(rows)==65 and all(int(r['unresolved'])>0 for r in rows)
assert sum(int(r['occurrences']) for r in rows)==31586
assert audit['decisions']==dict(RESULT_UNRESOLVED=15793,RESULT_NO_OBJECT=15780,RESULT_STDLIB_OBJECT=13)
md=['# 65 个候选逐项汇总','','全部候选均有未定用法；下表对象数与未定数按出现位置计，不是包边。逐处实参与缺口在相应压缩 JSON 内。','','| 序 | 候选 | 全部位置 | 无对象 | std 对象 | 未定 | 证据 |','| --- | --- | ---: | ---: | ---: | ---: | --- |']
for r in rows:md.append(f"| {r['ordinal']} | `{r['candidate']}` | {r['occurrences']} | {r['no_object']} | {r['stdlib_object']} | {r['unresolved']} | [{r['evidence']}]({r['evidence']}) |")
(OUT/'CANDIDATES.md').write_text('\n'.join(md)+'\n')
groups=Counter()
for reason,n in audit['unresolved_reason_counts'].items():
    if reason.startswith('DEPENDENT_OR_LOCAL_NAMES'):category='模板形参/局部名字查找未闭合（具体名字逐项保留，不全是缺材料）'
    elif 'CUDA' in reason or '命名空间' in reason:category='目标或命名空间不适用宿主探针'
    elif '具体类型已测得' in reason:category='具体结果已知但分类口径待定'
    else:category='未选投影/模板模板使用/词法与上下文不足（见逐项原因）'
    groups[category]+=n
save(OUT/'UNRESOLVED_GROUPS.json',dict(groups))
measurements=[json.loads(p.read_text()) for p in (OUT/'measurements').glob('*.json')]
stats=Counter(m['status'] for m in measurements)
for m in measurements:
    if m['status']=='MEASURED':
        assert m['repeat_count']==5 and len(m['runs'])==5
        assert sha(ROOT/m['source'])==m['source_sha256']
        for run in m['runs']:assert int((ROOT/(run+'.exitcode')).read_text())==0
save(OUT/'MEASUREMENT_AUDIT.json',dict(measurement_files=len(measurements),statuses=dict(stats),measured_occurrence_basis=audit['basis']['MEASURED_BOTH'],static_bool_occurrences=audit['basis']['STATIC_DEFINITION_CHAIN'],note='表达式复用测量；非按每个位置重新运行五次。探针控制重复验证，不重跑原24项分类。'))
start=json.loads((OUT/'raw/001_gate.time.json').read_text())['start'];now=datetime.datetime.now(datetime.timezone.utc);elapsed=(now-datetime.datetime.fromisoformat(start)).total_seconds()
save(OUT/'TIMING.json',dict(start=start,report_time=now.isoformat(),elapsed_seconds=elapsed,elapsed_hours=elapsed/3600,limit_hours=6,overlap_seconds=1.834358,rerun_evidence=['raw/012_measure15_frozen','raw/013_classify15_frozen']))
(OUT/'STATUS.md').write_text(f'''# 投影审计交接

状态：**PARTIAL（65/65 全量命中已处理；语义结果未全闭合），待人工审阅**。

| 阶段 | 状态 | 产物 |
| --- | --- | --- |
| 全量命中与原始实参提取 | 65/65；74,954 行 → 30,523 个候选/SHA/行 → 31,586 处 | COVERAGE_INVENTORY.tsv、occurrences/ |
| 结果类型 | 15,780 处无对象；13 处 std 对象；15,793 处未定 | results/、CANDIDATES.md |
| 候选聚合 | 整项无对象 0；含 std 对象 2（与未定重叠）；未定 65 | CANDIDATE_RESULTS.tsv |
| 对象边界 | 13 处均未闭合新的异包边；确认新增 0 | OBJECT_BOUNDARIES.tsv |
| 跨包面 | 已确认 16 包对 / 21 符号边不变（下界） | EDGE_COUNTS_BEFORE_AFTER.json |
| 保护与覆盖自检 | 484 个旧文件未变；6,952 来源路径 SHA 通过 | FINAL_AUDIT.json |

截至汇总生成用时 {elapsed/3600:.3f} 小时；原始时间见 TIMING.json。第15/30/45/60项已分别提交推送；末5项与汇总作最终提交。

资源 light、RLIMIT_AS 9,921,875,558 字节（MemTotal 的30%），分析 nice19/ionice3；Git 普通优先级。不用板子、不构建平台包、不改平台代码、不推 Gerrit。串行执行曾有1.834358秒偏差，已重新串行取证，见 FINAL.md；不隐去偏差。

原 BUILTIN_ALIAS/EMPTY_TYPE 结论与 plan.json 不变。完整结果、未定原因及自行判断见 FINAL.md、UNRESOLVED_REASONS.tsv。完成本轮后停止，交人工审阅。
''')
scripts=list((OUT/'code').glob('*.py'))+list((OUT/'probes').glob('*.cpp'))+[REC,ROOT/'docs/progress/R119_batch1/code/resume_limited.py']
save(OUT/'SCRIPT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in scripts})
for path,digest in json.loads((OUT/'FROZEN_INPUT_SHA256.json').read_text()).items():assert sha(ROOT/path)==digest,path
print(json.dumps(dict(unresolved_groups=dict(groups),measurement_statuses=dict(stats),elapsed_hours=elapsed/3600),ensure_ascii=False,indent=2))
