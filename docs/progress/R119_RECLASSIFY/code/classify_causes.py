#!/usr/bin/env python3
"""按已逐项审读的旧记录建立成因表；此步不改任何最终分类。"""
from common import *
from collections import Counter
plan=json.loads((OUT/'code/plan.json').read_text()); allrows=inputs(); rows=[]
for item in allrows:
    old=item['row']; rank=item['rank']; key=str(rank)
    if old['result']!='NOT_OBSERVED': continue
    note=item['dossier']['review']; sub=next((k for k in plan if k.startswith('A_') and key in plan[k]),None)
    if sub: primary='A'; groups=['A']; reason='结构核查候选：'+sub[2:]+'；须两侧逐项实测和定义核对通过后才重判。'
    elif rank in plan['B_REVIEW']: primary='B';groups=['B'];reason='旧记录限定头文件用途已复核，原缺口为范围授权；重判前复核全部已存上下文，不使用类型名字的 ELF 零交集。'
    elif rank in plan['C_PRIMARY']: primary='C';groups=['C','D'];reason='公开别名、返回类型、外层类型或异常对象不可靠地保留在普通修饰名中，需函数身份/调用路径取证。'
    else:
        primary='D';groups=['D'];reason='实际对象或公开形态已见，但真实外层对象/模板实例/消费者/绑定链缺口尚未关闭。'
        if rank in plan['D_FUNCTION_ID_ALSO']:groups.append('C')
    if primary=='A' and ('::type' in note.get('note','') or rank in [12,38,48,49,80,140,142,143,161,166,184,192]): groups.append('C')
    if primary in ['C','D']:
        can='部分可解；可在现有源码/ELF 继续核对，但可能需组件方提供生成配置、实际加载关系或缺失产物。' if rank in plan['D_EXTERNAL_OR_MIXED'] else '本线可尝试：现有源码与冻结 ELF 可继续按函数/外层对象身份核查；不保证一定能闭合。'
    else:can='本线可解：本轮结构探针/限定范围证据复核。'
    rows.append(dict(rank=rank,candidate=old['candidate'],primary_cause=primary,all_causes=','.join(groups),structural_subtype=sub[2:] if sub else '',reason=reason,actual_observation=note.get('note',''),individual_gap=note.get('missing',''),self_resolvable=can,source=str(item['dossier_path'].relative_to(ROOT)),source_sha256=sha(item['dossier_path'])))
assert len(rows)==156 and len({r['rank'] for r in rows})==156
tsv(OUT/'CAUSES_156.tsv',rows)
counts=Counter(r['primary_cause'] for r in rows); overlap=Counter(g for r in rows for g in r['all_causes'].split(','))
save(OUT/'CAUSES_SUMMARY.json',dict(primary=dict(counts),overlapping=dict(overlap),total=156,classification_changed=False))
frozen=[ROOT/'docs/progress/R119_batch1/W1/REVIEW_001_100.tsv',ROOT/'docs/progress/R119_batch2/W1/REVIEW_101_200.tsv']
save(OUT/'INPUT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in frozen})
(OUT/'CAUSES.md').write_text('# 未定项成因分组（重判前）\n\n共 156 项；主因互斥，辅因可重叠。A/B 是待本轮证据检查的处理通道，不是已通过的结论。\n\n| 主因 | 数量 | 样例 |\n|---|---:|---|\n'+''.join(f"| {g} | {counts[g]} | "+'、'.join(r['candidate'] for r in rows if r['primary_cause']==g)[:160]+' |\n' for g in 'ABCD')+'\n逐项原观察、实际缺口、本线可解性、来源摘要见 CAUSES_156.tsv。未修改两个原定界表。已有 allocator/hash/less 成员存储链、duration 与特化的区分保持不动。\n')
print(dict(counts))
