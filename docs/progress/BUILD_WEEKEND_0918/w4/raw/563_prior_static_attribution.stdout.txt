# 同配置 libstdc++ 对照：同样崩溃，机制一致

## 输入与可比性

使用与上一轮 libc++ 探针相同的 x86_64 Tizen 构建根、Clang 22.1.8、glibc
2.40-1.10、libgcc 14.2.0-1.10。GNU 标准库为 libstdc++ 14.2.0-1.10。
本轮选用任务书允许的 **Clang + libstdc++**，不是声称用 GCC driver 编译了本轮探针。
`toolchain-runtime-identities.json` 另记录根内 GCC 14.2.0 的实际版本。

探针取自运行时分支 `2955f679d0be0b7aee96ae49125955940d01467f` 的
`docs/progress/R111/code/condition_cancel_probe.cpp`，复用上轮归档原文，未改逻辑。
编译线自己的同编号文档不是本探针来源。源码副本及 SHA256 一并归档。

| 项目 | 上轮 libc++ 静态组 | 本轮 libstdc++ 静态组 |
| --- | --- | --- |
| driver / 优化 / 调试 | Clang 22.1.8；C++20、O0、g、fno-inline | 相同 |
| 标准库选择 | `-stdlib=libc++`；指定对应头文件及两份 `.a` | `-stdlib=libstdc++`；driver 默认 GNU 头文件/静态库 |
| 展开器选择 | `-static-libgcc` | 相同 |
| 标准库静态选择 | `-static-libstdc++`；显式 libc++.a/libc++abi.a 组 | `-static-libstdc++` |
| 线程及探针自身依赖 | `-pthread -latomic` | 相同 |
| glibc | 动态 | 动态 |
| export-dynamic | 未添加 | 未添加 |

转换标准库所必需的头文件/归档路径也随之改变，没有把它们描述为逐参数完全相同。
`gnu-static-driver.json`、`gnu-static-compile.json` 记录实际完整命令；link map
确认 personality 来自 libstdc++.a，展开器对象来自 libgcc_eh.a。
GNU 动态对照用共享 libstdc++/libgcc_s；同配置静态 GNU 组没有这些 DT_NEEDED。

## 结果

| 形态 | GNU 静态标准库＋静态 libgcc | GNU 动态标准库＋动态 libgcc | GNU 静态标准库＋动态 libgcc（额外单变量复核） |
| --- | --- | --- | --- |
| wait | 5/5 SIGABRT，断言未通过 | 5/5 PASS | 5/5 PASS |
| system_clock | 5/5 SIGABRT，断言未通过 | 5/5 PASS | 5/5 PASS |
| steady_clock | 5/5 SIGABRT，断言未通过 | 5/5 PASS | 5/5 PASS |
| custom_clock | 5/5 SIGABRT，断言未通过 | 5/5 PASS | 5/5 PASS |

20 次普通运行中 bwrap 返回 134；GDB 独立确认 SIGABRT。动态组及额外复核均断言
wait 已进入、清理恰好一次、cancel/join 返回 0、join 值为 PTHREAD_CANCELED。
额外复核仅删除 GNU 静态命令中的 `-static-libgcc`，保留静态 libstdc++，不依赖
“同时换成动态标准库”解释修复效果。结果见 `gnu-matrix.json` 与
`gnu-counterfactual-matrix.json`。收集程序退出 0 不代表静态取消成功。
除 wait 的回溯和寄存器检查，还对其余三种形态分别收集一次 GDB 回溯，均在
同一 `_Unwind_SetGR.cold` 链上 SIGABRT，见 `gnu-gdb-abort-*.json`。

## 回溯逐层对照

| 执行层（由取消向外展开） | 上轮 libc++ | 本轮 GNU |
| --- | --- | --- |
| 取消点 | glibc pthread_cond_wait / signal / 取消内部函数 | 相同类别 |
| 强制展开入口 | `/lib64/libgcc_s.so.1::_Unwind_ForcedUnwind` | 同一共享库入口 |
| phase2 | libgcc_s 内部帧 | libgcc_s 内部帧 |
| personality | 可执行文件内的 libc++abi `__gxx_personality_v0` | 可执行文件内的 libsupc++ `__gxx_personality_v0` |
| 写回辅助函数 | 静态 `_Unwind_SetGR`，经 libc++abi 的 set_registers | 静态 `_Unwind_SetGR` |
| 失败出口 | `_Unwind_SetGR.cold` → abort → raise → SIGABRT | 相同 |

GNU 回溯 #3 为 `_Unwind_SetGR.cold`，#4 为 personality，#6 为共享库
`_Unwind_ForcedUnwind`。两组不同函数内部的额外帧、地址并不逐字相同。
上一轮证据：`../BUILD_STATIC_0916B/static_gdb_abort.json`、
`static_gdb_registers_pc.json`；本轮：`gnu-gdb-abort.json`、`gnu-gdb-registers.json`。

GNU 组在实际 `_Unwind_SetGR` PC 停下时，rsi=0，静态
`dwarf_reg_size_table` 前 17 字节均为 0。实际 PC 反汇编读取该表，比较尺寸 8，
不等跳往 cold/abort，与 libc++ 组一致。没有按同名函数猜地址，也没有放宽
GDB auto-load 安全设置；libthread_db 警告原样保留。

静态 libgcc_eh 与动态 libgcc_s 均归属同一 RPM 版本，SHA256 在
`toolchain-runtime-identities.json`，本轮没有观测到两者版本错配。

## 源码对应与因果边界

只读 `codes/gcc` revision `49c5060508ce8a6ed6beca375233688a6bdd9140` 中：

```c
/* libgcc/unwind-dw2.c:149 */
static unsigned char dwarf_reg_size_table[__LIBGCC_DWARF_FRAME_REGISTERS__+1];
/* :280–281 */
gcc_assert (index < (int) sizeof(dwarf_reg_size_table));
size = dwarf_reg_size_table[index];
/* :1316–1318 */
init_dwarf_reg_size_table (void)
{
  __builtin_init_dwarf_reg_size_table (dwarf_reg_size_table);
```

`:1340–1347` 在 uw_init_context_1 中通过 once 初始化本份私有状态。
`libstdc++-v3/libsupc++/eh_personality.cc:728–732` 调用 `_Unwind_SetGR`
设置返回寄存器，随后 `_Unwind_SetIP`。实际内容和上下文见两个 gcc-source JSON。
该只读源树作为机制对应，不冒充已逐文件证明就是本机 RPM 的精确构建输入；
结论主要依据当前二进制 GDB 和单变量反事实实验。

**归因：不是 libc++ 独有，而是本次动态 glibc＋静态 libgcc 场景中的两份展开器
及其私有状态混用。** 不能把这句话外推为“任何静态 libgcc 程序必崩”；是否到达
取消/展开路径、静态状态是否此前初始化、架构和绑定方式都会影响可见行为。

本轮未测异步取消、aarch64 取消及 ARM EHABI。armv7l 为
`NOT_OBSERVED_BOARD_OFFLINE`。
