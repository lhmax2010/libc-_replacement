# R19：chromium-efl C++ 运行时形态实测

## 口径与输入

本报告只复用 R11/R13 已落盘的平台数据，并对其中已经解包的 42 个 Chromium 运行时 ELF 做定向 `nm -D -C --with-symbol-versions`；没有重扫平台仓。二进制包口径为 15 个 RPM 实例（5 个 Name × 3 架构），运行时口径为 42 个 ELF。输入路径、大小与 SHA256 见 `../inputs/input_identity.tsv`，逐 ELF 命令与退出码见 `tables/chromium_targeted_nm_commands.tsv`。

SRPM 为 `chromium-efl-1.1.144-1.src.rpm`，大小 7,223,059,319 字节。开始前 `/home` 可用 631 GiB。旧 R11 的 `rpm2cpio` 失败原文为 `files over 4GB not supported by cpio, use rpm2archive instead`。本轮没有展开源码归档：先以错误的精确成员名调用一次（退出 2、空输出，保留为失败证据），随后用 `rpm2archive | tar -xzOf --wildcards '*chromium-efl.spec'` 只提取 spec，退出 0；spec 为 54,254 字节，SHA256 `a58b9c12e1a8226a86a438e3fe45675f1481f6831c5f9ff75fd601dbe6f90485`。

## 二进制侧结论

- 42/42 ELF 的 DT_NEEDED 含 `libstdc++.so.6`；0/42 含 `libc++.so.1`，0/42 含 `libc++abi.so.1`。
- 定向动态符号记录计数：`std::__1` 0，`__gnu_cxx` 103，`std::__cxx11` 988。计数单位是“逐 ELF 去重后的动态符号记录”，ELF 分母为 42。
- 40/42 ELF 直接 DT_NEEDED `libgcc_s.so.1`；其余精确依赖见 `tables/chromium_dt_needed.tsv`。

据此，当前这批 chromium-efl 二进制使用系统动态 `libstdc++.so.6`，不是只靠静态链入的自带 libc++，也没有动态装载平台 `libc++/libc++abi` 的证据。源码树内部是否另含未进入当前二进制的 libc++ 副本，本任务没有展开源码归档，记 `NOT_AVAILABLE`。

## 展开器与闭包

R13 中 6 个含运行时 ELF 的 Chromium 二进制 RPM 实例均为 `DIRECT`，属于风险集；逐实例见 `tables/chromium_closure_assignment.tsv`。11/42 ELF 有 `dlopen` 未定义引用。

对真正以 `_Unwind` 开头的 UND 引用，实测为 0 条。R11 的宽泛候选表曾命中一个 V8 名称 `_ZN2v88Unwinder...`，它不是展开器 `_Unwind*` ABI；本报告分别保留 `chromium_unwind_candidates_from_R11.tsv` 和收紧后的 `chromium_unwind_undefined.tsv`，不把该相似名称计入展开器引用。

## 构建配置事实

spec 第 343 行把 `_clang` 缺省设为 1，第 351–354 行据此设置 `__use_clang`，第 693–696 行把 `is_clang=true/false` 传给 GN 包装脚本。spec 中没有 `use_custom_libcxx`、`libc++`、`libstdc++` BuildRequires 或等价标准库选择字面量。GN 包装脚本及其默认参数位于未展开的源码归档内，因此“GN 是否另有自定义 libc++ 开关”记 `NOT_AVAILABLE`；当前产物的实际运行时形态由上一节二进制证据给出。

## ABI 标识对照

Chromium 42 个 ELF 中 `std::__1` 动态符号记录为 0；我方 c2 三架构 `libc++.so.1.0` 各实测 1,759 条 `std::__1` 动态符号记录，见 `tables/project_libcxx_abi_marker.tsv`。两者不是同一种当前标准库形态，故“两个自带 libc++ 的 ABI 版本是否一致”在当前 Chromium 产物上不适用。

## 双 libc++ 路径事实

当前 Chromium 没有动态 libc++，所以没有观测到“当前 Chromium 自带 libc++ + 未来系统 libc++”的既存共载路径。若未来某产物同时装载两份使用同一 `std::__1` 内联命名空间的 libc++，两者对应 C++ 符号名会相同；这只是符号命名事实，本报告不对行为作评级或建议。

完整数据见 `tables/`；无构建、无安装、无源码归档展开。
