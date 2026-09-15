# 静态库与 bpftrace-static 夜间任务

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 libcxx 静态库 | COMPLETE_TARGET_BUILDS | 944 秒 | runtime-*.build.log、specs 副本 | x86_64 优先；原源码和构建根只读挂载 |
| W2 libclang.a | NOT_STARTED | 0 | 待补 | 仅相关目标，限 5 小时 |
| W3 bpftrace-static | NOT_STARTED | 0 | 待补 | 依赖 W1/W2；候选不推包仓 |
| W4 21.1.1 归档来源 | NOT_STARTED | 0 | 待补 | 编译器身份须独立核查 |

项目分支 codex/r5-ehabi-diagnosis；候选仅 tmp/NIGHT_0917/。
内存限制 ulimit -v 16148878 KiB（每进程虚拟地址空间）；单任务 -j1、nice19、ionice3。
构建监控脚本按 500 个 Ninja 目标探测 I/O，超时暂停进程组 10 分钟，最多三次；阶段硬截止时间写入 events。
小时进度见 HOURLY_STATUS.md（含每格启动记录）；开发板离线，不连接、不等待。

## 已知技术性重跑

- 最初固定 PATH 未含 rg 实际目录；补入工具安装目录后重新定位。
- raw/006 检索范围过宽，终止本次两个检索进程后改查确切 BUILD 路径；未终止他人任务。
- codes 检出未含 f203923a 对象；已找到 tmp/BUILD_W1BC_0911/target-fetch 的干净 f203923a，不把旧检出当基线。
- bwrap 首次绑定 /task 位于只读根，失败；改绑定新建临时挂载 /tmp/night，不更改原根。
- 直接 rpmspec 不自动加载 project_config 的 toolchain_is；采用实际配置第 134 行宏定义显式传入，保留原表达式。不是把门禁替换为常量。

## 兼容性口径

libcxx-runtimes 原 spec 强制 Clang，与 _toolchain=gcc 的平台选择无关。
本轮该 GCC 分支继续使用原 Clang 编译器，不擅自将运行时改为 GCC 构建；
验证新静态开关在 GCC/未定义配置均 OFF，且展开后的 spec 与基线相同（忽略空行）。
新静态归档的 %files 条目也必须同条件保护，否则开启安装后会产生未打包文件。
完整 RPM 写包尚未执行，不把 CMake 目标构建写成全包验证。
