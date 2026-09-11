# 数字核对：逐项来源、单位和判定

原文全部阿拉伯数字出现位置在 [NUMBER_OCCURRENCES.tsv](NUMBER_OCCURRENCES.tsv)。该词法索引
保留日期、版本、架构名、URL 中的 SHA／上游评审号、章节编号、代码常量；这些不能都当作
实验计数。下表按**事实主张**合并重复出现处；33 类型逐行表涵盖其全部字节数和对齐。
中文文字数词（两处、八项等）也在下面核对，不以数字正则替代人工阅读。

## API／ABI 数字

| 原文数字／位置 | 原始材料与本轮核对 | 判定及单位 |
|---|---|---|
| 日期 2026-09-09 | 两报告稿头；本次为 2026-09-11 核对，不推断其撰写过程 | 文档标注日期，不是所有实验日期 |
| x86_64、GCC 14.2、libc++ 22.1.8（ABI:5） | R80 raw/017 的 GCC 13.3.0，raw/032 的 Clang 21.1.1；R80 实际链接 T4 libc++/libc++abi。本轮 raw/038 在实际 T4 头中核到 `_LIBCPP_VERSION=220108`；GCC 源树 BASE-VER 是14.2.0，但不是布局探针编译器 | **版本口径混淆，M06**。x86_64及所用LLVM头版本22.1.8成立；不能把来源版本与编译工具混为一体 |
| 33 个类型 | R80 code/layout_probe.cpp；raw/041；本轮与 HQ 附表逐行相等 | 33 个具体类型／模板实例，不是 33 个家族／所有标准库类型 |
| 10 个不同 | 原始 sizeof/alignof 比较行重新统计 | 10 个指标不同；必定不具相同完整对象布局，但没有穷尽内部成员，M06 |
| 23 个一致 | 33−10＝23 | 仅两个指标相同，**不是“23 个布局一致”**，M06 |
| 各类型 8／16／24／32／40／48／56／80 等 | [33 行表](LAYOUT_RECHECK.tsv)与 R80 两个 raw/layout_*.tsv、raw/041 | 全部所列 size 数字一致；对齐也是独立列，不可省略后称完整布局 |
| ABI 十个差值 +16,+16,−32,−24,−24,−16,−16,−8,−8,−16 | 每行 libcxx_size − libstdcxx_size | 全部正确，单位字节；大／小不代表性能或优劣 |
| `106884723786536` | R80 raw/036 的 consumer_view；provider_view=4 | 当次原始错值确实存在；不是固定可预测长度，M07 |
| `{1,2,3,4}`／长度 4 | 原 provider `{11,22,33,44}`，实际 size=4 | 长度一致，元素被改写；不能称该改写版已跑，M07 |
| `_Z9make_datav` 的 9 | c++filt 解为 make_data()；名称 9 字符 | 编码本身正确，但原用例是 `_Z12make_payloadv`，不是其原始符号，M07 |
| API 返回 0、−1、−2 | 示例成功／错误码，不是实测结果 | 自定义协议常量，未定义完整失败输出／取消契约，M02 |
| 两条附加条件、三类不能转换、信息清单三项 | 文档枚举数能对上条目 | **枚举数正确不证明充分性或技术不可能**，M02、M08、S05 |
| 6,057 | GCC x86_64 baseline_symbols.txt 原文件；raw/028 重放 parser。4,546 FUNC＋1,511 OBJECT 键 | 数值正确；去版本名字，不是实测库纯函数数，M09 |
| 1,969 | T4 `libcxx.so.1.0_defined_exports.log` 原文件，SHA 46ef17…；raw/028 重放 | 数值正确；与 GNU 输入性质不同，M09 |
| 17 | 两输入去版本名字交集；R80 d09_common17.tsv 恰 17 行 | 名字含构造／析构变体；**并非尚未核查含义**，M10 |
| 约 50 处 GNU 识别子句 | 本轮固定 GCC 14.2 源树生产目录完整跟踪文件，无后缀头也纳入；raw/013 | **同口径未得到 50**：65 个 typed catch 句法点、16 文件。不是运行时实例数，M11 |
| 0 处 libc++ 识别子句 | 固定未补丁 LLVM 源树 libcxx/include、libcxx/src；本轮带对照检索 | 在此静态范围内成立；不能用于修补后树或扩大到整个 ABI 支持库，M11 |
| 已补 50 处防护 | 最终 R69 semantic_split_audit 原文：header typed 20、library typed 16、其中聚合 2、直接 14 | 50 是站点＝20＋(14＋16)；新增 handler＝20＋14＋2＝36，M11 |
| 五项未覆盖 | 原 ABI 第五节确有五条 | 条目数不是完整缺口分母；17 项语义“未核实”已过时，且漏其他范围，M10、M15 |
| 预计数日 | 没有随稿给出工期核算原始记录 | 估算依据 `NOT_OBSERVED`，不是实验事实，S05 |

### 十种差异类型的所有数值

| 类型实例 | GNU size | LLVM size | 差值 | GNU align | LLVM align |
|---|---:|---:|---:|---:|---:|
| string | 32 | 24 | −8 | 8 | 8 |
| deque<int> | 80 | 48 | −32 | 8 | 8 |
| map<int,int> | 48 | 24 | −24 | 8 | 8 |
| set<int> | 48 | 24 | −24 | 8 | 8 |
| unordered_map<int,int> | 56 | 40 | −16 | 8 | 8 |
| unordered_set<int> | 56 | 40 | −16 | 8 | 8 |
| function<void()> | 32 | 48 | +16 | 8 | 16 |
| any | 16 | 32 | +16 | 8 | 8 |
| future<int> | 16 | 8 | −8 | 8 | 8 |
| promise<int> | 24 | 8 | −16 | 8 | 8 |

其余 23 行连同具体模板参数、size 和 align 全在 LAYOUT_RECHECK.tsv。附带 HQ TSV 本身有
一行 SUMMARY；它不是第 34 个类型。本轮纠正检查脚本的初始计行后才形成最终数字。

## Chromium 中英文：全部数量、版本和时间主张

两稿与原稿逐字一致不是唯一依据；以下对关键数字另查原始输出／源码／提交元数据。

| 数字／主张 | 原始证据 | 结果与限定 |
|---|---|---|
| 截至 2026-09-07 | R116 抓取／运行时间记录、两原稿截止声明 | 历史证据截止，不是 9 月 11 日镜像运行结果 |
| Chromium ec7cb9…、libcxx 97b436…、本地 LLVM 5ed6c… | sources/chromium_head.txt、deps_pinned.txt、raw/053；本轮 codes/llvm HEAD；五份分仓源码重抓 SHA 一致 | 三个不同身份没有被混用。SHA 中数字不是计数 |
| ABI 版本 2、命名空间 __Cr | sources/configsite_actual.txt 的明确 define | 静态配置，正确，不是 ELF 符号版本节点数 |
| 505,965 普通文件 | raw/054_scan_full_archive.stdout | 正确；不是压缩包全部条目（原始 archive_members=552300） |
| 195,796 已扫文本＋310,156 非选后缀＋13 NUL | raw/054、code/scan_archive.py | 合计 505,965；文件单位互斥划分。未覆盖另拉 DEPS、宏展开、别名／二进制 |
| 1 个文件、2 行、1 个实际 pthread_cancel 调用 | chromium_usage.tsv；对应 thd.cc 固定源码／raw/057 | 204 行是调用，206 行是诊断字符串。不能数成两个调用点或全部依赖仅一处 |
| thd.cc:204/206、pool:162、BUILD.gn:2285 | raw/057 的源码上下文及实际抓取源文件 | 固定快照行号，正确；不是调用次数／产品触发记录 |
| std::shared_mutex 两行一文档；shared_timed_mutex 零 | chromium_usage.tsv；raw/054/057 正向控制 | 正确；两个文本提及，不是两个 C++ 实例化站点 |
| wbuffer_convert 一项、boost include 一项 | 同上；头清单与 boost.cpp | 正确；不是两个产品使用结论。boost.cpp:1 是行号 |
| 取消模式两词零命中 | raw/054、057 的每词合成正向控制；原扫描代码 | 选定文本范围的零命中成立，不扩成产品无异步取消 |
| HTTP 403（Starfish 历史材料） | raw/048_lwe_remote.stderr | 当时访问失败，正确；不是推断仓库不存在或作者身份 |
| glibc 2.40 源码／宿主 2.39 | snapshots/glibc-unwind-link.c；raw/051_host_environment.stdout | 两者原稿明确区分；不能把宿主探针说成 Tizen 镜像 |
| glibc 六个 x86 入口 | 原源码快照＋raw/041 导出检查 | 六个名称子集都有；不是跨架构全 GNU ABI |
| 2017 公告、C++14 | 原公告快照与 REFERENCES.md 索引 | 浏览器迁移动机／标准模式，不是整个 ChromeOS 重编证明 |
| 2021 提案、2021-10-27 NaCl、2021-11 链接／Breakpad、2021-12 全板／SDK／依赖 | raw/052_cros_changes.stdout 对应提交 metadata、状态、作者测试字段 | 原稿时间范围与合入状态一致；外部作者报告非本组复现 |
| 2022-02 llvm-libgcc 上游、2023-07 glibc bootstrap | R116 sources 的上游评审快照、raw/052 | 时间线正确；不是 2022 才开始 ChromeOS 下游或全面回退 |
| 2021 ARM 修复、2023 shared_mutex 回到 base::Lock | 所引 LLVM 评审／维护者讨论快照 | 历史范围正确；不证实全产品取消验收或性能普遍改善 |
| arm32、64-bit、armv7a／armv7l／aarch64 | 提交描述、raw/034 宏及 raw/053 ELF | 架构／位宽标识，不能相互充当运行验证；原稿有相应限定 |
| 提供方物理文件 libunwind.so.1.0／SONAME .so.1；libgcc_s.so.1 | raw/023_experimental_identity.stdout | 文件身份，不是三个独立展开器 |
| 198／231／108 个符号版本组合 | 本轮从 raw/036、037 重新解析，与 symbol_comparison.tsv 同口径 | 正确；198 含 15 个 ABS 版本节点，108 不是缺函数／失败二进制数（S02） |
| 八项适用性、七类问题、四项处理、11 项问题库 | 两稿各表逐行及 REFERENCE_ISSUES.md 的 11 小节 | 编排条目数对应，不是所有历史问题或待修补数 |
| GNU 八处独有／无同形防护 | R46/tables/no_counterpart_8.tsv 逐条 | 4 executor＋2 vstring＋1 cv-any relock＋1 TR2＝8。是历史源码映射，不属于我方 50 站点子集 |
| 两处防护不生效 | R58/KNOWN_GAPS.md 指 shrink_to_fit、ostream sentry 析构的不抛边界 | 不是 R67/R69 已修正的另外两项代码缺陷；名称未展开易误解（S03） |
| 两个异步组件 | R108/W3 的 lightweight-web-engine、OpenMP 条件源码和原生独立探针 | 两组件条件路径，不是每个产品都执行；本轮未运行组件本体 |
| 五个不重建预编译 DSO | R66 vendor_rebuildability.tsv／R108 W2 的 pen-wave 五个架构产物清单 | 五个产物，不是五个源码包／五家外部供应商；原稿用 DSO 正确 |
| 30 格、每格五次、150 次 | measurements.tsv 150 行、measurement_summary.tsv 30 行；本轮 raw/028 逐原始日志复核 | 2 提供方×15 个组合×5＝150；GNU／LLVM 是合计两列，不是 30×2×5 |
| 普通及三个时钟变体／四种等待 | 2 规格×4 形态＝每提供方8格；probe 源码与 raw/runs | 正确，内联两路径配套头重编；不是每个库所有等待 API |
| 普通／定时写者两处 | 2 路径×2 版本＝每提供方4格 | 正确；每个模板参数未穷尽 |
| 5/5、清理 0／1、状态 0、整数 42、返回86、SIGABRT | 150 份 stdout/stderr/exitcode 与探针源码逐条复核（RAW_MEASUREMENT_RECHECK.tsv） | 正确；40 个86是自定义 handler，10 个-6是 SIGABRT，100 个0含损坏对照 |
| 2147483648 写者位 | shared_probe.cpp 的位掩码与状态输出；raw 中阻塞状态2147483649为写者位＋原读者1，随后损坏标记writer_bit=1 | 对象状态数值，不是退出码、读者总数或内存地址 |
| 栈帧至少3、实际整数42／清理1 | api_probe.cpp 和所有 api 格输出，raw/028另断言主／插件栈帧均≥3 | 最低阈值满足，不是所有优化栈、信号栈、JIT 都通过 |
| 13 个 URL | 只对新增“为什么不采用”节去重；两稿集合一致 | 正确：该节13／13 HTTP200；全文36，31成功＋5错误；不能说全文只有13 |
| 五处边界／四个补丁／两个写者 | 固定任务对象的已命名清单与 R69、R111、R114、R115 原始源码矩阵 | 语义对象／补丁／算法路径三个不同单位，不是某一个扫描数 |

源码地址中的评审号、SHA、C++／架构名及章节序号在出现位置索引中保留，**不赋予实验数量
意义**。未发现其他未在上述事实组、代码常量、版本／行号或编排编号分类内的数值主张。

## 独立重算的证据强度

本轮重新读原始输出和输入重算，不是重跑实验。代码示例只做语法检查；HTTP 检查只是当前
访问性。五个源码链接另抓 TEXT／解码，比对旧快照 SHA，才确认了替换地址内容身份。
“未观察 Engine 实验”依据指定证据入口及其中探针源码，不能由有限查找推断全历史无此实验。
