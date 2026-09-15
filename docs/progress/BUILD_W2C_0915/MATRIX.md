# 构建及产物核查矩阵

| 包 | 架构 | 路径 | 编译器 | 构建 | 标准库依赖 | 核查 | 来源 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| bcc-tools | aarch64 | libcxx | Clang 22.1.8 | PASS | libc++.so.1 | 1次 | 沿用 |
| bcc-tools | aarch64 | gcc | GNU 14.2.0 | PASS | libstdc++.so.6 | 1次 | 沿用 |
| bcc-tools | x86_64 | libcxx | Clang 22.1.8 | PASS | libc++.so.1 | 1次 | 沿用 |
| bcc-tools | x86_64 | gcc | GNU 14.2.0 | PASS | libstdc++.so.6 | 1次 | 沿用 |
| bcc-tools | armv7l | libcxx | Clang 22.1.8 | PASS | libc++.so.1 | 5次 | 本轮 |
| bcc-tools | armv7l | gcc | GNU 14.2.0 | PASS | libstdc++.so.6 | 5次 | 本轮 |
| bpftrace | armv7l | libcxx | Clang 22.1.8 | PASS | libc++.so.1 | 1次 | 本轮 |
| bpftrace | armv7l | gcc | GNU 14.2.0 | PASS | libstdc++.so.6 | 1次 | 本轮 |
| bpftrace | aarch64 | libcxx | Clang 22.1.8 | PASS | libc++.so.1 | 1次 | 本轮 |
| bpftrace | aarch64 | gcc | GNU 14.2.0 | PASS | libstdc++.so.6 | 1次 | 本轮 |

bcc-tools 六格全部闭合，已推送。bpftrace 四格中的 PASS 指完整构建、
两个源码构建程序及实际 LLVM/BCC 提供方的标准库检查；不包含对预编译
static 子包内嵌标准库的确认，不能据此将整包推送门禁写为已闭合。
bpftrace x86_64 不在 ExclusiveArch，未列入矩阵，不算失败格。
bcc-tools armv7l 每格的五次为同一批 RPM 的重复核查，不是五次独立构建。
本轮未使用开发板，未进行 bcc/bpftrace 内核跟踪功能实测。
