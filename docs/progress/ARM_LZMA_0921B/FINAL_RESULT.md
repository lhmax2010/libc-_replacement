# GDB 三点观测：返回时 encoding 已为 0

## 结果

**失败仍可复现。** 有效轮 2026-09-21 15:13:01—16:06:48（含即时取证约 53 分 47 秒），rpmbuild 退出 **1**，GDB 正常退出 **0**。仍有 18 条 `Wrote:`，第 7559/7560 行与上一轮逐字相同：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
error: create archive failed: cpio: write failed - Function not implemented
```

见 `cells/gdb-preserved/build.log:7559`、`:7560`，`POSTCHECK_GDB.json`。这不是“加 GDB 后失败消失”。没有启动下一轮、没有修复。

**本轮现场值是：返回处已经 encoding=0；到写入口仍为 0，且指针未换。** 不将它改写成“Fdopen 返回之后换了指针”，也不根据本轮三点推定 encoding 为何为 0。

## 三个观察点（全部 close_count=18）

| 观察点 | 实际值 | 原始依据 |
| --- | --- | --- |
| `lzopen_internal` 返回处 | 返回指针 `0x9a4958`；`encoding=0`；`file=0x6b1fe0`；`strm.internal=0x925500` | `cells/gdb-preserved/gdb-observations.jsonl:21` |
| `lzwrite` 入口 | `lzfile=0x9a4958`；`encoding=0`；`len=6`；与返回指针相等，差值 **0**；`file` 与 `strm.internal` 也相同 | 同文件 `:23` |
| `rpmcpioHeaderWrite` 入口 | `cpio=0x447000`；`fd=0x72ef68`；`fps=0x41ff18`；`io=0x40b1c564`；`ioname="xzdio"`；`name="xz"`；`fp=0x9a4958`，与返回值相等，差值 **0**；路径 `./usr/lib/debug` | 同文件 `:22` |

三条原始输出，按发生顺序原样列出：

```json
{"event": "lzopen_return", "monotonic_ns": 337706242645560, "close_count": 18, "pointer": "0x9a4958", "encoding": 0, "file": "0x6b1fe0", "internal": "0x925500"}
{"event": "cpio_header_entry", "monotonic_ns": 337706248831348, "close_count": 18, "cpio": "0x447000", "fd": "0x72ef68", "fps": "0x41ff18", "io": "0x40b1c564", "ioname": "xzdio", "name": "xz", "fp": "0x9a4958", "path": "./usr/lib/debug", "open_pointer": "0x9a4958", "equal_to_open": true}
{"event": "lzwrite_entry", "monotonic_ns": 337706250471126, "close_count": 18, "pointer": "0x9a4958", "length": 6, "open_pointer": "0x9a4958", "equal_to_open": true, "encoding": 0, "file": "0x6b1fe0", "internal": "0x925500"}
```

完整 GDB 输出：`cells/gdb-preserved/gdb.log`，SHA256 `b8ab0619323fdbce3940ec8ad508cd29ead624a0c13fcb5eb982a3f1a394b491`。结构化原始输出 SHA256 `158869f112ccf0dd8389e0fd55e6b2d939bf7956725d9677b6caee3efcc56920`。

补充边界：

- 前 18 次 `lzclose` 的入口均观测到 encoding=1；第 19 次为 0。**地址复用不代表跨包是同一对象生命周期，不能据此直接判定内存踩踏。**
- 本轮首次头路径实测是 **`./usr/lib/debug` 目录**，不是五个普通 debuginfo 文件之一；目录身份另见 `raw/129_numbered_evidence.stdout.txt` 的 stat 输出。
- “第 19”按本任务指定的 18 次关闭门槛识别。本轮没有额外读取包头 NAME；不凭这个各包可能共有的目录路径，独立认定失败 RPM 的包名。
- 三点各命中一次；没有 observer_error；没有激活条件 b 的额外返回断点。见 `GDB_RUN_SUMMARY.json`、`POSTCHECK_GDB.json`。

## 与任务预列情形的对应（只列事实，不裁决）

- a 的“encoding=0”成立，但“与返回指针不同”不成立；**返回处就已为 0**。没有观测到返回处→写入口之间换指针，也没有观测到这个区间的 encoding 从 1 变 0。
- b 的 encoding=1 不成立，因此没有追加 Fwrite / cpio 返回断点。
- c 的字段名需区分：这版实际定义为 `ioname="xzdio"`、`name="xz"`，并非 name 应为 xzdio。`../ARM_RPM_DIAG/upstream-rpmio.c:1029` 的实际内容为：

```c
static struct FDIO_s xzdio_s = {
  "xzdio", "xz",
  lzdRead, lzdWrite, NULL, lzdClose,
  NULL, xzdFdopen, lzdFlush, NULL, zfdError, zfdStrerr
};
```

  本轮 IO 身份与该定义一致，不把 name=xz 误报为错误派发。
- d 不成立：第 1 点实际命中，返回非 NULL。没有追加 Fdopen/cvtfmode 断点。

对应的既有源码 `../ARM_RPM_DIAG/upstream-rpmio.c:939`：

```c
static ssize_t lzwrite(LZFILE *lzfile, void *buf, size_t len)
{
    lzma_ret ret;
    size_t n;
    if (!lzfile || !lzfile->encoding)
	return -1;
```

**代码对应而非额外返回值实测**：本轮入口 encoding=0 满足这段早退条件。实际部署 ELF 的 ARMv7 反汇编也从 `LZFILE+0x805c` 读 encoding，再比较 0；见 `raw/132_lzwrite_armv7_disassembly.stdout.txt:14` 起。本轮没有断在 lzwrite 返回处，不能把返回 -1 的寄存器值写成另外一条实测。

## 仪表可信度与本轮实际变化

- 以 QEMU gdbstub 连接 guest rpmbuild（PID 938038）；未加载 LD_PRELOAD 包装器，未加 -vv，原 host strace 过滤器保留。
- 原 argv：`../ARM_REPRO_0920/original-argv.json`；有效轮：`cell_commands/gdb-preserved.json`。**完整 argv 并非完全相同**：必要的显式 QEMU `-g 49192 /bin/rpmbuild` 入口，以及证据输出路径有差异，逐项见 `ARGV_DIFF_CORRECTED.txt`；rpmbuild 参数尾部逐字节相同。
- 原 binfmt 没有 F 标志，构建根中的 QEMU 实为 **9.2.3**；本轮使用同一二进制，SHA256 `8ee43bc800f9d708a8606310fa9cce476ce4fb02d31cfd910fd9b50cdbba470a`。不是宿主另有的 5.2.0。见 `raw/047_binfmt_identity.*`、`048_guest_qemu_debug_help.*`、`096_corrected_guest_identity.*`。
- 独立下载/解包 GDB 与 RPM 符号，未安装或替换构建根工具。三个库的 debuglink CRC 全部吻合，见 `SYMBOL_IDENTITY.json`。
- 符号包有函数位置但没有完整 LZFILE 类型，故按**实际 ELF 反汇编确认的偏移**读取；encoding 为 +0x805c，file 为 +0x8058，strm.internal 按本根头文件和 ARM ABI 位于 +0x8024。没有把缺失类型信息伪称为完整 DWARF 变量打印。
- `lzopen_internal` 断在 link-time 0x13840：此前 0x1383c 已把返回值装入 r0，之后是栈恢复和返回；见 `raw/044_armv7_disassembly.stdout.txt:357`。没有 finish、手工单步、inferior 函数调用或变量修改。
- 启动时使用 loader rendezvous 解析库基址；此后只保留关闭计数，在第 18 次关闭时启用三点，命中后自动继续。这些控制动作与源码脚本均已申报在 `GDB_PLAN.md`。
- 执行脚本为 `gdb_observer.py`，SHA256 `68a492c61491fd089e3a3a9194946e3b5604255096461a748403cf4c2eb39b00`；执行副本与 guard/capture_failure 一并保存在 `cells/gdb-preserved/executed-*.py`。
- 仍使用共享增量 BUILD 树，没有恢复历史时刻的全部文件状态；不能声称历史环境所有时变因素都已控制。

## 失误与技术性非零申报

第一次尝试错误加入了 `-0 rpmbuild`，未保留 binfmt 的实际 guest argv[0]。只读 shell 对照发现后，主动停止，**未进入任何 lzclose，未到写包**。该轮约 14 分 51 秒，退出 -15；GDB 因目标终止而断线退出 1。记录保留在 `cells/gdb-original/`、`raw/072_binfmt_argv0_check.*`、`073_stop_argv0_mismatch.*`、`074_stopped_state.*`。它既不算复现失败，也不算“失败消失”。随后去掉 -0、使用原解析路径 /bin/rpmbuild，才执行上面的有效轮；没有延长六小时截止。

准备阶段还包括调试符号 URL 的 404、宿主 GDB 不支持 ARM、默认 ARM 解码不适合 ARMv7、符号搜索路径未命中等修正，详情见 GDB_PLAN.md；都保留原始记录。`raw/126_corrected_progress` 退出 1 是末尾 ps 查询的两个进程已退出，与正式 rpmbuild 的退出码分别记录，不能混同。

## 资源与现场留存

- medium 门禁退出 0；磁盘初查超过 20 GiB，收尾仍有约 186 GiB。
- 初始 I/O 探测 0.080 秒，修正轮前 0.087 秒。
- cgroup MemoryMax **16536457216 字节**（总内存 50%）、nice 19、ionice idle；实际 Ninja `-j1 -j 1`。每项实查记录在 `events.jsonl` 与 `raw/096_corrected_guest_identity.*`。
- 没有达到 500 个新增构建目标（本轮 96），故未触发该周期的 I/O 探测；没有 I/O 暂停事件。内存上限/OOM 的抽查计数均为 0，不据此判断 guest 地址空间是否耗尽。
- 失败后立即取目录权限，再记文件大小、mtime/ctime、SHA256 与输出目录状态：`failure-permissions.txt`、`failure-file-metadata-immediate.json`、`failure-file-metadata.json`、`failure-processes.json`。**未清理失败现场。**
- 上一轮真实失败现场已复制到 `tmp/ARM_LZMA_0921B/previous-preload-failure/`，16,992 条目核验通过。当前失败现场仍在原 `tmp/WEEKEND_0918/rpm-arm-llvm/`。
- rpmbuild、三个 RPM 库、liblzma、spec 收尾 SHA256 全部与运行前一致，见 POSTCHECK_GDB.json。没有改平台源码、spec、project_config、根内 rpm/liblzma；没有推 Gerrit 或包仓。
- 完整日志仍有八条早先 spec 查询错误，与上一轮的文本及行号一致；不是本轮新增，也未修复。未删除这些记录以突出末尾错误。

## 未观测与待人工裁决

以下均 **NOT_OBSERVED**：本次 lzopen_internal 的入口 mode 原文；encoding 在函数内部的赋值过程；是谁、何时使其成为 0；本次编码器初始化返回值；本次 lzma_code 调用次数；lzwrite/Fwrite/cpio 的实际返回寄存器值。

因此不自行定为内存踩踏、mode 错误、初始化问题、liblzma 错误或 VA 耗尽。**本轮新增、足以交人工审阅的事实是“lzopen 返回时 encoding 已经为 0，而非仅在其后观察到不同指针”。** 按要求停下，不新增试验、不重编 librpmio。

## 交付状态

此前 ARM_LZMA_0921 全部材料已推送，远端确认 SHA：`7c31ad73f48878cf77375fe137277d248be56481`，见 DELIVERY_VERIFIED.json。

本轮新材料保存在 `progress/ARM_LZMA_0921B/`。已非阻塞询问是否把本轮新材料也归档并推送项目仓；截至本报告写成尚未收到答复，因此本报告不声称已推送。

收尾只读核对发现，项目 HEAD 与远端现已前移到 `787d1b88865922698867ab5adda90141e21bf77a`（15:19:29，中途归档第一节与部分调试记录）。该提交不含本文件，不能当作本轮最终报告已交付的凭证。本执行阶段未创建这个提交，也未覆盖其已归档材料。见 `raw/135_project_head_check.*`、`136_remote_snapshot_contents.*`；后者退出 128 是 `git cat-file -e` 确认最终报告路径不在该提交中。
