# 第一节：包装器日志完整性核查（仅已有材料）

## 先推送的回执

原 `progress/ARM_LZMA_0921/` **681 份文件全部归档后提交并推送**，未修改任何原文件内容。项目分支 `codex/r5-ehabi-diagnosis` 远端已核实为：

```text
7c31ad73f48878cf77375fe137277d248be56481
```

见 `raw/007_commit_previous.*`、`008_push_previous.*`、`009_remote_receipt.*`。远端材料在 `docs/progress/ARM_LZMA_0921/`。390981728 字节的 host trace 与 16410332 字节的调用表采用无损 xz 存储；两件均逐件解压并核验原 SHA256，未删减内容。其余原件逐字节复制；另增存储说明与完整交付清单。共 683 件提交文件，凭据自检通过，未带入工作区其他改动。原报告“未推送”作为历史状态原样保留，未在推送前改写。

## 核查结论

**日志文件本身完整、有序；但“第 19 包绝对没有调用 lzma_code”尚不能作为无条件实测结论。** 已证实的是：现存日志中有 209303 条完整的返回记录，18 次正常流结束按顺序与 18 个已写出 RPM 的 ARCHIVESIZE 全部一致，其后没有调用记录。

没有发现实际漏记证据；然而包装器并非“所有进程、入口与出口、日志提交结果均完备”的仪表，现有材料不能严格排除尾部漏记或覆盖外调用。**第一节的严格前置条件未完全闭合，第二节未启动；未编写/替换扩大后的包装器，未重跑 rpmbuild。**

### 1. 每次是否立即写日志？

**每次真实 lzma_code 返回后立即用 write(2) 写；不是 stdio 缓冲。** 不需要等 fflush/fclose 才把这条日志从用户态送到内核。实际源码 `../ARM_LZMA_0921/script-snapshots/lzma_tap.c:25–33`：

```c
static void write_all(int fd, const char *p, size_t n)
{
    while (n) {
        ssize_t done = write(fd, p, n);
        if (done < 0 && errno == EINTR) continue;
        if (done <= 0) return;
        p += done; n -= (size_t)done;
    }
}
```

每条调用记录在同文件 `:98–110` 的顺序是：

```c
lzma_ret ret = real_code(stream, action);
int after_errno = errno;
struct timespec now;
clock_gettime(CLOCK_MONOTONIC, &now);
char line[512];
unsigned long long seq = ++sequence;
/* snprintf(...)：完整实码见源码快照 */
write_all(trace_fd, line, (size_t)n);
```

日志 fd 在 `:67` 用 `O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC` 打开。**没有 O_SYNC、fsync/fdatasync；不能称每条已同步到稳定介质。** 本轮并无系统崩溃证据，不能凭未 fsync 反推实际丢失。

**确实存在的完整性边界**：`:30` 遇到非 EINTR 的负返回或 0 会静默返回，不向调用方报告“日志未写完”；计数在写入之前已增加。若中段丢记录且后续仍记录，会留下序号缺口；若尾部连续丢失，现存前缀仍可连续。该包装器没有最终计数/写日志失败计数/结束标记来独立核实尾部。

只记录失败调用的宿主 trace 中，负返回 write/pwrite64 仍为 0。这是支持未见日志写失败的证据，但未保留每次成功的日志 write 返回值，**不等于每条日志均写全的独立确认**；也不能据此宣称发生了静默丢尾。

### 2. 最后一行是否完整、序号是否连续？

**是。** 本轮重新只读解析全部 16410332 字节，而非沿用上一份汇总：

| 核查 | 实际结果 |
| --- | --- |
| 数据行数 | 209303（不含身份行、表头） |
| 列数 | 每条 14 列，无坏行 |
| 文件结尾 | LF 换行，无半行 |
| 序号 | 严格为 1…209303，无缺口、重复、倒退 |
| 单调时间戳 | 不倒退 |
| 最长实际数据行 | 84 字节（含换行） |
| 返回值 | 209285 次 0；18 次 1，全部 action=3 |
| 文件 SHA256 | f652b37e366028646437840cf9bb5eecbff05a6cb37fe4a58413ec8dd64a590c |

最后一行原文（字段间为 Tab，末尾 LF 已核实）：

```text
209303	329405	11734888	0x9ac958	3	1	38	38	0	32768	0	32584	28811180	5210308
```

证据：`LOG_INTEGRITY.json`、`raw/013_existing_log_audit.*`，可复查脚本 `audit_existing.py`。**“末行完整”只证明落盘文件没有半条尾行，不独立证明没有整条尾记录缺失。**

### 3. 是否拦截其他三个 lzma 函数？

**没有。** 实际 DSO 动态符号表仅定义拦截入口 `lzma_code`；未定义 `lzma_end`、`lzma_stream_encoder_mt`、`lzma_easy_encoder`，也未拦截 fdopen/dup。源码只用 `dlsym(RTLD_NEXT, "lzma_code")`，见快照 `:72` 与 `:93`，以及 `raw/011_symbols_and_package_code.stdout.txt` 的 readelf 实际输出。

因此本轮日志不能给出第 19 包编码器初始化、释放、fdopen/dup 的实际返回值。所有调用记录中的 strm 地址都是 `0x9ac958`；**没有生命周期事件，地址本身不能区分再次使用的编码器实例**。18 个归档的对应依据是正常流结束顺序及 RPM 载荷字节数，不是“18 个不同指针”。

### 4. 加载范围和 PID 是否对应？

**使用 LD_PRELOAD；rpmbuild 本体匹配，但不是所有子进程都覆盖。**

- 包装器 header 实测 `pid=824601`。
- 既有宿主进程快照 PID=824601，实际 argv 以 `/usr/bin/qemu-arm-static /bin/rpmbuild` 开始，包含本次 `--noprep --noclean -bb`；status Name=rpmbuild、Pid/Tgid=824601。见 `../ARM_LZMA_0921/rpmbuild-host-snapshot-125730.json:2` 起与本轮 `raw/010_wrapper_source.stdout.txt`。
- guest maps 记录该 DSO 已加载；宿主 trace 第 3172956 行为 `824601 +++ exited with 1 +++`，见 `LOG_INTEGRITY.json`。它没有以 SIGKILL/SIGSEGV 的退出记录结束；此项不等于所有函数调用的完整性证明。
- 源码 `:87–88` **有意执行**：

```c
/* Scope preload to this process; do not instrument the build's children. */
unsetenv("LD_PRELOAD"); unsetenv("QEMU_SET_ENV"); unsetenv("LZMA_TAP_PREFIX");
```

  配合日志 fd 的 O_CLOEXEC，后续 exec 的子 shell/构建程序不继承这套加载和日志 fd。单纯 fork、尚未 exec 的孩子会继承映射/描述符，这是 fork 与 exec 的区别，不能笼统说“所有孩子都没有”。调用行未带 PID/TID，header 仅标明初始化者 PID，不能逐条以 header 排除 fork-only 记录。

**这一范围缺口不等于已经证明第 19 包在未覆盖的子 shell 中压缩。** 既有固定源码 `../ARM_RPM_DIAG/upstream-pack.c:651–711` 在包循环里直接调用 writeRPM，`:549–551` 直接调用 cpio_doio，`:80–86` 调用 Fdopen/rpmPackageFilesArchive；这条所示归档路径本身没有把写包交给外部 shell。见 `raw/012_archive_execution_path.*`。这是静态核查，不能代替第 19 包的动态 PID/调用链观测。

### 5. 第 19 包“零调用”究竟是哪一种结论？

**准确说法：日志覆盖内零新增返回记录是实测；把它扩大为绝对零调用，可能受漏记/覆盖边界影响，尚未严格证实。**

三种情况必须区分：

1. 文件中无第 19 个归档的记录：已核实。18 次 STREAM_END 的 total_in 与 18 个已写出 RPM 的 ARCHIVESIZE 全相等，最后结束后无下一行。
2. 包装器中已返回调用是否全部成功写入：中间无缺口、末行完整、宿主失败 write 零命中构成支持；**末尾日志写入完整性缺独立确认**。
3. 所有进程中是否从未进入第 19 包的 lzma_code：**NOT_OBSERVED**。仪表只在真实调用返回后记录，且未覆盖 exec 子进程；没有入口/生命周期事件或逐调用 PID。

上一轮提到 `lzwrite:943` 的对象检查，只是列出未观测的源码分支，**没有测得 lzfile 无效，也不能拿它当成已证实原因去反驳人工的初始化推理**。本次不把无效对象、fwrite 短写或初始化失败中的任何一种补成事实。

## 门禁与交接

结论标记：`LOG_FILE_INTACT_BUT_ABSOLUTE_ZERO_CALLS_NOT_CONFIRMED`。

没有发现实际漏记的阳性证据，但现有材料不足以无条件闭合“绝对零调用”前置。按“第一节答完先停一次”，**本轮到此停止，第二节 NOT_STARTED**。是否接受上述限定下推进加宽仪表，交人工裁决；未自行改变前置或换包装器。

若后续获准，除任务列出的初始化/结束与 fdopen/dup 事件外，需明确日志写入失败的可见性、调用入口/返回区分、逐事件 PID/TID 及指针重复使用的生命周期对应；这里仅登记证据缺口，**没有实施**。

## 本轮操作和限制

- 开工 2026-09-21 14:30+08；第一节在一小时内完成。medium 门禁返回0，可用191GiB。只做发布及已有文件核查，无构建/guest执行/板上操作。
- 压缩与扫描命令使用 nice19、ionice idle、systemd scope MemoryMax=16536457216；发布很快完成，随后查 scope 已 inactive、MemoryMax=infinity，**这是结束后的查询，不能冒充运行时上限实测**，原输出在 raw/004。没有据此把任何运行期内存测量写入结论。
- 收尾只读校验单独在其进程内读取 cgroup 和 memory.max，实测为16536457216；重新逐件核验原681件内容均未变化，见 `DELIVERY_VERIFIED.json` 与 raw/014。这项只证明该校验进程的实际限制，不追溯替代已退出的发布进程采样。
- 本轮未做新 I/O 写探针；没有启动构建。资源门禁/磁盘检查、命令及退出码均保留。
- 原 ARM_LZMA_0921 正文与证据不修改。报告中的限定是本轮新增核查结论；未静默更正旧报告。
- 没有推包仓/Gerrit，没有 force，没有处理用户其他未提交改动。
