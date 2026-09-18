# 静态取消问题：可对外使用的限定结论

在已测 x86_64、glibc 2.40、libgcc 14.2 组合中，动态 glibc 的线程取消走入共享 `libgcc_s.so.1`，但静态 personality 所调的 `_Unwind_SetGR` 落到程序内另一份静态展开器，其私有寄存器尺寸表未初始化而 abort。libc++ 与 libstdc++ 两组同形态均20/20崩溃，说明该已复现机制不是 libc++ 独有。两组保留静态标准库、改用共享libgcc_s后均20/20通过。这里libstdc++对照用的是Clang driver选择GNU标准库，不冒称GCC driver实测。

这些是前轮原始实测的复用，不是本周末重跑。来源：`docs/progress/BUILD_STATIC_0917/W1_REPORT.md`、`W4_REPORT.md`，其 SHA256 见本轮 raw/574；具体回溯/寄存器/逐次结果路径在原报告中。

| 架构与标准库 | 静态标准库 + 静态 libgcc | 静态标准库 + 共享 libgcc_s | 证据身份 |
| --- | --- | --- | --- |
| x86_64 libc++ | 20/20 SIGABRT | 20/20 PASS | 前轮已归档实测，沿用 |
| x86_64 libstdc++ | 20/20 SIGABRT | 20/20 PASS | 前轮同配置对照，沿用 |
| armv7l libc++ | **20/20 PASS** | **20/20 PASS** | 本周末物理板新增，四种等待各5次 |

**ARM 结果与“静态对照预期崩溃”不同。** 本次不能声称两架构同样崩、任何动态glibc+静态libgcc都必崩，或删除该选项在所有平台上都已证明“根治”。ARM链接map证明实际引入GNU `libgcc_eh.a`，不是遗漏静态选项；其行为为何不同，尚未完成源码/调用级归因。ARM板上glibc为2.40-2.8、libgcc为14.2.0-1.14，x86复用环境的RPM release为1.10，因此也不能声称两平台环境逐项相同。

上游构建事实沿用已归档社区核查：v0.24.2 静态CI基于Alpine/musl、使用 `-static-libgcc`，不走本次已复现的glibc路径；这不等于已实测所有musl场景安全。官方AppImage内部程序是另一种带共享GNU运行库、LLVM/BCC等依赖的形态，不能与musl静态CI混写。原始出处与下载身份见 `docs/progress/BUILD_STATIC_0917/W3_REPORT.md`（本轮574哈希确认）。

原 Source1002 资产已strip、配方未知，不能用近似形态探针断言原二进制必崩。本轮ARM原包正常安装预检又因缺BCC/Clang库未通过，真实bpftrace功能和产品内取消行为仍未证明。源码搜索未检出直接调用，不等于排除所有间接取消路径。

发布结论独立于探针：本轮候选的RPM/安装/功能等价门禁仍有缺口，**尚不可作为已验收的发布资产替换件**。
