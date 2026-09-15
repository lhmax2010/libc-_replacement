# bcc-tools / bpftrace 续跑

开始：2026-09-15 12:58 +08:00；八小时截止：2026-09-15 20:58 +08:00。

- 资源门禁：PASS（load1=3.33，limit=20，可用磁盘约 319 GiB）。
- 小规模 I/O：约 8.5 MiB 写入、fsync、读回比对与删除合计 0.151 秒，PASS。
- 遗留进程：未见 GBS、Ninja、编译器；旧工作树与构建根保留。
- 产物定位：已知 armv7l 输出目录仅剩 libstdc++ 对照 RPM；旧 libc++ 的
  debuginfo 不可替代运行库。广域目录遍历过慢后主动中止（143），改查
  上一轮已登记的目录。
- LLVM 重建耗时估计：原 libc++ armv7l 完整日志末尾为 6338 秒
  （约 1 小时 46 分）；同架构 GCC 基线为 7188 秒。暂估 2–3 小时，
  低于四小时；这是基于历史实测的估计，非完成保证。
- LLVM 重建输入：`tmp/BUILD_W1BC_0911/target-fetch` 的 HEAD 为
  `f203923a1508c9344f5fc6b17bd8822f011655c4`。
- bcc-tools：保留原 15 行 spec 候选与四格 PASS，armv7l 待补。
- bpftrace：待 bcc-tools 门禁闭合。现有 `ExclusiveArch` 不含 x86_64，
  已提出范围确认；尚未自行改动。

## 13:19 更新

结论：`PARTIAL`，按 I/O 异常停止要求中止。

- LLVM 已选中 Clang 22.1.8、libc++ 和 TF 2.18 资产，完整构建到
  `801/7147`；`ninja: build stopped: interrupted by user` 已出现。
- 同一 I/O 探针复测超过 30 秒，`timeout` 退出 124；观测到探针处于
  `wait_on_buffer`、编译器处于 `folio_wait_bit_common`。
- 未形成新的 LLVM RPM；bcc-tools 两格尚未开始，未提交/推送；
  bpftrace 仍未实施。
- 构建断点保留在 `tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/`。
