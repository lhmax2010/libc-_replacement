# 当前状态：第二节已完成，停下待人工审阅

结论：`FAILURE_REPRODUCED_ENCODING_ZERO_AT_OPEN_RETURN`。有效轮退出 1，18 条 Wrote，build.log 第 7560 行仍为原 cpio 错误；GDB 退出 0。三个点各命中一次：返回指针、FD 层 fp、lzwrite 参数均为 0x9a4958；返回处 encoding 已为 0，写入口仍为 0；ioname=xzdio/name=xz；len=6，头路径 ./usr/lib/debug。失败现场未清理，未修复、未追加试验。完整报告 FINAL_RESULT.md，核对 POSTCHECK_GDB.json。

旧轮 ARM_LZMA_0921 已推送，SHA 7c31ad73f48878cf77375fe137277d248be56481。收尾核实当前远端为 787d1b88865922698867ab5adda90141e21bf77a（15:19 的中途快照，含第一节及部分调试记录），不含本轮最终报告。本轮最终材料是否推项目仓已另询问，尚未收到确认，不宣称完整推送。

## 第二节运行历史

人工已核定第 19 包零 lzma_code 调用；本轮去掉包装器，用 QEMU gdbstub 观测三个位置。方案见 GDB_PLAN.md。第一次尝试 14:56:36 启动，因发现 `-0 rpmbuild` 未保留 binfmt 的实际 guest argv[0]，15:11:20 主动终止，未到写包，记录保留在 cells/gdb-original/，不计入复现结果。已去掉 -0，改用原解析路径 /bin/rpmbuild；修正轮 15:13 启动，原始输出在 cells/gdb-preserved/。截止时间仍为 20:42:54+08:00。此时尚未取得第 19 包观测，不预判结果。

## 第一节历史状态（保留，不代表当前前置仍阻断）

第二节小时进度（15:42:35）：当前有效轮已写出 5 个 RPM，lzclose 计数 5；目标三个点未启用；cgroup memory.events 的 max/oom/oom_kill 均为 0。仍在运行，尚无复现结论。证据 raw/106_corrected_progress.*。

原 ARM_LZMA_0921 全部681件材料已原样归档并推送；remote HEAD已核实为 `7c31ad73f48878cf77375fe137277d248be56481`。两件大文件无损压缩，解压后哈希一致，未改原内容。

第一节：`LOG_FILE_INTACT_BUT_ABSOLUTE_ZERO_CALLS_NOT_CONFIRMED`。

日志末行完整，209303行连续，PID匹配824601；逐调用write直写，但静默写失败路径、仅返回侧记录、未拦截初始化/结束、exec子进程不覆盖等限制仍在。没有实际漏记证据，也不能把绝对零调用作为无条件实测结论。

第二节：`NOT_STARTED`。未编译新包装器，未重跑rpmbuild，未启动任何构建。

本轮报告：`STAGE1_RESULT.md`；机器可读核查：`LOG_INTEGRITY.json`。第一节到此停止。
