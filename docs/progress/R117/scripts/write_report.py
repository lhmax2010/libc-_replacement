from common import *
import csv,collections,datetime
s=json.loads((OUT/'SUMMARY.json').read_text());p=json.loads((OUT/'platform/SUMMARY.json').read_text());h=json.loads((OUT/'headers/COVERAGE_SUMMARY.json').read_text())
rows=list(csv.DictReader((OUT/'SKELETON.tsv').open(),delimiter='\t'));byname={r['类型/函数']:r for r in rows}
headers=list(csv.DictReader((OUT/'HEADER_COVERAGE.tsv').open(),delimiter='\t'))
sets=list(csv.DictReader((OUT/'HEADER_SETS.tsv').open(),delimiter='\t'))
header_categories=[]
for lib in ['libcxx','libstdcxx']:
 for category in ['STANDARD_HEADER_CANDIDATE','TS_EXPERIMENTAL','VENDOR_EXTENSION','VENDOR_ABI_EXTENSION']:
  selected=[r for r in headers if r['library']==lib and r['category']==category]
  if selected:header_categories.append(f'| {lib} | {category} | {len(selected)} | {len({r["header"] for r in selected})} |')
high_table=[]
for name in s['high_entities']:
 r=byname[name];high_table.append(f'| `{name}` | {r["ID"]} | {r["平台使用包数"]} | {r["去重包族数"]} | {r["生产样式包族数"]} |')
pri='\n'.join(f'| {k} | {v} |' for k,v in s['priority'].items())
start=json.loads((OUT/'raw/001_gate.time.json').read_text())['start'];now=datetime.datetime.now(datetime.timezone.utc);hours=(now-datetime.datetime.fromisoformat(start)).total_seconds()/3600
report=f'''# 两套标准库差异清单：范围候审

状态：**PARTIAL_SCOPE — 骨架和原始索引已形成；尚不能签署“完整公开接口全集 + 真实语义使用数”**。本轮只定范围，没有验证条目的布局、行为或兼容性差异。

## 1. 审阅入口

- [骨架 TSV](SKELETON.tsv) / [完整 Markdown](SKELETON.md)：{s['entities']:,} 个已解析实体名/直接公开嵌套类型；每行有实际头文件位置。
- [逐入口覆盖](HEADER_COVERAGE.tsv)、[逐头实体索引](header_entities.tsv.gz)、[完整声明](headers_repair/declarations.jsonl.gz)、[成员索引](public_members.tsv.gz)、[实体的全部源码位置](entity_sources.tsv.gz)。各批声明合并，不能只读其中一个 gzip 当作全集。
- [平台覆盖与缺文件](platform/SUMMARY.json)、[逐 RPM 状态](platform/STATUS.json)、[实体包集合](platform/entity_package_counts.tsv)、[用法候选样例](platform/entity_examples.tsv)、[接口类型候选队列](platform/INTERFACE_CANDIDATES.tsv)。
- [高优先级场景](HIGH_PRIORITY_SCENARIOS.tsv)、[既有材料对照](EXISTING_MATERIAL_CROSSWALK.md)、[25 项横切范围](CROSSCUTTING_SCOPE.tsv)、[疑问与自行判断](QUESTIONS.md)。

“完整性待确认”不是把未解析接口删掉：每个入口仍在覆盖表，所有未完整解析诊断在 HEADER_ERRORS.tsv，非活动条件及原文声明候选在 source_index/conditional_declarations.tsv.gz。

## 2. 固定输入与计数单位

libc++ 头文件版本宏为 220108（22.1.8），LLVM 工作树 revision `5ed6c77278dfa7a470667cf1a137723d3c96fe60`；GCC `BASE-VER` 为 14.2.0，revision `49c5060508ce8a6ed6beca375233688a6bdd9140`。源码路径为 `codes/llvm/libcxx/include/`、`codes/gcc/libstdc++-v3/include/`，另收 GCC 安装于公共 include 路径的 libsupc++ 入口（exception/new/typeinfo/initializer_list/compare/cxxabi.h，以实际存在为准）。运行库源码只用于既有风险的范围定位，未改动。

平台分母固定使用 T1/scan_status.tsv 的 **{p['source_rpm_identities']} 个 source RPM 身份 / {p['unique_package_names']} 个去重包名**，包名映射用 R32/tables/scan_status.tsv。当前 extraction_results.tsv 有 372 行，多出的 `chromium-efl-1.1.144-1.src.rpm` 抽取失败且不在原扫描名单；没有把它计进 371，也没有默默删除缺口。[逐行对照](platform/MANIFEST_RECONCILIATION.json)

头文件名、物理变体、声明/重载、具体模板实例、source RPM、包族、命中行、符号边都不是同一单位，不能相加。本表的包数列是**源码文本中限定名或简单命名空间别名候选命中的 RPM 集合大小**，不是运行实例数，也不是完整语义使用数。

## 3. 公开接口范围从何而来

libc++ 以其 header_information.py 和 include 目录枚举标准入口候选、C 兼容包装、experimental 入口；`ext/` 单列扩展；排除 `__` 内部入口、构建文件。GCC 以 std、C 头变体及 Makefile.am 安装名单定位入口；ext（含 pb_ds）、debug、parallel、backward、decimal、experimental、tr1/tr2 分组，不把 bits 内部文件当作独立标准头。实际声明可能在内部头中，这些位置仍保留用于追溯。

| 库 | 源树分类 | 物理入口文件 | 去重入口名 |
|---|---|---:|---:|
{chr(10).join(header_categories)}

合计 **{h['physical_entries']} 个物理入口候选、{s['header_names']} 个并集入口名**；libc++ {s['header_names_by_library']['libcxx']} 名、GNU {s['header_names_by_library']['libstdcxx']} 名。这些数包括扩展和兼容/辅助安装头，**不是标准规定头文件数量**。GCC 的 C 头物理变体并不同时部署；部分 extension 安装头是辅助片段。标准归属采用库自己的目录/安装分类，未逐项对规范目录做独立证明，因此所有 STANDARD_HEADER_CANDIDATE 保留“候选”标记。`TS_EXPERIMENTAL` 是技术规范/技术报告或实验入口，不冒称供应商独创，也不混入当前标准主体。

使用同版本 Clang/libclang 22 前端，只解析声明、跳过函数体；标准入口覆盖 C++03/11/14/17/20/23/26，扩展主要 C++23；解析选项和每批结果完整保存。LLVM 生成配置来自现有 native 构建，GNU generated config/备用头来自已有 GCC14 SDK；目标为 x86_64。**解析 GCC 头使用的是 Clang 前端，不等于 GCC 前端的所有条件分支**。没有跑标准库差异测试、没有请求开发板。

共 {s['parse_operations']} 次入口/模式解析（包含修正后重扫），{s['parse_status']['PARSED']} 次无错误诊断，{s['parse_status']['PARTIAL_PARSE_ERRORS']} 次带错误诊断；后者包含在旧语言模式下受条件限制的入口，不能直接算接口缺失。{len(h['never_fully_parsed'])} 个物理候选在所有已试模式中均未完整解析；具体是 GNU C 头替代变体、部分辅助头、decimal、experimental/simd 等，逐项可查 HEADER_COVERAGE.tsv。libc++ string.h 的 include_next 主文件上下文问题已改用真正的 include 包装解析，未把工具问题记为库差异。

实体按限定名合并重载/模板特化，原名与 ABI 内联命名空间、USR、签名、实际声明位置均留在原始声明中；直接公开嵌套类型单列，成员操作在成员索引和所属类型场景中。有限模板声明不能枚举无限实例。`std::views` 的文本归并只作已知库别名处理；一般命名空间/类型别名仍需语义解析。骨架的章节根据实际声明路径/命名空间做阅读分组，不是 ISO 章节编号，也不是唯一语义归属。

**交集与差集的强度**：{s['both_entities']:,} 个实体名两侧解析见到；{s['only_observed_one_side']:,} 个只在一侧解析见到。后者一律标另一侧 `NOT_OBSERVED`，**不是 {s['only_observed_one_side']} 个已证迁移编译失败项**。真实入口文件差集另在 HEADER_SETS.tsv；不能由入口名差集推出同名设施在其他入口下不存在，更不能推出行为兼容。声明中保留 GNU 命名空间、ext 源位置等扩展标记；对于普通 std 命名空间内的扩展，尚需标准归属复核。

## 4. 平台扫描覆盖及明确缺口

371 个根目录均完成所选扩展名扫描，原抽取状态均为 EXTRACTED_OK；这不是证明每个历史源码包包含全部外部下载依赖。选定 C/C++/ObjC 扩展文件 {p['eligible_files']:,} 个，成功读取 {p['read_files']:,} 个，读取 {p['read_bytes']:,} 字节；{p['file_status'].get('NOT_OBSERVED_SYMLINK_NOT_FOLLOWED',0)} 个符号链接未跟随，{p['file_status'].get('NOT_OBSERVED_NUL_ENCODING',0)} 个含 NUL 文件未作文本解析。{p['package_status']['SCANNED_WITH_GAPS']} 个 RPM 因上述文件有缺口，{p['package_status']['SCANNED_SELECTED_EXTENSIONS']} 个在选定文件规则内未遇读取缺口。逐文件身份/哈希在 files.tsv.gz，漏项在 unread_files.tsv。

去注释/普通字符串/原始字符串后检索 std、__gnu_cxx、__gnu_parallel 的限定名；简单 namespace 别名单独标候选。共 {p['token_hits']:,} 次 token 命中，匹配 {s['matched_usage_entities']:,} 个骨架实体，另有 {s['unmatched_tokens']:,} 个未匹配限定名 token，全部单列，不丢弃。**9531 个 using-namespace 上下文涉及 164 个 RPM**，证明无限定名使用不能忽略。

用法形态只做显式候选分类：头部签名候选、成员或局部变量未区分、直接使用或声明未区分；未以括号/分号启发式冒充 AST 语义。测试/样例、第三方捆绑、捆绑工具链、生产样式未确认四类分别计数；目录分类有误判可能，“生产样式”不等于已进入产品构建。

本轮不覆盖：有效编译条件和模板实例化、宏生成与 ADL、无限定名和一般类型别名、跨文件命名空间作用域、无后缀源文件/非选定语言、运行期调用图、所有 C 全局 API 的使用、__gnu_pbds/__gnu_debug 的全平台使用、操作符/字面量的隐式调用、外部闭源应用和源码包未带入的下载件。对这些行不用 0 代替 `NOT_OBSERVED`。跨包接口候选队列共 **{s['interface_candidate_type_names']} 个类型名**，不是已确认的跨包类型总数；SDK 暴露和实际消费关系仍需核对。

## 5. 优先级与使用场景

| 暂定标签 | 实体名数 |
|---|---:|
{pri}

高标签采用本线已有对外/插件接口签名作为证据，限于下表三项；**不代表全平台只有三个高优先级类型**。其余接口候选未闭合跨包证据，没有强行全部归高。中候选的“至少 10 个包族”是本轮显式工作假设，不是产品阈值；“仅在包内”尚未证明。低候选表示较少限定名命中，不保证真实使用稀少；零仅表示覆盖内限定名零命中，有正向对照，不能改写成“平台没用”。

| 类型 | 骨架行 | RPM 限定名命中 | 去重包族 | 生产样式包族 |
|---|---|---:|---:|---:|
{chr(10).join(high_table)}

HIGH_PRIORITY_SCENARIOS.tsv 按每个类型分别列构造/赋值/拷贝/移动、迭代及顺序、比较/hash、容器元素/map key、线程/DSO、C API、错误及其他场景，并给出源码/签名索引。std::function 无迭代/data/c_str；vector/function 不预设有可用 std::hash，map key 也须满足比较器等约束。每种候选差异都标“待查”，没有使用 P9 的固定大小样本来推导全类型兼容结论。

估算（人为工作量估计，不是实测耗时）：对已有三项，逐类型建立双库、输入、生命周期、错误路径和跨 DSO 可复现验证，约 **8–24 人时/类型**，三项约 **24–72 人时**，不含缺失 provider 的完整构建及额外架构环境。简单声明/约束类条目初查约 0.5–2 人时；复杂所有权/回调/取消类可超出上述范围。候选队列先做约 10–30 分钟/候选的签名/调用方定界，然后才可估全量高优先级；其总数目前为 `NOT_OBSERVED`，不能把 7405 行乘固定时间当承诺。

## 6. 与既有材料的逐项映射

P9 的 10 个布局不同实例逐个映射到对应类型；R66 25 项全部保留为横切维度；ABI 报告第四节 5 行、原报告七个问题及本轮指明的评审议题均有落点。宏/展开器/静态 runtime/预制产物/driver/版本脚本并非标准公共类型，单列而不伪造类型条目。第七个取消问题已包含 25 项风险，重复引用不重复计数。

旧报告的架构、版本和时间口径保留，不把旧数字当本次结论。原七问题文档位于旧工作区 docs/hq，本轮已冻结带哈希原件；R74/R75 记录过旧示例的修正，映射不表示认可旧版示例全部准确。无序容器迭代顺序/hash 值按本轮任务书纳入；已读 adaptor 评审文件未定位对应逐字出处，明确标 `NOT_OBSERVED`，不捏造评审来源。

## 7. 自检与交付边界

头文件正向控制覆盖 vector/string、变量模板 is_same_v/已移除模式下的 is_literal_type_v、ranges::sort、concept 和 GNU pb_ds；源码扫描含直接限定名、别名、多行、注释/字符串负对照。失败尝试均保留：extract-api 漏内部头声明、一次 closure 解析崩溃、整套 sysroot LD_LIBRARY_PATH 下的绑定进程异常、初始 372/371 分母断言、Python 版本不支持 file_digest；均作为方法修正，不当成库差异。

大体积 AST 原始 stdout 无损 gzip，解压 SHA256 已核对，见 headers/AST_ARCHIVES.json；删除的只是可从 gzip 逐字节恢复的重复未压缩副本。脚本与生成配置快照、输入身份、证据清单和 SHA256 随材料提交。未改平台源码、四个补丁或配置，未推 Gerrit，未运行开发板任务。工作树原有 R115 两份修改未纳入。

本轮仍待确认的是范围粒度、条件分支/规范归属、真实语义使用数与完整跨包名单，不是等待某个差异实验结果。**请先审阅范围与缺口，再进入差异查证阶段。**

生成时间：{now.isoformat()}；自资源闸门起约 {hours:.2f} 小时（最终提交/推送以 delivery 记录为准，时限六小时）。
'''
(OUT/'SCOPE.md').write_text(report)
questions='''# 自行判断与待确认项

1. **计数粒度（已发非阻塞询问，尚未收到答复）**：主表按命名空间限定名合并重载/特化，直接公开嵌套类型单列，成员操作留索引；不是逐重载、逐模板实例计数。若需成员重载也独立成主表行，需要按现有原始声明展开，数量会变。
2. **完整性**：当前交付是 PARTIAL_SCOPE。所有宏/目标/ABI 配置分支的公共声明、每个 std 扩展的规范归属、深层嵌套/受保护接口未全部穷尽。未完整解析入口和原文条件索引均已交付，没有把缺口填成不存在。
3. **使用包数**：用户要求的真实语义使用数，本轮未取得；只能给冻结分母下的限定名/别名词法候选数。完整语义计数需要逐包有效编译配置、生成头与依赖，并解析类型别名/实例化/ADL；本轮没有为此重建 371 包。请确认下一阶段是否将这项作为单独前置，而非把词法数直接用于检测工具召回率基准。
4. **跨包优先级**：现有三项签名证据可列高关注，但不等于已获得全平台高优先级全集。INTERFACE_CANDIDATES.tsv 仅是待人工/语义筛选队列；没有默认任意头文件里的参数都是 SDK/跨包 API。中候选 ≥10 去重包族为自行选定工作阈值，待确认。
5. **371 与 372**：自行选择原 scan_status.tsv 作为冻结分母，明确排除额外的抽取失败 chromium-efl 行，并把既有 EWK 头/实验记录作为分母外补充证据，不混入 371 的计数。
6. **扩展范围**：把 GNU ext/pb_ds、debug、parallel、backward、decimal、TR/TS 入口收进候选；安装可见不等于稳定公共契约。C 全局 API、操作符、__gnu_pbds/__gnu_debug 等未被本次平台词法扫描覆盖，明确未分级，不填零。
7. **规范和旧评审出处**：无序容器顺序/hash 值来自本轮明确要求，已读 adaptor 评审原文未找到其逐字出处；需原评审文本才能补上精确归属。没有据记忆补引文。
8. **时间估算**：8–24 人时/复杂高关注类型和 10–30 分钟/候选筛选都是人为预估，不是测量或承诺；不会在本轮执行差异验证。
9. **方法选择**：使用现有 Clang22 解析两侧头并保留 GNU SDK 配置；不声称覆盖 GCC14 专有前端行为。放弃 extract-api 的覆盖不足路径，保留失败记录。所有可能差异点仅拟定验证维度。
'''
(OUT/'QUESTIONS.md').write_text(questions)
(OUT/'STATUS.md').write_text(f'''# 范围候审状态

总状态：PARTIAL_SCOPE；仅范围调查，未进入差异查证。

| 阶段 | 状态 | 产物 | 缺口 |
|---|---|---|---|
| light 资源闸门 | PASS / exit 0 | raw/001_gate.* | 无 |
| 头文件与实体索引 | 完成有限配置索引，全集未证 | HEADER_COVERAGE.tsv / SKELETON.tsv | 条件分支、规范归属、{len(h['never_fully_parsed'])} 个物理入口候选未完整解析 |
| 平台扫描 | 371/371 身份完成所选文件规则 | platform/STATUS.json | 112 个文件未作文本解析；真实语义使用数未取得 |
| 场景/优先级 | 候审 | HIGH_PRIORITY_SCENARIOS.tsv | 全部高优先级类型总数 NOT_OBSERVED |
| 旧材料对照 | 10 + 25 + 5 + 7 逐项映射 | EXISTING_MATERIAL_CROSSWALK.tsv | 原评审的顺序/hash 逐字出处未定位 |
| 自检/提交推送 | 见 SELF_CHECK.json 及交付记录 | SCOPE.md / QUESTIONS.md | 完成后停止等待审阅 |

开始：{start}；本状态生成：{now.isoformat()}；已用约 {hours:.2f} 小时。任务时限六小时。
''')
print('report written',len(rows),'rows',flush=True)
