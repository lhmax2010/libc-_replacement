"""Apply explicit human dispositions to the preserved R3 inventory."""
import csv,json
from pathlib import Path
E=Path('docs/progress/LLVM_W4_0923')
rows=list(csv.DictReader((E/'spec-audit_R3/LOCAL_SPECS_R3.tsv').open(),delimiter='\t'))
groups=[
(['S003','S110','S019','S020','S021','S022'],'旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。'),
(['S008','S026'],'旧LLVM配方，缺f203923a已有的MLGO libc++资产与build_with_libcxx定义；已被取代。'),
(['S029','S035'],'按人工裁决为R104旧版本的flags块位置差异；采用R105验收版99cdc598…，该版与sandbox一致。'),
(['S051','S053','S054','S056'],'人工将此组归为含r105_validation_force_gcc的验证专用配方/基线对照；按设计不推送，不作为QuickBuild配方遗漏。'),
(['S023',*['S%03d'%n for n in range(97,105)]],'R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。'),
(['S069','S070','S071'],'R29早期实验；采用与sandbox一致的R104验收版。')]
decisions={i:why for ids,why in groups for i in ids}
assert len(decisions)==26
expected=json.loads((E/'spec-audit_R3/COMPARISON_SUMMARY_R3.json').read_text())['other_different_ids']
assert set(decisions)==set(expected)
out=['# 本地 spec 清单：R4 人工裁决登记','',
'**QuickBuild 所需的全部本地配方改动均已进入 sandbox。** 此结论按本轮人工确定的Base-Toolchain当前验收配方范围作出，不把所有历史试验配方作为待发布输入。R3原始清单、diff、FAIL及R2全根表/NOT_OBSERVED原样保留。','',
'## 26 项逐项关闭','',
'下表“处置”是人工裁决登记，不重新宣称已对旧配方做构建验证。原diff的真实头为reference（sandbox）在---侧、历史used副本在+++侧：`-`表示sandbox有而旧副本无，`+`表示历史副本有；不将这一符号方向反读为遗漏。','',
'| ID | 包名 | 历史spec路径 | 人工处置 | 结论 |',
'| --- | --- | --- | --- | --- |']
for r in rows:
    if r['id'] in decisions:out.append('| '+' | '.join([r['id'],r['package'],r['path'],decisions[r['id']],'CLOSED_BY_HUMAN_DECISION'])+' |')
out += ['', '## 27 项无参照记录：不属于 Base-Toolchain 范围','',
'按人工裁决为测试夹具及R29 Unified实验包，仅登记排除；原NOT_AVAILABLE不改写为已核实与原仓一致。','',
'| ID | 包名 | 路径 | 原记录 | R4范围处置 |','| --- | --- | --- | --- | --- |']
for r in rows:
    if r['classification']=='NOT_AVAILABLE':out.append('| '+' | '.join([r['id'],r['package'],r['path'],'NOT_AVAILABLE','OUT_OF_SCOPE_BY_HUMAN_DECISION'])+' |')
out += ['', '## 本轮两项候选的6个副本','',
'远端5c169afc的树与本地db5b49af候选完全相同，故下列已授权功能改动已进入sandbox；不再标待推送。','',
'| ID | 包名 | 路径 | R4状态 |','| --- | --- | --- | --- |']
for r in rows:
    if r['authorized_current_change']=='True':out.append('| '+' | '.join([r['id'],r['package'],r['path'],'PUSHED_5c169afc'])+' |')
out += ['', '## 范围与保留项','',
'其余38项与sandbox一致、15项与原仓一致的R3记录沿用。R3中111条未能定位宿主路径的历史命令记录继续保留NOT_OBSERVED，不声称此次恢复了其路径或重做全根溯源；它们不成为在人工已批准的当前配方范围之外无限追溯的任务。','',
'本次关闭的是当前QuickBuild配方与分支前置核查，不是QuickBuild构建、服务端配置或镜像安装验证通过。']
(E/'LOCAL_SPECS_R4.md').write_text('\n'.join(out)+'\n')
print('human dispositions: 26 closed; 27 out of scope; 6 authorized copies pushed; R2/R3 unchanged')
