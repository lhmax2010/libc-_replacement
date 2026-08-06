# R16：libgcc_s 可替代性、glibc 依赖与上游设施状态实测

日期：2026-08-06。结论仅陈述实测事实，不作方案推荐、可行性裁决或工作量估算。

## 执行摘要

1. **D106703 本身未合入**：LLVM Phabricator 当前标记 `Abandoned`。其功能方向后来以独立的 `llvm-libgcc` 子项目落地，初始提交为 `c5a20b518203613497fa864867fc232648006068`；LLVM 22.1.8 本地源码与查询时 upstream main 均仍有该设施。
2. **现存合成设施有明确边界**：`llvm-libgcc` 用 compiler-rt builtins 与 libunwind 组成共享对象，使用 `gcc_s.ver.in`，并生成 `libgcc_s.so.1.0/.1/.so` 名称链；需要 `LLVM_LIBGCC_EXPLICIT_OPT_IN=Yes`。ChromeOS 当前公开 ebuild 也默认启用 `+synth_libgcc`，把 builtins 与 libunwind 静态对象链接成带版本脚本的 `libgcc_s.so.1`。
3. **平台 glibc 三架构均按 SONAME 硬编码依赖**：三份 `libc.so.6` 都包含 `libgcc_s.so.1`、unwind 函数名以及“unwinding / pthread_cancel / pthread_exit”三类 fatal 文本；对应 `_Unwind_*` 动态 UND 为 0，且 `DT_NEEDED` 不含 libgcc_s。因此这是运行时按名称加载/查找，不是普通链接依赖。
4. **板端运行实证本轮不可得**：`192.168.108.25` ping 2/2 成功、TCP/26101 可连接，但 `sdb connect` 两次失败，后续命令均为 target not found。宿主 x86_64 旁证中，基线进程未加载 libgcc_s；调用 glibc `backtrace()` 后 `LD_DEBUG=libs` 实测查找并初始化 `/lib/x86_64-linux-gnu/libgcc_s.so.1`。
5. **平台 compiler-rt builtins 与平台 libgcc_s helper 导出面并不相同**：按精确符号名，未覆盖数为 armv7l `986/1137`、aarch64 `25/122`、x86_64 `27/139`。这只是一项符号集合事实，不等价于运行时兼容性判定。

## 0. 上游设施现状

### D106703

- 状态：`ABANDONED_NOT_MERGED`。
- 网页原文状态与 URL 检查：`commands/29_d106703_d108416_status_pages.txt`、`commands/28_upstream_urls_status.txt`。
- 本地 LLVM 全历史检索未发现 `D106703` 或 `LIBUNWIND_SYNTH_LIBGCC` 落地：`commands/23_d106703_git_absence.txt`。
- D106703 草案里的“直接在 libunwind 增加合成开关”没有成为当前源码形态。

### 当前 llvm-project 设施

- 独立子项目：`llvm-libgcc/`；初始提交 `c5a20b518203613497fa864867fc232648006068`，主题 `[llvm-libgcc] initial commit`。提交与包含分支原文见 `commands/24_c5a20_commit.txt`。
- 22.1.8 文件：`CMakeLists.txt`、`docs/LLVMLibgcc.rst`、`gcc_s.ver.in`、`generate_version_script.py`。完整 file:line 原文见 `commands/22_local_llvm_libgcc_sources.txt`。
- 关键形态：显式 opt-in；组合 compiler-rt builtins 与 libunwind；预处理 GCC 版本脚本；生成 `libgcc.a`、`libgcc_eh.a` 与 `libgcc_s.so*` 名称。逐项索引见 `tables/upstream_facilities.tsv`。
- 查询时 upstream main HEAD 为 `4442d7022c94ff041b65236670243014fa40f4d1`；其 `CMakeLists.txt`、`gcc_s.ver.in` 与生成器均可取得，见 `commands/20_upstream_remote_head.txt`、`commands/25_main_*.txt`。文档旧路径在 main 返回 404，故不以其判断设施消失。

### 发行版公开记录

- `CONFIRMED`：ChromeOS 的公开 `chromiumos-overlay/sys-libs/llvm-libunwind/llvm-libunwind-9999.ebuild` 当前声明默认 `+synth_libgcc`；行 230-269 明确生成 `libgcc_s.so.1`、使用架构版本脚本、链接 compiler-rt builtins 与 libunwind，并创建 libgcc/libunwind 名称链。完整原文见 `commands/26_chromeos_llvm_libunwind_ebuild.txt`。
- 本任务只把上述可回链 ebuild 记为已证实记录；没有把搜索结果、评审中的计划或测试声明扩写成其他发行版的部署事实。

## 1. glibc 硬编码依赖

逐架构明细见 `tables/glibc_dependency.tsv`，原始字符串、动态符号、版本信息和动态段分别见 `commands/30_*_glibc_strings.txt`、`commands/31_*_glibc_dynsym.txt`、`commands/32_*_glibc_dynamic_comment.txt`。

| 架构 | `libgcc_s.so.1` 字面量 | 三类 fatal 文本 | `_Unwind*` 动态 UND | `DT_NEEDED libgcc_s` |
|---|---:|---:|---:|---:|
| armv7l | 有 | 3/3 | 0 | 无 |
| aarch64 | 有 | 3/3 | 0 | 无 |
| x86_64 | 有 | 3/3 | 0 | 无 |

涉及 API 的实测边界：

- `pthread_cancel`：三架构均有专名 fatal 文本；GNU glibc 2.40 `nptl/pthread_cancel.c` 调用 `__libc_unwind_link_get()`。
- `pthread_exit`：三架构均有专名 fatal 文本；同版本源码 `nptl/pthread_exit.c:28-31` 在加载失败时 fatal。
- `backtrace`：三架构均有 `_Unwind_Backtrace` 名称与一般 unwinding fatal 文本；同版本源码 `debug/backtrace.c` 通过 unwind link 获取入口。
- 其他集中入口：同版本 `sysdeps/nptl/unwind-forcedunwind.c` 与 `sysdeps/generic/unwind-link.h` 集中处理 `_Unwind_ForcedUnwind`、`_Unwind_GetCFA`、`_Unwind_GetIP`（armv7l 为 VRS 形态）、`_Unwind_Resume`、personality。源码全文与命中行见 `commands/35_glibc_240_unwind_source.txt`。

结论标识：`PLATFORM_GLIBC_HARDCODES_LIBGCC_S_SONAME_CONFIRMED`。该标识描述名称依赖事实；不把“磁盘上的 libgcc_s 必须由 GCC 实现”作为附加推断。

平台 glibc 构建工具链：`NOT_AVAILABLE`。三份 RPM 的 BUILDHOST/BUILDTIME 均为空或 0，三份 `libc.so.6` 均无 `.comment`；已尝试范围为 RPM 元数据、ELF comment、notes 与动态段。

## 2. 运行时实证

### 开发板

结论：`NOT_AVAILABLE_NO_SDB_PROTOCOL_SESSION`。

- ping：2/2，退出 0，见 `commands/50_board_ping.txt`。
- TCP 26101：连接成功，退出 0，见 `commands/58_board_tcp_26101.txt`。
- `sdb connect 192.168.108.25:26101`：两次均退出 1；见 `commands/51_board_sdb_connect.txt`、`commands/59_board_sdb_retry.txt`。
- 因无 target，身份/工具探测退出 1；未运行板上系统命令，未设置板上 `LD_DEBUG`，没有观察到板端 libgcc_s 加载行为。

### 宿主旁证（不得冒充板端实测）

- 基线：`LD_DEBUG=libs python3 -c 'pass'`，没有 libgcc_s 查找/初始化记录。
- 触发：同一 Python 载体以 `ctypes` 调用 glibc `backtrace()`，正常退出；日志新增 `find library=libgcc_s.so.1` 与 `calling init: /lib/x86_64-linux-gnu/libgcc_s.so.1`。
- 原文：`commands/55_host_ld_debug_baseline.txt`、`commands/56_host_ld_debug_backtrace.txt`、差异提取 `commands/57_host_libgcc_load_delta.txt`。

结论标识：板端 `NOT_OBSERVED`；宿主 `GLIBC_RUNTIME_DLOPEN_CORROBORATED_ON_HOST_ONLY`。

## 3. libgcc_s 导出面量化

输入身份与 SHA256 见 `tables/input_identity.tsv`；`readelf` 全量原文见 `commands/40_*_libgcc_symbols_versions.txt`。计数口径见 `METHODOLOGY.md`。

| 架构 | 唯一导出基础名分母 | unwind | ARM EHABI | compiler helper | 其他 |
|---|---:|---:|---:|---:|---:|
| armv7l | 1163 | 13 (1.117799%) | 7 (0.601892%) | 1137 (97.764402%) | 6 (0.515907%) |
| aarch64 | 159 | 19 (11.949686%) | 0 | 122 (76.729560%) | 18 (11.320755%) |
| x86_64 | 177 | 19 (10.734463%) | 0 | 139 (78.531073%) | 19 (10.734463%) |

完整输出：

- 每个导出基础名及类别：`tables/libgcc_export_classification.tsv`。
- 每个符号及版本节点：`tables/libgcc_symbol_versions.tsv`。
- 版本节点×类别计数：`tables/libgcc_version_node_distribution.tsv`。
- “其他”逐一列明：`tables/libgcc_other_symbols.tsv`。

armv7l `__aeabi_*`：唯一基础名 `69` 个，完整列表见 `tables/armv7l_aeabi_symbols.tsv`；与平台 compiler-rt builtins 精确同名者 `62/69`，未命中 `7/69`：

```text
__aeabi_unwind_cpp_pr0
__aeabi_unwind_cpp_pr1
__aeabi_unwind_cpp_pr2
__aeabi_uread4
__aeabi_uread8
__aeabi_uwrite4
__aeabi_uwrite8
```

前三项归于 ARM EHABI，后四项归于 compiler helper；此处的 `__aeabi_*` 独立清单故同时覆盖两类。

## 4. compiler-rt 覆盖差集

平台三架构 `compiler-rt-22.1.8-19.1` 均包含对应 builtins 静态库，故本项可执行。`nm -g --defined-only` 全量原文见 `commands/41_*_compiler_rt_builtins_symbols.txt`。

| 架构 | libgcc_s helper 分母 | compiler-rt 精确同名覆盖 | 未覆盖 | compiler-rt 自身唯一符号分母 | compiler-rt 有而 libgcc_s 无 |
|---|---:|---:|---:|---:|---:|
| armv7l | 1137 | 151 (13.280563%) | 986 (86.719437%) | 255 | 102 (40.000000%) |
| aarch64 | 122 | 97 (79.508197%) | 25 (20.491803%) | 321 | 222 (69.158879%) |
| x86_64 | 139 | 112 (80.575540%) | 27 (19.424460%) | 195 | 79 (40.512821%) |

- 缺失 helper 全列：`tables/compiler_rt_missing_helpers.tsv`。
- 反向差集全列：`tables/compiler_rt_only_symbols.tsv`。
- 汇总及各自分母：`tables/libgcc_export_summary.tsv`。

该差集回答的是当前平台制品的符号集合关系。上游 `llvm-libgcc` 的生成器也以 builtins、libunwind 和参考 libgcc_s 的集合交集生成版本脚本；本任务没有构建合成库，因此未观测最终共享对象导出面。

## 5. 三项说明事实

1. **LLVM 是否有“libunwind + compiler-rt 合成 libgcc 名称/ABI 表面”的设施**：`YES, LLVM_LIBGCC_PRESENT`；D106703 草案被放弃，落地形态是独立 `llvm-libgcc`，ChromeOS 有公开启用记录。
2. **平台 glibc 是否使 `libgcc_s.so.1` 这个 SONAME 依赖不可直接消失**：三架构二进制均 `HARDCODED_SONAME_CONFIRMED`；加载对象的实现身份不由本项判定。
3. **当前平台 compiler-rt builtins 是否与 libgcc_s helper 导出面完全相同**：`NO, NONEMPTY_EXACT_NAME_DIFFERENCE`，三架构缺口如第 4 节。

以上三项均为设施、名称依赖和符号集合事实；不延伸为替代方案可行/不可行裁决。

## 证据导航

- 全部命令原文与退出码：`commands/`。
- 输入与结果 TSV：`tables/`。
- 分析脚本：`tools/analyze_symbols.py`。
- 外部来源 URL 与查询日期：`SOURCES.tsv`。
- 完整校验清单：`MANIFEST.sha256`。
