# 静态展开器核查：FAIL（静态 0/20，动态 20/20）

本轮不能判定候选可替换发布资产。失败是实际取消行为不满足断言，不是命令工具
执行失败，也不是仅缺少观察。没有自行修正链接配置或修改上游 C++。

## 被测对象与方法

探针原文来自运行时分支提交 `2955f679d0be0b7aee96ae49125955940d01467f` 的
`docs/progress/R111/code/condition_cancel_probe.cpp`，由 git show 取得，未切换或
合并分支。编译线自己的 R111 是另一项任务，未混用。原文与执行副本哈希见
probe_source_identity.json；复制校验允许的唯一差异是文件末尾换行，不改逻辑。

使用与候选相同的 x86_64 构建根、Clang 22.1.8、本夜原始 libc++.a/libc++abi.a
和 libgcc_eh.a。保留候选所用 -stdlib=libc++、-static-libgcc、-static-libstdc++
及两标准库静态归档链接组；不为探针增加 export-dynamic 或另换展开器。
动态对照用同次生成的 libc++.so/libc++abi.so 与 libgcc_s，实际加载路径已由 ldd
记录。探针只链自己所需的线程/标准库等组件，不链无关的 LLVM、BCC 等库。
这是同标准库/展开配置的最小实验，不是直接在 bpftrace 内注入取消。

四种等待形态均裸 pthread_create、显式 deferred cancellation，等候主线程
确实取得 worker 持有并在 wait 中释放的 mutex 后才取消。断言同时检查取消穿过、
RAII 清理恰好一次、pthread_join 得到 PTHREAD_CANCELED，而非仅看退出码。

| 等待形态 | 静态（每格 5 次） | 动态（每格 5 次） |
| --- | --- | --- |
| wait | 5 次退出 134，清理/join 标记均未输出 | 5/5 全部断言通过 |
| system_clock | 同上 | 5/5 全部断言通过 |
| steady_clock | 同上 | 5/5 全部断言通过 |
| custom_clock | 同上 | 5/5 全部断言通过 |

逐次结果在 cancel_matrix.json，完整命令和 stdout/stderr 在 w3_commands.json。
收集脚本自身退出 0 仅表示收集完成；不等于行为通过。独立判据检查将返回 1。
134 是 bwrap 所报告的退出值；另由 GDB 实测确认目标收到 SIGABRT。

## 内嵌展开器及补丁身份

两架构原候选的 link map 证明实际拉入 libgcc_eh.a，不是 LLVM libunwind；
libc++abi.a 的 cxa_personality.cpp.o 也实际拉入。ar 提取的该成员与其编译目录
对象逐字节一致。对应哈希、静态/动态 marker 符号在 static_abi_identity.json。
两架构可见 _Unwind_ForcedUnwind、__gxx_personality_v0、__forced_unwind 的
析构、RTTI 与 vtable；__gcc_personality_v0 本次未匹配到，不据此否认 libgcc。

任务书给出的字符串 `_ZN10__cxxabiv114__forced_unwind` 没有命中；实际标记是
`_ZN10__cxxabiv115__forced_unwind...`，类型名长度是 15。原字面检索结果和
实际阳性同时保留，没有把错误的长度前缀导致的零命中说成类型不存在。

源树为 f203923a，cxa_personality.cpp:822–837 包含 forced-unwind typed catch
识别；代码原文见 raw/038。606–609 为向展开上下文写回寄存器，1119 调用它。
static/dynamic 均有补丁 marker，但本轮只执行条件变量取消形态，不宣称完整重跑
四个补丁的所有场景。ARM EHABI 是 ARM32 路径，本轮 x86_64/aarch64 不适用；
armv7l 未执行，NOT_OBSERVED_BOARD_OFFLINE。

## 失败位置：实测与解释分开

raw/037 的 SIGABRT 回溯给出这条实际调用链：

```
libc.so.6 的取消路径
  → libgcc_s.so.1::_Unwind_ForcedUnwind
  → 探针内 __gxx_personality_v0（静态 libc++abi）
  → cxa_personality.cpp:606 的 _Unwind_SetGR
  → 探针内 _Unwind_SetGR.cold（静态 libgcc）
  → abort
```

尽管静态探针无 libgcc_s 的 DT_NEEDED，运行时 shared-library 列表确实出现它。
因此“没有直接动态依赖”不能推出“取消时不会载入动态展开器”。静态与动态
libgcc 文件都归属 **libgcc-14.2.0-1.10.x86_64**，不是已观察到的包版本不一致。

raw/049 在实际静态 _Unwind_SetGR 入口停止：rsi=0，对应静态
dwarf_reg_size_table 的前 17 个字节全为 0；实际 PC 的反汇编从这张表读出
尺寸并与 8 比较，不等则进入 cold/abort 分支。结合 SIGABRT 回溯，可以定位
到静态寄存器尺寸状态未就绪时执行了寄存器写回。

“共享展开流程与静态访问函数/私有状态混用”是这些证据支持的机制解释；本轮
没有改变绑定方式或初始化逻辑验证修法，不外推为所有静态 libc++ 组合必失败。
也不声称已经在 bpftrace 产品功能中观察到了同一崩溃；本轮实证对象是同配置探针。

GDB 首次停在取消用 SIG32，后按原样传递信号并继续至 SIGABRT。另一次按名称
反汇编解析到了共享库同名函数，raw/049 改以实际 $pc 取指，避免混淆两个定义。
GDB 的 libthread_db 自动加载警告保留，未放宽自动加载安全设置。

## 边界

此结果只覆盖当前 x86_64、glibc、两份展开实现以及 deferred cancellation。
没有 aarch64 取消运行结果、没有异步取消结果、没有修复后的结果。
当前结论为静态行为门禁失败，交人工决定后续；不能仅补齐 RPM 就改判可发布。
