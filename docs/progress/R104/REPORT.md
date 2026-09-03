# R104 最终交接报告

结论：`PARTIAL`

11 个包的代码身份、分支创建与改法设计已完成。经人工确认进入实施后，5 个
独立包完成三架构 × libc++/GCC 六格验证并已推送；3 个独立包在验证中暴露
决定性问题，均未提交、未推送；LLVM/MLGO 链及依赖它的两个包按要求一起
暂停。没有推送正式分支，没有 force，没有修改平台源码。

## 1. tensorflow2 分支和范围登记

- Base Toolchain 基线：`1cdba73549f741720f11dd53da1bb516b03763a2`；已在该
  revision 上创建既有目标 `sandbox/lhmax2025/libcxx-migration`，创建时只建
  分支、无提交。
- Unified Toolchain 的 `accepted/tizen_unified_toolchain` 为
  `ea3f134d35fc64667c2eebd72992bbb3c8e0069d`。R103 已证明
  `inference-engine-tflite`、`nnstreamer`、`nntrainer` 的现存消费产物实际
  使用这一版本的静态归档。
- Unified revision 未在本轮处理，是因为本轮配置决策与分支实施范围明确为
  `Tizen-Base-Toolchain`，不是遗漏；应在 Unified 包集进入迁移实施时基于
  `ea3f134d…` 或届时 manifest 的新钉住 revision 单独建分支、复核和验证。

创建前后分支全量记录、仓库身份和最近三条提交分别见
`TENSORFLOW_BRANCH_CHECK.md`、`TENSORFLOW_BRANCH_RESULT.md`、
`GIT_INVENTORY.md` 与 `branch_ops/`。

## 2. 改法设计

共同启用判据严格限定为：`_toolchain` 已定义且 `toolchain_is clang`。这样：

- Base Toolchain 默认选择 Clang 时启用 libc++；
- `_toolchain_override gcc` 时不注入 GCC 明确拒绝的 `-stdlib=libc++`；
- 不定义 `_toolchain` 的非迁移配置保持旧行为。

具体到 CMake、autotools、Boost.Build、Meson 和 TensorFlow 构建路径的逐包设计
见 `IMPLEMENTATION_DESIGN.md`。`llvm` 只启用其既有
`LLVM_ENABLE_LIBCXX=ON`，没有设置 `CLANG_DEFAULT_CXX_STDLIB`，也没有改
driver 源码；设计目标是改变宿主 LLVM 程序自身运行时，不改变新 clang 的
默认标准库。由于 MLGO 阻断发生在完成新 clang 的 `-###` 验证之前，该项仍为
`NOT_OBSERVED`，不能按设计推断为已验证。

## 3. 逐包实施结果

完整机器可读状态见 `PACKAGE_RESULTS.tsv`，详细过程见
`STAGE4_INDEPENDENT_IMPLEMENTATION.md`。

### 3.1 已完成并推送的五包

| 包 | 提交 SHA | 三架构 libc++ | 三架构 GCC | 远端 |
| --- | --- | --- | --- | --- |
| `abseil-cpp` | `7a4f04dad6d30ba09c406837cdd23ad779a69210` | PASS | PASS | 已核对一致 |
| `icu` | `dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330` | PASS | PASS | 已核对一致 |
| `jsoncpp` | `9cbab147f44097d5b8da2b3ef38f75376a282d01` | PASS | PASS | 已核对一致 |
| `pcre` | `09a5bfc9a96c4235628ce800db3b95bf28ef2f23` | PASS | PASS | 已核对一致 |
| `taglib` | `8b552bdf27c913a93bca664c41c3fafb214962a1` | PASS | PASS | 已核对一致 |

每个 PASS 都是完整 GBS 构建。libc++ 格实际使用 Clang 22.1.8，最终 ELF 至少
一项依赖 `libc++.so.1`、零项依赖 `libstdc++.so.6`，并通过五处补丁头文件
检查；GCC 格实际使用 GCC 14.2.0，日志没有 libc++ 参数，最终 ELF 依赖
`libstdc++.so.6`、零项依赖 `libc++.so.1`。提交前均确认只改对应 spec；推送
后目标 SHA 与本地一致，其他既有远端分支不变。

### 3.2 `libsigc++`：六格构建完成，严格判据失败

三架构 libc++ 格全部完整构建成功，实际使用 Clang 22.1.8，编译/链接命令
包含 `-stdlib=libc++` 与 `-lc++abi`；三架构 GCC 格也全部完整构建成功，
实际使用 GCC 14.2.0、无 libc++ 参数，最终库依赖 `libstdc++.so.6`。

但 libc++ 三格的唯一 ELF 均没有 `libc++.so.1` `DT_NEEDED`。其 C++ 对象没有
留下需要 libc++ 动态定义的引用，链接器在 `--as-needed` 下移除了该依赖。
这不能满足已确认的“产物依赖 libc++.so.1”判据，故未提交、未推送。不能为
制造依赖而擅自加 `--no-as-needed -lc++`；是否允许以真实 consumer 的模板
实例化和链接证据代替该判据，需要人工裁决。

### 3.3 `boost`：已确认的 Boost.Build 改法未覆盖最终产物

armv7l/libc++ 的首次 `b2` 收到了 libc++ 的 `cxxflags/linkflags`，并使用 Clang
22；但 `%install` 再次执行不带这两组参数的 `b2 install`，重新构建进入 RPM
的最终库。结果 39 个 ELF 中 36 个仍依赖 `libstdc++.so.6`，零个依赖
`libc++.so.1`。armv7l/GCC 对照通过。

失败机制已确定且不依赖架构，故为控制资源终止了已启动的 aarch64/libc++
格，x86_64 未运行。该包未提交、未推送，也未擅自在第二次 `b2 install`
追加参数。下一步需要先补充并确认 Boost.Build 的改法，再从六格重跑。

### 3.4 `tensorflow2`：需要源码兼容处理，超出本轮授权

armv7l/libc++ 完整构建实际使用 Clang 和 `-stdlib=libc++`。在 4,230 个 Ninja
步骤的第 1,504 项，`tensorflow/lite/kernels/elementwise.cc` 的
`std::abs<float>` / `std::abs<int32_t>` 无法匹配 `std::function<T(T)>`，报：

```text
error: no matching function for call to 'EvalImpl'
note: candidate function template not viable: no overload of 'abs' matching
      'std::function<float (float)>' for 3rd argument
```

这不是 flags 未生效，而是迁到 libc++ 后暴露的源码兼容问题。当前授权只允许
spec 与必要构建配置，不允许改源码，故立即停止；其余五格、静态归档成员和
真实 consumer 重链接均为 `NOT_OBSERVED`，未提交、未推送。

另外，现有 spec 在本任务三个架构上把 `USE_CLANG` 固定为 ON，并在 CMake 中
写死 `clang/clang++`。因此 `_toolchain_override gcc` 只会关闭 libc++ 注入，
不会让包实际由 GCC 编译。这与“每包必须在 GCC 下实际跑”的字面要求存在
冲突，即使先修复上述源码错误也需要人工明确验证口径。

### 3.5 LLVM、bcc-tools、bpftrace

三包均未推送实施提交。LLVM 的本地 spec 改动保留为未提交证据；`bcc-tools`
与 `bpftrace` 没有继续修改。按人工确认，三者必须协调生效，因为已证明：

- `bcc-tools -> llvm`：584 个布局敏感 C++ 符号；
- `bpftrace -> llvm`：201 个；
- LLVM 包内部：17,589 个布局敏感符号。

## 4. MLGO/XLA 资产可行性事实

完整证据见 `MLGO_ASSET_ASSESSMENT.md`，本节只给裁决所需摘要。

### 4.1 资产、来源与生成方式

`llvm.spec` 的三份 `mlgo_{arm,aarch,x86}_model.tar.gz` 各含 12,782 个条目：
MLGO 模型 `.h/.o`、5 个 TensorFlow/XLA runtime `.cc.o`、sysroot 和预编译
archive。spec 在 `%prep` 解包，再把这些对象直接嵌入 LLVM；它们是源码包随附
的预编译输入，不是当前 LLVM 构建生成。

独立标记显示 runtime 对象由 Clang 21.1.1 编译，头文件属于 TensorFlow
2.15.1。上游技术路径是 `tf_compile`/`saved_model_cli aot_compile_cpu`，但
现有仓库和归档没有记录真实生成者、完整命令、输入 revision 或 CI 任务。
因此“谁生成”与“内部还是第三方流程”均为 `NOT_AVAILABLE`。

### 4.2 是否能重生成

当前缺少 SavedModel、训练数据/导出物、训练/导出脚本、准确 TensorFlow
2.15.1 源码与补丁、构建 flags 和 Clang 21.1.1 构建根，不能可靠重生成。
若补齐这些输入，技术路径存在；模型与 runtime 对象含目标机器码，需要三架构
分别生成。训练时间、算力和总成本因输入缺失记 `NOT_AVAILABLE`。

### 4.3 C 接口路径

当前接口直接使用生成模型类、`ReleaseModeModelRunner`、
`XlaCompiledCpuFunction` 以及含 `std::string` 的方法，是 C++ ABI。可以另行
设计旧 ABI 桥，通过 opaque handle、标量/明确缓冲区暴露纯 C API，但需要处理
对象所有权、异常、错误传递、双运行时和性能；现有代码没有该桥。这是可设计
但代价显著的新实现，不是现有可选开关。按要求，禁用 MLGO 没有列为方向。

### 4.4 LLVM 三包暂留 libstdc++ 的后果

三者一起保留 libstdc++，上述 17,589/584/201 个内部边仍处在同一 ABI，不会
因只迁 LLVM 而立即断裂。但已经观测到 Unified 的 `rpi4-linux-kernel` 所产
`perf`/`trace` 与 `libLLVM.so.22.1` 之间有 4 条 ELF 边、9 个去重 C++ 符号；
它们有 C++ ABI 耦合，现有证据不能证明或排除布局传递。因此该状态可作为避免
内部已知断裂的阶段性事实形态，但**没有证据支持其长期安全**，也不能把其余
14 个仅有 BuildRequires 的 consumer 推断为同进程风险或安全。

## 5. 内联命名空间阻断与同类资产

libc++ 把标准库实体编码为 `std::__1`，libstdc++ 新 ABI 编码为
`std::__cxx11`。当字符串等类型进入参数、返回值或成员函数签名时，Itanium
mangled name 不同。LLVM 的预编译 XLA 对象提供 `std::__cxx11` 定义，迁移后
LLVM 请求 `std::__1` 定义，链接器明确报未定义符号。这正是总部报告 4.2/
6.2 所述类别的实际命中，不是“同名返回类型不编码”那种静默错读。

已有证据的其他候选包括：`bpftrace` 的
`prebuilt-static-bpftrace.tar.gz`、TensorFlow 的 AOT/静态归档路径，以及 R13
登记的若干 vendor-prebuilt 载荷。对后者目前只证明了展开器风险或存在 C++，
不能直接宣称存在标准库布局边。

系统性方法是：对全量 SRPM 的所有 Source 按文件 magic 解包，枚举 ET_REL、
ET_DYN 和 ar 成员（不依赖扩展名），记录 `.comment`/DWARF producer 和目标
架构，分类 `std::__1`/`std::__cxx11`/`GLIBCXX_*`，再从 spec 与真实链接命令
确认是否进入产物，最后做资产 DEF 与迁移后 consumer UND 的精确交集并按
C/C++/布局敏感分类。本轮只完成已有语料的低成本 spec 指示扫描，未完成全平台
Source magic 全扫，故候选全集为 `NOT_OBSERVED`。

## 6. 分支和文件审计

- 五个已通过包的 sandbox SHA 与本地逐项一致；其余六仓的 sandbox 仍停在
  建分支时的基线。
- 11 仓创建前已存在的远端分支逐项不变；`tensorflow2` 因快照目录布局不同，
  初次通用核对脚本失败，随后用明确路径重新核对通过。
- 本地未提交改动只有四个 spec：`boost.spec`、`libsigc++.spec`、
  `llvm.spec`、`tensorflow2.spec`。它们分别作为失败/暂停证据保留；源码零改动。
- `bcc-tools`、`bpftrace` 工作树干净；五个已推包工作树干净。
- 未推正式分支、未向 LLVM 上游提交、未使用 force。

## 7. 未完成项、人工裁决点与自行判断

### 需要人工裁决

1. `libsigc++`：是否允许用真实 consumer 的编译/链接和命名空间符号证据代替
   “库本身必须有 libc++.so.1 DT_NEEDED”；若不允许，需要明确期望的技术做法。
2. `boost`：是否批准把相同 libc++ flags 同步传给 `%install` 的第二次
   `b2 install`，然后六格重跑。
3. `tensorflow2`：是否另开源码兼容修正，并明确固定 Clang 包的 GCC 验证口径。
4. LLVM：由资产提供方补齐重生成输入、选择重生或 C 桥方向；本报告不代选。

### 自行判断

- 在 Boost 的 armv7l 失败机制确定后停止其他架构，避免为一个已不能过门禁的
  方案继续消耗资源；中止和恢复状态完整落盘。
- 对 libsigc++ 不把“编译命令带 libc++”等同于“产物依赖 libc++.so.1”，按
  人工给定的严格判据保持失败。
- 对 TensorFlow 首个源码兼容错误按授权边界停止，没有修改源码或绕过错误。
- 把 MLGO 模型重训练、模型 AOT 编译、XLA runtime 重编译分开评估，避免用
  “有生成工具”掩盖关键输入缺失。

## 8. 材料索引

- `GIT_INVENTORY.md`：11 仓身份、HEAD、最近提交与漂移；
- `TEN_BRANCH_CREATION.md`、`TENSORFLOW_BRANCH_RESULT.md`：分支创建；
- `IMPLEMENTATION_DESIGN.md`：11 包改法设计；
- `STAGE3_LLVM_BLOCKER.md`：LLVM 实际失败；
- `MLGO_ASSET_ASSESSMENT.md`：资产调查与边界分析；
- `STAGE4_INDEPENDENT_IMPLEMENTATION.md`：八包实施细节；
- `PACKAGE_RESULTS.tsv`：逐包机器可读结果；
- `RECOVERED_TECHNICAL_ERRORS.md`：技术性命令错误与恢复；
- `raw/`：命令原文、stdout、stderr、退出码；
- `code/`：本轮日志包装、验证与核对脚本。
