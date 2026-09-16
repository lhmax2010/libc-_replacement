# 静态取消失败归因与修正验证

## 结论

**不是 libc++ 特有。** 同一 x86_64/glibc 构建根中，Clang + 静态 libstdc++ +
静态 libgcc 的四种等待形态 20/20 SIGABRT；GNU 动态对照 20/20 通过。
GDB 确认与上一轮 libc++ 相同的机制：共享 libgcc_s 执行取消展开，静态 personality
调用静态 `_Unwind_SetGR`，后者使用尚未初始化的本份寄存器尺寸表而 abort。

**只删 `-static-libgcc` 后，本次失败机制消除。** 静态 libc++/libc++abi 保留，
取消探针 20/20 通过；额外静态 libstdc++＋共享 libgcc 的单变量对照也 20/20 通过。
新 x86_64 bpftrace 候选新增 libgcc_s.so.1 动态依赖，没有引入 LLVM/BCC/libc++ DSOs；
version/help 与旧候选一致，strip 后 120,832,008 字节。

**原发布资产是否同样崩溃仍未确认。** 缺精确链接配方、可定位展开器的符号/调试文件
及可触发取消的入口；不能借本地候选的结果替它下结论。

**社区已有同类展开器混用说明，但未确认上游 bpftrace 官方形态复现此故障。**
上游静态 CI 用 musl；官方 AppImage 内部用动态 libgcc_s/libstdc++。本次不是一个
已证明的上游产品 bug，未向上游提交。

## 阅读与证据索引

| 文件 | 内容 |
| --- | --- |
| W1_REPORT.md | 同配置、逐次结果、回溯逐层对照、源码解释 |
| W2_REPORT.md | 原 tar/二进制身份、符号局限、三个项目取消调用点 |
| W3_REPORT.md | 上游构建形态、搜索口径、可访问社区出处、上报边界 |
| W4_REPORT.md | 精确链接变更、候选核查、取消反事实、GCC 回退复查 |
| QUESTIONS.md | 自行判断、未知项、技术性错误与修正 |
| STATUS.md / selfcheck.json | 阶段状态与机器可读自检 |
| raw/ | 包装器的原命令、stdout、stderr、退出码、时间、资源上限 |
| *-matrix.json | 每种等待每次运行的结果；完整命令输出在逐项 JSON |
| relink_plan_v2.json | 实际执行的候选链接命令与原命令 |
| inputs/ | 同一探针、引用输入的身份与相关源文件快照 |
| SHA256SUMS.tsv | 本次提交材料哈希 |

大体积原始命令/符号记录以 `.gz` 原样压缩，映射及解压后哈希见
`COMPRESSED_FILES.json`；报告提及未压缩名时，可据此找到对应归档。

## 交付范围

候选及大对象保存在 `tmp/STATIC_0917/`，项目仓提交可复核材料，不提交百 MB 二进制。
`codes/`、旧 `tmp/NIGHT_0917/`、Source1002 未修改；构建根与旧输入只读绑定。
不推包仓，不推 Gerrit，不覆盖发布资产。项目材料只推编译线分支。

本轮核心 x86_64 归因/修正已完成；原资产取消行为未定、armv7l 板离线，
前轮完整 RPM/功能等价等未闭合项也没有被本轮消除，**不能据此批准发布替换**。
