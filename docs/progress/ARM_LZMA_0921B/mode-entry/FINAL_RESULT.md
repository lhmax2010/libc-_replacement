# 入口 mode 原始字节：终止符后实测含有 r

## 结论

**失败仍然复现，且本轮命中人工指定的机制确认判据。** 在 `lzclose` 计数等于 18 时，`lzopen_internal` 入口的 `mode` 为 `"w5T1\0"`；其第一个终止符之后、下一个终止符之前，实际包含 `0x72`（字符 **r**）。不是终止符后紧接另一个 `\0`。

同一次调用返回时 `encoding=0`，之后 `lzwrite` 使用同一指针且 `encoding=0`。仍在写出 18 个 RPM 后报原错误。没有修复、没有另起试验，现场保留。

## 先行推送

开始试验前已原样归档并普通推送上一轮 ARM_LZMA_0921B 最终报告及全部材料，远端核实 SHA：

`85e8238b72a2fab9505a9fc306b57ecf2ff69134`

895 个源文件全部入清单；三份大记录无损压缩并解压回验。只提交指定文档子树，凭据扫描通过。见 `raw/004_publish_previous.*`、`009_scan_staged.*`、`010_commit_previous.*`、`011_push_previous.*`、`012_remote_verify.*`。上一报告里“尚未推送”是保留的历史原文，由上述实际推送记录补充。

## 16 字节原始观测

`mode` 指针：**`0x407fef5c`**；观察条件：**close_count=18**。

```text
偏移   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
十六进 77 35 54 31 00 b0 b1 40 68 ef 72 00 80 ef 7f 40
可打印 w5T1...@h.r....@
```

可打印行以 `.` 表示非 ASCII 可打印字节；十六进制行完整保留全部 16 字节。

- 第一个 `\0`：偏移 **4**。
- 下一个 `\0`：偏移 **11**。
- 两者之间的全部 **6 字节**：`b0 b1 40 68 ef 72`。
- 该片段的可打印形式：`..@h.r`。
- `r` 位于偏移 **10**，即地址 **`0x407fef66`**。

原始记录：`cells/gdb-mode-entry/gdb-observations.jsonl:21`，原文：

```json
{"event": "lzopen_entry", "monotonic_ns": 344593722534459, "close_count": 18, "mode_pointer": "0x407fef5c", "raw_hex": "77 35 54 31 00 b0 b1 40 68 ef 72 00 80 ef 7f 40", "raw_printable": "w5T1...@h.r....@", "first_nul": 4, "next_nul": 11, "after_first_nul_hex": "b0 b1 40 68 ef 72", "after_first_nul_printable": "..@h.r", "segment_complete": true, "contains_r_in_segment": true}
```

## 同轮原有三点与结果

| 位置 | 实测值 | 原始记录行号 |
| --- | --- | --- |
| lzopen 返回 | `0x9a4958`，encoding **0**，file `0x6b1fe0`，internal `0x925500` | JSONL:22 |
| cpio 写头入口 | IO `xzdio` / `xz`，fp `0x9a4958`，路径 `./usr/lib/debug` | JSONL:23 |
| lzwrite 入口 | 指针 `0x9a4958`，与返回值相同，encoding **0**，len **6** | JSONL:24 |

rpmbuild 退出 **1**，GDB 退出 **0**。第 7559/7560 行与上一轮完全相同：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
error: create archive failed: cpio: write failed - Function not implemented
```

运行时间 2026-09-21 **17:06:42—18:01:41 +08:00**，含即时取证约 **54 分 58 秒**。只有一次正式复现。入口新增点命中一次，旧三点各一次；没有 observer_error，也未启用额外返回断点。见 `RESULT_CHECK.json`。

## 配置与仪表边界

1. 以已失败的 `gdb-preserved` 配置为基线，保留原有三点和 lzclose 计数，只新增入口点。与上一轮 argv 的唯一差异为 strace 输出证据路径；rpmbuild 参数、原 strace 过滤器、QEMU 入口、输出 RPM 目录完全一致。见 `ARGV_DIFF.json`、`COMMAND.txt`。
2. 未加 `-vv`、LD_PRELOAD 或 guest strace。使用同一 QEMU 9.2.3；入口位置是已核对实际 ARMv7 ELF 的函数起始地址（link-time `0x132b4`），从 r0 读 mode，读取恰好 16 字节。
3. 命中后自动 continue；不单步、不 finish、不调用 inferior 函数、不改任何变量。旧任务条件 b 的额外断点逻辑在本轮禁用，避免超出仅新增入口点的授权；本轮 encoding=0，该条件实际未成立。
4. 入口脚本 SHA256：`f993051e0953c22b357d5d301d23f8b96a2d7877a61a3cd48b28fc0b65f10ea0`。执行副本在 `cells/gdb-mode-entry/executed-*.py`；完整 GDB 输出及脚本均落盘。
5. 字节解析用两个人工样本核对了“终止符后含 r”与“紧接第二个终止符”两种情况，见 raw/024；这只是解析器测试，不冒充 guest 实测。

**结论强度**：本轮直接测得的是入口 16 字节、随后 encoding=0 与同点失败，满足人工给定的“出现 r 即机制确认”判据。逐指令证明解析循环如何越过终止符、哪个指令写 encoding，仍属 **NOT_OBSERVED**；本轮没有执行这种额外跟踪，也不把静态推导伪称为逐指令实测。

## 资源、完整性与现场

- medium 门禁退出 0；初始可用空间 186 GiB，结束 181 GiB；两次启动前 I/O 探测为 0.116 秒、0.077 秒。
- 实际进程 cgroup MemoryMax **16536457216 字节**，nice 19、ionice idle；Ninja `-j1 -j 1` 已实查。累计构建目标 96，未达到 500 的周期探测门槛；没有暂停事件。抽查的 memory.events 中 max/OOM 计数为 0。
- 上一失败输出树完整备份至 `tmp/ARM_LZMA_0921B/previous-gdb-failure/`，16,993 条目核验通过；原目录未删除。复现后失败现场仍在 `tmp/WEEKEND_0918/rpm-arm-llvm/`，即时权限、大小、时间、SHA256 和进程记录在本轮 cell 下。
- spec、rpmbuild、RPM 三个库、liblzma、QEMU 的运行前后 SHA256 均相同。没有改平台源码或配置，没有改构建根工具；`codes/` 未写入。无包仓或 Gerrit 推送。
- 共享增量 BUILD 树限制仍在：没有声称恢复历史时刻全部文件状态。18 是本轮 `Wrote:` 条数，不能以输出目录中的历史 RPM 总数替代。
- `raw/017_observer_diff` 的退出 1 是预期脚本差异；`raw/029_run` 的退出 1 是本次真实复现失败，两者不混用。
- 收尾未见遗留 rpmbuild/QEMU/GDB/strace/Ninja 进程。未清理当前失败文件。

本轮不再扩展排查或实施修复，停下交人工审阅。
