# 本轮方法与证据边界

## 已确认的执行口径

人工选择保留原宿主 strace 过滤器。正式轮只增加独立的 `lzma_code` 包装器及其必要的加载/输出绑定；strace 输出改到本轮目录，避免覆盖旧证据。没有加入 `-vv` 或客体 `-strace`。`ARGV_DEVIATIONS.json` 记录全部 argv，去掉仪表加载部分、恢复旧 trace 输出路径后，其余 argv 与原件一致。

第 2c 项（前一次成功与失败之间成功的文件访问序列）为 **NOT_OBSERVED**，不以只记录失败调用的 strace 补推。替代证据是包装器逐调用序号和 `CLOCK_MONOTONIC` 时间戳。时间戳在真实调用返回后采集，并非调用开始时间。

## 仪表

guest 未提供可用的 ltrace，改用在 guest 内独立编译的 ARM ELF32 LD_PRELOAD 库。包装器调用 `dlsym(RTLD_NEXT, "lzma_code")`，原样透传输入指针、action 和返回值，恢复真实调用返回时的 errno，不修改 lzma_stream 字段。只记录顶层 rpmbuild，不向构建子进程传播 preload。

实际执行版本：

- C 源码 SHA256：`8dd71b1ec006a045354290b06a2655ee2a0aeae1c28bcab3c99f92ad15735c87`
- DSO SHA256：`a663845ffa1e25895507de2e56b355946a443fa0bb2e281c848c4d0d21095f2f`
- 源码/二进制快照：`script-snapshots/`。

`PREPARATION.md` 是确认前的历史记录，其中旧 CLOCK_REALTIME 版本与“等待确认”状态已被 `STATUS.md` 的启动记录及本文件取代；旧版本未用于正式轮。

正常压缩、主动分配失败、非法 action 三个阳性校准均做无包装/有包装对照；返回序列、errno、流计数、压缩输出 SHA256 一致。实际单调时间戳版本重新校准，见 `CALIBRATION_MONOTONIC.json` 与 `MONOTONIC_LOG_CALIBRATION.json`。校准中的 MEM_ERROR/PROG_ERROR 是人为构造，不是目标失败的证据。

首次非 OK/非 STREAM_END 返回后保存 guest 读取的 status/maps/limits。正常 FINISH 的 STREAM_END 不属于错误；结果解析还会单独识别 RUN 时意外返回 STREAM_END 的情况。

## 资源与共享输入限制

正式轮 2026-09-21 12:56:20+08 启动，沿用 17:20+08 截止。实际 rpmbuild PID 为 824601，cgroup `memory.max=16536457216`（总内存的 50%），nice 19、I/O idle、构建并行度 1。启动及中途核查见 `rpmbuild-host-snapshot-125730.json`、`rpmbuild-host-snapshot-132743.json`。这些不是失败瞬间快照。

共享 BUILD 增量树没有回滚或冻结。旧成功输出树已另存并逐项核验 16993 项；没有删除旧失败现场。相同 argv 不等于所有文件状态和时序完全相同，本轮不能排除这类变量。

## 不能扩大解读的部分

1. 包装器不改压缩参数和返回值，但加载 DSO、诊断输出可能改变地址布局和时序，不能称零侵入。失败消失即停止，不再加仪表。
2. guest `/proc/self/status` 的 VmSize/VmPeak 不能直接等同纯 32 位 guest 已占用 VA：校准中这两项已超过 4 GiB，含 QEMU 进程计数因素。另按 guest maps 的 32 位地址区间计算条目数和空洞。
3. maps 最大空洞只是不被映射的区间，不保证能够按某个分配请求成功分配；MEM_ERROR 也不自动证明 VA 耗尽或碎片。
4. 未记录失败的分配请求尺寸，不能只凭返回码证明 `-vv` 使失败消失的具体机制。
5. 若全部 lzma 调用均正常返回而 RPM 仍报错，不能把 fwrite 短写直接写成实测结论；该返回值尚需另一项直接证据。
6. 本轮未修改平台源码、spec、project_config、构建根中的 rpm 或 liblzma；不推包仓或 Gerrit。
