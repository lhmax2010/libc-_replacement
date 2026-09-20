#!/usr/bin/env python3
from common import *
import datetime
def stamp(tag,key):return datetime.datetime.fromisoformat(json.loads((OUT/f'raw/{tag}.time.json').read_text())[key])
def elapsed(tag):return (stamp(tag,'end')-stamp(tag,'start')).total_seconds()
a=json.loads((OUT/'A_EXECUTION_TIME.json').read_text())['elapsed_seconds']+json.loads((OUT/'A_VALIDATION_TIME.json').read_text())['elapsed_seconds']
b=sum(elapsed(tag) for tag in ['011_b_contexts','013_b_secondary','014_finalize_b'])
# 阶段跨度有交错，不相加成纯人工用时。
a_span=(stamp('012_validate_a','end')-stamp('009_push_causes','end')).total_seconds()
b_span=(stamp('014_finalize_b','end')-stamp('011_b_contexts','start')).total_seconds()
info=dict(A=dict(candidates=89,measured_serial_execution_seconds=a,amortized_execution_seconds=a/89,phase_span_seconds=a_span,phase_span_per_candidate=a_span/89),B=dict(candidates_considered=6,reclassified=5,measured_serial_execution_seconds=b,amortized_execution_seconds_per_considered=b/6,amortized_execution_seconds_per_reclassified=b/5,phase_span_seconds=b_span,phase_span_per_considered=b_span/6),manual_exclusive_average='NOT_OBSERVED',remaining295_total_estimate='NOT_OBSERVED',limitation='共享索引/既有审读复用；阶段交错，不能把阶段跨度相加或当纯人工工时。')
save(OUT/'TIMING.json',info)
(OUT/'TIMING_AND_SCALING.md').write_text(f'''# 分组耗时与规模化边界

| 组 | 项数 | 实测串行执行合计 | 摊销执行平均 | 观测阶段跨度 | 阶段跨度/项 |
|---|---:|---:|---:|---:|---:|
| A | 89 | {a:.3f} 秒 | {a/89:.3f} 秒/项 | {a_span:.3f} 秒 | {a_span/89:.3f} 秒/项 |
| B（包括转 C 的 cv_status） | 6 审读、5 重判 | {b:.3f} 秒 | {b/6:.3f} 秒/审读项；{b/5:.3f} 秒/重判项 | {b_span:.3f} 秒 | {b_span/6:.3f} 秒/审读项 |

执行时间由命令时间记录与脚本单调/墙钟实测取得：A 为两侧共享编译/5次运行/定义提取及逐项证据校验；B 为完整既存上下文显示、补充资料读取及逐项核验。不是逐项从零调查耗时。

阶段跨度：A 从成因章节推送完成至 A 证据核对结束；B 从完整 B 上下文读取至 B 证据核对结束。两段因穿插复核存在重叠，**不可相加**。期间包含设计、共享脚本、证据复用与阅读。逐项纯人工专属时间未计时，记 NOT_OBSERVED，不由机器秒数伪造。

最终主因 A+B=94/156=60.26%，C+D=62/156=39.74%。因此“现有未定项多数可由结构/限定范围收窄”有本轮数据支持；“只靠名字交集自动完成全部定界”不成立。剩余 62 项需要不同的方法与材料，不把它们再送同一个名字扫描器就称为完成。

后续 295 项尚未按成因审读，且冻结排序按包族数而非随机抽样。本轮重判复用了两批完整安装头索引、ELF 索引和既有逐项记录，而后续候选未必具备相同输入。因此虽 A+B 过半，**不足以给整个后续 295 项可信总工时**：总工时记 NOT_OBSERVED，不作线性承诺。

可复用的成本模型是：前置索引和逐项成因审读的时间 + A 类定义/实例确认与共享探针时间 + B 类上下文全审与消歧时间 + C/D 按函数/对象身份取证与等材料时间。C/D 每项规划量级另见 CD_GAPS_62.tsv（20–180 分钟一次尝试，缺外部材料无有限完成保证）。待后续候选先分组且逐项专属时间开始记录后，才可把分组数量与对应实测平均相乘。这里不把 60.26% 当作后续 295 项的实测分布，也不据 0.x 秒执行平均宣称几分钟可完成。
''')
print(json.dumps(info,ensure_ascii=False))
