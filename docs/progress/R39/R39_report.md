# R39：R36 与 R38 结果分歧定位诊断

## 结论

结论为 **SETUP_DIFFERENCE**。

R36 与 R38 实际加载的实验版 `libc++abi.so.1` 逐字节相同，SHA256 均为 `21a7bae558296c19ac3e9762c26ac6bd3dc0e218a7f7be245ec3fb4b8b1cb598`，`cmp` 退出 0（`commands/006_experiment_identity_comparison.log:3-15`）。分歧来自测试件的动态符号可见性：

- R36 测试件把 `_ZTIN10__cxxabiv115__forced_unwindE` 与 `_ZTSN10__cxxabiv115__forced_unwindE` 作为 `WEAK DEFAULT` 符号放入 `.dynsym`；运行时，实验版 libc++abi 对这两个符号的引用均绑定到 R36 主程序（`commands/007_typeinfo_static_symbols.log:3-9`；`commands/023_board_binding_observations.log:1658-1670`）。结果是 `forced-handler` 命中，然后在重抛路径退出 134（同日志 `2074-2132`）。
- R38 原测试件虽在普通符号表中含有同名 `WEAK DEFAULT` 定义，但 `.dynsym` 零命中；实验版 libc++abi 因而绑定到自身定义（`commands/007_typeinfo_static_symbols.log:10-12`；`commands/023_board_binding_observations.log:3960-3974`）。LSDA 使用的测试件 typeinfo 与 personality 使用的共享库 typeinfo 未统一，运行结果落入 `catch-all` 并由 glibc 报 `FATAL: exception not rethrown`，退出 134（同日志 `4465-4482`）。
- 决定性控制保持 R38 的原对象文件、静态 `libc++.a`、实验 libc++abi 与全部库不变，只在链接时增加 `--export-dynamic`（完整命令见 `commands/041_relink_export_dynamic_control.log:3-11`）。控制件的两个 typeinfo 符号进入 `.dynsym`（同日志 `50-55`）；板上加载器随即把 libc++abi 的两处引用绑定到控制件，运行从 `catch-all` 改为 `forced-handler`，之后与 R36 一样在重抛阶段退出 134（`commands/045_board_export_control_observation.log:1827-1839,2327-2391`）。

因此，R38 的“映射未生效”不是 ARM 映射随机失效；它是测试件没有导出 typeinfo、导致运行时类型身份未统一的实验搭建差异。消除该差异后，armv7l 的实测形态回到 R36：**映射生效、forced handler 命中、卡在重抛**。本任务不实现重抛修法。

## 1. 两次实验对象身份

完整身份表见 `object_identity.tsv`，完整链接命令原文已复制到 `code/r36/evidence/028_compile_armv7l_probes.log` 与 `code/r38/evidence/022_compile_strict_probes_with_ehabi_field_map.log`。

| 项目 | R36 | R38 | 是否相同 |
|---|---|---|---|
| 实验 libc++abi | `tmp/R36/build-armv7l/lib/libc++abi.so.1.0` | `tmp/R38/probes-armv7l/patched/libc++abi.so.1.0` | 是，SHA 相同且 `cmp=0` |
| 测试件 | `s1_libcxxabi`，SHA `664c650c…` | `s1_libcxx_strict`，SHA `933bb068…` | 否 |
| 测试源码 | 无标准库依赖的自定义 `RealException` 变体 | R34-B 原始 `std::runtime_error` 变体 | 否 |
| libc++ 链接 | 没有链接 `libc++.so` 或 `libc++.a` | 静态链接 `libc++.a` | 否 |
| DT_NEEDED libc++ | 无 | 无（静态） | 表面相同、成因不同 |
| DT_NEEDED libc++abi | 有 | 有 | 是 |

R36 的完整命令明确使用 `-nostdlib++` 后只链接 `-lc++abi`，没有 libc++ 实现库（`progress/R36/commands/028_compile_armv7l_probes.log:2`）。R38 的完整命令在 `-nostdlib++` 后显式传入 `libc++.a`（`progress/R38/commands/022_compile_strict_probes_with_ehabi_field_map.log:2,12`）。两者 DT_NEEDED 原文见 `commands/006_experiment_identity_comparison.log:16-90`。

R36 实验库的 CMake 配置为 armv7l、`RelWithDebInfo`、`LIBCXXABI_USE_LLVM_UNWINDER=OFF`、shared ON、static OFF，完整配置位于 `code/r36/build/CMakeCache.patched-armv7l.txt`。R38 复用了这份实验库；`code/r38/build/CMakeCache.unpatched-armv7l.txt` 仅对应 R38 的未打补丁对照库，不是 R38 patched 运行所用库。

## 2. typeinfo 唯一性与运行时解析

静态符号与运行时绑定的全量结构化结果分别见 `typeinfo_symbols.tsv` 和 `runtime_binding.tsv`。

嫌疑中“静态 `libc++.a` 自身又携带一份 forced-unwind typeinfo”未被实测支持：对该归档执行 `nm -A -C` 为零命中（`commands/007_typeinfo_static_symbols.log:27-28`）。实际问题是测试目标自身由 catch 类型产生了一份 typeinfo：

- R38 测试件中的定义只在 `.symtab`，运行时不能参与共享库的全局符号解析；libc++abi 保留自身定义。进程内存在两个未被动态合一的有效地址来源，`can_catch` 未匹配。
- R36 测试件中的定义位于 `.dynsym`；ELF 全局符号抢占使 libc++abi 的引用解析到主程序定义，personality 与 LSDA 使用同一运行时定义，`can_catch` 匹配。

这里的“唯一”按 **运行时类型身份是否被解析为同一定义** 计，不是仅按磁盘文件内是否出现同名字符串计。R38 原件不唯一；R36 与 `--export-dynamic` 控制件均被动态链接器统一到主程序定义。

## 3. 改变链接形态的对照

逐格结果见 `linkage_comparison.tsv`。

### 动态 libc++ 尝试

按任务要求尝试构造动态 libc++ 等价测试件，但该格未获得业务行为结果：

1. 直接使用冻结 RPM `libc++-22.1.8-1.armv7l` 时，发布库要求 `LLVM_22` 版本节点，而实验 libc++abi 未使用发布版版本脚本；链接退出 1（`commands/027_relink_dynamic_control.log`）。
2. 用 R38 同一 `libc++.a` 仅重链为测试用动态 `libc++.so.1` 后，等价测试件成功生成，DT_NEEDED 同时含 `libc++.so.1` 与同一实验 `libc++abi.so.1`（`commands/033_relink_dynamic_generated_v2.log:7-100`）。运行时 typeinfo 已绑定到主程序，但临时共享库含板上加载器不接受的 ARM relocation `0x03`，在进入 `main` 前退出 127；cancel、handler 与重抛均为 `NOT_OBSERVED`（`commands/038_board_dynamic_binding_observation.log:2175-2190,2270-2272`）。

因此“动态 libc++ 格”的行为结论为 `NOT_OBSERVED`，没有拿装载前绑定替代业务行为。

### 单变量动态导出控制

为隔离决定性变量，另做 `--export-dynamic` 控制。它与 R38 原件共享同一个对象 SHA `84bd7366…`、同一个静态 libc++ SHA `f15e1c7b…` 和同一实验 libc++abi；唯一申报变化为链接器选项 `--export-dynamic`。该格取得完整 cancel 行为，证明“符号动态可见性 → 运行时 typeinfo 统一 → forced handler 命中”的因果链。

## 4. personality 直接断点

状态：`NOT_REQUIRED_AFTER_DECISIVE_CONTROL`。

第 2、3 步已用同板 `LD_DEBUG=bindings,symbols` 和单变量控制闭合成因；没有为观测修改 libc++abi，也没有新增第四处改动。按任务的条件式要求，没有继续引入调试器附加变量。

## 5. 资源门禁、载体异常与证据有效性

独立门禁脚本为 `tools/resource_gate.sh`，已先通过 `bash -n` 和干跑（`commands/002_script_syntax.log`、`commands/003_resource_gate_dry_run.log`）。每次重链均由上层脚本显式读取退出码，非零路径打印 `RED_STOP_RESOURCE` 并退出；三次实际门禁均 PASS：`commands/025_resource_gate_before_relink.log`、`028_resource_gate_before_generated_dynamic_relink.log`、`031_resource_gate_before_generated_dynamic_relink_v2.log`、`039_resource_gate_before_export_dynamic_control.log`。所有链接命令均为 `nice -n 15 ionice -c 3`，并申报并行度 2；本任务未重建 libc++abi。

传输阶段发现 SDB 宿主退出码不传播远端命令失败。本任务修正为每条远端命令打印并解析 `REMOTE_COMMAND_EXIT_CODE`，并剥离 SDB 的 CRLF。此前失败、无效或仅部分有效的尝试均保留在 `attempts_and_validity.tsv`，未用于结论。

## 6. 未排除变量与边界

- 板上 `libgcc_s` 为 `14.2.0-1.14.armv7l`，R34-B 宿主为 `14.2.0-1.13.x86_64`；本任务没有比较不同架构实现对重抛失败的贡献。该变量不影响本任务已闭合的“映射是否命中”分歧。
- 动态 libc++ 行为格因临时共享库 relocation 不兼容而未观测，不能据此判断正式动态 libc++ 制品的 cancel 行为。
- 本任务结论只定位 R36/R38 的映射分歧，不扩展为 S1 修法结论，也不评价方案。

## 7. 板上清理

部署目录 `/var/tmp/r39_s1_20260814` 已删除，精确目录不存在断言退出 0；通过 `/proc/*/exe` 检查，无任何进程从该目录执行（`commands/047_final_board_cleanup.log:4-17`；结构化表见 `cleanup.tsv`）。未安装 RPM，未修改系统库、配置或服务。

## 8. 代码交付

`code/` 包含：

- R36/R38 的全部相关测试源码、编译链接命令原文与运行身份原文；
- 实验 libc++abi 三处改动的完整 diff 与被改文件完整内容；
- R36 patched 与 R38 unpatched-control 的完整 CMakeCache；
- 本任务所有门禁、部署、运行、清理与重链脚本；
- `CODE_INDEX.tsv` 中逐文件来源与 SHA256。

材料仅供人工审阅；本报告不据此实现修法。
