# R14：x86_64 双 unwinder 共载实验

状态：`RED_STOP_INVALID_SAME_SIDE_CONTROL`。实验件静态身份符合 A/B 输入定义，96 个有意义格均已实际启动；但 48/48 个同侧对照格也全部在首次 `THROW` 后以 `SIGABRT` 终止，catch、rethrow 和两个析构事件均未出现。因此 fail-closed 红停，既有运行不能用于裁决双 unwinder 共载行为。没有修改链接命令、没有构造第二版实验件、没有重跑替代矩阵。

## 1. 输入与实验件

平台方案 B 输入为 `libc++-0:22.1.8-19.1.x86_64`，RPM SHA256 `d26445e70789d212f9eb59d31f36cad3aa3eae051f7f9712ec0aab118cf3bfe7`；legacy 输入为平台 `libstdc++-0:14.2.0-1.13.x86_64` 与 `libgcc-0:14.2.0-1.13.x86_64`。方案 A 输入为本项目 c2 的 `libc++abi-0:22.1.8-1.x86_64`，RPM SHA256 `c141c89a8d7781cb0f06f035c73d56d4bf40fbedc54385767ab83afe8f0a151d`，并使用同一平台 libgcc。五个 RPM 的 NEVRA、实测 SHA 与冻结值均 `MATCH`，见 `tables/input_rpm_identity.tsv`。

实验在宿主 x86_64 原生构造，所有二进制、解包库和逐格工作目录只落 `tmp/R14/`，没有安装到系统目录，也没有修改 `ld.so` 配置。完整编译/链接命令见 `commands/01_build_experiment.log`。new 与 legacy DSO 使用同一异常类型，均包含 throw、内部 catch 后 rethrow、inner/outer 两个析构对象以及外部 catch；主程序以回调逐事件记录类型、析构次数和顺序。观察器用 `LD_AUDIT` 的 `la_symbind64` 与 `la_x86_64_gnu_pltenter` 记录四个目标符号的绑定和实际 PLT 调用，并原样返回提供者地址，不改写符号落点；源文见 `experiment_sources/audit.c`。

静态构成实测如下：

- B/new DSO 的 DT_NEEDED 为 `libc++abi.so.1`、`libunwind.so.1`、`libc.so.6`，其 `_Unwind_Resume` UND 无版本。
- A/new DSO 的 DT_NEEDED 含 `libc++abi.so.1`、`libgcc_s.so.1`，其 `_Unwind_Resume@GCC_3.0`。
- legacy DSO 的 DT_NEEDED 含 `libstdc++.so.6`、`libgcc_s.so.1`，其 `_Unwind_Resume@GCC_3.0`。
- B/libc++abi 的 `_Unwind_*` UND 无版本；A/libc++abi 与平台 libstdc++ 的对应 UND 为 `GCC_*` 版本化引用。

逐 ELF SHA、DT_NEEDED 与 UND 原文分别见 `tables/component_identity.tsv`、`tables/component_dt_needed.tsv`、`tables/component_unwind_undefined.tsv` 和 `commands/04_inspect_experiment_elf.log`。

## 2. 矩阵覆盖

原始笛卡尔积为 2 方案 × 2 加载方式 × 2 顺序 × 2 作用域 × 2 绑定时机 × 4 异常方向 = 128 格。DT_NEEDED 的 `RTLD_GLOBAL/RTLD_LOCAL` 不是运行时可控维度，故该分支合并为 `scope=N/A`；dlopen 分支完整保留两种作用域。实际执行 96/128 格（75% 原始笛卡尔积），亦即 96/96 个有意义格：每方案 DT_NEEDED 16 格、dlopen 32 格。每格均有独立命令、PWD、退出码、信号、core 状态、程序输出、`LD_DEBUG=bindings,versions` 原文和 audit 记录。

逐格账本见 `tables/matrix_execution.tsv`，逐格判定见 `tables/cell_determinations.tsv`，覆盖口径见 `tables/coverage.tsv`。

## 3. fail-closed 红项

96/96 格均为 `SIGABRT`；A 为 48/48，B 为 48/48。四种异常方向在每方案均为 12/12 `SIGABRT`。其中本应建立实验有效性的同侧对照 new→new 与 legacy→legacy 共 48 格，也为 48/48 `SIGABRT`、0/48 正常退出。

程序事件原文共 96 条，每格仅观察到一条 `THROW:new` 或 `THROW:legacy`；`DTOR:*:inner`、`RETHROW:*`、`DTOR:*:outer` 与 `CATCH:*` 均为 `NOT_OBSERVED`。因此类型命中、析构次数和析构顺序均不可判定。宿主 core soft limit 为 0，96 格均无 core 文件；终止文本和信号仍逐格保留。

只读结构取证显示，四个实验侧 DSO 有 `.eh_frame` 与 `.gcc_except_table`，但没有 `.eh_frame_hdr`，program headers 中也没有 `GNU_EH_FRAME`；两份输入 libc++abi 均有 `.eh_frame_hdr` 和 `GNU_EH_FRAME`。这是实测到的构造差异，见 `commands/06_control_failure_readonly_inspection.log`；本任务没有通过修改/重跑验证其因果性，故原因记 `UNDETERMINED_CONSTRUCTION_DIFFERENCE_OBSERVED`，不得补成已证实根因。

## 4. 红停前的有限符号观测

`LD_DEBUG` 共记录 552 条目标符号绑定；其中 eager binding 不能等同于实际调用。audit 共记录 624 条目标事件，其中 72 条为实际 PLT call。因所有有效性对照失败，以下数据只作原文登记，不用于共载裁决：

- A：48 格中有 24 格实际观察到 `_Unwind_RaiseException` 由隔离目录内 `libgcc_s.so.1` 提供，另 24 格未由 audit 观察到该调用。
- B：48 格中 12 格观察到 `_Unwind_RaiseException` 由 `libgcc_s.so.1` 提供，12 格由 LLVM `libunwind.so.1` 提供，24 格未由 audit 观察到该调用。
- 两方案全部 96 格中，`_Unwind_Resume`、`_Unwind_DeleteException`、`_Unwind_ForcedUnwind` 的实际调用均 `NOT_OBSERVED`。

逐事件、逐绑定和逐格四符号表见 `tables/audit_unwind_events.tsv`、`tables/ld_debug_unwind_bindings.tsv`、`tables/cell_symbol_observations.tsv` 与 `tables/actual_call_provider_summary.tsv`。

## 5. 判定边界

本次没有形成“同一次异常的 raise/resume/delete 是否来自同一实现”的有效观测；legacy 版本化引用的运行稳定性、新侧无版本引用按加载顺序/作用域的最终行为、跨侧 catch、析构遗漏或重复释放均记 `NOT_OBSERVED_INVALID_SAME_SIDE_CONTROL`。只观察到 terminate/SIGABRT，没有观察到段错误或 core；由于异常未进入 cleanup/catch，这不能外推为内存安全结论。

某格未出现某类问题不能解释为安全；本次 96 格只覆盖了加载和首次 throw 之前/当时的动态绑定，未覆盖一次完整异常展开。报告不做方案推荐，不判良恶。
