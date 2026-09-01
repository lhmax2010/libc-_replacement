# R82：平台 Boost 边界的跨运行时实测

## 结论

状态：`PARTIAL`。

在 R22/R80 已确认的 90 条真实 Boost C++ 符号边中，63 条可从公开签名或 Boost 类型定义确认会传递标准库布局敏感状态，27 条在当前边界签名中未见标准库布局证据。选取的 filesystem、iostreams 两类边界在 x86_64 与 armv7l 用户态模拟环境中均稳定复现不兼容；额外选取的 program_options 边界在 x86_64 上也稳定复现。结果明确支持此前结论：两套运行时可以在同一进程中共存，但不能据此推导跨边界传递 C++ 语义是安全的。

本轮没有修复或处置建议。由于执行异常退出用例时环境连续出现保护性中止提示，为避免反复触发，停止了新增异常退出实验；program_options 的 armv7l 格与物理开发板实测记为 `NOT_OBSERVED`，因此不把结论提升为 `BOUNDARY_VERIFIED`。

## 1. 输入、口径与可复核材料

- 输入边：`docs/progress/R22/tables/split/candidate_edge_evidence.part*.tsv` 中 `reason_class=TRUE_CPP_ABI_COUPLING` 且 provider 为 Boost 的 90 行。
- 既有规模：13 个 consumer 源码包、75 个 `(arch, consumer ELF path)`、90 条边。
- 本轮完整边表：`tables/boost_edges_90.tsv`。
- 带 demangle、布局证据和分类的表：`tables/boost_edges_90_classified.tsv`。
- 去重符号：`tables/boost_distinct_symbols.tsv`。
- 25 次运行的逐格结果：`tables/mixed_runtime_matrix.tsv`。
- 所有命令原文、标准输出、标准错误与退出码：`raw/`。原始输出未经摘要替换；对话中仅给短统计。

75 与一度统计到的 84 的差别已经机械核对：75 的身份是 `(arch, consumer ELF path)`；若把二进制包名也计入，同一路径在 security-manager 的多个二进制包身份下重复出现，得到 84。本文沿用原始任务的 75 个 ELF 口径。

## 2. 90 条边的性质分类

| 分类 | 边数 | 含义 |
| --- | ---: | --- |
| `HIDDEN_STD_LAYOUT` | 33 | 签名显示 Boost 自有类型，但该类型内部持有 `std::string`、`std::wstring` 等标准库对象 |
| `HIDDEN_STD_LAYOUT+DIRECT_STD_SIGNATURE` | 6 | 同时具有隐藏标准库布局和公开签名中的标准库类型 |
| `DIRECT_STD_SIGNATURE` | 24 | 公开符号签名直接带标准库类型 |
| `NO_STD_LAYOUT_EVIDENCE` | 27 | 从已观察签名和相关类型定义未确认跨界标准库布局；不是安全性证明 |
| 合计 | 90 | — |

按 provider 归纳：

- boost-filesystem：30 条隐藏布局；`boost::filesystem::path` 的 `string_type` 是 `std::basic_string`，对象持有 `m_pathname`。
- boost-iostreams：3 条隐藏布局；`boost::iostreams::detail::path` 持有 `std::string` 与 `std::wstring`，mapped 参数持有该 path。
- boost-program-options：6 条隐藏布局并直接带标准库签名；相关结构持有 `std::vector`、`std::map`、`std::string`。
- boost-test：24 条直接标准库签名，另 18 条未确认布局证据。
- boost-thread：9 条未确认布局证据；当前 POSIX `once_flag` 证据表现为整数/原子状态。

按 consumer 源码包去重：

- 隐藏布局风险形态 3 包：`capi-appfw-capmgr`、`dotnet-launcher`、`security-manager`。
- 直接标准库签名 5 包：`capi-appfw-capmgr`、`cert-svc`、`key-manager`、`libwebappenc`、`security-manager`。
- 两类并集 6/13 包；其中 3 包属于符号名不显露标准库类型的静默风险形态。
- 其余 7 包在这 90 条已观察边上未确认标准库布局传递；不代表包内其他路径已获证明。

## 3. 实测对象及选择理由

### 3.1 filesystem

覆盖 30 条、数量最多，并存在生产 consumer。用例调用平台 Boost 的真实 `status(path, error_code)` 和返回 `path` 的 `read_symlink(path, error_code)`；consumer 读取具体状态、路径长度与路径值，并在自己的 libc++ 一侧销毁 provider 返回的对象。这同时覆盖输入、返回值、具体数据断言和异侧销毁。

### 3.2 iostreams

边界对象的 Boost 外壳内部含 `std::string/std::wstring`，适合作为“符号能匹配但隐藏布局不匹配”的代表。用例调用平台真实 `mapped_file_source::open_impl`，检查映射是否成功、长度和具体字节，并让 consumer 离开作用域执行销毁。

### 3.3 program_options（额外样本）

`variables_map` 持有标准容器；用例调用平台库导出的同名构造函数，由 provider 初始化对象，再由 libc++ consumer 查询、插入和销毁。x86_64 已完成；为避免连续触发执行环境的保护性中止，armv7l 未追加执行。

## 4. 构建与运行时身份

- consumer 使用 Clang 与 libc++/libc++abi 构建。
- provider 是平台已发布的 Boost RPM，动态依赖 `libstdc++.so.6`。
- x86_64 与 armv7l consumer 的动态依赖均包含 `libc++.so.1`、`libc++abi.so.1`。
- filesystem 两个目标符号在 consumer 中为 UND、在真实 provider 中为 DEF，mangled 名逐字匹配；iostreams 的 `open_impl` 也逐架构确认 UND/DEF 匹配。
- armv7l 使用真实 armv7l 二进制、armv7l sysroot 与 `/usr/bin/qemu-arm-static` 用户态执行；不是物理开发板结果。
- 构建受 `nice -n 15`、`ionice -c 3` 约束，实际串行，未超过并行度 2。

## 5. 运行结果

每个已执行格重复 5 次；逐次输出在 `raw/030_*`、`raw/034_*`、`raw/042_*`，解析表为 `tables/mixed_runtime_matrix.tsv`。

| 架构/环境 | 用例 | 5 次结果 | 数据断言 | consumer 销毁路径 |
| --- | --- | --- | --- | --- |
| x86_64 | filesystem | 5/5 返回 41 | 5/5 状态和值错误，返回长度在运行间变化 | 5/5 执行完成，未崩溃 |
| armv7l / qemu-user | filesystem | 5/5 返回 41 | 5/5 状态和值错误，返回长度稳定为 108 | 5/5 执行完成，未崩溃 |
| x86_64 | iostreams | 5/5 返回 134 | 在取得可断言数据前终止 | 未到达完成标记 |
| armv7l / qemu-user | iostreams | 5/5 返回 139 | 在取得可断言数据前终止 | 未到达完成标记 |
| x86_64 | program_options | 5/5 返回 139 | provider 构造后初始 size 为 0；在插入/读取完成前终止 | 未到达完成标记 |
| armv7l | program_options | `NOT_OBSERVED` | `NOT_OBSERVED` | `NOT_OBSERVED` |

这些结果同时展示了两类后果：filesystem 是链接、装载均成功且进程继续运行，但数据静默错误；iostreams/program_options 在访问布局不兼容状态时终止。filesystem 的异侧销毁路径虽然 10/10 返回，却不能据此证明这种销毁安全，只能说明该具体路径本轮未立即崩溃。

非零退出在此是被测现象，不是命令门禁失败。`raw/011_*` 的退出 1 是 `rg` 无匹配与 `set -e` 组合造成，已用不改变判据的脚本重跑为 `raw/012_*`；`raw/026_*` 是符号匹配表达式错误，随后以原始 mangled 名在 `raw/036_*`、`raw/038_*` 独立确认。两者均属于命令技术性错误。

## 6. 影响评估

1. 平台 Boost 边界在混合运行时下确实会出问题：已实测的三类全部出现可重复的不兼容；后果从静默错值到进程异常终止。
2. 13 个 consumer 包中，6 个从真实符号边和类型定义可确认处于标准库布局敏感形态；其中 3 个的标准库状态隐藏在 Boost 类型内部，链接器无法从边界表面阻止误配。
3. 其余 7 个包当前只能说“这 90 条边中未发现标准库布局证据”，不能外推为全面安全。
4. 结果进一步支持“可以共存，但禁止跨界传递 C++ 语义”的既有边界：此前 ICU 的 C API 成功不与本轮结果矛盾；Boost 这些真实边传递的正是 C++ 对象语义。

## 7. 未覆盖范围

- 没有逐条动态执行 90 条边；动态样本覆盖三种代表性 provider 类别。
- 27 条 `NO_STD_LAYOUT_EVIDENCE` 未逐条做运行期安全证明。
- program_options 的 armv7l 格未执行。
- armv7l 结果来自 QEMU 用户态，物理板行为 `NOT_OBSERVED`。
- aarch64 不在本任务要求的两架构范围，未执行。
- 未使用 ASan/Valgrind 等检查已完成销毁路径的潜在堆损坏或泄漏。

## 8. 自行判断与尚存疑问

自行判断：

1. 分类优先使用实际边界签名；签名是 Boost 类型时，再以对应 Boost 1.83 类型定义确认是否隐藏持有标准库对象。
2. 同一 ELF 路径被多个 RPM 身份重复承载时，按任务原有的 75 个 ELF 路径口径去重。
3. 因执行异常退出用例后连续出现环境保护提示，停止新增异常退出实验；不尝试绕过保护，不再发起远程开发板操作，保留现有证据并将状态降为 `PARTIAL`。

尚存疑问：无需要立即人工裁决的岔路。若未来继续补齐，需由人工确认允许的物理板执行窗口和不会触发环境保护的运行通道。

## 9. 纪律确认

- 未修改平台源码或任何补丁。
- 未做修复或处置建议。
- 未向 Gerrit 或外部源码仓推送。
- 原始命令、输出和退出码均落盘。
