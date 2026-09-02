# R100 第一步：Base Toolchain C++ 包三分类规则（已确认并应用）

状态：`CONFIRMED_AND_APPLIED`

本文件保留第一阶段经人工确认的机械判定规则。逐包判定与全局/逐包两种方案的
事实对照见本任务的主报告；本任务不作推荐。

## 1. 对象和分母

### 1.1 项目身份

本任务的 `Base` 明确定义为：

- `Tizen-Base-Toolchain`；
- 静态输入锚点为 R98 使用的
  `tizen-base-toolchain_20260828.101647`；
- 配对依赖输入为
  `tizen-unified-toolchain_20260829.015247`。

不把 R95 的 `Tizen-Base/tizen-base_20260827.214809` 直接当作本任务分母。
R95 的符号分类方法可以复用，其包归属和数量只能作辅助线索，最终判定必须回到
Toolchain 项目的精确 RPM、spec、依赖和符号证据。

### 1.2 “每个 C++ 包”的机械定义

判定单位为去重后的源码包名。一个源码包只要在任一受支持架构、任一子包命中
下列任一条件，就进入 `BT_CPP` 候选全集：

1. 实际构建日志中出现 C++ 编译动作（目标 `clang++` / `g++` 或明确以 C++
   language mode 编译的 translation unit）；
2. 产出的可执行 ELF / 共享对象含 Itanium C++ ABI 符号；
3. 产出的可执行 ELF / 共享对象直接依赖 `libstdc++.so*`、`libc++.so*` 或
   `libc++abi.so*`；
4. 产出的静态归档成员含 Itanium C++ ABI 符号或 C++ unwind/typeinfo 证据。

头文件-only 包不因“文件后缀为 C++”自动进入上述运行产物集合；单独列为
`HEADER_ONLY_CPP_API`，并纳入完整审计候选分母。它自身不消费全局编译选项，
但其头文件会进入 consumer，所以在判断 consumer 时按 consumer 的实际实例化
结果处理。

同一源码名有多个 Release 或架构时先保留逐记录结果，再合并到源码名。任一记录
命中“需要改”时，源码包级不能标成“不需要改”；架构间或 Release 间结论不同则
进入冲突表，不用多数票覆盖。

### 1.3 当前已知候选不等于最终分母

R98 的 `runtime_signal_source_summary.tsv` 在 Base Toolchain 下有 38 条记录、
36 个去重源码名；其中 `patchelf`、`pcre` 各有两个 Release。这是按已发布包的
C++ runtime requirement 得到的候选，不证明已经覆盖只产出静态库、静态链接
ELF、或构建过程中使用 C++ 但最终产物没有动态 C++ runtime 依赖的源码包。

因此第二步先按 1.2 对 Base Toolchain 全部源码包建立精确 `BT_CPP`，再逐包分类。
R95 的 38 个去重源码名属于另一个项目，不能拿来补齐差额。

## 2. 证据等级和禁止事项

每个判定至少引用下列一种可复核证据；包名、用途印象或“通常如此”不算证据。

| 代码 | 证据 |
| --- | --- |
| E1 | 同轮实际构建日志中的编译器、语言和命令行 |
| E2 | 精确 revision 的 spec / 构建脚本中的工具链覆盖和硬编码 |
| E3 | 精确 RPM 内 ELF 的 `DT_NEEDED`、UND/DEF 交集及 demangle 结果 |
| E4 | 精确静态归档成员的符号、typeinfo、unwind 信息 |
| E5 | `dlopen`/插件注册配置及被装入同一进程的实际证据 |
| E6 | 精确镜像清单与进程/插件归属；镜像共存本身不等于同进程 |
| E7 | 精确源码/API/头文件中的参数、返回值、对象所有权、回调和异常边界 |
| E8 | repository metadata 中的构建期、运行期依赖；只证明拓扑候选，不单独证明 C++ 类型跨界 |

禁止从以下事实直接推导“需要改”或“不需要改”：

- 只因包是 C++；
- 只因两个包同时进入同一镜像；
- 只因存在 `DT_NEEDED`；
- 只因符号中未出现显式 `std::`；
- 只因当前没有观察到边；
- 把 R95 的标准 Base 结果无条件外推到 Base Toolchain。

## 3. 三类规则

### 3.1 `NEED_LIBCXX`（需要改用 libc++）

命中以下任一条：

- **N1：已证明的跨运行时布局边。** Base 包与计划迁移到 libc++ 的包之间，
  实际同进程调用边传递/返回/拥有布局敏感的标准库类型；包括已由 API 规则证明
  隐藏标准库布局的 Boost/第三方类型。
- **N2：C++ ABI 耦合但布局尚未排除。** 实际 UND/DEF、插件、回调、异常或对象
  所有权边使用 C++ ABI，而现有证据不能证明只传 C 数据。此类不因“未看到
  `std::`”下调为安全。
- **N3：同进程装载且存在 C++ 语义交换。** 包进入设备镜像，并有 E5/E6/E7
  证明它与 libc++ 代码同进程，且跨边界存在 C++ 对象、异常、allocator/new/delete
  所有权或 C++ 回调。单纯同镜像、甚至同进程但已证明为纯 C API，不命中 N3。
判定依据必须指向具体 consumer、provider、ELF 和符号/API。只存在构建依赖不命中。
运行期 consumer 数保留为影响规模字段，不再作为独立规则：一旦存在 C++ ABI
耦合且布局未被排除，N2 已经给出“需要改”判定；重复增加“广泛链接”规则不会
产生新的分类结果。

### 3.2 `NO_LIBCXX_NEEDED`（不需要改）

必须有正面隔离证据，并命中以下至少一条，同时不命中任何 N 规则：

- **O1：纯构建期工具。** 产物不进入所核设备镜像，不被设备运行时包依赖或动态
  加载，只在构建机以独立进程执行；输入输出为文件、命令行或已证明的 C 数据。
- **O2：Base 内部闭包。** 所有实际运行期 consumer/provider 均留在不迁移的
  Base 闭包内，没有通向 libc++ 迁移集合的运行期或动态加载边。
- **O3：已证明的纯 C 边。** 虽被 libc++ 包链接/加载，但 E3 与 E7 联合证明
  对外 ABI 只有 C 接口，跨界没有 C++ 对象、异常、所有权或 C++ 回调。
- **O4：独立进程边界。** 即使进入镜像或属于测试/诊断工具，也只以独立进程
  运行；与平台交换的是命令行、文件、socket/IPC 的稳定 C/字节协议，没有同进程
  C++ ABI 边。

`NO_LIBCXX_NEEDED` 不是“未发现问题”的默认值。若缺少排除动态加载、静态链接或
隐藏 C++ API 所需的材料，先记 `UNDETERMINED`。

### 3.3 `CANNOT_SWITCH_CURRENTLY`（当前不能改）

命中以下任一条：

- **C1：精确 spec/构建脚本显式选择 GCC。** 包定义
  `_toolchain_override gcc`，或硬编码目标 GCC/G++/GFortran，先机械归入“当前
  工具链约束”。这只表示当前配置不允许直接套用 libc++，不声称永远不能迁移。
- **C2：有受控构建证据证明 Clang/libc++ 不满足该包所需的语言、目标、插件、
  GCC runtime 或 binutils 集成要求。** 必须记录失败点，不能凭包名推断。
- **C3：包本身提供 GCC/libstdc++ ABI 或必须生成/测试该 ABI，切换后会改变其
  被验证的产品对象。** 必须由 spec 子包、安装内容和测试目标共同证明。

只有 `_toolchain_override gcc` 时，结果字段写
`CANNOT_SWITCH_CURRENTLY(C1_DECLARED)`；有 C2/C3 实测时再写更强的
`C2_PROVEN`/`C3_PROVIDER_ROLE`，避免把“当前声明”偷换成技术上永远不可能。

## 4. 规则冲突和证据不足

三类不采用简单覆盖优先级，按以下决策树执行：

1. 同时命中 N 与 C：记 `CONFLICT_NEED_BUT_CANNOT`，单独列出；不能用“不能改”
   掩盖实际边界，也不能无视工具链约束强行归为“需要改”。
2. 命中 N、未命中 C：`NEED_LIBCXX`。
3. 命中 C、未命中 N：`CANNOT_SWITCH_CURRENTLY`。
4. 未命中 N/C，但 O 的正面隔离条件完整：`NO_LIBCXX_NEEDED`。
5. N/C/O 均未被充分证明，或不同架构/Release 的证据冲突：
   `UNDETERMINED`/`CONFLICT_EVIDENCE`，列出所缺材料；不得强塞进“不需要改”。

`UNDETERMINED` 是审计状态而不是第四种处置方案。补齐证据后，每个包最终仍落入
人工要求的三类之一；若人工要求首轮表必须强制三分，则需另行指定“未知项”的
保守归类政策。

## 5. 计划迁移集合的定义

边界规则中的“将迁移到 libc++ 的代码”暂定义为：Unified Toolchain 中会消费
全局 `CXXFLAGS/LDFLAGS` 且没有 GCC 工具链约束的 C++ 源码包。具体集合要由
同一快照的实际构建日志、spec 和产物证据生成，不能把“项目内所有包”与“真正
编译 C++ 的包”混为一谈。

Unified 中显式 `_toolchain_override gcc` 的包属于例外集合；它们与 Base 包形成
的边仍记录，但不能预先假定其迁移后 runtime 身份。

## 6. 第二、三步将输出的字段

逐包表至少包含：

```text
source_name | exact_sourcerpm | arches | C++_evidence | image_membership
build_only_or_runtime | runtime_consumers | cpp_edge_class
toolchain_constraint | matched_rules | final_class | conflict_or_gap
evidence_paths
```

方案比较另建两张表：

1. 逐包加 spec：只统计 `NEED_LIBCXX`，并单列 `CONFLICT_NEED_BUT_CANNOT`；
2. Base Toolchain 全局注入：以项目全部源码包为暴露分母，不只统计 C++ 包；逐包
   判断构建系统是否实际消费 `CXXFLAGS/LDFLAGS`、实际 driver 是 GCC 还是 Clang。

driver 小探针只证明选项被 GCC/Clang 接受、拒绝或告警；不能替代某个包完整构建
系统是否传递/覆盖这些变量。未做完整包构建时，包级结果写 `NOT_OBSERVED`。

## 7. 人工确认结果

1. 是否接受 `BT_CPP` 使用“实际 C++ 编译动作或 ELF/静态归档 C++ 证据”的并集，
   而不是直接沿用 R95 的 38 个标准 Base 源码名？
2. 是否接受“同镜像/同进程本身不自动触发迁移，必须存在 C++ 语义交换”的 N3？
3. 是否接受 `_toolchain_override gcc` 先归为
   `CANNOT_SWITCH_CURRENTLY(C1_DECLARED)`，同时明确这不是永久不可迁移的结论？
4. 是否接受证据不足时先用审计状态 `UNDETERMINED`，而不是违反“禁止推断补全”
   将其默认塞进“不需要改”？

上述口径已于人工确认后生效；原待确认的“广泛链接”N4 按人工意见删除。

上述规则已用于本次逐包判定。
