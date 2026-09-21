# 当前状态：第二节 GDB 单轮复现运行中

人工已核定第 19 包零 lzma_code 调用；本轮去掉包装器，用 QEMU gdbstub 观测三个位置。方案见 GDB_PLAN.md。第一次尝试 14:56:36 启动，因发现 `-0 rpmbuild` 未保留 binfmt 的实际 guest argv[0]，15:11:20 主动终止，未到写包，记录保留在 cells/gdb-original/，不计入复现结果。已去掉 -0，改用原解析路径 /bin/rpmbuild；修正轮 15:13 启动，原始输出在 cells/gdb-preserved/。截止时间仍为 20:42:54+08:00。此时尚未取得第 19 包观测，不预判结果。

## 第一节历史状态（保留，不代表当前前置仍阻断）

原 ARM_LZMA_0921 全部681件材料已原样归档并推送；remote HEAD已核实为 `7c31ad73f48878cf77375fe137277d248be56481`。两件大文件无损压缩，解压后哈希一致，未改原内容。

第一节：`LOG_FILE_INTACT_BUT_ABSOLUTE_ZERO_CALLS_NOT_CONFIRMED`。

日志末行完整，209303行连续，PID匹配824601；逐调用write直写，但静默写失败路径、仅返回侧记录、未拦截初始化/结束、exec子进程不覆盖等限制仍在。没有实际漏记证据，也不能把绝对零调用作为无条件实测结论。

第二节：`NOT_STARTED`。未编译新包装器，未重跑rpmbuild，未启动任何构建。

本轮报告：`STAGE1_RESULT.md`；机器可读核查：`LOG_INTEGRITY.json`。第一节到此停止。
