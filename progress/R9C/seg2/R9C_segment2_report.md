# R9-C 第 2 段：按新锚重建三架构

日期：2026-08-06

执行结论：**PASS；三架构重建成功，身份自检通过，T4 14 项门禁三架构全部通过。**

## 1. SOURCE_PROVENANCE 修订

构建前已修订 `packaging/SOURCE_PROVENANCE`，其 SHA256 为
`1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37`。

新主锚为本机 GBS 标准导出对象：

- Source0：`llvm-22.1.8.tar.gz`
- SHA256：`1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2`
- 上游 tag peeled commit：`ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`
- 本地平台源：`tizen_base@3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`
- 官方 release 内容对照锚：`922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888`

旧平台冻结 tarball 锚 `16a932e1…1d122e` 和旧 VCS 测试值
`c12cbe88…a9146` 均保留并标为 `SUPERSEDED_BY_R9C`。相对平台冻结
tarball，差异仍恰为 `clang/bindings/python/.git_archival.txt`；成因是
本地标准导出的 `export-subst` 展开，并引用第 1 段的
`NO_BUILD_IMPACT` 结论。

六个导出补丁的完整文件名、SHA256、patch-id 与对应提交见
`identity/frozen_patch_inventory.tsv`。三架构 SRPM 均确认 EHABI
backport 为 `0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch`
及 spec `Patch5`，patch-id 为
`ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479`。

## 2. 三架构构建

三次均在新的单用途根中按 armv7l、aarch64、x86_64 顺序执行，使用
GBS 标准 patch export，命令中无 `--no-patch-export`，并显式指定
单一 `libcxx-runtimes.spec`。三个根均已写入 `RETIRED_SINGLE_USE`。

| 架构 | 状态 | 二进制 RPM | SRPM | buildroot |
|---|---:|---:|---:|---|
| armv7l | PASS | 7 | 1 | `tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260806-c2` |
| aarch64 | PASS | 7 | 1 | `tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260806-c2` |
| x86_64 | PASS | 7 | 1 | `tmp/GBS-ROOT/LIBCXX-2218-x86_64-20260806-c2` |

armv7l 的 `cxa_personality.cpp.o` 明确编译成功。实际 LLVM/clang 工具链
三架构均解析为 `22.1.8-19.1`；全部 NEVRA 见
`architectures/<arch>/toolchain_nevra.tsv`。完整 GBS 命令、工作目录、
repository 行和退出码见 `commands/03_build_<arch>.log` 与
`build/<arch>.full.log`。

制品位于 `artifacts/rebuild_2218_c2/<arch>/`；24 个 RPM 的逐件 SHA256
见 `identity/new_artifact_sha256.tsv`。

## 3. 身份自检

结果：`IDENTITY_SELFCHECK=PASS`。

- 三个 SRPM 内 Source0 SHA256 均为新主锚 `1f5fe8da…6f7d2`。
- 三个 SRPM 内六个 patch 的文件名、数量、SHA256、patch-id 完全一致，
  且与导出前冻结表一致。
- 三个 SRPM 的解包 payload 逐文件路径、模式、SHA256 完全一致。
- 三份 `SOURCE_PROVENANCE` 完全一致，并与工作树版本逐字相同。
- SRPM 外层文件 SHA 不同，但解包源 payload 无差异；未将外层 RPM
  容器哈希相等误作本项判据。

首次身份脚本因把冻结 TSV 表头纳入数据比较而退出 1；六条数据本身
一致。原失败保留于 `commands/04_identity_selfcheck.log`，随后只读复核
在 `commands/04a_validate_identity_evidence.log` 中通过，分类为
`SPEC_ERROR_IN_EVIDENCE_ASSERTION_HEADER_INCLUDED`，未重建任何制品。

## 4. T4 门禁重跑

结果：`T4_RERUN=PASS_ALL_14_GATES_ALL_3_ARCHITECTURES`。

P01–P14 的逐架构结果见 `gates/gate_results.tsv`。重点结果如下：

- `libc++.so.1.0` 与 `libc++abi.so.1.0` 的导出版本节点均为 `LLVM_22`。
- 三架构直接依赖和 `ldd -r` 闭包均无 `libstdc++`、无 `libunwind`、
  强未解析符号为 0。
- armv7l 使用 `qemu-arm-static -L <root>`；aarch64 本轮实测发现可用的
  `/usr/bin/qemu-arm64-static`（版本输出为 qemu-aarch64 5.2.0），同样以
  `-L <root>` 执行；x86_64 原生执行。三者显式 `ldd -r` 均通过。
- 三架构 green fixture 均为 `PASS_GREEN`，2/2 关键绑定进入候选
  `libc++abi`；red fixture 均为 `EXPECTED_RED_DETECTED`，2/2 非版本化
  绑定进入 `libstdc++`；smoke 均退出 0。
- provider family 三方一致：`__cxa_finalize → glibc`、
  `_Unwind_RaiseException → libgcc_s`，三项 C++ ABI 符号进入候选
  `libc++abi`。
- `libc++-devel` 的 `/usr/include/c++/v1` 布局与实际 fixture 编译均通过。

冻结静态工具三次均退出 0。外围 P04 初始断言错误地要求所有
`DW_AT_producer` 都是 clang，而实际合法地同时存在 `GNU AS 2.43`；按
T4 已登记判据“存在 clang version 22.1.8 记录”复核后，六个 DSO
debuginfo 全部通过。原非零与修正记录均保留。显式 ldd 载体也保留了
一次 shell 把 sed 行尾 `$` 误展开为 `$#` 的无效尝试，修正后要求
非空真实 ldd 输出并全部通过；无效尝试未用于结论。

## 5. 新旧制品与隔离

新旧 24 个同名 RPM 的 SHA256 均不同，逐项表见
`comparisons/new_vs_old_rpm_sha256.tsv`。申报的归因范围为：

- 旧 armv7l：R6 整棵 git tree/`--no-patch-export` 改为标准六补丁导出，
  同时工具链仓快照由旧记录的 18.1 变为本轮 19.1；
- 旧 aarch64/x86_64：平台冻结 tarball 基线改为本地标准六补丁导出，
  同时工具链仓快照变为 19.1；
- SRPM：标准导出对象、自动补丁与 provenance 修订共同导致变化。

以上仅申报差异来源，不判断差异良恶。

旧 `artifacts/rebuild_2218/` 与空的 `artifacts/rebuild_2218_std/` 已只移动、
未删除至 `artifacts/quarantine/20260806_pre_c2/`，并写入
`SUPERSEDED_BY_R9C`。不存在的 `rebuild_2218_p0/` 已明确记录为
`NOT_FOUND`。移动前后清单与 SHA 见 `comparisons/quarantine_*`。

## 6. 覆盖边界

本次覆盖构建期、静态 ELF、加载期 `ldd -r`、动态绑定 fixture 与 smoke。
ARM EHABI 和 Itanium/DWARF 的**运行时异常展开正确性未做板测**，登记为
`PENDING_BOARD_VERIFICATION`。

本任务到此停止；未进入 candidate repo、晋级台账或任何外部仓 push。
