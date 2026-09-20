#!/usr/bin/env python3
from common import *
rows=list(csv.DictReader((OUT/'CAUSES_156.tsv').open(),delimiter='\t'))
plan=json.loads((OUT/'code/gap_plan.json').read_text()); assessments=[]
for r in rows:
    if r['primary_cause'] not in ['C','D']:continue
    task,can,dependency,lo,hi=plan[r['rank']]
    assessments.append(dict(rank=r['rank'],candidate=r['candidate'],primary_cause=r['primary_cause'],other_causes=r['all_causes'],specific_gap=r['individual_gap'] if r['rank']!='154' else '实际返回 cv_status 的函数已见；真实消费者和模板实例尚未闭合。',next_evidence_needed=task,self_resolvability=can,dependency=dependency,estimate_minutes=f'{lo}–{hi}',estimate_kind='规划估计，非实测；范围是一次取证尝试，非承诺闭合',performed_this_round='NO',original_evidence=r['source']))
assert len(assessments)==62 and set(plan)=={r['rank'] for r in assessments}
tsv(OUT/'CD_GAPS_62.tsv',assessments)
tsv(OUT/'C_FUNCTION_ID_ASSESSMENT.tsv',[r for r in assessments if 'C' in r['other_causes'].split(',')])
tsv(OUT/'D_MATERIAL_GAPS.tsv',assessments) # C 组也有待补材料，按允许多组归属保存，不重复计主因。
(OUT/'CD_ASSESSMENT.md').write_text('''# 函数身份方法与缺材料评估（不执行新取证）

主因 C 23 项、D 39 项，共 62 项仍未定。逐项见 `CD_GAPS_62.tsv`，C 辅因另见 `C_FUNCTION_ID_ASSESSMENT.tsv`。C 主因同时属于 D 的待补调用/实例材料，不把它重复算为新增候选。

函数身份方法要用完整的公开函数名/类名、参数列表、对应头版本和原始修饰名；返回类型、别名结果、隐式 this 成员、C 入口参数和异常传播分别从源码还原。随后才能核 UND/DEF、DT_NEEDED/SONAME、包含链与具体实例归属。若是函数指针/虚调用，还需注册与实际调用对象；若是异常，还需抛出入口与调用端、异常配置，RTTI 相同不能替代传播证明。

估计量级（明确为未实施的规划，不是测得速度）：简单已知函数/周期别名 20–60 分钟；外层对象/返回迭代器/别名投影 30–90 分钟；模板或同名库绑定 45–120 分钟；真实异常/间接回调链 60–180 分钟。拿不到产物/构建配置时，耗时没有可信有限上界。每行给出取证目标、具体依赖与本线可解性。

`YES_LOCAL_RESEARCH` 表示本线可在现有源码/ELF 发起下一步核查，不承诺最终一定有阳性或一定能完整关闭；`PARTLY` 表示本地部分可查但可能需要组件方提供构建/注册/部署材料；`NOT_OBSERVED` 表示当前不足以判断是否能仅靠本线补齐。没有把尚未尝试的缺材料写成不可得。

本轮没有执行 C 组函数身份新实验，也没有访问开发板。`cv_status` 的完整旧上下文复核发现公开返回声明，故从初始 B 转 C；未据此追加包边。其他已确认阳性存储链没有改变。
''')
print('62 gap assessments written; no C identity experiment executed')
