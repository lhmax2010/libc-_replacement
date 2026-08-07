# R23：定点算术符号引用扫描与两项补充核查

## 结论摘要

第一部分结论为 `ZERO_REFERENCES_CONFIRMED`，严格限定于 R11 的冻结
Base/Unified 仓快照：11,332 个二进制包记录构成包级分母，实际逐文件
检查 36,829/36,829 个运行时 ELF，读取失败 0。armv7l 的 980 个 GCC
Embedded C 定点算术导出符号中，被任何运行时 ELF 作为未定义动态符号
引用的符号数、ELF 数、二进制包数均为 0。零命中是完整实测零，不是
“未查到数据”。修正后的 compiler-rt 精确符号名缺口为 armv7l 6、
aarch64 25、x86_64 27。

第二部分确认：R11 源码仓内按明确名称目录识别到的测试框架源码包只有
`gtest-1.15.2-0.src`，它同时产出 gtest 与 gmock，已在 R22 第一阶段；
因此已识别框架全部落在第一阶段。第三部分已把“板上没有可执行路径”
更新为被 R22 解决，并保留 R15/R15b 当时的历史结论。

## 1. 输入、快照与计数口径

所有输入及 SHA256 见 `inputs/input_identity.tsv`。仓快照不是一个跨仓单值，
而是四个 revision + repomd SHA256 组成的复合身份，逐项见
`tables/input_snapshot_identity.tsv`：Base binary、Unified binary、Base
source、Unified source 均保留原始 revision 与 SHA256。

本项没有重新访问或扫描平台仓，没有下载、安装或构建。扫描对象是 R11
已经下载并解包的 ELF 缓存：包级分母为 11,332 个二进制包记录；文件级
分母为 `runtime_elf=YES` 的 36,829 个 ELF。二者不能互换。源码包单位不
用于第一部分比率。逐项定义见 `tables/scan_scope_denominators.tsv`。

## 2. 定点算术符号集

符号集不是由本任务凭前缀重建。它取 R17 实际分类为 `FIXED_POINT` 的成员，
再与 R16 三架构 `libgcc_s.so.1` 实测导出集逐符号相交：980/980 全部回连
成功，均属于 armv7l；aarch64 与 x86_64 的该家族实测导出数均为 0。
完整三架构 libgcc_s 导出表见 `tables/libgcc_exports_full.tsv`，980 个成员、
版本节点和逐条归类依据见 `tables/fixed_point_symbols.tsv`。

## 3. 全平台 UND 引用实测

扫描器逐 ELF 只读解析 ELF section header、`.dynsym` 及其链接的
`.dynstr`，以动态符号 `st_shndx == SHN_UNDEF` 为 UND 判据，然后与该
架构的实测符号集做精确名字相交。完整逐 ELF 结果在
`tables/runtime_elf_scan_results.tsv`，空命中表
`tables/fixed_point_undefined_references.tsv` 仍保留表头作为“实测零”
证据。

| 架构/范围 | 运行时 ELF 分母 | 成功检查 | 家族符号数 | 被引用符号 | 引用 ELF | 引用包 | 结论 |
|---|---:|---:|---:|---:|---:|---:|---|
| armv7l | 11,453 | 11,453 | 980 | 0 | 0 | 0 | ZERO_REFERENCES_CONFIRMED |
| aarch64 | 11,531 | 11,531 | 0 | 0 | 0 | 0 | ZERO_REFERENCES_CONFIRMED |
| x86_64 | 10,946 | 10,946 | 0 | 0 | 0 | 0 | ZERO_REFERENCES_CONFIRMED |
| 全部 R11 运行时 ELF | 36,829 | 36,829 | 980 | 0 | 0 | 0 | ZERO_REFERENCES_CONFIRMED |

三行主架构之外还有 2,899 个 R11 运行时 ELF（如 noarch/i586/riscv64
记录），亦被逐文件解析；这些架构没有来自第 2 节的定点家族符号集，
不把其他架构符号推断移植给它们。

唯一专用解析器不能读取 section table 的对象是 aarch64
`u-boot-rubikpi3` 的 `/boot/u-boot.mbn`。随后 GNU
`readelf --dyn-syms -W` 对同一文件退出 0、输出为空，故登记为
`PASS_READELF_FALLBACK_NO_DYNSYM_OUTPUT`；原始失败与闭合证据分别见
`tables/elf_parser_fallbacks.tsv`、`commands/016_failed_elf_readelf_fallback.log`。
最终失败数为 0。

另取首/中/末与全部厂商对象组成 36 个样本，以 GNU readelf 交叉核对
动态符号数和本家族 UND 集。GNU 输出不打印有名称的第 0 号 null symbol，
显式归一化这一项后 36/36 `MATCH`，见
`tables/elfparser_validation_samples.tsv` 与 `commands/019_...log`。

## 4. 厂商预编译与其他非 clang 形态

R13 裁定的五个厂商包名在三架构共对应 18 个运行时 ELF；18/18 解析
成功，定点家族 UND 均为 0。逐包、逐 ELF、NEVRA、架构与 `.comment`
证据见 `tables/vendor_prebuilt_elf_scan.tsv`。

全扫描同时从 `.comment` 实测识别出 1,289 个 GCC 标记 ELF，分属 490 个
二进制包实例、160 个源码 RPM 身份，完整清单见
`tables/gcc_comment_identified_elfs.tsv`。该证据只能说明编译器生产者
形态，不能从 `.comment` 证明“第三方/预编译”所有权；除上述五个已裁定
对象外，所有权均标 `NOT_DETERMINABLE_FROM_ELF_COMMENT`，没有推断补全。

## 5. 回滚兼容盲区与修正后缺口

R11 输入是当前冻结 Base/Unified 仓 RPM，并不包含 OTA 回滚镜像的二进制
清单。平台 OTA 回滚机制的具体库存状态在现有输入中 `NOT_AVAILABLE`；
因此本轮的 `ZERO_REFERENCES_CONFIRMED` 不外推到未提供的历史/回滚镜像。
范围表见 `tables/rollback_coverage.tsv`。

在当前冻结扫描范围内，980 个定点符号可从精确符号名缺口中扣除：

| 架构 | R16/R17 原缺口 | 定点家族 | 实测被引用 | 修正后缺口 |
|---|---:|---:|---:|---:|
| armv7l | 986 | 980 | 0 | 6 |
| aarch64 | 25 | 0 | 0 | 25 |
| x86_64 | 27 | 0 | 0 | 27 |

armv7l 剩余 6 个是 `__aeabi_uread4`、`__aeabi_uread8`、
`__aeabi_uwrite4`、`__aeabi_uwrite8`、`__clrsbdi2`、`__clrsbsi2`。
本报告不评价这些符号，也不做实现建议或工作量估算。

## 6. 测试框架第一阶段核查

R22 第一阶段 13 个源码包的完整名单见
`tables/phase1_source_packages.tsv`。以源码包名称中的
`gtest|gmock|googletest|catch2|cppunit|doctest` 为明确检索目录，R11 的
1,450 个源码包实例只命中 `gtest-1.15.2-0.src`；R22 将其列为
`PHASE1`，备注为 `test framework`。gmock 不构成独立源码包，而由同一
gtest 源码包产出。故已识别测试框架源码包 1/1 全在第一阶段，不存在
需要解释的第二/三阶段框架包。

RPM 文件清单实测显示：`gtest` 运行包提供
`libgtest.so`、`libgtest_main.so`、`libgmock.so`、`libgmock_main.so`
及版本文件，属于共享库形态；`gtest-devel` 提供头、CMake 与 pkgconfig
元数据；未发现静态库或源码 payload。三架构完整 `rpm -qlp` 清单与退出码
见 `tables/test_framework_payload_files.tsv`、
`tables/test_framework_rpm_query_commands.tsv`。

R11 source RPM 的 `requires` 元数据作为 BuildRequires 口径：153/1,450 个
源码包实例引用 gtest/gmock 相关名称；其中第一阶段 4、第二阶段 121、
第三阶段 10、厂商单列 1、R22 的 483 包范围外 17。第二、三阶段合计 131。
逐包及具体 relation name 见
`tables/test_framework_buildrequires_consumers.tsv`。

R11 没有保留或索引全源码树的 `add_subdirectory` 调用，故源码内嵌
gtest 的包数为 `NOT_AVAILABLE_FULL_SOURCE_CONTENT_NOT_RETAINED`；本任务
没有为补这个数字而重新展开或重扫平台源码。见
`tables/vendored_test_framework_status.tsv`。

## 7. 板上阻塞文档状态更新

仓内仍保留 R15 当次会话“无 SSH/SDB 执行载体”和 R15b 当次任务
“未实际执行探针”的历史事实。为避免被误读为当前阻塞，已在以下策展
文档增加显式状态更新，不改历史表格、不改既有结论：

- `docs/progress/R15/R15_report.md` 与 `README.md`：标记 R15 状态只适用
  当次会话，已被 R22 后续实测取代；
- `docs/progress/R15b/R15b_report.md`：登记 R22 已在六个路径实际运行
  ARM 静态探针；
- `docs/progress/R22/R22_board_path_report.md`：明确 `/tmp noexec` 不再
  构成“无可执行路径”阻塞。

六个可写可执行路径为 `/root`、`/opt/usr`、`/opt/home`、
`/home/owner`、`/var/tmp`、`/opt/var/tmp`。仍存在的能力边界已同步记录：
项目当前只有一块已识别 armv7l 板；SDB 曾在 R15 不可达、在 R15b/R22
可达，未做跨会话连续稳定性监测；板端没有 clang/clang++/gcc/g++。
逐文件原位置、更新位置和内容摘要见 `tables/documentation_updates.tsv`。

## 8. 证据完整性

`tables/cross_checks.tsv` 的 6 项机械核对全部 PASS。早期五种扫描实现因
性能或内存原因被显式中止，退出 130，均不进入结论；第一次抽样校验因
null symbol 展示口径差一而退出 1，修正计数口径后 36/36 MATCH。所有
尝试、原因与是否用于结论见 `tables/discarded_attempts.tsv`，原始日志
保留。最终结论只使用退出 0 的 `commands/014`、`016`、`017`、`019` 和
`020` 证据链。

本报告不做方案推荐、不判工程可行性、不估人日。
