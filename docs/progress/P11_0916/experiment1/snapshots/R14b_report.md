# R14b：双 unwinder 共载实验装置修复与分级实测

达到的最高级别：`L2`。L0 与 L1 的 A/B 四个门槛全部通过；L2 的 48/48 个同侧格全部正常退出且类型、析构次数、顺序均 PASS，因此本轮实验装置满足同侧有效性门槛。随后才执行 48 个跨侧格。总覆盖为 96/96 个有意义格（原始 128 格中合并 DT_NEEDED 不可控制的作用域重复，覆盖原始笛卡尔积 75%）。

本报告只陈述逐格观测，不将未出问题的格外推为“安全”，不做方案推荐或良恶判断。

## 1. 实验输入与隔离

实验在 x86_64 宿主原生运行，全部实验二进制和运行时副本位于 `tmp/R14b/`，依靠逐方案 `LD_LIBRARY_PATH` 隔离，未安装到系统路径、未修改 ld.so 配置。

- 方案 A：本项目 c2 `libc++abi-22.1.8-1.x86_64.rpm`，SHA256 `c141c89a8d7781cb0f06f035c73d56d4bf40fbedc54385767ab83afe8f0a151d`，加平台 `libgcc-14.2.0-1.13.x86_64.rpm`。
- 方案 B：平台 `libc++-22.1.8-19.1.x86_64.rpm`，SHA256 `d26445e70789d212f9eb59d31f36cad3aa3eae051f7f9712ec0aab118cf3bfe7`，取其中 libc++abi 与 LLVM libunwind。
- legacy：平台 libstdc++ 14.2.0 与同一平台 libgcc_s。

NEVRA、RPM SHA、运行时副本 SHA 原文见 `L0/commands/01_input_rpm_identity.log`、`L0/commands/04_runtime_hashes.log` 与 `L2/static/02_hashes.log`。

装置修复采用预先冻结的一种方式：所有 C++ 实验件由 `g++` 驱动链接，以 `-nodefaultlibs` 禁止隐式 C++ 运行时，再显式列出目标 libc++abi/libunwind 或 libstdc++/libgcc_s。没有在失败后切换链接法。与上一轮 raw `ld -shared` 装置不同，本轮 DSO 实测同时具有 `.eh_frame_hdr`、`.eh_frame` 和 `GNU_EH_FRAME` 所需布局；构建命令和断言原文见 `L2/commands/01_build_l2.log`。

## 2. L0：单方案、单二进制

A 与 B 均只执行一次正式运行，且每次均带 `LD_DEBUG=bindings,libs`：

- catch 命中内建类型 `int`，值 42；
- inner、rethrow_guard、outer 三个析构对象各执行恰一次，顺序符合代码事件序列；
- 进程正常退出 0；
- A 的 `_Unwind_RaiseException`、`_Unwind_Resume`、`_Unwind_DeleteException` 绑定到隔离目录内 libgcc_s；B 对应绑定到隔离目录内 LLVM libunwind。

完整编译/链接命令见 `L0/commands/05_compile_A.log` 至 `08_link_B.log`，逐方案输出与动态绑定见 `L0/runtime/16_run_A_ld_debug.log`、`18_run_B_ld_debug.log`，结构与 UND 原文见 `L0/static/`，汇总见 `L0/result.tsv`。

## 3. L1：单方案、跨 DSO

A、B 均由主程序调用一个 DSO，异常在 DSO 中 throw、catch(...) 后 rethrow，并跨 DSO 由主程序 catch(int)。两方案均捕获值 73，DSO 三个析构合计 3，退出 0。

typeinfo 处理方式固定为内建 `int`：没有用户定义类型、没有 key function；DSO 以 `-fvisibility=hidden` 构建，仅 C 入口显式 default visibility。静态实测 A 的 DSO/主程序 `_ZTIi@LLVM_22`，B 为无版本 `_ZTIi`。完整命令见 `L1/commands/`，结果见 `L1/result.tsv` 与 `L1/runtime/`。L1 全绿后才进入 L2。

## 4. L2 覆盖与同侧门

矩阵维度为方案 A/B × DT_NEEDED/dlopen × legacy-first/new-first × GLOBAL/LOCAL × lazy/LD_BIND_NOW × 四个异常方向。DT_NEEDED 在进程启动时已全局装载，无法按 `RTLD_LOCAL/GLOBAL` 重复改变作用域，故每个 DT_NEEDED 格只执行 `scope=N/A`；dlopen 保留两个作用域。实际为每方案 48 格、合计 96 格。覆盖申报见 `L2/tables/coverage.tsv`。

执行严格分两段：先只运行 48 个 `new→new`/`legacy→legacy` 同侧格。`L2/commands/06_run_same_side_gate.log` 实测 `SAME_SIDE_GATE_FAILED=0`：A 24/24、B 24/24 全部正常退出 0，类型、析构数与顺序均 PASS。只有该命令退出 0 后才运行跨侧 48 格，见 `L2/commands/07_run_cross_side.log`。

## 5. L2 跨侧逐格结果

方案 B 的 24 个跨侧格：

- DT_NEEDED 8/8 正常退出 0；
- dlopen + RTLD_GLOBAL 8/8 正常退出 0；
- dlopen + RTLD_LOCAL 8/8 以信号终止：`new→legacy` 4/4 为 SIGABRT，`legacy→new` 4/4 为 SIGSEGV；lazy 与 LD_BIND_NOW 结果相同；
- 16 个正常格的类型、析构数与顺序均 PASS；8 个信号格只观察到终止前事件，catch 结果为 `NOT_OBSERVED`。宿主 core soft limit 为 0，故没有 core 文件。

方案 A 对照的 24 个跨侧格均为正常退出 1：24/24 析构数 PASS，但 catch 落到 `catch(...)`，类型与事件顺序检查均 FAIL；没有 terminate、SIGABRT 或 SIGSEGV。静态记录同时显示 new/A 侧引用 `_ZTIi@LLVM_22`，legacy 侧引用 `_ZTIi@CXXABI_1.3`；本报告仅并列这两项实测，不补写因果结论。

逐格完整结果为 `L2/tables/matrix_all.tsv`，A/B 对照为 `L2/tables/scheme_A_B_comparison.tsv`，汇总为 `L2/tables/outcome_summary.tsv`。每格的程序 stdout/stderr、LD_DEBUG 原文、PWD、完整命令、退出码、信号和 core 状态均位于 `L2/raw/` 与 `L2/commands/matrix_*`。

## 6. 符号实际落点

每格均设置 `LD_DEBUG=bindings,versions`；另用不改写返回地址的 LD_AUDIT 观察器记录 lazy PLT 的实际调用。四个目标符号逐格落点见 `L2/tables/symbol_landing.tsv`，全部 1,384 条目标动态绑定见 `L2/tables/ld_debug_bindings.tsv`，逐格摘要见 `L2/tables/ld_debug_binding_summary.tsv`。

实际观测事实：

- lazy 的 48 格中，28 格对 raise/resume/delete 三者形成完整的同实现调用观测，20 格只形成部分调用观测；所有已观察到的同一异常调用均落在单一实现，未观察到一次异常内部从 libgcc_s 切到 LLVM libunwind或反向切换。
- LD_BIND_NOW 的 48 格由 LD_DEBUG 完整记录绑定，但 audit 的 PLT-call 钩子没有记录实际调用，故“实际调用”栏严格记 `NOT_OBSERVED`，没有用 eager binding 冒充调用。
- B、legacy-first 且全局可见时，new DSO 的 `__cxa_throw` 实测绑定到 libstdc++，new 侧的展开调用落到 libgcc_s；B、new-first 且全局可见时，legacy DSO 的 `__cxa_throw` 可绑定到 libc++abi。
- 在两个 lazy、new-first、全局可见、实际执行 `legacy→new` 的格（C006、C018）中，LD_DEBUG 与 audit 均记录 legacy DSO 的 `_Unwind_Resume@GCC_3.0` 实际提供者为 LLVM `libunwind.so.1`。这是本矩阵实测落点，不扩展为其他未执行调用的结论。
- RTLD_LOCAL 跨侧的 8 个 B 格保持各自局部运行时落点，并在跨侧 catch 前发生上述 SIGABRT/SIGSEGV。
- `_Unwind_ForcedUnwind` 在 96/96 格均 `NOT_OBSERVED`。

## 7. 覆盖与未覆盖面

本轮覆盖普通 `throw int`、内部 catch/rethrow、两个 cleanup 对象、跨 DSO catch，涵盖 DT_NEEDED/dlopen、两种加载顺序、dlopen 两种作用域、lazy/LD_BIND_NOW 和四个方向。未覆盖 forced unwind、线程取消、嵌套异常、异常规格/foreign exception、跨线程展开、卸载后展开、压力/并发、armv7l EHABI 与板端运行。

LD_BIND_NOW 格只观测实际绑定和程序行为，未由 audit 观测实际 PLT 调用；信号格未生成 core，故无 backtrace。某格正常退出只说明该格中的这一次事件序列，没有形成“安全”断言。

## 8. 观测参数偏差申报

审计时发现，L0/L1 的四次正式运行使用了 `LD_DEBUG=bindings,libs`，没有按总观测条款使用 `LD_DEBUG=bindings,versions`；这是执行偏差。四次均有动态绑定原文，且 ELF 的版本引用另由 readelf 静态原文保存，但不能冒充缺失的动态 `versions` 输出。发现时 L2 已完成；为避免事后补跑掩盖原始执行序列，本次没有重跑 L0/L1。L2 的 96 格均实际使用 `LD_DEBUG=bindings,versions`。该偏差保留交人工裁决。
