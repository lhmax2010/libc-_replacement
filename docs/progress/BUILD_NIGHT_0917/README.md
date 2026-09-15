# 静态库与 bpftrace-static 夜间交接

总状态 **PARTIAL**：已经生成两架构 libc++ 静态 bpftrace 候选，尚未证明功能等价，
不作为可以替换发布资产的结论。只提交项目证据，不推 Gerrit 或包仓。

建议阅读顺序：

1. `STATUS.md`：四阶段状态、时间、执行期间的进度与技术性修正。
2. `W1_REPORT.md`、`W2_REPORT.md`：新静态运行时/Clang 库、条件保护与实际验证范围。
3. `W3_REPORT.md`：两个候选、真实 GCC 回退构建、哈希对照及未闭合项。
4. `W4_REPORT.md`：旧 libclang 归档的引入提交、GCC 注记与生产配方线索。
5. `KNOWN_ITEMS.md`、`QUESTIONS.md`：需要保持单列和人工确认的内容。

`candidate_specs/` 是本夜临时 spec、脚本的交付副本；真正候选和二进制位于
工作区 `tmp/NIGHT_0917/`。原源码仓只读，不把历史脏工作树当本轮修改。
大型构建根、RPM 与二进制未提交；其精确路径/哈希在各 JSON 中。

`raw/` 记录命令、退出码、起止时间和 stdout/stderr；构建 stdout 另有
`*.build.log`，实际命令/监控器哈希在 `*.events.jsonl`。
重跑的日志保存为 `previous-N`；有一次早期配置日志覆盖已单独标为恢复记录。
最终进程核查的对外记录只保留本任务匹配行，不提交无关进程命令参数；
首次未过滤的辅助记录隔离在 tmp/NIGHT_0917/private-audit/，未纳入归档。
`*.executed-vN.py` / `*.executed-vN.sh` 保留旧执行版本，不覆盖历史失败输出。

大型符号/字符串输出和 link map 无损压缩保存。`COMPRESSED_OUTPUTS.tsv` 给出
压缩前路径、字节数、SHA256 与交付路径；`SHA256SUMS.tsv` 核对实际归档文件。
例如 `gzip -cd bpftrace-x86_64-symbols.txt.gz` 可复核原 nm 输出，不需要重做构建。

资源：单编译任务、-j1、nice19、ionice3；`ulimit -v 16148878` KiB（每进程
地址空间限额，为宿主总内存一半，不声称是 cgroup 聚合 RSS 限额）。
各阶段截止时间按原始开始时间写入监控事件，没有重置或放宽。
状态表用时为阶段开始至结果汇总时刻；提交/归档另有 raw/ 时间记录。
目录日期标签沿用任务书；实际执行时间以 raw/ 和事件中的 +08:00 时间戳为准。
armv7l 板子离线，不连接；aarch64 命令执行为 QEMU 用户态。
