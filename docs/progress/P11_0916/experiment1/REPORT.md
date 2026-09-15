# 异常运行时落点与配置对照

## 结论

本轮观测完成，**包含确定失败，不代表兼容性通过**。x86_64 原生；armv7l 为 QEMU 用户态，未用物理板。程序与库均在隔离目录中使用，没有安装或替换平台库。

1. 原 `BoundaryError{77}` 夹具在两架构各五次普通运行及五次调试观察中仍退出 12。**不是 foreign 异常路径，而是类型身份/匹配失败**：throw 与 personality 都进入 libc++abi；异常标识是 LLVM 的 `0x434c4e47432b2b00`；typed-catch 匹配确实执行。捕获侧与抛出侧类型名同为 `13BoundaryError`，但 RTTI 对象及名字地址不同；ARM 的 `can_catch` 返回 false。x86_64 的返回断点未取得返回值，不能冒充该项直接观测。
2. 正向及干预对照进一步限定原因：x86_64 的 GNU 同侧夹具 5/5 捕获 77、guard=1；LLVM 同侧但保持原符号导出条件的夹具也 5/5 退出 12。**故原失败不能写成混用两套标准库才会发生的普遍规律。** 只在隔离用例的链接中加 `--export-dynamic`，跨库调用 5/5 捕获 77、guard=1；另五次轨迹显示两侧 RTTI 与名字地址已相同。没有改宏或改运行库源码，也没有把该链接选项作为产品处置建议。
3. **foreign 是另一个确实存在的机制，已在另一夹具确认**：版本隔离的 GNU→LLVM `throw int` 路径中，GNU `__cxa_throw` 产生 `0x474e5543432b2b00`，实际传给 libc++abi personality，最终仅 catch-all。不能拿这个因果链替代上一条 `BoundaryError` 的因果链。
4. `LIBCXX_CXX_ABI=libsupc++` 已隔离构建并实测，提升为**关键路径候选**有数据依据；**不能因此关掉整条异常契约**。尤其 ARM 的 GNU ABI + 预加载 LLVM 展开器在普通 C 边界对照通过后，跨界异常 5/5 SIGABRT，构成实测限制。

## 一、原异常的事实链与可反驳点

原始二进制来自既有共存夹具；源码、旧构建报告、实际二进制与运行库摘要见 [IDENTITY.json](IDENTITY.json)、[快照](snapshots/R78_report.md)。未重新编译原失败二进制；新增对照另有文件名和构建记录。

**事实链（实测）**：插件调用 throw → `__cxa_throw` 在 libc++abi → GNU 展开器收到 LLVM 异常标识 → libc++abi personality 收到同一标识 → 进入 `__class_type_info::can_catch` → 同名类型的两个 RTTI/名字地址不同 → ARM 匹配返回 false → catch-all/退出 12。两架构各 5/5；原始 [逐次记录](R78_OBSERVATIONS.json) 包含每个 PC、所属 DSO、参数与结果。例：[x86_64](raw/r78_x86_64_1_trace.stdout)、[armv7l](raw/r78_armv7l_1_trace.stdout)。

**静态核查**：`cxa_exception.h` 的 LLVM 标识常量、`cxa_personality.cpp` 的 native 判定与 typed-catch 分支、`private_typeinfo.cpp` 的 `is_equal`/`can_catch`、`typeinfo` 中 unique-name 指针比较均归档在 snapshots。源码只作为对应实现解释，实际落点不是从链接参数猜出。

**已排除**：该固定失败是“GNU 异常被 LLVM personality 当作 foreign”这一解释；以及“类型名相同就必然匹配”。同侧 LLVM 失败和导出条件对照还排除了把失败仅归结于标准库品牌不同的解释。对照见 [CONTROLS.json](CONTROLS.json)、[RTTI 统一后的实际轨迹](raw/export_trace_r1.stdout)；后者另有 r2–r5。

**反驳需要什么**：在所列相同二进制/库、相同加载条件下，取得相反的 throw/personality/异常标识或类型匹配轨迹；或指出调试器参数读取无效。其他加载方式有不同结果，会改变适用范围，不会自动否定这组固定样本。

## 二、96 格到底测了什么

原始 96 格是 **x86_64** 的运行库方案 A/B × 装载方式/顺序/作用域/绑定时机 × 异常方向。两方案均使用 libc++abi；A 为版本隔离的 libc++abi + GNU 展开器，B 为另一套 libc++abi + LLVM libunwind 与 GNU 侧共存。**并不是 `LIBCXX_CXX_ABI=libcxxabi/libsupc++` 两设置的 96 格，也不是 ARM 矩阵。** 编译器为 g++，通过 `-nodefaultlibs` 显式链接 ABI 库；夹具传递 int，不是完整 libc++ 对象模型测试。[原构建命令](snapshots/R14b_build_l2.log)、[原矩阵](snapshots/R14b_matrix_all.tsv)、[原报告](snapshots/R14b_report.md)。

本轮沿用原二进制，先过同侧门，再跨侧，每格 5 次，共 480 次：

| 范围 | 本轮结果 | 精确断言 |
| --- | --- | --- |
| 48 个同侧格 | 240/240 退出 0 | 类型、cleanup_count、顺序均 PASS |
| A 的 24 个跨侧格 | 120/120 退出 1 | catch-all；cleanup_count 通过，类型和顺序失败 |
| B 的 16 个全局可见跨侧格 | 80/80 退出 0 | 类型、cleanup_count、顺序均 PASS |
| B 的 8 个局部作用域跨侧格 | 40/40 信号终止：15 SIGABRT、25 SIGSEGV | 未完成的 catch/清理判据不能记通过 |

有 **5 次的信号种类**与对应历史单次记录不同；没有将历史 4/4 SIGABRT 当作本轮不变事实。具体到格的结果与全部命令在 [R14B_REPEATS.json](R14B_REPEATS.json)、[分组表](R14B_SUMMARY.json)。

另选 A/B × 两个全局加载顺序 × new→new/legacy→new，共 8 格各 5 次，记录实际 throw、personality、RaiseException 落点与标识：[R14B_TRACES.json](R14B_TRACES.json)。A 的 GNU→LLVM 路径实测 GNU 标识进入 LLVM personality；B 全局路径随加载顺序由 GNU 或 LLVM ABI 实现处理，typed catch 通过。两种候选成因不能只从 catch-all 的外观区分，必须看这一层轨迹。

## 三、两种 ABI 构建与两种展开器设置

现有 libcxxabi 版采用原夹具库及对应 CMakeCache。libsupc++ 版使用只读平台 LLVM 源码，在 `tmp/P11_0916/build-supc-clang` / `build-supc-arm` 构建 libc++，缓存明确为 `LIBCXX_CXX_ABI=libsupc++`。使用平台 Clang 22 包装器及 GCC 14 的 ABI 头和 `libsupc++.a`；**GNU ABI 实现被链接进实验 libc++.so，不是声称存在一个独立的 libsupc++.so。** [x86 缓存](snapshots/supc_CMakeCache.txt)、[ARM 缓存](snapshots/arm_supc_CMakeCache.txt)。

两种展开器设置是：显式链接 GNU libgcc_s 的默认运行、进程级 `LD_PRELOAD` 指定真实 LLVM libunwind 的运行。**后者仍可能同时映射 libgcc_s；不是宣称系统已改成单展开器，也不是将 libsupc++ 伪装成有 `LIBCXXABI_USE_LLVM_UNWINDER` 开关。** 实际 RaiseException 落点由调试器核对；没有仅据参数命名判定。旧 libcxxabi 与新 libsupc++ 并非只改一个位的同产物构建，版本/构建与导出差异在快照中保留，不能声称已经排除全部混杂因素。

每格五次，使用原 `BoundaryError{77}`/guard 判据：

| 架构/环境 | libcxxabi + GNU | libcxxabi + LLVM 预加载 | libsupc++ + GNU | libsupc++ + LLVM 预加载 |
| --- | --- | --- | --- | --- |
| x86_64 原生 | catch-all，12，5/5 | catch-all，12，5/5 | typed=77、guard=1，5/5 | typed=77、guard=1，5/5 |
| armv7l QEMU 用户态 | catch-all，12，5/5 | catch-all，12，5/5 | typed=77、guard=1，5/5 | SIGABRT，5/5；typed/最终清理未完成 |

这八格异常运行另有八格普通 C 参数缓冲区对照，全部断言 `old:ping`、长度 8、guard=1，各 5/5。两架构合计 **80 次程序运行**，异常另有 **40 次初始调试轨迹**；ARM LLVM 预加载格另补 **10 次修正轨迹**，不把这些重复混算成新配置格。原始 [x86 数据](ABI_CONFIG_RESULTS.json)、[ARM 数据](ARM_ABI_CONFIG_RESULTS.json)、[ARM 校验](ARM_SUMMARY.json)。

ARM 失败的实际诊断是 `personality function returned unknown result 5`，随后 `_Unwind_Resume() can't return`。无调试器的五次程序均因 SIGABRT 终止；调试器则停在 SIGABRT，`$_exitcode` 为 void，不把 gdb 的退出 0 误记成程序正常退出。[原 stderr](raw/arm_config_libsupcpp_LLVM-preload_exception_1.stderr)、[修正轨迹](raw/arm_corrected_libsupcpp_1.stdout)。此处不推断未定位的内部根因，不修展开器。

**标识表示须区分架构**：LLVM 样本读取的原始字节为 `00 2b 2b 43 47 4e 4c 43`；GNU x86_64 为 `00 2b 2b 43 43 55 4e 47`。GNU ARM 观察到 `47 4e 55 43 43 2b 2b 00`，按小端数打印为 `0x002b2b4343554e47`。不将这个字节表示差别伪造为又一种来源，完整原字节与所属 personality 同时保留。

## 四、为何不能据此关闭异常契约

**事实链**：实际 libsupc++ 构建能让固定 BoundaryError 在 GNU 展开器下两架构通过 → x86_64 预加载 LLVM 也通过 → ARM 对应配置却中止 → 单个设置不是所有部署组合的充分条件。

**已排除的过强结论**：“只切 ABI 库就可以允许任意跨库异常”“96 格早已验证所有 libsupc++ 配置”“展开器选择不影响本问题”。

**反驳需要什么**：在明确版本、加载顺序与作用域、架构下，证明实际 throw/catch/delete/展开路径与对象契约一致，并解释和解决该 ARM 失败；还要覆盖含标准库成员/基类的异常、嵌套/重抛、取消、其他旧二进制及卸载等目前未覆盖场景。统一 ABI 运行时最多消除满足其前提时的 foreign 来源问题，不能自动统一任意类型身份、布局或生命周期。

## 五、方法、偏差与自行判断

- 新构建、薄 C maps 观察器和测试二进制仅在 tmp；源码不变。maps 观察器无 C++ 依赖，在 ARM 正常退出时保存 `/proc/self/maps`；失败路径另由暂停时准确识别的 QEMU 进程 `/proc/<pid>/maps` 补核，并明确它是宿主 QEMU 地址映射。早期 ARM GDB 的 `info sharedlibrary` 不冒充 maps。
- **观测错误已纠正**：初始 10 次 ARM LLVM RaiseException 源码行断点处，r0 已过函数序言，读取出的“header”不是可靠入参，标为 `INVALID_MEASUREMENT`，不得用于因果判定。修正后尝试 DWARF 参数仍不可得，此处为 `NOT_OBSERVED`；在随后的 personality 上直接读取到同一异常头的可靠字节。原记录不删，旧脚本在 `snapshots/trace_register_v1.gdb`，修正数据见 [ARM_CORRECTED_TRACES.json](ARM_CORRECTED_TRACES.json)。
- 构建尝试中：宿主 GCC 13 不支持源码所用 builtin；换现有 Clang 22 后曾缺显式 GNU 展开库；ARM C 编译器驱动模式与 Ninja 跨编译 RPATH 生成曾报错。只修隔离构建参数并保存所有失败，不把这些失败解释成 libsupc++ 机制不成立。
- 为观察 ARM，下载并解包 gdb-multiarch 到 tmp，未系统安装。QEMU 调试服务随每个短命实验进程退出，无板子占用。正向、反例与源码摘录均保留；脚本和快照 SHA256 最终封存。
- 自行判断：复用既有两架构夹具；两 ABI 构建对照用默认 GNU/进程预加载 LLVM 两种设置并明示范围；增加 GNU 同侧、LLVM 同侧及导出条件对照以辨别类型身份原因。没有新增 `_LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION=2` 或平台补丁实验。
- 未覆盖：物理 ARM 板、aarch64、完整 libc++abi/libsupc++ 平台整包迁移、异常全集、部署兼容性与取消矩阵。静态/运行时诊断足以区分本次两条因果链，**不足以宣告跨标准库异常普遍可用**。
