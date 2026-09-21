# armv7l RPM 失败轮 lzma_code 实测结果

## 结论

**失败复现，但没有 lzma_code 错误返回。** 本轮退出 1，仍在第 7559 行写出第 18 个 RPM `clang-devel-debuginfo-22.1.8-1.armv7l.rpm`，第 7560 行报：

```text
error: create archive failed: cpio: write failed - Function not implemented
```

共记录 **209303 次** `lzma_code`：**209285 次 LZMA_OK(0)、18 次 LZMA_STREAM_END(1)**，后者全部是 LZMA_FINISH(action=3)。MEM_ERROR(5)、PROG_ERROR(11) 及其他错误返回均为 **0 次**。序号严格为 1…209303，单调时间戳不倒退。见 [RESULT.json](RESULT.json)、[POSTCHECK.json](POSTCHECK.json)、[完整构建日志](cells/preload-original/build.log:7559)。

这不是“加仪表后失败消失”，也不是已测到 MEM_ERROR；**任务三选一的 c 项只有前半句得到支持，后半句“失败来自 fwrite 短写”尚未测得，不能补写成事实。** 当前状态为 `TARGET_FAILURE_WITHOUT_LZMA_ERROR_RETURN_SHORT_FWRITE_NOT_YET_OBSERVED`。本轮一轮结束即停止，未加第二种仪表、未继续构建或修复。

## 执行身份与唯一仪表

- 正式轮：2026-09-21 **12:56:20.445363+08 至 13:48:33.111652+08**，含失败现场取证 **3132.666289 秒（52 分 12.666 秒）**。总任务沿用 17:20 截止，未延时。
- 原 argv 的宿主 strace 过滤器 `-e status=failed` 保持；binfmt 入口保持，未加入 `-vv`、客体 `-strace`。
- 增加 LD_PRELOAD 包装器及必要的独立目录绑定/加载环境；仅 strace 输出文件换成本轮路径保护旧证据。去除仪表加载部分、恢复旧输出路径后，其余 argv 完全一致。完整命令与差异在 [ARGV_DEVIATIONS.json](ARGV_DEVIATIONS.json)、[执行命令](cell_commands/preload-original.json) 和 `raw/030_preload_repro.*`。
- 输出仍为 `/tmp/task/rpm-arm-llvm/`，`_binary_payload w5T1.xzdio`、`_build_create_debug 1`、`--noprep --noclean -bb` 不变。
- spec SHA256：`cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`，收尾复核未变。
- 实际 DSO SHA256：`a663845ffa1e25895507de2e56b355946a443fa0bb2e281c848c4d0d21095f2f`；源码 SHA256：`8dd71b1ec006a045354290b06a2655ee2a0aeae1c28bcab3c99f92ad15735c87`。实际快照在 `script-snapshots/`；旧 CLOCK_REALTIME 版只是准备历史，未用于正式轮。
- 真实 rpmbuild 与 liblzma SHA256 前后保持：分别 `1b3dbf19fed47cd7d30f6ed68ebf8a050f6b09204f086180d04bc43f91976578`、`d29fa8718e3a5ab66836df7553328e3241b7221aeaf2f8b151b97ed4423bc10e`，见 `raw/091_root_identities.*`。

包装器逐调用原样转发参数/返回值、恢复真实调用后的 errno；记录序号与返回后的 CLOCK_MONOTONIC 时间。正常/MEM_ERROR/PROG_ERROR 三种阳性校准均做无包装/有包装对照，返回序列、errno、计数、输出哈希一致；单调时间戳版本重新校准通过。见 [METHOD.md](METHOD.md)、[CALIBRATION_MONOTONIC.json](CALIBRATION_MONOTONIC.json)、[MONOTONIC_LOG_CALIBRATION.json](MONOTONIC_LOG_CALIBRATION.json)。人工构造的错误码不作为正式轮错误证据。

## 调用序号、时间戳与归档流位置

| 项 | 序号 | 单调时间戳（秒） | action | ret | errno 前/后 | total_in | total_out |
| --- | ---: | --- | ---: | --- | --- | ---: | ---: |
| 最后一次 LZMA_OK | 209302 | 329405.011674728 | 3 | 0 | 38 / 38 | 28811180 | 5210124 |
| 最后一次正常流结束 | 209303 | 329405.011734888 | 3 | 1 | 38 / 38 | 28811180 | 5210308 |
| 错误返回调用 | NOT_OBSERVED | NOT_OBSERVED | — | 未记录错误返回 | — | — | — |

两次最后正常返回相隔 **60160 ns**；这不是“成功到失败”的间隔，因为没有失败调用可配对。逐行原始数据在 [rpmbuild-lzma.calls.tsv](instrument-output/rpmbuild-lzma.calls.tsv:209304)，表前另有身份注释与表头，数据行号为序号+2。

**18 次正常流结束的 total_in，与按 Wrote 顺序读取的 18 个 RPM 的 ARCHIVESIZE，18/18 相等。** 最后一次结束的 28811180 字节对应 `clang-devel-debuginfo`。见 `POSTCHECK.json` 内每个 `rpm -qp --qf` 的命令、退出码与输出。这是在现有产物上的只读对应核查，不是新实验。调用记录在该正常结束后终止，未记录第 19 个流的 lzma_code 调用。

所有 209303 次调用的 errno_before/after 都是 **38/38**。它证明在已记录的正常调用中 errno 持续保留；**未直接读取 RPM 报错瞬间的 errno**，不能把这一限定删掉。

## VA 与第 2c 项

- 实际 guest 进程身份 PID **824601**；包装器 header 与宿主 selected-environment 核查都给出 **QEMU_RESERVED_VA=0x100000000**。
- **失败瞬间 VmSize/VmPeak/VmRSS、maps 条目数/最大空洞：NOT_OBSERVED。** 快照触发器是首次非 OK/非 STREAM_END 返回，本轮该条件未出现，因而没有该快照。错误消息出现后 rpmbuild 已退出；不得用初始或中途数据冒充失败瞬间。
- 初始化快照（仅作明确区分）：VmSize=4404504 kB、VmPeak=4404504 kB、VmRSS=10028 kB；guest maps 82 条，映射长度合计 17944576 字节；[0,2^32) 中最大算术空洞为 `0x41117000–0xffff0000`，3203239936 字节。见 `instrument-output/rpmbuild-lzma.initial.*`。**status 的 QEMU 进程计数不是纯 guest 已占用 VA，maps 空洞也不保证分配成功。**
- 中途宿主 status/maps/cgroup 已分别保留在 `rpmbuild-host-snapshot-125730.json`、`rpmbuild-host-snapshot-132743.json`，不是失败现场 VA。
- **第 2c 成功文件访问序列：NOT_OBSERVED**，遵照人工确认，未扩大原 strace 过滤器。

因此没有实测依据将本轮归因为 32 位 VA 耗尽/碎片，也没有证明 `-vv` 改变堆布局而使成功的具体机制。

## 回查 fwrite 与错误分支：能说明什么、不能说明什么

1. 扫描本轮完整宿主 trace（390981728 字节），所选范围内返回 -1 的 write/pwrite64（含 resumed 行）为 **0**。原始 trace SHA256 为 `c87a70c5d0bce6b3040fb9cce9ebb9dfd20f2e9137b135a978d5afa45738a4eb`。计数代码在 `postcheck.py`，结果在 `POSTCHECK.json`。这不等于直接观测了 guest 的 fwrite 返回值或 stdio 状态。
2. 既有固定源码 `../ARM_RPM_DIAG/upstream-rpmio.c:939–961`：lzwrite 对正常对象的循环内，lzma_code 非 OK 或 fwrite 返回不足都可返回 -1；**本轮只测了前者**，没有拦截 fwrite/Fwrite/ferror。其 `:943–944` 还有对象/encoding 检查；`Fwrite` 的 `:1333–1349` 也有分派前提，不能只靠顶层错误文本确认具体分支。
3. `../ARM_RPM_DIAG/upstream-pack.c:26–67` 中，文件数据复制失败会在 `:45–46` 设置 failedFile；`:88–95` 区分带路径和不带路径的错误消息。本轮实际是不带 `on file` 的消息。**这是静态控制流核查，不是失败 PC 的实测**；它提醒不能仅据“write 而非 read”就断言已经走进某个文件数据块循环。
4. `../ARM_RPM_DIAG/upstream-cpio.c` 有多处返回 RPMERR_WRITE_FAILED，除了 `:224–225` 的 fileend/offset 检查，还有模式、写头、对齐、尾部路径。**本轮仍未测得具体失败 PC、Fwrite 返回值或 fileend/offset，不能把某一分支当成已测前提。**
5. 当前能反驳的是：“只剩 lzma_code 必返回错误这一环”的排他性推断没有被本轮支持。**不能替换成另一条未经实测的排他结论“所以一定 fwrite 短写”。** 尚缺直接的失败分支/FD 分派及 fwrite 返回、ferror 等状态；下一轮若扩大仪表范围，需另行确认，本轮未实施。

源码摘录与哈希保留在 `raw/085_rpmio_followup.*`、`086_source_followup.*`、`088_pack_source.*`、`090_evidence_identity.*`。未从未记录的成功系统调用补推文件序列。

## 现场、产物与资源

- `capture_failure.py` 已实际触发，先保存元数据再做 SHA256；`cells/preload-original/failure-file-metadata-immediate.json`、`failure-file-metadata.json`、`failure-permissions.txt`、`failure-processes.json` 保留。五个 LLDB 调试文件总计 **344668300 字节**，散列期间 size/mtime/ctime 稳定；这不等于证明整个归档期间都没有变化。没有清理现场。
- **本轮写出 18 个 RPM，不是 21 个或 22 个。** 目录现有 21 个，另 3 个 `lldb-devel-debuginfo`、`compiler-rt-debuginfo`、`libomp-debuginfo` 是旧成功轮残留，不能算本轮成果。`RESULT.json` 为每件标注 `wrote_in_this_run`。现存 21 件摘要核验均通过，不改变本轮失败的结论；此前 22 件及旧失败树另存未删。
- 启动及中途确认实际 rpmbuild cgroup 上限 **16536457216 字节（50%）**、nice 19、I/O idle、并行 1。预检门禁返回 0，I/O 探测 0.056955 秒，结束磁盘仍有 191 GiB。增量 Ninja 实际完成 96 目标，未达到每 500 目标探测触发点；不能声称构建中每分钟做过 I/O 探测。
- 共享 BUILD 增量树未冻结/回滚，argv 一致不是整个状态字节级一致，仍是实验限制。

## 技术性非零与交付

- 正式轮退出 1 是目标故障，日志完整保留。`raw/082` 中缺少 error 快照的 ls 报不存在，原因如上；`raw/086` 最后 rg 无匹配返回 1；`raw/084` 内 ps 返回 1 表示目标进程已退出。均不当作压缩层错误码。
- 后处理第一次（raw/087）使用当前 Python 不提供的 hashlib.file_digest，退出 1；只将后处理哈希改成分块 sha256，raw/089 重跑成功，**没有重跑构建或改变包装器**。
- 本轮新增的方法解释及证据充分性判断如本报告所列；未宣称 fwrite 短写、内存耗尽或某个具体源码分支已证实。未修改平台源码/spec/config/rpm/liblzma，未推包仓/Gerrit。
- 完整命令、stdout/stderr、退出码在 `raw/`；结果机器表为 `RESULT.json`、`POSTCHECK.json`；报告仅据本轮实际记录填写。材料保存在本项目本地 `progress/ARM_LZMA_0921/`，未执行新的推送操作。

**已停，交人工审阅。**
