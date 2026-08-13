# R33 三个候选方案的决定性未知摸底报告

日期：2026-08-13  
执行边界：S1 仅修改 `tmp/R33/llvm-exp/` 实验副本；S2 用例仅在 `progress/R33/src/`；S4 全程只读。`codes/llvm`、平台包源码、冻结制品均未修改。所有运行均为 x86_64 宿主 CPU 原生执行、目标用户态 loader 隔离，未使用 QEMU。

## 结论摘要

| 候选 | 决定性未知答案 | 实测事实 |
| --- | --- | --- |
| S1：libc++abi 补类型 | **成立（仅针对“personality 是否有足够信息”）** | `actions & _UA_FORCE_UNWIND` 足以触发合成 typeinfo；libc++ 侧正例确实进入 `forced-handler`。但仅补类型和匹配逻辑后，`throw;` 仍因 libc++abi 对外来异常调用 `_Unwind_RaiseException` 而 134；取消与 `pthread_exit` 两格均未与 libstdc++ 对齐。因此本次最小实现整体未通过行为一致性断言。 |
| S2：null 辅助检测 | **不成立** | Itanium ABI 没有承诺“外来异常必返 null”；libc++abi 是显式实现策略，libstdc++ 是布局相关结果。更关键的是，本次成功构造并运行了非强制外来异常，两侧均得到 type `NULL`、`exception_ptr=EMPTY`，与强制展开的检测结果相同，差集实测非空。 |
| S4：逐包定制 B/C/D | **不可判定为可用；现有证据不能成立** | 50/50 处均无法只靠上下文证明 B 的异常集合完备、C 的取消可禁用，或 D 不改变真实异常语义；0 处确认可用 B/C/D。owner/运行策略资料不可得，不作猜测。 |

这里把 S1 的两个层次分开：决定性未知“有没有足够信息映射类型”答案是成立；“题定的最小改动是否已成为完整可用实现”答案是否。报告不据此选择方案。

## 一、输入、资源与构建边界

输入及 SHA256 见 `tables/input_identity.tsv`。S4 的 50 个位置严格取自 R32 `impact_category_locations.tsv` 中 `feature=forced_unwind_confirmed_rethrow` 的 50 行；分母为 50 个防护位置，涉及 10 个源码 RPM 身份、8 个去重包名。

开工前主机为 20 CPU；各次构建前 available memory 均为 20.2–20.7 GiB，1 分钟负载为 1.36–3.24，低于阈值 10。有效 libc++abi 构建明确使用：

```text
CMAKE_BUILD_PARALLEL_LEVEL=4
LLVM_PARALLEL_LINK_JOBS=1
nice -n 15 ionice -c 3 cmake --build tmp/R33/build-s1-r3 --target cxxabi -- -j4
```

只配置 `LLVM_ENABLE_RUNTIMES='libcxx;libcxxabi'`，不设置 `LLVM_ENABLE_PROJECTS`；实际唯一链接目标是 `libc++abi.so`，libc++ 部分只生成构建所需头文件。构建 6 秒完成，监控在开始、结束采样；因不足 5 分钟没有周期中间点。资源原文在 `commands/001`、`005`、`007`、`010`、`013`、`016`、`018`、`021`、`023`、`026`、`029` 及 `results/*resource_monitor.log`。

全部构建结束后的 `commands/037_resource_final.log` 发现 load1=13.74，高于阈值 10，记 `RED_STOP_RESOURCE`；该时点以后未启动任何构建。available memory 仍为 20.659 GiB。失败尝试均保留，不纳入有效结果：首次配置缺目标 linker 运行库、首次 libc++abi-only 配置缺 libc++ 头、首次 probe 缺 LLVM_22 version script、一次对象数断言错误、S2 首次显式 `-lunwind` 与当前 libgcc_s 方案不符。

## 二、S1 机制差异

### 2.1 libc++abi 外来异常路径

证据原文集中在 `commands/035_source_and_upstream_evidence_r2.log`：

- `libcxxabi/src/cxa_personality.cpp:1043-1045` 只依据 exception class 判定 `native_exception`；`scan_eh_tab` 同时收到 `actions` 和原始 `_Unwind_Exception*`。
- `:806-818` 的 catch-all 对 native/foreign 都直接匹配；`:822-846` 的 typed catch 仅在 `native_exception` 为真时调用 `can_catch`。因此现状是“foreign 仅 catch-all，typed catch 永不匹配”。
- `_UA_FORCE_UNWIND` 位是显式输入，故缺口不是底层信息不可得，而是 libc++abi 没有公开类型 token，也没有把 force 位映射到这个 token 的逻辑。

### 2.2 libstdc++ 对照

`codes/gcc/libstdc++-v3/libsupc++/eh_personality.cc:563-573` 明确把 force 映射到 `typeid(abi::__forced_unwind)`，把其他 foreign 映射到 `typeid(abi::__foreign_exception)`，之后走常规类型匹配。类型定义在 `cxxabi_forced.h:48-54`；它是抽象占位类型，防止按值捕获。

还有一个与题定最小改动分开的机制差异：

- libc++abi `cxa_exception.cpp:627-662` 的 foreign `__cxa_rethrow` 清空 caught stack 后调用 `_Unwind_RaiseException`；若返回则 terminate。
- libstdc++ `eh_throw.cc:101-136` 对 foreign 清空 caught stack后调用 `_Unwind_Resume_or_Rethrow`（非 `_LIBUNWIND_STD_ABI` 配置）。

本次行为差异恰落在这里：类型 handler 已命中，但 libc++abi 的 `throw;` 没有继续 glibc 正在驱动的 forced unwind。

## 三、S1 最小实现、构建与矩阵

### 3.1 实验改动

实验副本由 `codes/llvm@5ed6c77278dfa7a470667cf1a137723d3c96fe60` 的 `libcxx/`、`libcxxabi/`、`runtimes/`、`cmake/` 和 `llvm/cmake/` 只读复制而来；改前目录 diff 为零。改动仅：

1. `libcxxabi/include/cxxabi.h` 新增空类 `__cxxabiv1::__forced_unwind`；
2. `libcxxabi/src/cxa_personality.cpp` 的 `scan_eh_tab` 新增 17 行：仅当 `!native_exception && (actions & _UA_FORCE_UNWIND)` 时以该 typeinfo 调用 catch type 的 `can_catch`。

完整 diff 在 `patches/` 和 `commands/004`。实验库因引用 typeinfo 新增两个弱动态符号：`_ZTI...__forced_unwind@@LLVM_22` 与 `_ZTS...__forced_unwind@@LLVM_22`；现有 `__gxx_personality_v0` 的行为仅在 foreign+force+typed-handler 路径改变。非 force 分支条件上不可达。ABI 答案是：没有删除或改签任何既有符号，但新增了两个导出 typeinfo/name，属于 ABI 表面的加法；同时改变了既有 personality 的一条行为路径。详见 `tables/s1_change_scope.tsv`。

平台 map 的 LLVM_22 版本节点仅用于让实验库能与冻结 libc++ 链接；对象文件复用成功构建的同一 18 个 libc++abi 对象，只重新执行一次链接，没有重复编译。

### 3.2 行为矩阵

| 标准库 | 格 | 关键输出 | 退出码 | 判定 |
| --- | --- | --- | --- | --- |
| libc++ | pthread_cancel 正例 | `cleanup:cancel`、`cancel:forced-handler`、随后 `terminating due to uncaught foreign exception` | 134 | 类型映射命中；重抛失败 |
| libstdc++ | pthread_cancel 对照 | `cancel:forced-handler`、`join=PTHREAD_CANCELED` | 0 | 通过 |
| libc++ | runtime_error 反例 | `real:runtime_error:r33-real` | 0 | 未误入 forced handler |
| libstdc++ | runtime_error 对照 | 同上 | 0 | 通过 |
| libc++ | pthread_exit 反例 | `cleanup:exit`、`exit:forced-handler`、随后 terminate | 134 | forced handler 命中；重抛失败 |
| libstdc++ | pthread_exit 对照 | `exit:forced-handler`、`exit:join=0x2a` | 0 | 通过 |

有效机器表为 `tables/s1_matrix_r3.tsv`，逐格原文在 `results/s1_matrix_r3/`。旧 `tables/s1_matrix.tsv` 是首次 probe 未生成时的无效尝试，仅作失败留痕。

结论：libc++ 侧与 libstdc++ 侧不是逐格一致（4/6 共同为 0，2/6 不一致）。本次不扩展实验改动去修改 `__cxa_rethrow`，因为任务指定的是补类型及 personality 映射的最小版本，且不得自行发展完整修法。

### 3.3 上游调研

- LLVM issue #40741 “libc++abi should report forced unwinding on thread termination” 已关闭；2022 年 closure comment 称 libc++abi12 已修复其 `std::future` 示例，但 issue Development 无关联 PR。它未提供本任务的类型 token/mapping 实现。
- LLVM issue #53849 仍 open，是 armv7 forced-unwind 测试的类型声明编译问题，不是本任务的映射实现。
- GitHub API 搜索 `forced unwind` 得 6 项、`forced_unwind` 得 2 项、`pthread_cancel libcxxabi` 得 0 项；完整 JSON、状态和 URL 在 `commands/035`，#40741 评论/事件在 `commands/036`。
- 当前 llvm-project 树没有 `__forced_unwind` 类型实现。公开搜索找到 GCC 2007 年 libstdc++ 方案及携带该 GNU 实现的 Android 旧 prebuilt，没有找到其他发行版对 libc++abi 做同类修改的记录，记 `NOT_FOUND`，不外推为绝对不存在。

## 四、S2 契约与非强制 foreign 差集

### 4.1 契约性

Itanium C++ ABI 对 `__cxa_current_exception_type()` 的文字只规定返回“first caught exception”的类型，或没有 caught exception 时为 null，并称该接口 optional；没有规定 foreign exception 必须为 null。相反，规范的 foreign-exception 示例允许运行时将其伪装为 `__foreign_exception` 及子类。因此 `foreign => null` 不是跨实现契约。

- libc++abi `cxa_exception.cpp:599-612` 写有明确注释并显式检查 exception class，foreign 返回 null：这是明确的 libc++abi 实现策略。
- libstdc++ `eh_type.cc:33-51` 没有等价 foreign 判断，而是读取其 caught header 的 `exceptionType`。本次对象零初始化使其返回 null；这是实测结果，不是该函数源码提供的 foreign 契约。
- `std::current_exception()` 的 foreign 空值同样没有被上述 ABI 文本承诺为强制展开专用标识。

因此 S2(a) 结论为 `IMPLEMENTATION_SPECIFIC_NOT_CONTRACT`，依赖它存在静默失效条件。

### 4.2 差集受控实验

`src/s2_foreign_probe.cpp` 分配自定义 `_Unwind_Exception`，exception class 为 `R33XTEST`，以 `_Unwind_RaiseException` 发起普通两阶段展开；没有调用 `_Unwind_ForcedUnwind`，因此它是实测的“外来但非强制”实例。native 对照为 `std::runtime_error`。

| 标准库 | 实例 | current type | current_exception | 退出码 |
| --- | --- | --- | --- | --- |
| libc++ | non-forced foreign | NULL | EMPTY | 0 |
| libstdc++ | non-forced foreign | NULL | EMPTY | 0 |
| libc++ | native runtime_error | `St13runtime_error` | NONEMPTY | 0 |
| libstdc++ | native runtime_error | `St13runtime_error` | NONEMPTY | 0 |

两侧 4/4 行为一致，但这恰好证明 null/empty 谓词会把非强制 foreign 误判成 force。S2(b) 结论为 `DIFFERENCE_SET_NONEMPTY_CONFIRMED`，不是平台语言生态推断。该实例已实际运行，所以无需用 Rust/Java 是否存在来代替验证。

## 五、S4 的 50 处上下文分类

每处前后各 30 行完整原文在 `results/s4_contexts_30_lines.txt`，逐处表在 `tables/s4_classification.tsv`。判据严格为：

- B 只有能从上下文证明 catch-all 的业务异常集合有限且列举完备才记成立；
- C 只有能证明取消可推迟或不需要才记成立；
- D 只有能证明 catch-all 后没有依赖“异常被吞掉/存储/翻译/映射”的逻辑才记成立；
- 无证据即 `NOT_ESTABLISHED`，不把可能性当结论。

### 5.1 位置结果

| 指标 | 数量 | 分母/口径 |
| --- | --- | --- |
| B 已确认可用 | 0 | 50 防护位置 |
| C 已确认可用 | 0 | 50 防护位置 |
| D 已确认可用 | 0 | 50 防护位置 |
| 无 B/C/D 可确认 | 50 | 50 防护位置 |

上下文事实按组如下：

- Boost coroutine/fiber 22 处：catch-all 存储任意 `current_exception`、设置 future 异常，或对非预期异常 terminate；B/D 会改变这类现有语义，C 的线程取消策略无 owner 证据。
- GCC 两版共 6 处：2 处（版本重复后）为测试对 catch-all 的显式验证，4 处为 libstdc++ 实现刻意吞掉任意异常；不是可由上下文确认的 B/C/D。
- key-manager 9 处：生产位置将 unknown 映射为日志、callback 或错误返回；1 处测试没有后续 catch-all，属于 B/C/D 之外的测试改写问题。
- libcynara-commons 1 处：catch-all 映射为 `CYNARA_API_UNKNOWN_ERROR`。
- LLVM 内嵌 pybind11 6 处、TensorFlow 两版本内嵌 pybind11 共 4 处：catch-all 驱动任意注册 exception translator。
- security-manager 与 webauthn 各 1 处：多组 typed handlers 后仍保留 unknown fallback，不能证明异常集合已经有限完备。

“0 可确认”不表示经 owner 决策后永远不能改；它表示本任务只读证据不足以证明任一 B/C/D 在不改变语义的条件下可用。由此 S4 决定性未知不能闭合为成立。

### 5.2 工作量特征（不估时）

50 处落在 10 个源码 RPM 身份、8 个去重包名；PHASE1 30 处、PHASE2 20 处。输入没有 owner 团队字段，逐包 owner 全记 `NOT_AVAILABLE_NO_OWNER_METADATA_IN_INPUT`，见 `tables/s4_packages.tsv`。

就改动性质：无一处可由证据确认为单行 B/C/D；47/50 处生产/库路径至少需要异常语义、取消策略确认或 runtime support，表中记 `MULTILINE_SEMANTIC_CHANGE_OR_RUNTIME_SUPPORT`；3/50 处（两版 GCC forced test 与 key-manager 单测）记 `TEST_ONLY_REWRITE_OUTSIDE_BCD`。这只是代码形态，不是人日、难度或方案评价。

## 六、证据边界

- S1/S2 只覆盖 x86_64 Itanium、glibc/libgcc_s 当前组合；armv7l EHABI 明确留待方案定案后。
- S1 没有修改 rethrow 实现，也没有验证 LLVM libunwind 组合；观察不能外推到该组合。
- S2 自定义 non-force foreign 证明差集非空，但不枚举平台所有外来异常源。
- S4 没有调用图、owner 设计约束、动态取消测试或业务异常全集；因此对不可证明项保持 `NOT_ESTABLISHED`。
- 本报告不推荐、不评价、不选定方案。
