from common import *
import csv,difflib,io
SUM=ROOT/'docs/progress/RUNTIME_PHASE_SUMMARY_0921'
BASE='bf35a916d9a9d058e91e851a74129a4efb76f9c8'
gate('summary_gate')
snapshot=OUT/'summary_v1';snapshot.mkdir(exist_ok=True)
listing=run('summary_baseline_files',['git','ls-tree','-r','--name-only',BASE,'--',str(SUM.relative_to(ROOT))])
baseline={}
for path in listing['stdout'].splitlines():
 p=ROOT/path;relative=p.relative_to(SUM);target=snapshot/relative
 if not target.exists():
  target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 baseline[str(relative)]=sha(target)
save('SUMMARY_V1_SHA256.json',baseline)
def readold(name):return (snapshot/name).read_text()
def replace_once(s,a,b):
 assert s.count(a)==1,(a,s.count(a));return s.replace(a,b,1)
def evidence(name,en=False):
 return f'[{"Evidence" if en else "证据"}: {name}](../R119_ENUM_RETEST/{name}) — SHA256 `{sha(OUT/name)}`'
evtypes=evidence('TYPES.tsv');evq=evidence('BOUNDARY_QUERIES.json');evsym=evidence('SEEK_SYMBOLS.json');evvirtual=evidence('VIRTUAL_CALL.json');evctrl=evidence('BOUNDARY_CONTROL.json')
sections={
'zh':f'''### 流状态枚举：armv7l 上仍存在的类型身份分歧

**事实链（新实测，版本 2 补入）**：fmtflags、iostate、openmode 在 GNU 中分别是 `_Ios_Fmtflags`、`_Ios_Iostate`、`_Ios_Openmode` 枚举，在 libc++ 中均是 `unsigned int`；seekdir 在 GNU 中是 `_Ios_Seekdir`，在 libc++ 中是 `ios_base::seekdir`。armv7l 与 aarch64 均 4/4 分歧，两库各五轮一致；四项两侧 size/align 均为 4/4。使用与先前目标类型测量相同的探针、目标配置和 QEMU 用户态，无板上测量。{evtypes}

| 类型 | GNU 编码 | libc++ 编码（两架构相同） |
| --- | --- | --- |
| fmtflags | `St13_Ios_Fmtflags` | `j` |
| iostate | `St12_Ios_Iostate` | `j` |
| openmode | `St13_Ios_Openmode` | `j` |
| seekdir | `St12_Ios_Seekdir` | `NSt3__18ios_base7seekdirE` |

**与整数别名不同**：先前 13 项在已测 armv7l 配置中底层整数选择相同；这四项的枚举/内建类型或枚举身份差异仍在，不能由数据宽度相同排除。它们独立登记为已确认类型差异，不代表四个产品故障。

真 ARM podofo 头编译得到 GNU `_ZN6PoDoFo14PdfInputDevice4SeekExSt12_Ios_Seekdir` 与 libc++ `_ZN6PoDoFo14PdfInputDevice4SeekExNSt3__18ios_base7seekdirE`。前面的 streamoff 编码均为 `x`，剩余差异来自 seekdir。显式限定调用用来取得 UND 名称；普通虚调用的两侧 IR 均从虚表 address point 选择从零开始索引 6 的入口并间接调用。这只是调用方编译观察，不是提供方虚表或混合运行兼容性测试。{evsym} {evvirtual}

**已排除的解释**：不是 4/8 字节宽度差；也不能由函数名不同直接推出虚表槽位不同或虚调用必失败。对已确认的 ChecksumStream::flags 两重载、open、PdfInputDevice::Seek、Clear 共 5 条公开签名（4 个身份）作函数身份查询，限定 x86_64 集合内没有对应异包 UND；bundle::Add 正向对照通过。旧索引的 61/85/87/12 个位置不是 245 条已确认公开签名。虚调用、内联和范围外产物未被这个阴性排除；已有 fmtflags/openmode 流对象成员边保留。新增边 0，仍为 18 包对 / 23 边。{evq} {evctrl}

**反驳需要什么**：若实际目标配置下两库完整类型编码相同，本配置的分歧结论需修正；若要判断产品影响，需真实异包调用（包括虚调用）及提供方、参数语义和对象生命周期证据。本轮没有以符号/槽位观察代替这些运行验证。

''',
'en':f'''### Stream-state enums: type identities still diverge on armv7l

**Fact chain — new measurements added in version 2:** GNU fmtflags, iostate and openmode are the named enums `_Ios_Fmtflags`, `_Ios_Iostate` and `_Ios_Openmode`; libc++ uses `unsigned int` for all three. GNU seekdir is `_Ios_Seekdir`, whereas libc++ uses `ios_base::seekdir`. Both armv7l and aarch64 show 4/4 divergences, with five consistent repetitions per library. All four have size/alignment 4/4 on both sides. The earlier target-type probe/configurations and QEMU user mode were reused; no board was used. {evidence('TYPES.tsv',True)}

| Type | GNU encoding | libc++ encoding (same on both architectures) |
| --- | --- | --- |
| fmtflags | `St13_Ios_Fmtflags` | `j` |
| iostate | `St12_Ios_Iostate` | `j` |
| openmode | `St13_Ios_Openmode` | `j` |
| seekdir | `St12_Ios_Seekdir` | `NSt3__18ios_base7seekdirE` |

**Unlike the integer aliases:** the earlier 13 expressions selected the same integer type on the measured armv7l configuration. These four enum/builtin or enum-identity differences remain despite equal widths. They are separately recorded type findings, not four demonstrated product failures.

Compiling the real ARM podofo header produced GNU `_ZN6PoDoFo14PdfInputDevice4SeekExSt12_Ios_Seekdir` and libc++ `_ZN6PoDoFo14PdfInputDevice4SeekExNSt3__18ios_base7seekdirE`. The streamoff encoding is `x` on both sides; seekdir accounts for the remaining difference. A qualified call exposes the UND name. Both ordinary virtual-call IR bodies instead select zero-based index 6 relative to the vtable address point and call indirectly. This observes caller compilation, not provider-vtable or mixed-execution compatibility. {evidence('SEEK_SYMBOLS.json',True)} {evidence('VIRTUAL_CALL.json',True)}

**Excluded explanations:** this is not a 4/8-byte width difference. Different function names do not directly establish different virtual-table slots or inevitable virtual-call failure. Function-identity queries covered the two ChecksumStream::flags overloads, open, PdfInputDevice::Seek and Clear: 5 confirmed declarations under 4 identities. No corresponding external UND was found in the scoped x86_64 set; the bundle::Add positive control passed. The old index's 61/85/87/12 positions are not 245 confirmed public declarations. Virtual calls, inline code and out-of-scope artifacts remain open. Existing fmtflags/openmode stream-member edges are retained. New edges: 0; the total remains 18 pairs / 23 edges. {evidence('BOUNDARY_QUERIES.json',True)} {evidence('BOUNDARY_CONTROL.json',True)}

**What would overturn the finding:** equal complete type encodings under the actual target configuration would require revising the divergence claim. Product consequences require real cross-package calls, including virtual calls, actual providers, parameter semantics and object-lifetime evidence. Symbol/slot observations do not substitute for those execution tests.

'''}
updates={}
for lang in ['zh','en']:
 name=f'SUMMARY_{lang}.md';s=readold(name)
 title_end=s.index('\n')
 note=('版本 2（2026-09-21）：仅补入四项流状态枚举复测与相关挂账；其余原结论不动。旧版、diff 与本次自检见 [版本说明](VERSION_20260921_2.md)。' if lang=='zh' else 'Version 2 (2026-09-21): adds only the four stream-state enum retests and related tracking; other conclusions are unchanged. See [version notes](VERSION_20260921_2.md) for the previous version, diff and current audit.')
 s=s[:title_end]+ '\n\n'+note+s[title_end:]
 marker='## 四、挂账：状态和重启条件' if lang=='zh' else '## Paused work and restart conditions'
 s=replace_once(s,marker,sections[lang]+marker)
 if lang=='zh':
  s=replace_once(s,'| 其余 572 个表达式的 armv7l 复测 | 不测，状态仍未测 |','| 其余 568 个表达式的 armv7l 复测 | 不测，状态仍未测 |')
  s=replace_once(s,'| 第 201–495 个候选 |','| 四项流状态枚举 | 类型分歧已确认（armv7l / aarch64），已确认直接公开签名消费未闭合；独立跟踪 | 真实消费者或虚调用链材料；见枚举补充与 BACKLOG 独立条目 |\n| 第 201–495 个候选 |')
  s=replace_once(s,'**不测决定的理由需纠正**：剩余 572 项中，按既有 x86_64 结果分解为相同 283、仅命名空间/标签差异 114、实现类/组合类型差异 111、枚举相关差异 4、不可得 60。','**不测集合更新**：原 572 项中的枚举相关差异 4 项已独立复测并跟踪；其余 568 项按既有 x86_64 结果分解为相同 283、仅命名空间/标签差异 114、实现类/组合类型差异 111、不可得 60。')
 else:
  s=replace_once(s,'| Remaining 572 expressions on armv7l |','| Remaining 568 expressions on armv7l |')
  s=replace_once(s,'| Candidates 201–495 |','| Four stream-state enums | Type divergence confirmed on armv7l / aarch64; consumers of the confirmed direct declarations remain unclosed; tracked separately | Actual consumers or virtual-call paths; see the enum addition and separate BACKLOG entries |\n| Candidates 201–495 |')
  s=replace_once(s,'**Correction to the rationale for not testing:** the remaining 572 expressions break down, by their existing x86_64 results, into 283 identical, 114 namespace/tag-only differences, 111 implementation/composite-type differences, 4 enum-related differences and 60 unavailable results.','**Updated no-test set:** the 4 enum-related differences have been separated from the former 572 expressions for retesting and tracking. The remaining 568 break down, by their existing x86_64 results, into 283 identical, 114 namespace/tag-only differences, 111 implementation/composite-type differences and 60 unavailable results.')
 updates[name]=s
 name=f'BACKLOG_{lang}.md';s=readold(name)
 s+='\n\n'+('## 版本 2：四项枚举独立条目\n\n原 62 项及其分类保持不变；下列是新增的类型差异跟踪，不是四条新增包边。\n\n' if lang=='zh' else '## Version 2: four separately tracked enum findings\n\nThe original 62 entries and classifications are unchanged. The entries below track type findings, not four new package edges.\n\n')
 s+=sections[lang]
 updates[name]=s.rstrip()+'\n'
old=list(csv.DictReader(io.StringIO(readold('BACKLOG.tsv')),delimiter='\t'));new=list(old)
queries=json.loads((OUT/'BOUNDARY_QUERIES.json').read_text())
for r in queries:
 row={k:'' for k in old[0]};row.update(candidate=r['expression'],primary_cause='ENUM_IDENTITY',status='TYPE_DIVERGENCE_CONFIRMED_EXPOSURE_NOT_CLOSED',missing_zh='已确认公开身份 '+r['query']+' 在限定 x86_64 ELF 集合内未见异包 UND；不排除虚调用/内联/范围外。',restart_zh='取得真实异包消费或虚调用链、提供方和构建配置，再验证；本轮不下产品结论。',restart_en='Obtain real external consumers or virtual-call paths, providers and build configuration; no product conclusion from this retest.',self_resolvability='PARTLY',dependency_zh='实际消费路径/间接调用与提供方材料',source=str((OUT/'BOUNDARY_QUERIES.json').relative_to(ROOT)),source_sha256=sha(OUT/'BOUNDARY_QUERIES.json'),original_evidence='docs/progress/R119_DIVERGENT/PUBLIC_DECLARATIONS.tsv',original_evidence_sha256=sha(ROOT/'docs/progress/R119_DIVERGENT/PUBLIC_DECLARATIONS.tsv'))
 new.append(row)
buf=io.StringIO();w=csv.DictWriter(buf,fieldnames=list(old[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(new)
updates['BACKLOG.tsv']=buf.getvalue()
diff=[];changes=[]
for name,text in updates.items():
 oldtext=readold(name)
 current=(SUM/name).read_text()
 assert current==oldtext or current.rstrip()==text.rstrip(),('unexpected user change',name)
 (SUM/name).write_text(text)
 diff+=difflib.unified_diff(oldtext.splitlines(keepends=True),text.splitlines(keepends=True),fromfile='a/docs/progress/RUNTIME_PHASE_SUMMARY_0921/'+name,tofile='b/docs/progress/RUNTIME_PHASE_SUMMARY_0921/'+name)
 changes.append(dict(file=name,before_sha256=baseline[name],after_sha256=sha(SUM/name)))
(OUT/'SUMMARY.diff').write_text(''.join(diff))
note='''# 版本 2：枚举补充（2026-09-21）

原 23 边汇编已经审阅通过。本次仅修改 SUMMARY_zh/en.md 与 BACKLOG_zh/en.md、BACKLOG.tsv：加入四项枚举复测、独立挂账和 572 → 568 的不测范围调整；原 62 项不动，跨包面仍为 18/23。

- [本轮完整报告与限制](../R119_ENUM_RETEST/FINAL.md)
- [改前原件](../R119_ENUM_RETEST/summary_v1/)
- [完整 diff](../R119_ENUM_RETEST/SUMMARY.diff)
- [新旧 SHA 对照](../R119_ENUM_RETEST/SUMMARY_UPDATE.json)
- [本轮自检](../R119_ENUM_RETEST/AUDIT.json)

旧版对应提交 bf35a916d9a9d058e91e851a74129a4efb76f9c8。归档为逐字节快照，归档内相对链接按原目录布局解释，可在该提交查看；不重新改写归档链接。

原目录中的 code/assemble.py、code/body_*、AUDIT.json、CHECKS.json、SELF_CHECK.md 是版本 1 的生成及核查记录，本次按“其余内容不动”保留，不代表本轮增补的自检。不要用旧生成器覆盖版本 2；增量维护入口是 ../R119_ENUM_RETEST/code/update_summary.py。本版本仅对新增条目另行核查，旧候选、边表、SHA 索引和其他结论未重判。

Version 2 adds only the enum retests and their separate tracking. The prior 62 entries and 18/23 boundary count are unchanged. The original generator and audit files describe version 1; the current incremental update and audit are linked above. Do not regenerate version 2 with the old generator. The archived bytes retain the original relative-link context.
'''
(SUM/'VERSION_20260921_2.md').write_text(note)
for name,h in baseline.items():
 if name not in updates:assert sha(SUM/name)==h,name
save('SUMMARY_UPDATE.json',dict(version=2,baseline_commit=BASE,changed=changes,new_version_note='VERSION_20260921_2.md',all_other_original_files_unchanged=True,original_backlog_rows=62,new_separate_findings=4,remaining_unmeasured=568,diff_sha256=sha(OUT/'SUMMARY.diff')))
print('summary updated',len(changes),'files; other original files unchanged')
