# R91：libcxx-runtimes.spec 明确使用 Clang

## 结论

任务已完成。平台源码只修改了 `packaging/libcxx-runtimes.spec`：新增
`%define _toolchain_override clang` 一行，其他 CMake 参数、交叉编译设置和源码
均未改变。

- Gerrit 目标分支：`sandbox/lhmax2025/libcxx-ehabi-backport`
- 推送前 HEAD：`48fc3a299ba20dd321a7d4a37c4e13ce095176cd`
- 推送后 HEAD / 本次提交：`dd4beffc30be18841818651756f4dbe1f54b0016`
- Change-Id：`Ib5ad0875af75aed81c767ae96b954b065e7c2877`
- 推送方式：普通快进 `git push`，未使用 force，未新建分支
- x86_64 实际 GBS 构建：通过
- 实际编译器：Clang 22.1.8
- 展开器：产物直接依赖 `libgcc_s.so.1`
- 四补丁产物核验：通过；五处头文件与此前 R85 x86_64 QuickBuild 产物逐字节一致

正式分支 `tizen_base` 保持
`8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`，其他
`sandbox/lhmax2025/*` 分支 SHA 均未变化；没有向 LLVM 上游提交。

## 1. 第一阶段事实摘要

Base Toolchain 的 `project_config` 定义：

```spec
%__cc_clang %{_host}-clang
%__cxx_clang %{_host}-clang++
%__cc_gcc %{_host}-gcc
%__cxx_gcc %{_host}-g++
%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}
%__cc %{expand:%%{__cc_%{_toolchain}}}
%__cxx %{expand:%%{__cxx_%{_toolchain}}}
```

Base 与 Unified 当前三架构均实际选择目标前缀 Clang；但宏允许
`_toolchain_override gcc` 切换成 GCC，因此评审意见成立。此前 QuickBuild 的
armv7l、aarch64、x86_64 日志均显示 Clang 22.1.8，原先的
`--rtlib=libgcc` 确实由 Clang driver 处理，此前产物和验证结论不受影响。

完整第一阶段记录见 `STAGE1_REPORT.md`。

## 2. 修改及放置位置

最终 diff：

```diff
 %define llvm_major 22
 %define llvm_version 22.1.8
+%define _toolchain_override clang
```

放在文件开头的 LLVM 版本宏之后、`Name:` 等包元数据之前。参照对象是平台
Base source package `llvm-22.1.8-19.1` 的 `llvm.spec`：它同样在 spec 顶部、
进入包元数据与构建逻辑之前定义 `%define _toolchain_override clang`，之后的
CMake 仍使用 `%__cc` / `%__cxx`。

选择该位置和写法的原因：

1. 遵循平台 LLVM 自身惯例；
2. 只增加一行，不改动已有 CMake compiler/target 参数；
3. `%{__cc_clang}` 继续展开为 `%{_host}-clang`，保留交叉目标前缀；
4. `BuildRequires: clang = %{version}` 保证 driver 包进入构建根，新增行负责
   明确选择它。

## 3. GCC override 的明确行为

结论：若 project config 或 GBS 命令预先设置
`_toolchain_override gcc`，本 spec 内的 `%define _toolchain_override clang`
会覆盖该值；不会报宏冲突，最终仍使用 Clang。

对实际 spec 做了两次预处理：不设置外部 override，以及外部显式设置 GCC。
两次结果都为：

```text
-DCMAKE_C_COMPILER=x86_64-tizen-linux-gnu-clang
-DCMAKE_CXX_COMPILER=x86_64-tizen-linux-gnu-clang++
-DCMAKE_ASM_COMPILER=x86_64-tizen-linux-gnu-clang
```

因此 GCC 路径不会进入 CMake，`--rtlib=libgcc` 不会再被传给 GCC。证据见
`raw/064_external_gcc_override_behavior_probe.*`。

## 4. 实际构建

### 范围与命令

选择 x86_64，满足任务书“至少一个架构”；用单包线程 1、包内 `-j2`、
`nice -n 15` 与 `ionice -c 3` 控制资源。成功命令原文见
`raw/054_build_x86_64_retry1.command.txt`，完整输出见同名前缀的
`.stdout` / `.stderr` / `.exitcode`。

构建使用验证 Release `91.1`，避免与仓内正式 Release 混淆，不修改源码。
结果为 1 个 source RPM、7 个 binary RPM，GBS 汇总：

```text
=== Total succeeded built packages: (1) ===
```

完整 RPM 文件名、大小与 SHA256 见 `raw/056_inventory_build_outputs.stdout`。

### 首次命令错误

第一次调用在工作区根目录运行，GBS 2.0.8 未采用尾部 worktree 参数，导出
阶段找不到指定 spec，0 个包进入构建并以 1 退出。随后从 sandbox 工作树
目录内执行同一验证，成功完成。该次属于命令定位错误，不是补丁、spec、
编译或测试失败；两次原始记录均保留。

## 5. 三项指定验证

### 5.1 实际编译器

成功构建日志中的 CMake 命令为：

```text
-DCMAKE_C_COMPILER=x86_64-tizen-linux-gnu-clang
-DCMAKE_CXX_COMPILER=x86_64-tizen-linux-gnu-clang++
-DCMAKE_ASM_COMPILER=x86_64-tizen-linux-gnu-clang
```

CMake 随后报告 C/CXX compiler identification 均为 `Clang 22.1.8`。
结论：`PASS`。

### 5.2 libgcc_s 展开器

采用三层核验：

1. 构建命令保留 `--rtlib=libgcc`；
2. CMake 配置保留 `LIBCXXABI_USE_LLVM_UNWINDER=OFF`、
   `LIBCXXABI_USE_COMPILER_RT=OFF` 及静态 unwinder 关闭项；
3. 对解包后的 ELF 执行 `readelf -dW`，`libc++.so.1.0` 与
   `libc++abi.so.1.0` 均有 `DT_NEEDED: libgcc_s.so.1`；libc++abi 的
   `_Unwind_Resume_or_Rethrow` 等未定义符号带 `GCC_3.x` 版本，RPM Requires
   也明确列出 `libgcc_s.so.1`。

结论：产物仍以 libgcc_s 为展开器，`PASS`。完整证据见
`raw/062_unwinder_and_dynamic_dependency_checks.stdout`。

### 5.3 四个补丁与五处头文件

成功生成的 SRPM 明确声明并在 `%prep` 应用 Patch6–Patch9；二进制中存在
`_Unwind_Resume_or_Rethrow` 引用及 `__cxxabiv1::__forced_unwind` 的 typeinfo、
vtable 和析构符号。

按要求解开 `libc++-devel` 并检查五处：

| 文件 | 检查结果 | 与 R85 x86_64 比较 |
| --- | --- | --- |
| `__ostream/basic_ostream.h` | 命中 forced-unwind catch/rethrow | SHA256 相同 |
| `future` | 命中 forced-unwind 恢复 deferred 状态后重抛 | SHA256 相同 |
| `istream` | 命中 forced-unwind 分支和 `__setstate_nothrow` | SHA256 相同 |
| `string` | 命中 forced-unwind catch/rethrow | SHA256 相同 |
| `cxxabi.h` | 命中 `class __forced_unwind` 定义 | SHA256 相同 |

实际代码片段和 SHA256 见 `raw/061_five_header_patch_checks.stdout`；与 R85
逐文件比较见 `raw/063_compare_headers_with_prior_quickbuild.stdout`。结论：
`PASS`。

由于验证 Release 和构建时间不同，本次 RPM 与 R85 RPM 的整包字节级相等
不适用，记为 `NOT_OBSERVED`；指定的行为/内容锚点以及五个头文件字节级
一致性均已确认。

## 6. 已知警告与覆盖边界

- 构建仍报告 `libc++.so.1.0`、`libc++abi.so.1.0` 缺少 GNU Build ID。
  这是此前已登记的发布事项，并非本次单行改动新增。
- 第二阶段实际新构建仅覆盖 x86_64；armv7l 与 aarch64 未在本轮重新构建。
  第一阶段已有此前 QuickBuild 三架构使用 Clang 22.1.8 的直接日志证据，
  但不能替代本轮对另外两架构的新构建，故如实列为未覆盖。
- 未运行 LLVM 官方全量测试；本次只改变构建器选择宏，且任务要求的重点是
  包构建和产物核验。

## 7. 提交与推送核对

提交作者和提交者均为 `hao.lin <hao.lin@samsung.com>`。提交内容只有：

```text
packaging/libcxx-runtimes.spec | 1 +
1 file changed, 1 insertion(+)
```

推送前再次 fetch，确认远端 HEAD 等于新提交的父提交；普通 push 成功。推送
后 `ls-remote` 证明本地与远端均为
`dd4beffc30be18841818651756f4dbe1f54b0016`，旧 HEAD 仍为其祖先。

其他分支推送前后保持：

| 分支 | SHA |
| --- | --- |
| `sandbox/lhmax2025/mlgo` | `08ffd8cd7c72f5dd6e612d8395362def96d84029` |
| `sandbox/lhmax2025/mlgo_aot` | `88ff57cadcff52ab50ca6865db7faed7388358fe` |
| `sandbox/lhmax2025/llvm_only_clang` | `f8277158cf44f872e2b2d62a2e839cb33bd0afea` |
| `tizen_base` | `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c` |

## 8. 可直接用于 Gerrit 的回复

完整措辞见 `GERRIT_REPLY.md`。

## 9. 自行判断、疑问与异常

### 自行判断

- 选择 x86_64 作为“至少一个架构”的实际构建目标，以较低资源成本验证
  compiler macro、driver 选项和 ELF/RPM 结果。
- 使用验证 Release `91.1` 区分本地 RPM；不改变提交中的 Release。
- 对“产物一致”采用内容/行为锚点与五头文件 SHA256 比较，不把构建时间和
  Release 不同的 RPM 整包 hash 当作可比判据。

### 尚存疑问

- 无阻塞疑问。
- armv7l/aarch64 本轮新提交的实际重构建结果为 `NOT_OBSERVED`。

### 技术性异常

- 第一阶段记录器参数错误一次、两次过宽只读枚举被终止；均未改源码。
- 第二阶段记录器参数错误一次、首次 GBS 工作目录定位失败一次、第一次 RPM
  解包括号转义错误一次、一次补充符号管道因 `rg -m` 得到 141；均已原样
  记录并以限定范围/正确参数重跑，最终判据全部通过。

## 10. 证据索引

- `STAGE1_REPORT.md`：第一阶段四组事实。
- `SPEC_CHANGE.diff`：唯一平台源码改动。
- `VALIDATION.tsv`：验证结论索引。
- `GERRIT_REPLY.md`：可直接提交给总部的回复。
- `raw/`：全部命令原文、stdout、stderr、退出码；成功构建的完整原始输出为
  `raw/054_build_x86_64_retry1.stdout`。
