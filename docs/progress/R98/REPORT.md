# R98：LLVM Toolchain 过渡态中的 GCC 构建例外与依赖拓扑

## 1. 结论先行

本轮按人工更正后的范围完成。状态为：

`ANALYSIS_COMPLETE_FOR_REVISED_SCOPE / POST_MIGRATION_RUNTIME_IDENTITY_PENDING`

1. 当前可取得的 LLVM Toolchain 成对快照并不是迁移后镜像，而是
   **Clang 编译 + libstdc++** 的过渡态。Unified 中有 311 个产生 C++
   runtime 依赖的源码包直接依赖 `libstdc++.so.6`，直接依赖
   `libc++.so`/`libc++abi.so` 的为 0。
2. 对两个项目三架构的 3,848 条成功构建日志逐条读取实际导出的
   `CC/CXX`；瞬时超时全部重试成功，最终无 `NOT_AVAILABLE`。实际选择
   GCC 的是 **11 个源码包**，其余 3,817 条记录选择 Clang。
3. 11 个 GCC 包是：Base 的 `gcc`、`gcc-aarch64`、`gcc-armv7hl`、
   `gcc-armv7l`、`gcc-riscv64`、`glibc`；Unified 的 `lapack`、
   `nntrainer`、`onnxruntime`、`openblas`、`yaca`。这 11 个包的精确
   spec 均含 `%define _toolchain_override gcc`，与构建日志相互印证。
4. 其中产生直接 C++ runtime 依赖 ELF 的源码包有 4 个：`gcc`、
   `nntrainer`、`onnxruntime`、`yaca`。`lapack`/`openblas` 的可见
   运行期边为 C/Fortran 接口，不能仅因其由 GCC 构建就归为 C++ 边。
5. 稳定的非系统共享库拓扑共 30 个源码对、417 条实际 ELF 边：
   - 已证明传递/解析布局敏感标准库类型：2 个源码对；
   - 有 C++ ABI 耦合但布局传递未证明：2 个源码对；
   - 仅 C 接口符号交集：24 个源码对；
   - 无符号交集：2 个源码对。
6. **不能从当前快照回答“迁移后两侧实际使用哪套标准库”**。所以
   `nntrainer` 和 `yaca` 相关 C++ 边是否成为最终混合运行时边记为
   `UNABLE_TO_DETERMINE_UNTIL_POST_MIGRATION_RUNTIME_IDENTITY`，没有据配置
   预期冒充构建事实。
7. 本任务没有修改配置或平台源码，没有实施迁移，没有推送 Gerrit，
   也没有给出处置建议或优先级。

## 2. 输入身份

### 2.1 最终采用的成对输入

- Base：
  `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/`
- Unified：
  `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247/`

选择理由：这是线上最后一组有明确派生关系、且 Unified 同时提供六个设备
镜像清单的完整成对输入。发现的另一个候选
`tizen-base-toolchain_20260901.103155` 只有 Base 项目，没有对应的新
Unified 快照或镜像，不能与旧 Unified 拼接做依赖及镜像归属判定。

候选发现、派生关系及线上目录证据见：

- `raw/020_list_current_toolchain_snapshots.*`
- `raw/021_inspect_current_reference_and_candidate.*`
- `raw/022_inspect_new_base_snapshot.*`

### 2.2 数据规模与校验

| 数据集 | binary RPM 记录 | source RPM 记录 | 去重源码名 |
| --- | ---: | ---: | ---: |
| Base Toolchain | 1,705 | 292 | 254 |
| Unified Toolchain | 9,607 | 1,061 | 1,061 |

六个镜像为 headed/headless/boot 的 armv7l 与 aarch64 组合，清单合计
4,393 行。repomd 中引用的 primary/filelists/source-primary 均按声明的
SHA256 校验通过，见 `raw/010_fetch_llvm_snapshot_metadata.*` 和
`raw/060_validate_analysis_coverage.*`。

本轮进一步下载了：

- 104 个边界分析所需的精确 binary RPM，共 626,687,028 字节；
- 6 个精确 source RPM，共 428,753,894 字节；
- 两组均 100% 下载与 SHA256 校验通过；104 个 binary RPM 100% 解包，
  ELF 分析失败 0，符号边未解析 0。

### 2.3 当前快照的适用边界

当前快照可以证明：

- 哪些构建记录实际选择 GCC 或 Clang；
- 当前包依赖与构建依赖拓扑；
- 当前 ELF 之间实际存在的 C/C++ 符号交集；
- 当前六个镜像的精确包归属。

当前快照不能证明：

- 全量迁移后每个 ELF 实际链接 libc++ 还是 libstdc++；
- 迁移后 C++ 符号能否链接、会否改变或被隐藏；
- 迁移后最终镜像中哪些拓扑成为真正的混合运行时边。

## 3. GCC 构建包的判定

### 3.1 判定方法

判定没有使用包名或经验：

1. 配置事实：Base Toolchain 的 `project_config:133-149` 定义默认工具链
   为 Clang，只有 spec 定义 `_toolchain_override` 才会改变 `%{__cc}` /
   `%{__cxx}`；实际代码见 `raw/023_locate_toolchain_override_mechanism.*`。
2. 实际构据：抓取全部成功构建日志前 2 MiB，提取 `%build` 实际导出的
   `+ export CC=... CXX=...`。3,848/3,848 条均命中；44 次瞬时网络超时
   经原 URL 重试后全部补齐。
3. spec 交叉验证：对 6 个包下载精确 source RPM 后取 spec；GCC 家族
   使用工作区已有 Git 对象中的精确 VCS 提交
   `3d5dd5bec16591dbf3277df37fb43e9cc5b890de`。11 份 spec 均明确写有
   `_toolchain_override gcc`。
4. ELF `.comment`/producer：对已下载的 GCC 包边界 ELF 检查 `.comment`。
   只有部分 `gcc` 产物保留 GCC marker；其余多数已剥离 `.comment`。
   未下载独立 debuginfo，因此 producer 为 `NOT_OBSERVED`。该阴性没有被
   用来否定日志与 spec 的阳性证据。

完整逐记录证据为 `tables/buildlog_compiler_evidence.tsv`；spec 原文在
`inputs/specs/`；`.comment` 结果为 `tables/gcc_elf_comment_evidence.tsv`。

### 3.2 spec 的明确覆盖点

| 源码包 | 精确 spec 行 | 原文性质 |
| --- | ---: | --- |
| gcc | 30 | `Always build gcc with gcc compiler` 后定义 override |
| gcc-aarch64 / gcc-armv7hl / gcc-armv7l / gcc-riscv64 | 33 | 同上 |
| glibc | 36 | 注明使用 GCC extensions，定义 override |
| lapack | 16 | 定义 override |
| nntrainer | 18 | `Support only gcc compiler` 后定义 override |
| onnxruntime | 36 | 定义 override |
| openblas | 128 | `%prep` 中定义 override |
| yaca | 23 | 定义 override |

完整输出见 `raw/049_confirm_spec_overrides.*`。

### 3.3 决策用包表

完整表：`tables/gcc_package_decision_table.tsv`。下表中的“进入镜像”按
该源码包实际产生的 binary 子包精确关联，不等同于主包同名项。

| 源码包 | 项目 | GCC 架构 | C++ runtime ELF | 直接链接 libstdc++ | 进入六镜像 | 稳定非系统 C++ 边 | “必须处置”事实值 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| gcc | Base | aarch64/armv7l/x86_64 | 是 | 是 | 是，运行时子包 | 无，所测为 zlib C 边 | 当前可见边：否 |
| gcc-aarch64 | Base | armv7l/x86_64 | 否 | 否 | 否 | 无 | 当前可见边：否 |
| gcc-armv7hl | Base | 三架构 | 否 | 否 | 否 | 无 | 当前可见边：否 |
| gcc-armv7l | Base | aarch64/x86_64 | 否 | 否 | 否 | 无 | 当前可见边：否 |
| gcc-riscv64 | Base | 三架构 | 否 | 否 | 否 | 无 | 当前可见边：否 |
| glibc | Base | 三架构 | 否 | 否 | 是 | 无；可见边为 C 或无交集 | 当前可见边：否 |
| lapack | Unified | 三架构 | 否 | 否 | 否 | 无；suitesparse 为 C/Fortran 符号 | 当前可见边：否 |
| nntrainer | Unified | 三架构 | 是 | 是 | 是 | 2 个布局敏感、1 个 C++ 未证明 | **迁移后身份未产出，无法判定** |
| onnxruntime | Unified | 三架构 | 是 | 是 | 否 | nnstreamer 为 C 接口 | 当前可见边：否 |
| openblas | Unified | 三架构 | 否 | 否 | 是 | 4 对均为 C/Fortran 接口 | 当前可见边：否 |
| yaca | Unified | 三架构 | 是（仅 tests 命中） | 是 | 是（yaca 主包） | 对 boost-test 有 C++ 耦合，布局未证明 | **迁移后身份未产出，无法判定** |

这里的“当前可见边：否”只描述已经解析的符号边，不等于建议“不处理”，
也不覆盖尚未构建的迁移后新边。

### 3.4 任务书点名工具的核对

`bcc-tools`、`bpftrace`、`gdb`、`llvm`（含 lldb 所在源码）、`elfutils`、
`binutils`、`openfst` 在本快照的实际构建日志中均选择 Clang，不是 GCC
例外。除 `bpftrace` 只存在 armv7l/aarch64 构建记录外，其余均覆盖三架构。
精确 CC 值见 `tables/requested_tool_compiler_checks.tsv`。

## 4. 依赖拓扑

### 4.1 构建期依赖

精确 source `BuildRequires` 到 GCC 包 binary provides 的匹配产生 173 条
记录、110 个去重 consumer 源码身份。完整清单：
`tables/gcc_inbound_build_dependencies.tsv`。

| GCC provider 源码 | 构建期 consumer 数 | 主要 requirement |
| --- | ---: | --- |
| gcc | 95 | gcc/gcc-c++/libgcc/libstdc++-devel 等 |
| gcc-aarch64 | 1 | gcc-aarch64 |
| glibc | 19 | glibc-devel/static/locale 等 |
| lapack | 4 | blas-devel/lapack-devel |
| nntrainer | 4 | capi-ml-training/pkgconfig(nntrainer) 等 |
| onnxruntime | 1 | onnxruntime-devel |
| openblas | 5 | openblas-devel |
| yaca | 2 | yaca/yaca-devel |

其余三个 cross-GCC 源码在本次精确 capability 匹配中没有独立 consumer
记录。这些是构建期依赖，不构成同一进程内对象跨界。

### 4.2 运行期边的口径

完整 RPM capability 匹配原始结果有 51,060 条，绝大部分是 libc、
libstdc++、libgcc_s 及同一 SONAME 的多个版本 capability。该完整表以
压缩形式保存为 `tables/gcc_runtime_dependency_edges.tsv.gz`。

为测量“不随换标准库而消失的拓扑”，符号分析排除了以下基础运行时
SONAME：加载器、libc/libm/libdl/librt/libpthread、libgcc_s、libstdc++、
sanitizer、Fortran/OpenMP runtime 等；没有删除原始表。armv7l/aarch64
剩余 168 条精确 RPM 边，聚合为 30 个源码对，并对每个 DT_NEEDED consumer
ELF 与唯一 provider ELF 做 `UND ∩ DEF`。

结果：417 条实际 ELF 边，未解析 0，分析失败 0。

### 4.3 逐类结果

完整逐对表：`tables/gcc_runtime_source_pair_classification.tsv`；完整逐 ELF
符号：`symbol_edges/actual_cross_elf_edge_evidence.tsv`。

#### 已证明涉及布局敏感标准库类型（2 对）

1. `webapi-plugins → nntrainer`：20 条 ELF 边，实际交集包含
   `std::__cxx11::basic_string<...>` 的构造、移动、append、reserve、析构等；
   consumer 为多个 `libtizen_ml.so`，provider 为 `libcapi-nntrainer.so` /
   `libnntrainer.so`。
2. `nntrainer → gtest`：84 条 ELF 边、65 个 C++ 交集符号；包括
   `testing::internal::MakeAndRegisterTestInfo(std::__cxx11::basic_string<...>, ...)`
   及多项接受/返回标准字符串的 gtest API。命中集中在
   `nntrainer-applications` 测试程序。

这里“已证明”来自实际符号参数或实际标准库成员调用的跨 DSO 解析，不是
因为“看到 C++ 文件名”。当前两侧都仍用 libstdc++；迁移后是否仍形成
同一解析关系必须重测。

#### 有 C++ ABI 耦合、布局传递未证明（2 对）

1. `nntrainer → opencv`：实际调用 `cv::Mat` 构造/析构、`Mat::zeros`、
   `cv::warpAffine` 等。存在 C++ ABI 耦合，但现有动态符号不能证明或排除
   其内部是否跨界传递标准库布局。
2. `yaca-tests → boost-test`：28 个 Boost.Test C++ 符号，包括
   `boost::function<void()>`、`lazy_ostream`、`basic_cstring` 等。没有据
   “未出现显式 std”推断安全，按任务要求保留在布局未证明档。

#### 仅 C 接口符号交集（24 对）

包括 `nnstreamer → onnxruntime`、`training-engine-nntrainer → nntrainer`、
OpenBLAS/LAPACK 的 Open3D/numpy/suitesparse consumers、GCC 工具到 zlib、
glibc 的 libresolv/libthread_db 等，以及 nntrainer 到 dlog/glib/iniparser/
nnstreamer/CAPI 等。判定依据是实际 `UND ∩ DEF` 只有非 `_Z` 符号；其类别
名称严格是“仅 C 接口符号交集”，没有扩写成“全面安全”。

#### 无符号交集（2 对）

`ot-br-posix → glibc` 与 `perl → glibc` 的相关 DT_NEEDED 候选中没有
consumer UND 与 provider DEF 的交集，故记 `NO_SYMBOL_INTERSECTION`。

### 4.4 反方向与动态加载

30 个源码对按方向各 15 对：

- 非 GCC consumer 依赖 GCC provider：12 个 C-only、2 个无交集、
  1 个布局敏感；
- GCC consumer 依赖非 GCC provider：12 个 C-only、2 个 C++ 布局未证明、
  1 个布局敏感。

46 个 GCC 包 runtime ELF 含 `dlopen` 未定义符号。对其字符串独立检查后，
可确认的非 DT_NEEDED 库名只有：

- onnxruntime 的 `libonnxruntime_providers_*.so`；当前仓实际提供的
  `libonnxruntime_providers_shared.so` 来自同一 onnxruntime 源码包；
- nntrainer 的 `libnntrainer.so` 自身名称。

没有从二进制中确认到另一源码包的 C++ 库被动态加载。对运行时拼接库名、
环境变量指定插件或外部 GCC plugin 的路径，二进制字符串法不能穷尽，记为
盲区；没有以“未命中”推断不存在。证据见
`tables/gcc_dlopen_string_evidence.tsv`。

## 5. 镜像归属

精确关联表：`tables/gcc_exact_image_membership.tsv`，共 58 条
image/binary/arch 记录。

- `gcc` 源码进入镜像的是 `libgcc`、`libstdc++`、`libatomic`、
  `libgomp`、`libgfortran` 等运行时子包，**不是 GCC 编译器程序本身**；
- `glibc`：六镜像均有；
- `nntrainer`：headed/headless 的两架构镜像；
- `openblas`：headed/headless 的两架构镜像；
- `yaca`：两个 headed 镜像；
- `onnxruntime`、`lapack` 与四个 cross-GCC 主包不在六镜像中。

包角色列是基于实际子包名、summary/description 与镜像项作出的说明性分类，
不是迁移处置建议；详见 `tables/gcc_package_decision_table.tsv`。

## 6. 必须等待全量编译的事项

以下均为 `NOT_OBSERVED`，不能由当前过渡态替代：

1. 每个新 ELF 的实际 `DT_NEEDED` 是 libc++ 还是 libstdc++；
2. 11 个 GCC override 包在新一轮中是否仍全部成功、是否产生相同子包；
3. `webapi-plugins ↔ nntrainer`、`nntrainer ↔ gtest/opencv`、
   `yaca-tests ↔ boost-test` 在新符号命名下是链接失败、边消失、仍有交集，
   还是形成运行期混合边；
4. 新镜像是否仍装入相同 GCC 运行时/应用子包；
5. 新构建是否新增显式或动态加载依赖。

补全所需材料：

- 完成全量编译后的 Base/Unified 成对 snapshot ID 与 `build.xml`；
- binary/source repodata、三架构完整构建日志；
- 精确 binary RPM 与 checksum；
- 六个镜像的新 `.packages` 清单；
- 若要穷尽动态插件路径，需上述 11 个源码包以及其 plugin loader 配置。

收到这些材料后应按本轮相同口径重做“runtime identity + 精确 RPM +
DT_NEEDED + UND/DEF + 镜像关联”，而不是只把配置预期填入表格。

## 7. R95 材料复用声明

复用了 R95 的以下方法和表结构：RPM metadata 解析、精确 provider/
consumer capability 匹配、RPM SHA256 门禁、ELF `DT_NEEDED` 与动态符号交集
分类。复用表的 SHA256 已记录于 `raw/007_inspect_reusable_r95_materials.*`。

R95 表属于 GCC 标准仓世界，只用于前态对照与方法校准；没有用它证明本轮
LLVM Toolchain 的编译器身份、包内容、镜像归属或符号边。本轮这些事实均由
所申报 LLVM Toolchain 快照重新取得。

## 8. 自行判断、疑问与盲区

### 自行判断

1. 选用最后一个有完整派生关系与六镜像的 20260828/20260829 成对快照；
   没有把 20260901 Base 与旧 Unified 拼接。
2. 编译器身份以实际 `%build` 导出的 CC/CXX 为主证据，精确 spec 为交叉
   证据；没有把 `.comment` 缺失解释成“不是 GCC”。
3. 为区分稳定拓扑与当前运行时实现，基础 libc/libstdc++ 等边保留在完整
   原始表，但不纳入 30 对非系统符号分类。
4. 因六个设备镜像只有 armv7l/aarch64，逐 ELF 符号分析覆盖这两架构；
   编译器日志核对仍覆盖 x86_64。x86_64 的同类符号交集未实扫。
5. “测试/诊断/运行时”角色只是材料归类；没有由此决定迁移范围。

### 尚存疑问/盲区

1. 迁移后快照尚不存在，是最主要的结论边界。
2. 动态加载路径可能运行时拼接；仅靠 ELF 与 strings 不能穷尽。
3. `UND ∩ DEF` 证明现有动态解析关系，但不能单独证明 C++ 对象的每个字段
   实际在调用时都被访问；因此非显式 std 的 C++ 边没有提升为布局敏感。
4. 当前 buildlog 目录中的部分名称与 source metadata 名称有 URL 编码或
   历史项目别名差异；GCC 阳性包均由 exact spec/VCS 和三架构 CC/CXX
   双重确认，不受该别名差异影响。

## 9. 操作纪律确认

- 未修改任何 project_config、平台源码或补丁；
- 未实施迁移或构建；
- 未推送 Gerrit 或其他外部源码仓；
- 未给出“应迁哪些包”的建议或优先级；
- 所有任务命令、stdout、stderr、退出码和技术性恢复说明位于 `raw/`。

