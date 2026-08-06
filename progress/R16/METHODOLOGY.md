# R16 口径与方法

日期：2026-08-06（Asia/Shanghai）。全程只读；未安装、未构建、未修改输入 RPM、源码树、spec、配置或开发板状态。RPM 仅解包至 `tmp/R16/extracted/`，GNU glibc 2.40 上游源码仅下载并解包至 `tmp/R16/`。

## 输入口径

- 平台对象：复用 R11 已下载的三架构 `glibc`、`libgcc`、`compiler-rt` 二进制 RPM；本任务重新计算 RPM 与所分析对象 SHA256，见 `tables/input_identity.tsv`。
- 上游 LLVM：本地 `codes/llvm` 为 LLVM 22.1.8 树；另以 `git ls-remote` 固定查询时 upstream main HEAD，并直接读取其 `llvm-libgcc` 文件。
- glibc 源码旁证：GNU 官方 glibc-2.40.tar.xz，SHA256 `19a890175e9263d748f627993de6f4b1af9cd21e03f080e4bfb3a1fac10205a2`。它用于说明同版本上游 API 调用链；平台二进制结论仍只取自平台 `libc.so.6`。

## 符号计数口径

- 分母单位是“每架构、每个 ELF 动态导出符号的唯一基础名”；只计 `.dynsym` 中 `GLOBAL/WEAK`、已定义且非 `ABS` 的符号，去掉 `@/@@版本` 后按基础名去重。
- 版本节点表的单位是“符号—版本节点关系”；同一基础名如在多个节点出现，分别计数，因此不得与唯一基础名分母直接相加比较。
- 类别互斥且优先级为 ARM EHABI → unwind → compiler helper → 其他。ARM EHABI 包含 `_Unwind_VRS*`、`__gnu_Unwind*`、`__gnu_unwind_frame`、`__aeabi_unwind_cpp_*`；unwind 包含其余 `_Unwind*` 与 `__gcc_personality_v0`；helper 依名称族匹配 `__aeabi_*`、算术/转换/原子/fixed-point 等编译器运行时例程；未命中者全部逐名进入“其他”。
- compiler-rt 覆盖仅为 `libgcc_s.so.1` helper 与 `libclang_rt.builtins-*.a` 的“精确符号名集合”交/差集。该口径不证明 ABI、语义、可见性、版本节点或最终共享库导出是否兼容。
- `armv7l __aeabi_*` 表独立于互斥分类：所有 `__aeabi_*` 均列出，包含 EHABI personality 名称。

## glibc 判定口径

- 对每个 `libc.so.6` 分别执行 `strings -a -t x`、`readelf --dyn-syms`、`readelf -d`。
- “硬编码 SONAME”要求二进制中实测存在 `libgcc_s.so.1` 字面量；“动态加载形态”要求 `_Unwind_*` 名称在字符串表出现、对应动态 UND 为零且 `DT_NEEDED` 不含 libgcc_s。三项同时成立才作该结论。
- API 归属由平台二进制 fatal 文本直接给出 `pthread_cancel`、`pthread_exit` 与一般 unwinding；`backtrace` 路径由平台二进制 `_Unwind_Backtrace` 字符串和 GNU glibc 2.40 `debug/backtrace.c`/`unwind-link.h` 同版本源码旁证。
- 构建工具链仅接受 RPM 元数据或 ELF `.comment` 原文。三架构 RPM 的 BUILDHOST/BUILDTIME 被清空，三份 libc 均无 `.comment`，故记 `NOT_AVAILABLE`。

## 运行时观察口径

- 板端只允许既有命令和只读环境变量；本轮 ping 与 TCP 端口可达，但 SDB 协议连接失败，未获得执行载体，因此没有板端运行行为。
- 宿主旁证以同一 `python3` 进程做基线与 `ctypes` 调用 glibc `backtrace()` 两次独立运行；只把 `LD_DEBUG=libs` 的新增 `find library=libgcc_s.so.1` 和 `calling init` 视为实测加载。结果不得冒充 Tizen 板端结论。
