# R101：Base 层 libc++ 判定完整清单与补充核查

## 1. 结论摘要

本轮严格沿用 R100 的判定，不重新分类 73 个 C++ 候选。把同一快照中未进入
C++ 候选的 181 个源码包补入后，形成覆盖 `Tizen-Base-Toolchain` 全部 254 个
去重源码包的清单：

| 判定 | 全部源码包数 | 其中 R100 C++ 候选 | 说明 |
|---|---:|---:|---|
| 需要改 | 10 | 10 | R100 的 `NEED_LIBCXX` 原样保留 |
| 不需要改 | 236 | 55 | 另含四路完整候选扫描均未命中的 181 个非 C++ 包 |
| 当前不能改 | 6 | 6 | 当前 GCC 配置/产品角色，不代表永久不能迁移 |
| 未定 | 2 | 2 | `gmp`、`tensorflow2` 的实际静态链接关系未闭合 |

Base 内部 provider 补查没有发现会扩大 10 包闭包的反例：121 条布局敏感 ELF
边归并为 4 个源码包对，consumer 与 provider 均已是 `NEED_LIBCXX`。不存在
“`NEED_LIBCXX` 消费 `NO_LIBCXX_NEEDED` provider”的布局敏感边。

## 2. 数据身份与分母

- Base 快照：`Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647`。
- 配对 Unified 快照：
  `Tizen/Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247`。
- Base 仓元数据：292 条 source 记录，按 `name` 去重后 254 个源码包；多出的
  38 条是同名包的不同 Release/架构仓记录，不作为新源码包计数。
- 已发布 binary 元数据：1,705 条。
- C++ 操作定义沿用 R100：实际 C++ 构建日志、动态 C++ runtime 信号、具有
  可解码全局 C++ 符号的静态归档、明确 C++ 头文件载体四路证据取并集，共 73 包。
- 181 个非 C++ 行不是以包名猜测，而是上述四路完整候选扫描均未命中；清单中
  将此写成明确的扫描依据，不写成泛泛的“无证据”。

与 R95 的关系仍按 R100 的交叉核算：R95 的 38 包来自 GCC 标准仓
`Tizen-Base` 且只统计 runtime ELF；本任务使用 `Tizen-Base-Toolchain` 并扩展到
构建日志、静态归档和头文件，二者不是同一项目、同一口径。R95 的 38 个全部
映射进本轮 73 个 C++ 候选，R100/R101 另有 35 个候选。

## 3. Base 内部布局敏感边的 provider 核查

### 3.1 方法

复用 R100 armv7l 的 257 条已解析 Base 内部 ELF 边和精确符号交集；其未解析项为
0。细表中的 `DIRECT_STD_LAYOUT` 与 `KNOWN_HIDDEN_STD_LAYOUT` 两类合并为
`LAYOUT_SENSITIVE_STD_TYPE`，再把 sourcerpm 精确映射回源码包，并同时连接 R100
decision 表的 consumer/provider 判定。

### 3.2 四个源码包对

| consumer | provider | 具体 ELF 边数 | C++ 符号计数之和 | consumer 判定 | provider 判定 |
|---|---|---:|---:|---|---|
| `bcc-tools` | `llvm` | 4 | 584 | NEED | NEED |
| `boost` | `boost` | 2 | 30 | NEED | NEED |
| `bpftrace` | `llvm` | 2 | 201 | NEED | NEED |
| `llvm` | `llvm` | 113 | 17,589 | NEED | NEED |

具体端点包括：

- `libbcc.so`、BCC 的 `PyPerf` 示例分别调用 `libLLVM.so.22.1` 与
  `libclang-cpp.so.22.1`；
- `libboost_log.so` / `libboost_log_setup.so` 调用 `libboost_filesystem.so`；
- `/usr/bin/bpftrace` 调用 `libLLVM.so.22.1` 与 `libclang-cpp.so.22.1`；
- LLVM/Clang/LLDB 工具和库的 113 条内部边调用 LLVM 自身库。

完整的 4 行逻辑表与 121 行 ELF 表分别见
`tables/base_internal_layout_sensitive_edges.tsv` 和
`tables/base_internal_layout_sensitive_elf_edges.tsv`。

### 3.3 核查回答

没有发现 `NEED_LIBCXX → NO_LIBCXX_NEEDED` 的布局敏感 Base 内部边。因此按现有
armv7l 精确边证据，仅切换既定 10 包不会因这些已知 Base 内部边新形成一个指向
NO provider 的混合运行时边界。本结论是对 R100 provider 方向的补齐，不是重新
判定。

## 4. 254 包完整清单的口径

完整 TSV 和 Markdown 表均严格一包一行，列出：源码包名、是否含 C++ 及依据、
与 libstdc++ 的关系、判定、通俗理由、具体证据、六镜像归属、测试/诊断/基准
工具属性。

### 4.1 与 libstdc++ 的分类

- `直接链接 libstdc++`：由该源码包产出的至少一个 binary RPM 的 `Requires`
  明确含 `libstdc++.so*`，共 35 个 C++ 候选。
- `仅通过 C 接口间接相关`：自身不直接链接 libstdc++，精确边只显示 C API。
  当前 254 行中没有需要单独落入这一档的源码包；具有纯 C 边但自身直接链接
  libstdc++ 的包仍放在“直接链接”，纯 C 性质写在理由中。
- `仅构建期使用，产物不含 C++ 运行时`：构建或头文件载体命中 C++，但没有
  已发布 C++ runtime 边界，共 31 包。
- `含 C++ 但不与外部交换 C++ 语义`：有 C++ 载体，但 R100 正面隔离证据没有
  通向迁移侧的 C++ 语义边，共 6 包。
- `自身提供 libc++/libc++abi，与 libstdc++ 无直接链接`：仅
  `libcxx-runtimes`。
- `无关`：四路扫描未命中的 181 个非 C++ 包。

### 4.2 镜像与工具属性

镜像列按源码 VCS 精确关联 R98 保存的六份清单：boot/headed/headless ×
aarch64/armv7l；93 个源码包至少进入其中一个。空集合写成“否：六个精确镜像
清单无该 VCS”，不外推到未取得的其他产品 profile。

测试/诊断/基准工具列以仓库 Summary/Description 的主要用途或明确命名的 binary
子包为准。例如 `boost-test`、`python3-testsuite`、`libunwind-tests`、LLDB、
GDB、BCC/bpftrace、rootstrap checker、QEMU 等记“是”。“否”表示该快照仓元数据
没有把其主要用途或子包标成这三类，不证明源码树中没有辅助测试。

## 5. 10 包实施输入

10 包仍为：`abseil-cpp`、`bcc-tools`、`boost`、`bpftrace`、`icu`、
`jsoncpp`、`libsigc++`、`llvm`、`pcre`、`taglib`。

本轮通过 Gerrit SSH 只读取得其 `tizen_base`，并从 R100 快照 revision 检出
`packaging/` 复核；随后又从 2026-09-03 实际分支 HEAD 导出当前 spec。当前
`bcc-tools`、`boost`、`jsoncpp`、`llvm` 已较 R100 快照前移，其余六个 HEAD 与
快照锚点相同。完整双 revision、仓库、spec 路径和 `%build` 行号见
`tables/implementation_10_packages.tsv`。

共同建议是显式增加 `libc++-devel`、`libc++abi-devel` BuildRequires，仅向
C++ 与链接阶段加入：

```sh
export CXXFLAGS="${CXXFLAGS} -stdlib=libc++"
export LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed"
```

包级差异：

- CMake 包在 `%build` 后、CMake 配置前导出；Meson 的 `libsigc++` 在 `%meson`
  前导出；ICU/PCRE 在 configure 前合入已有 flags。
- Boost 的 b2 已显式消费 `CXXFLAGS`/`LDFLAGS`；除加入 flags 外，
  `boost-devel` 的 `Requires: libstdc++-devel` 要改为 `libc++-devel`。
- PCRE 的 `pcre-devel` 同样要把 `libstdc++-devel` Requires 改为
  `libc++-devel`。
- LLVM 的 CMake 命令显式设置 shared/exe linker flags，不能只导出环境变量；
  必须把新的 `LDFLAGS` 合入这两个 `-D` 值，避免覆盖。这里改的是
  `packaging/llvm.spec`，不是已显式构建目标运行时的
  `packaging/libcxx-runtimes.spec`。

这些是后续实施建议，不是已验证补丁。实施时先产出并发布
`libcxx-runtimes`，再构建 LLVM，然后构建 `bcc-tools`/`bpftrace`；不能仅凭
`Support:` 推断 QuickBuild 自动排序。

每包验证至少包括：三架构日志确认 Clang 22.1.8 和 flags；所有该包 C++ ELF 的
`DT_NEEDED` 不再含 `libstdc++.so.6`、包含目标 libc++/libc++abi 依赖；符号表无
`GLIBCXX_*` 引用；重跑 Unified→Base/Base 内部边扫描；运行包自测及代表性 smoke
test。具体逐包放置点见 Markdown 实施清单。

## 6. 两个未定包

细节见 `UNDETERMINED.md`。两者继续保持 R100 的 `UNDETERMINED`：

- `gmp`：`libgmpxx.a` 三架构均含 11 个真实 C++ 全局符号；`eigen`、
  `python-pycrypto` 仅有 BuildRequires 关系，是否实际链接 C++ 归档未知。
- `tensorflow2`：`libtf_xla_runtime.a` 与 `libtensorflow2-lite.a` 含大量真实 C++
  符号；`inference-engine-tflite`、`nnstreamer`、`nntrainer` 仅有 BuildRequires
  关系，实际归档成员是否进入最终 ELF 未闭合。

临时处理建议是：不要把两者降为“不需要改”；在实施排期和验证预算里按“可能
需要改”预留，但在拿到真实链接命令/最终 ELF 归属前不直接修改 spec。这不是对
R100 判定的变更。

## 7. 为什么按人工决定采用逐包方案

这里记录人工已作出的方案决定及其事实基础，不追溯修改 R100 当时“不推荐”的
报告定位：

- 逐包方案只修改 10 个已证实需要的 spec；2 个未定项不计入已确认修改数。
- 全局 project_config 会把环境定义暴露给全部 254 个源码包，并触及 55 个已
  判定不需要迁移的 C++ 包、6 个当前 GCC 包、2 个未定包。
- 实际 C++ 构建日志中有 69 个规范化源码包消费 C++ 工具链；C-only 包是否把
  LDFLAGS 继续传到 driver 未逐包穷尽，因此不能把“254 全部实际受影响”写成实测。
- armv7l/x86_64 的 GCC 14.2.0 实测明确拒绝 `-stdlib=libc++`，未出现静默
  忽略；aarch64 GCC buildroot 的 driver 自身退出 139，该格仍为
  `NOT_OBSERVED`。
- 三架构 Clang 22.1.8 无诊断接受该选项，但 `-###` 证明它实际选择 `-lc++`
  并转发 libc++abi 链接参数，不是无害的静默忽略。

## 8. 覆盖边界、自行判断与疑问

### 覆盖边界

1. 精确符号交集及本次 provider 核查只覆盖 armv7l；构建日志、RPM 元数据和
   文件载体覆盖三架构。
2. 镜像列只覆盖六份已保存的标准镜像清单，不覆盖所有产品 profile、临时测试
   rootfs 或人工安装包。
3. 测试/诊断/基准属性来自仓元数据主要用途/明确子包，不是源码功能穷举。
4. `gmp` 与 `tensorflow2` 的静态链接归属仍未观测。
5. R100 锚点之后四个目标仓分支已前移；本轮只读检查了当前 spec，但没有在新
   HEAD 上重跑边界分析或构建。
6. 本任务未实施建议、未构建迁移包，所有实施后验证均待后续任务。

### 自行判断

1. 把四路完整候选扫描均未命中的 181 包在全量表中标为“非 C++/不需要改”，
   是对 R100 分母的机械补全，不改变其 73 包分类。
2. 工具属性采用“仓元数据主要用途或明确子包”的可复核口径；没有用包名印象
   扩大分类。
3. 两个未定包的临时处理只影响计划预留，不把它们写入 10 包实施集合。
4. 实施建议显式写 BuildRequires，以免依赖 project_config `Support:` 的非调度
   语义；是否接受该 packaging 形式仍由后续实施评审决定。

### 尚存疑问

1. 四个已前移仓的最新源码内容是否改变 R100 边界，需要在实际实施 revision
   确定后复核；本任务不自行改判定。
2. QuickBuild 是否有可观察的 provider→consumer 调度边仍为
   `NOT_OBSERVED`；实施前需读取任务图或采用两阶段构建。
3. 两个未定包需要 consumer 的真实链接命令、link map 或最终 ELF 对象归属。

整理与补查过程中未发现 R100 的 73 项判定存在问题；本轮发现并修正的仅是
R101 自身源码包级派生表曾混用“该源码包对的全部 ELF 边数”与“其中布局敏感
ELF 边数”的统计口径，最终表已由 121 条 ELF 明细重新聚合，判定和边的两端均
未变化。

## 9. 纪律确认

未修改任何 project_config、平台源码、spec 或 R100 材料；只保存只读副本和分析
产物。未推送 Gerrit。
