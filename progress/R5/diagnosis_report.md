# R5 armv7l EHABI 缺陷诊断报告

日期：2026-08-05  
执行边界：只读诊断；未构建；未修改 spec、源码或配置。仅在 `progress/R5/` 写入本报告、命令账本和证据副本。

## 0. 结论

结论为：`REAL_DEFECT_EHABI_UNWINDER_PATH` 得到完整证据链支持；更精确地说，是 LLVM 22.1.8 `libcxxabi/src/cxa_personality.cpp` 在“ARM EHABI + LLVM unwinder=OFF + libgcc_s”组合下发生的**编译期声明缺失**。

证据链如下：

1. 两代 `__cxxabi_config.h:12-14` 的 EHABI 判定相同；用两代实际 armv7l Clang 预处理后，均实测定义 `_LIBCXXABI_ARM_EHABI`，见 [`ehabi_macro_after_cxxabi_config.raw.txt`](ehabi_macro_after_cxxabi_config.raw.txt)。
2. 22.1.8 的唯一 `__gnu_unwind_frame` 出现在 `cxa_personality.cpp:1135`，位于 `cxa_personality.cpp:971` 的 `#if !defined(_LIBCXXABI_ARM_EHABI)` 之 `#else`（`cxa_personality.cpp:1126`），即 ARM EHABI 分支，见 [`22_gnu_unwind_frame_guard_stack.tsv`](22_gnu_unwind_frame_guard_stack.tsv)。
3. 完整版本 diff 显示：21.1.1 在同一 EHABI 分支的 `cxa_personality.cpp:1012-1013` 显式声明 `__gnu_unwind_frame`，22.1.8 删除这两行，但保留调用；这是直接触发差异，见 [`cxa_personality_21_1_1_to_22_1_8.full.diff`](cxa_personality_21_1_1_to_22_1_8.full.diff)。
4. 两代真实翻译单元头链分别选中 Clang 21/22 resource header `.../usr/lib/clang/<major>/include/unwind.h`；二者 SHA256 相同，且均没有 `__gnu_unwind_frame` 声明，见 [`translation_unit_unwind_header_paths.raw.txt`](translation_unit_unwind_header_paths.raw.txt)、[`unwind_header_sha256.tsv`](unwind_header_sha256.tsv) 和 [`preprocessed_unwind_declaration_assertion.tsv`](preprocessed_unwind_declaration_assertion.tsv)。
5. GCC 的 `unwind-arm-common.h:215-216` 确有该声明，且 `:30` 定义 `__ARM_EABI_UNWINDER__`；但它没有进入两代真实翻译单元头链。两代 Clang `-dM -E` 也均未定义 `__ARM_EABI_UNWINDER__`。
6. 两代 `CMakeCache.txt` 均为 `LIBCXXABI_USE_LLVM_UNWINDER=OFF`、`LIBCXXABI_USE_COMPILER_RT=OFF`、`LLVM_ENABLE_RUNTIMES=libcxxabi;libcxx`，生成的链接输入均为 `-lgcc_s`；CMake 决策没有发生版本漂移。
7. 两代 `libgcc_s.so.1` 均导出 `__gnu_unwind_frame@@GCC_3.5`。因此失败发生在 C++ 语义分析的“未声明标识符”，不是目标符号不存在或链接失败。
8. 21.1.1 成功日志实证 `cxa_personality.cpp.o` 被编译，并随后过 `check-files`、写出 7 个 binary RPM 和 1 个 SRPM；21.1.1 构建树中的该源码 SHA256 与 T0 打捞源码一致。

因此，21.1.1→22.1.8 在该组合上的关键变化不是 unwinder 配置、EHABI 判定、选中头文件或 libgcc_s 导出符号变化，而是：**源码侧原有的 GNU ARM 私有入口显式声明被删除，调用仍保留；实际 Clang unwind 头未补上该声明。**

## 1. 失败点上下文

22.1.8 原文 `libcxxabi/src/cxa_personality.cpp:1100-1170` 已逐行落盘于 [`22_cxa_personality_1100_1170.raw.txt`](22_cxa_personality_1100_1170.raw.txt)。关键行：

- `:971`：`#if !defined(_LIBCXXABI_ARM_EHABI)`
- `:1126`：`#else`
- `:1128-1131`：上游注释说明 ARM EHABI 7.3/7.4 的逐帧 unwind 语义
- `:1135`：`switch (__gnu_unwind_frame(unwind_exception, context)) {`
- `:1146`：后续独立条件 `#if !defined(_LIBUNWIND_VERSION)`；它开始于调用之后，不是 1135 行的外层守卫

1135 行的完整外层条件栈只有一层：

| 层级 | 打开位置 | 当前分支 | 实际含义 |
|---|---|---|---|
| 1 | `cxa_personality.cpp:971 #if !defined(_LIBCXXABI_ARM_EHABI)` | `:1126 #else` | `_LIBCXXABI_ARM_EHABI` 已定义 |

22.1.8 全文件仅 1 处 `__gnu_unwind_frame`，即 `:1135` 的调用；没有声明。原文及守卫分别见 [`22_gnu_unwind_frame_occurrences.raw.txt`](22_gnu_unwind_frame_occurrences.raw.txt) 和 [`22_gnu_unwind_frame_guard_stack.tsv`](22_gnu_unwind_frame_guard_stack.tsv)。

构建日志 `progress/T3R2/build_armv7l.full.log:2383-2389` 给出完整编译行与错误：编译目标进入 `[1700/1759]`，Clang 22.1.8 在源码 `:1135:11` 报 `use of undeclared identifier '__gnu_unwind_frame'`。逐字副本见 [`22_failure_log_cxa_personality.raw.txt`](22_failure_log_cxa_personality.raw.txt)。

## 2. 21.1.1→22.1.8 版本差异

### 2.1 `cxa_personality.cpp` 核心差异

完整 diff 见 [`cxa_personality_21_1_1_to_22_1_8.full.diff`](cxa_personality_21_1_1_to_22_1_8.full.diff)。与本缺陷直接对应的 hunk 是：

- 21.1.1 `:905 #if !defined(_LIBCXXABI_ARM_EHABI)`，`:1010 #else`；
- 21.1.1 `:1012-1013` 在 EHABI 分支显式声明 `extern "C" _Unwind_Reason_Code __gnu_unwind_frame(...)`；
- 21.1.1 `:1022` 调用该函数；
- 22.1.8 对应分支删除声明，仍在 `:1135` 调用。

两代按实际编译参数进行只读预处理后，21.1.1 预处理翻译单元同时出现声明和调用，22.1.8 只出现调用；见 [`21_preprocessed_tu_gnu_unwind_frame.raw.txt`](21_preprocessed_tu_gnu_unwind_frame.raw.txt)、[`22_preprocessed_tu_gnu_unwind_frame.raw.txt`](22_preprocessed_tu_gnu_unwind_frame.raw.txt) 和 [`22_preprocessed_tu_declaration_assertion.tsv`](22_preprocessed_tu_declaration_assertion.tsv)。

### 2.2 libcxxabi EHABI/unwind 相关文件清单

逐文件 SHA256 与状态原表见 [`ehabi_unwind_file_delta.tsv`](ehabi_unwind_file_delta.tsv)，变更量见 [`ehabi_unwind_changed_file_diffstat.tsv`](ehabi_unwind_changed_file_diffstat.tsv)。

| 文件 | 状态 | 差异性质 |
|---|---|---|
| `include/__cxxabi_config.h` | CHANGED | 30+/21-；EHABI 判定 `:12-14` 原样不变，其余为编译器判断清理、异常特性判断和 pointer-auth 宏 |
| `include/cxxabi.h` | SAME | SHA256 相同 |
| `src/aix_state_tab_eh.inc` | SAME | SHA256 相同 |
| `src/cxa_default_handlers.cpp` | SAME | SHA256 相同 |
| `src/cxa_exception.cpp` | CHANGED | 4+/2-；`memset` cast 和注释文字变化，无 EHABI/unwinder 条件变化 |
| `src/cxa_exception.h` | CHANGED | 16+/14-；pointer-auth 成员注解变化，EHABI 布局条件仍保留 |
| `src/cxa_exception_storage.cpp` | SAME | SHA256 相同 |
| `src/cxa_handlers.cpp` | SAME | SHA256 相同 |
| `src/cxa_handlers.h` | SAME | SHA256 相同 |
| `src/cxa_noexception.cpp` | SAME | SHA256 相同 |
| `src/cxa_personality.cpp` | CHANGED | 112+/11-；包含本次关键声明删除 |

三个非 personality 的完整变更分别见：

- [`include___cxxabi_config.h_21_1_1_to_22_1_8.full.diff`](include___cxxabi_config.h_21_1_1_to_22_1_8.full.diff)
- [`src_cxa_exception.cpp_21_1_1_to_22_1_8.full.diff`](src_cxa_exception.cpp_21_1_1_to_22_1_8.full.diff)
- [`src_cxa_exception.h_21_1_1_to_22_1_8.full.diff`](src_cxa_exception.h_21_1_1_to_22_1_8.full.diff)

libcxxabi 树内没有独立随包携带的 `unwind.h`；实际用的是工具链头。两代 Clang `unwind.h` 完整 diff 为空，GCC `unwind.h` 完整 diff 也为空，见 [`clang_unwind_21_to_22.full.diff`](clang_unwind_21_to_22.full.diff) 和 [`gcc_unwind_header_21root_to_22root.full.diff`](gcc_unwind_header_21root_to_22root.full.diff)。

### 2.3 CMake unwinder 逻辑差异

根 `libcxxabi/CMakeLists.txt` 完整 diff 见 [`libcxxabi_CMakeLists_21_1_1_to_22_1_8.full.diff`](libcxxabi_CMakeLists_21_1_1_to_22_1_8.full.diff)。其中没有任何 `LIBCXXABI_USE_LLVM_UNWINDER`、`LIBCXXABI_ENABLE_STATIC_UNWINDER`、`LIBCXXABI_LIBUNWIND` 或 `LLVM_NATIVE_ARCH MATCHES ARM` 的改动 hunk，机械断言见 [`root_CMake_unwinder_changed_hunks.raw.txt`](root_CMake_unwinder_changed_hunks.raw.txt)。

两代均为：

- `libcxxabi/CMakeLists.txt:55`：`LIBCXXABI_USE_LLVM_UNWINDER` 默认 `ON`；
- `:56-58`：若为 ON 但 `LLVM_ENABLE_RUNTIMES` 不含 `libunwind`，直接 fatal；
- `:59`：static unwinder 默认 `OFF`；
- 21.1.1 `:474-489` / 22.1.8 `:478-493`：LLVM unwinder 为 ON 或 native arch 匹配 ARM 时查找 `libunwind.h`；
- `libcxxabi/src/CMakeLists.txt:87-89`：LLVM unwinder 为 OFF 时将 `gcc_s` 加入库标志；
- `libcxxabi/src/CMakeLists.txt:106-110`：LLVM unwinder为 ON 时才尝试 `--unwindlib=none`；
- `libcxxabi/src/CMakeLists.txt:166-171` 等：LLVM unwinder为 ON 时才接入 LLVM unwind objects/libraries。

两代 `src/CMakeLists.txt` 完整 diff 见 [`libcxxabi_src_CMakeLists_21_1_1_to_22_1_8.full.diff`](libcxxabi_src_CMakeLists_21_1_1_to_22_1_8.full.diff)；unwinder 条件本身未变化。逐行聚焦原文见 [`21_CMake_unwinder_focused.raw.txt`](21_CMake_unwinder_focused.raw.txt) 与 [`22_CMake_unwinder_focused.raw.txt`](22_CMake_unwinder_focused.raw.txt)。

实际打包没有采用默认值，而是两代命令行都明确传入 `-DLIBCXXABI_USE_LLVM_UNWINDER=OFF`，详见 [`21_success_log_cmake_unwinder.raw.txt`](21_success_log_cmake_unwinder.raw.txt) 和 [`22_failure_log_cmake_unwinder.raw.txt`](22_failure_log_cmake_unwinder.raw.txt)。

## 3. 声明来源实测

| 实测项 | 21.1.1 退役根 | 22.1.8 退役根 |
|---|---|---|
| Clang 包 | `clang-21.1.1-2.5.armv7l` | `clang-22.1.8-18.1.armv7l` |
| GCC 包 | `gcc-14.2.0-1.11.armv7l` | `gcc-14.2.0-1.14.armv7l` |
| 翻译单元实际 `unwind.h` | `/usr/lib/clang/21/include/unwind.h` | `/usr/lib/clang/22/include/unwind.h` |
| 两个 Clang 头 SHA256 | `fb7175e889f60907dbd0fd02fdc1571fcf33d867f3d0bbffb21356d2bd9b69ea` | 同左 |
| Clang 头声明 `__gnu_unwind_frame` | 否 | 否 |
| Clang target 空输入 `-dM -E` 定义 `__ARM_EABI_UNWINDER__` | 否（`rg` 退出 1） | 否（`rg` 退出 1） |
| 包含 `__cxxabi_config.h` 后定义 `_LIBCXXABI_ARM_EHABI` | 是 | 是 |
| GCC `unwind-arm-common.h` 中声明 | 是，`:215-216` | 是，`:215-216` |
| GCC `unwind-arm-common.h` 守卫 | 文件守卫 `UNWIND_ARM_COMMON_H`；并在 `:30` 无附加条件地定义 `__ARM_EABI_UNWINDER__` | 同左 |
| GCC 头是否进入真实翻译单元头链 | 否 | 否 |
| `libgcc_s.so.1` 是否导出目标符号 | 是，`__gnu_unwind_frame@@GCC_3.5` | 是，`__gnu_unwind_frame@@GCC_3.5` |

头文件来源包原文见 [`21_unwind_header_owners.raw.txt`](21_unwind_header_owners.raw.txt) 和 [`22_unwind_header_owners.raw.txt`](22_unwind_header_owners.raw.txt)。真实翻译单元头链来自与构建相同 target、sysroot、resource-dir、架构宏、`-nostdinc++` 和 include 目录的**仅预处理**运行，完整原文见：

- [`21_cxa_personality.translation_unit_include_chain.full.txt`](21_cxa_personality.translation_unit_include_chain.full.txt)
- [`22_cxa_personality.translation_unit_include_chain.full.txt`](22_cxa_personality.translation_unit_include_chain.full.txt)

Clang 头本身在 `unwind.h:15-40` 只对 Apple 使用 `include_next`；Tizen/Linux 走其自身定义分支。该头在 `:64-73`、`:108-155` 确实提供 ARM EHABI 类型布局，但全文没有 GNU 私有入口 `__gnu_unwind_frame`。完整带行号原文见 [`21_actual_clang_unwind_h.full.txt`](21_actual_clang_unwind_h.full.txt) 和 [`22_actual_clang_unwind_h.full.txt`](22_actual_clang_unwind_h.full.txt)。

两代编译器的完整 `-dM -E` 原文见 [`21_armv7l_clang_dM_E.full.txt`](21_armv7l_clang_dM_E.full.txt) 和 [`22_armv7l_clang_dM_E.full.txt`](22_armv7l_clang_dM_E.full.txt)；显式的缺失断言及退出码见 [`arm_eabi_unwinder_macro_assertion.tsv`](arm_eabi_unwinder_macro_assertion.tsv)。

## 4. 21.1.1 armv7l 成功构建实证复核

找到日志，非 `NOT_FOUND`：

`/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-armv7l/local/repos/tizen_unified_standard/armv7l/logs/success/libcxx-runtimes-21.1.1-2/log.txt`

SHA256：`74e6b4865741b3cbd70e2ff70abbb85986192b976f45ce7c34ff56dc60933bd8`。

实证：

- 日志 `:7726`：`[1692/1760] Building CXX object .../cxa_personality.cpp.o`；见 [`21_success_log_cxa_personality.raw.txt`](21_success_log_cxa_personality.raw.txt)。
- 日志 `:9747` 执行 `check-files`，`:9748-9755` 写出 1 个 SRPM、7 个 binary RPM，`:9765` 正常结束；见 [`21_success_log_products.raw.txt`](21_success_log_products.raw.txt)。
- 构建树 `cxa_personality.cpp` SHA256 为 `034b11551af56efbdc0fdd70c1eaf393cac5df3412f6499255e2eb4b7a13eaea`，与 T0 打捞源码完全一致；见 [`source_vs_build_personality_sha256.tsv`](source_vs_build_personality_sha256.tsv)。
- Ninja 日志不是 verbose 模式，故日志仅显示目标名；对保留的 `build.ninja` 使用只读 `ninja -t commands` 取出解析后的完整编译命令，未执行构建。紧凑原文见 [`21_cxa_personality_compile_command.only.txt`](21_cxa_personality_compile_command.only.txt)。

21.1.1 `cxa_personality.cpp` 的解析后 `-D` 集合为：

`-DLIBCXX_BUILDING_LIBCXXABI`、`-DNDEBUG`、`-D_DEBUG`、`-D_FILE_OFFSET_BITS=64`、`-D_LARGEFILE_SOURCE`、`-D_LIBCPP_BUILDING_LIBRARY`、`-D_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER`、`-D_LIBCXXABI_BUILDING_LIBRARY`、`-D__STDC_CONSTANT_MACROS`、`-D__STDC_FORMAT_MACROS`、`-D__STDC_LIMIT_MACROS`。

22.1.8 仅额外有 `-D_LIBCPP_AVAILABILITY_MINIMUM_HEADER_VERSION=2`；完整对照见 [`cxa_personality_D_options_comparison.raw.txt`](cxa_personality_D_options_comparison.raw.txt)。两代命令均含 `--rtlib=libgcc`，没有用编译定义手工强制 `_LIBCXXABI_ARM_EHABI`；该宏由 `__cxxabi_config.h:12-14` 根据 target 宏产生。

## 5. 22.1.8 上游配置语义（只读调研）

`LIBCXXABI_USE_LLVM_UNWINDER` 在 22.1.8：

- 定义及默认：`libcxxabi/CMakeLists.txt:55`，默认 `ON`；
- 启用约束：`:56-58`，必须把 `libunwind` 加入 `LLVM_ENABLE_RUNTIMES`；
- 影响面：`libcxxabi/src/CMakeLists.txt:106-110` 控制 `--unwindlib=none`，`:166-171` 及静态库对应条件控制 LLVM unwind objects/libraries；
- 关闭时：`libcxxabi/src/CMakeLists.txt:87-89` 添加 `gcc_s`；实际生成的 22.1.8 `build.ninja:540` 为 `LINK_LIBRARIES = -lgcc_s`，见 [`22_libcxxabi_link_stanza.raw.txt`](22_libcxxabi_link_stanza.raw.txt)。

关闭选项时，源码 EHABI 分支没有一个与 `LIBCXXABI_USE_LLVM_UNWINDER` 对应的 C/C++ 条件宏；`cxa_personality.cpp:1128-1131` 的上游注释只陈述 EHABI 逐帧推进要求，随后无条件调用 `__gnu_unwind_frame`。在本实际配置中，CMake 的行为是链接 `gcc_s`，而翻译单元声明来源仍取决于源码/头文件。

两代实际缓存一致：

| 缓存项 | 21.1.1 | 22.1.8 |
|---|---:|---:|
| `LIBCXXABI_USE_LLVM_UNWINDER` | OFF | OFF |
| `LIBCXXABI_ENABLE_STATIC_UNWINDER` | OFF | OFF |
| `LIBCXXABI_USE_COMPILER_RT` | OFF | OFF |
| `LIBCXXABI_LIBUNWIND_INCLUDES` | 空 | 空 |
| `LIBCXXABI_HAS_GCC_S_LIB` | 1 | 1 |
| `LLVM_ENABLE_RUNTIMES` | `libcxxabi;libcxx` | `libcxxabi;libcxx` |

原文见 [`21_CMakeCache_unwinder_extended.raw.txt`](21_CMakeCache_unwinder_extended.raw.txt) 和 [`22_CMakeCache_unwinder_extended.raw.txt`](22_CMakeCache_unwinder_extended.raw.txt)。

## 6. 可选处置路径清单（不推荐、不实施）

以下仅列技术含义与影响面，不排序、不作优劣判断：

| 路径 | 技术含义 | 影响面 |
|---|---|---|
| 源码兼容声明路径 | 在 ARM EHABI/libgcc 组合可见处恢复与 21.1.1 等价的 `extern "C"` 声明，或回移包含该修复的上游源码变更 | libcxxabi 源码补丁；直接作用于 `cxa_personality.cpp` 的编译期声明可见性；不改变当前 unwinder 链接选择 |
| 工具链头声明路径 | 使实际选中的 `unwind.h` 在 ARM EHABI 下声明 GNU 扩展入口，或调整受控头策略使 GCC ARM unwind 声明进入真实头链 | Clang/GCC 头文件策略及全部消费该头的目标；需核对类型/ABI 声明一致性 |
| LLVM libunwind 路径 | 将 `libunwind` 加入 `LLVM_ENABLE_RUNTIMES` 并启用 `LIBCXXABI_USE_LLVM_UNWINDER` | 改变当前“不启用 LLVM libunwind”的打包决策；涉及构建目标、对象/库链接、unwinder 制品与运行时依赖 |
| 源码版本路径 | 采用另一个已经包含 ARM EHABI/libgcc 声明闭合处理的 LLVM 源码版本或经确认的上游修订 | 变更源码身份锚与版本迁移范围；仍需按三架构重新验证 |

本报告没有修改任何一条路径，也没有验证任何处置后的构建结果。

## 7. 命令、退出码与异常留痕

所有正式取证命令均在 `progress/R5/commands/*.sh`；每次命令的 `WORKING_DIRECTORY`、shell-escaped 完整命令和 `EXIT_CODE` 逐条写在对应 `*.log`，脚本总退出码写在 `*.exit.tsv`。

保留的非零项及解释：

- `diff` 命令退出 1：表示两版文件存在差异，是预期状态，完整 diff 已保存；
- 初始宽范围 `rg/find` 搜索耗时过长，精确 PID 被终止，终止命令和退出码保留于 `01a/01b/02a_stop_*.exit.tsv`；随后改用已知边界精确定位；
- `03_exact_inputs` 的 `28_personality_sha` 退出 1：初始猜测的 21.1.1 build 路径不正确，后续定位到实际 `BUILD/libcxx-runtimes-21.1.1` 并由 `08` 完成 SHA 核验；
- `06_header_probe_preflight` 的直接执行 ARM Clang 退出 255：宿主不能直接运行 ARM ELF；`07` 改为 `qemu-arm-static -L <root>`，两代 `--version`、`-dM -E` 与头链探针均退出 0；
- `09` 第一次退出 94：只在 22.1.8 personality 文件本身搜索直接 `unwind.h` include，未覆盖其经 `cxa_exception.h:18` 的传递包含；第二次扩大到两文件后退出 0；
- `10` 第一次退出 94：错误地把“预处理输出出现调用”当成“出现声明”；第二次改为精确匹配 `extern "C"` 声明，21 返回 0、22 返回预期 1，脚本退出 0。

上述失败尝试未触发构建、写配置或修改输入；其原始账本均保留，未覆盖。

