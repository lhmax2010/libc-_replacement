# R100：Tizen-Base-Toolchain 的 libc++ 迁移判定清单

## 结论摘要

本轮没有修改配置、平台源码或补丁，也没有推送 Gerrit。对准确的
`Tizen-Base-Toolchain` 快照重新生成分母后，共识别 73 个 C++ 候选源码包：

| 判定 | 数量 | 含义 |
|---|---:|---|
| `NEED_LIBCXX` | 10 | 已有符号证据证明与将迁移的代码跨界传递 C++ ABI |
| `NO_LIBCXX_NEEDED` | 55 | 已有正面隔离证据：纯 C 边、Base 内闭包、独立工具或仅头文件载体 |
| `CANNOT_SWITCH_CURRENTLY` | 6 | 当前 spec/产品角色明确使用 GCC；这是当前配置状态，不是永久技术结论 |
| `CONFLICT_NEED_BUT_CANNOT` | 0 | 没有包同时命中“需要”和“当前不能” |
| `UNDETERMINED` | 2 | 有 C++ 静态归档及 Unified BuildRequires，但不能证明是否真实静态链接 |

因此现有事实是：按包方案有 10 个已证实需要改的包，另有 2 个尚不能定性；
全局方案会同时触及 55 个已判定不需要改的 C++ 包，并暴露 6 个当前 GCC 包。
本报告不在两种方案之间作推荐或优先级排序。

## 1. 判定规则

人工确认后的规则见 `STAGE1_RULES.md`。按确认意见删除了原规则 N4
“广泛链接”：只要已证明 C++ ABI 耦合，N2 已经给出 `NEED_LIBCXX`，N4
不会产生独立判定。consumer 数量仍作为影响面数据保留。

核心边界仍是“是否跨界传递 C++ 语义”，而不是“进程中是否同时装载两套
运行时”。没有证据的项保留为 `UNDETERMINED`，不并入“不需要改”。

## 2. 分母重新生成

### 2.1 输入身份

- Base：`Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647`
- 配套 Unified：`Tizen/Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247`
- Base 元数据：292 条 source 记录、254 个去重源码名、1,705 条 binary 记录。
- 构建日志：749 条成功日志，覆盖三个架构和 256 个日志标签；将 URL 编码的
  `libsigc%2B%2B` 还原为 `libsigc++`，并将三个按目标拆分的
  `qemu-accel-*` 构建标签归并到实际 source package `qemu-accel`。

所有仓库身份、URL、记录数与命令见 `raw/004_*`、`006_*`、`009_*`、
`117_*`。

### 2.2 “C++ 包”的操作定义

使用四种正面证据的并集：

1. 实际构建日志出现 C++ 编译器识别、CXX 构建动作或真实 C++ 编译命令；
2. 已发布 binary RPM 声明 C++ runtime 依赖；
3. 静态归档存在外部可见、且能被 `c++filt` 实际解码的 C++ 符号；
4. RPM 安装 `.hpp/.hh/.hxx` 或明确的 C++ include 路径。

因此：仅有静态归档但没有全局 C++ 符号的不计入静态 C++；仅有头文件的包
计入候选分母并标为 `CPP_HEADER_PAYLOAD`，但不因此自动判为需要迁移。

初次构建日志扫描错误地以大小写不敏感方式匹配 `.C`，把普通 `.c` 算成
C++；修正后由 112 个降为 71 个原始 C++ 构建标签、69 个规范化源码名。
错误结果没有用于最终分母，修正与恢复记录见 `raw/053_*` 及脚本注释。

最终证据并集为 73 个源码包：69 个有实际 C++ 构建日志，36 个有动态
C++ runtime 信号，10 个有真实 C++ 静态归档，12 个带 C++ 头文件。
各集合会重叠，逐包证据见 `tables/base_cpp_package_decisions.tsv`。

### 2.3 与 R95 的 38 包为何不同

R95 的 38 是 GCC 标准仓 `Tizen-Base` 中“产出 C++ runtime ELF”的口径；
R100 使用的是另一个项目 `Tizen-Base-Toolchain`，且为了判断全局 flags 的
影响，操作定义扩展到实际 C++ 构建、静态归档和头文件载体。

- 共同：38；R95 的 38 个全部能映射到 R100 分母。
- R100 新增：35。
- R95 有而 R100 没有：0。

新增 35 个为：`automake`、`binutils-armv7hl`、`binutils-riscv64`、
`bison`、`c-ares`、`cereal`、`dejagnu`、`diffutils`、`expat`、`file`、
`flex`、`gcc-armv7hl`、`glibc`、`keyutils`、`krb5`、`libcxx-runtimes`、
`libdbuspolicy-header`、`libffi`、`libsndfile`、`libtool`、`libunistring`、
`libunwind`、`lttng-ust`、`lua`、`pkg-config`、`python`、`python3`、
`python3-base`、`rpm`、`sqlite`、`tensorflow2`、`update-alternatives`、
`util-linux`、`v4l-utils`、`zstd`。

它们的新增原因逐项记录在
`tables/r95_r100_denominator_crosswalk.tsv`；不能把 73 与 R95 的 38
解释为同一项目、同一口径下数量增长。

## 3. 符号级边界证据

为避免只凭依赖名判断，本轮对 armv7l 做了精确符号交集：

- Unified→Base：98 个精确 RPM、281,751,329 字节，逐包 SHA256 通过；
  解析 353 个 package/SONAME 候选、1,034 条实际 ELF 边，未解析项 0。
  源码对分类为 28 个布局敏感、19 个有 C++ ABI 但布局未排除、17 个纯 C、
  2 个无符号交集。
- Base 内部：81 个精确 RPM、106,704,169 字节；解析 257 条 ELF 边，
  未解析项 0。源码对为 4 个布局敏感、1 个 C++ ABI 未排除、19 个纯 C、
  1 个无符号交集。
- 静态归档：136 个 RPM、429,265,248 字节全部校验；2,218 个归档条目中
  805 个含外部可见且可解码 C++ 符号，归属于 10 个源码包。

静态归档初扫曾把局部 fortify 包装符号和 `_ZGV*` 向量 ABI 名称算作阳性；
经 `nm -g` 与 `c++filt` 独立校准后重扫，最终只采用收紧后的 805 条结果。
完整符号、demangle 和命令证据在 `tables/armv7l_*_edges/`、
`tables/static_archive_symbols_refined.tsv` 与 `raw/*individual*`。

只在 armv7l 做符号交集是本轮为控制资源作出的范围选择；构建日志、RPM
元数据和文件清单覆盖三个架构。不能把符号交集部分表述成三架构实测。

## 4. 逐包判定

### 4.1 已证实需要改用 libc++：10

`abseil-cpp`、`bcc-tools`、`boost`、`bpftrace`、`icu`、`jsoncpp`、
`libsigc++`、`llvm`、`pcre`、`taglib`。

其中 `abseil-cpp`、`boost`、`icu`、`jsoncpp`、`libsigc++`、`llvm`、
`pcre`、`taglib` 具有精确 Unified→Base C++ 符号边；`bcc-tools` 与
`bpftrace` 通过 Base 内部的布局敏感 LLVM C++ 边进入同一迁移闭包。

### 4.2 当前不能切换：6

`gcc`、`gcc-aarch64`、`gcc-armv7hl`、`gcc-armv7l`、`gcc-riscv64`、
`glibc`。

依据是 R98 已从精确 spec 与构建日志确认它们当前使用 GCC；其中 `gcc`
还承担 libstdc++ provider 角色。该标签只描述当前配置/角色，未来若能改用
Clang 或重构角色，必须重新判定。

### 4.3 不需要改：55

`automake`、`binutils`、`binutils-aarch64`、`binutils-armv7hl`、
`binutils-armv7l`、`binutils-riscv64`、`bison`、`c-ares`、`ccache`、
`cereal`、`cmake`、`db4`、`dejagnu`、`diffutils`、`doxygen`、
`elfutils`、`expat`、`file`、`flex`、`gdb`、`gettext`、`gn`、`golang`、
`groff`、`icecream`、`keyutils`、`krb5`、`libcxx-runtimes`、
`libdbuspolicy-header`、`libffi`、`libsndfile`、`libtool`、`libunistring`、
`libunwind`、`lttng-ust`、`lua`、`ncurses`、`nghttp2`、`ninja`、
`openfst`、`patchelf`、`pbzip2`、`pkg-config`、`python`、`python3`、
`python3-base`、`qemu-accel`、`rpm`、`sqlite`、`tiff`、`tinyxml2`、
`update-alternatives`、`util-linux`、`v4l-utils`、`zstd`。

这里的“无需改”不是“不是 C++”：具体依据分别是精确边仅有纯 C/无交集、
独立工具或 Base 内闭包没有 Unified runtime 边、仅由 consumer 编译的头文件，
以及没有迁移侧 consumer 的静态归档。`libcxx-runtimes` 已在自己的 spec
中明确构建目标运行时，不需要靠 Base 全局注入再次选择自身。

### 4.4 证据不足：2

- `gmp`：3 个归档条目含真实 C++ 全局符号；`eigen`、
  `python-pycrypto` 有 BuildRequires，但未证明是否链接 C++ 归档。
- `tensorflow2`：6 个归档条目含真实 C++ 全局符号；
  `inference-engine-tflite`、`nnstreamer`、`nntrainer` 有 BuildRequires，
  但未证明是否链接这些归档。

BuildRequires 只证明文件进入构建根，不能机械等同于链接。需取得这五个
consumer 的实际链接命令或产物符号/对象归属后才能定性。

每包的命中规则、架构、runtime consumer、BuildRequires consumer、镜像
归属、VCS 和边分类均在 `tables/base_cpp_package_decisions.tsv`。

## 5. 全局注入选项的实际驱动器探针

探针使用现有 Tizen buildroot 内的目标 GCC 14.2.0 与 Clang 22.1.8，测试：

```text
CXXFLAGS: -stdlib=libc++
LDFLAGS:  -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed
```

实际结果：

- armv7l 与 x86_64 的 GCC/G++ 均明确报
  `unrecognized command-line option '-stdlib=libc++'`；不是静默忽略。
- Clang++ 在三个架构都无诊断接受 `-stdlib=libc++`，但这不是“静默
  忽略”：`-###` 显示它选择 `-lc++`，并把 `--no-as-needed -lc++abi
  --as-needed` 传给链接器。
- Clang C driver 的编译探针对 `-stdlib=libc++` 给出
  `argument unused during compilation` 警告，因此也不是静默忽略。
- aarch64 GCC buildroot 中驱动器启动即退出 139，连 `--version` 都失败，
  该格记 `NOT_OBSERVED`，不能把 139 解释为选项拒绝。GCC 选项语义由
  armv7l 与 x86_64 两个实际 GCC 14.2.0 格确认。
- 部分真实链接动作受复用 buildroot 的动态加载器/`ld` 依赖缺失影响；
  选项是否被解析及转发以不执行链接器的 `-###` 为准，不把环境故障写成
  编译器语义。

因此本次没有发现“驱动器悄悄丢弃整组选项然后产物仍是 libstdc++”的格：
GCC 明确失败，Clang++ 实际改变库选择，Clang C 给警告。至于某个包的构建
系统是否完全丢弃环境变量，本轮没有逐包完整构建，记 `NOT_OBSERVED`。

摘要见 `tables/driver_option_probe.tsv`，完整命令和输出见 `raw/040_*`。

## 6. 两种配置形态的事实对照

### 按包 spec 设置

- 已证实需要处理：10 个，即 4.1 的名单。
- 尚待定：`gmp`、`tensorflow2`；未把它们计入 10。
- 已判定不需要的 55 个包不被主动改变。
- 当前 GCC 的 6 个包不接收 `-stdlib=libc++`。
- 具体形态是在上述包各自编译/链接 C++ 的路径中加入与 Unified 一致的
  `-stdlib=libc++` 及链接器组；本轮未修改任何 spec，也未验证十个包的
  完整构建。

### Base project_config 全局设置

- 环境赋值作用域是 Base 项目的全部 254 个去重源码名，而非只有 73 个
  C++ 候选。
- 在 73 个候选中：10 个属于预期改变，55 个属于不需要却被触及，2 个
  语义待定，6 个当前 GCC 包在变量实际传播时会明确拒绝。
- 69 个规范化源码名有实际 C++ 构建日志：10 个需要、51 个不需要、
  2 个待定、6 个 GCC。另有 4 个“不需要”包只有产物证据，当前日志中
  未观测到 C++ flags 消费。
- 对 Clang 构建的“不需要”包，这不是“多带一个无用选项”：一旦
  CXXFLAGS 被消费，`-stdlib=libc++` 会真实改变头文件与运行库选择；
  LDFLAGS 的 `--no-as-needed -lc++abi` 也会被转交链接器。
- 73 之外的 C-only 包是否消费全局 LDFLAGS 没有穷举；因此不能给出
  全部 254 包的“失败/无效”精确计数。这是全局方案仍需单独验证的面。
- 若采用全局形态而要保留六个 GCC 包，必须存在对这些包不传播该组选项的
  条件或例外；本轮只陈述这一机械后果，不选择如何实现。

逐包影响见 `tables/global_injection_package_effects.tsv`，方案计数见
`tables/configuration_strategy_comparison.tsv`。

## 7. 范围、盲区与自行判断

### 未覆盖/仍需材料

1. `gmp` 与 `tensorflow2` 的真实静态链接命令或迁移后 consumer 产物；
2. aarch64 GCC 驱动器选项格（当前复用 buildroot 启动即 139）；
3. x86_64/aarch64 的逐符号边界复核；本轮符号级分类仅 armv7l；
4. 动态加载、插件注册或 IPC 中隐藏的 C++ 对象边；RPM DT_NEEDED 无法证明
   不存在这类边；
5. 全部 254 个 Base 源码包对 LDFLAGS 的实际消费；
6. 两种配置形态的完整包构建均未执行。

曾生成一份对 43 个初始待定包做全 binary RPM 重扫的清单，但精确估算为
406 个 RPM、1,578,067,964 字节。为避免在已有符号级证据之外再引入约
1.58 GB 下载和更大的提取负载，该扫描没有启动；只按 SHA256 硬链接了
51 个既有缓存文件，未从中产生结论。清单和决定过程保留在 `raw/110_*`—
`113_*`，它不属于已完成覆盖面。

### 本轮自行判断

1. 按人工确认删除冗余“广泛链接”判据，只保留 consumer 数作为影响面；
2. 将三个 `qemu-accel-*` 构建标签归并为元数据中的一个 source package；
3. 为控制资源，符号级接口分类选择 armv7l，三架构元数据和日志继续保留；
4. 把“没有 Unified runtime C++ 边、只在 Base 内闭包或独立进程使用”作为
   `NO_LIBCXX_NEEDED` 的正面隔离证据；动态加载盲区另行保留；
5. 对有 C++ 静态归档且只有 BuildRequires 证据的两包不自行推断链接，保留
   `UNDETERMINED`；
6. 估算全 binary 补扫会增加 1.58 GB 下载后停止该可选扩展，不用部分缓存
   推断结果；
7. 不在全局与按包方案之间作推荐或排序。

### 技术性异常

构建日志正则、静态归档相对路径、HTTP 无响应、首次探针语言参数和
aarch64 驱动器启动问题均保留了原命令、非零退出码和恢复说明。它们没有
被当作判据不成立，也没有覆盖原始输出。

## 8. 不变事项

本任务只做分析；未修改 `project_config`、平台源码或补丁，未推送 Gerrit，
未实施迁移，也未作方案推荐。
