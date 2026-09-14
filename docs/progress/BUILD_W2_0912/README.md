# bcc-tools / bpftrace libc++ 适配续跑

结论：`PARTIAL`。

- LLVM 前置已满足：远端 `sandbox/lhmax2025/libcxx-ehabi-backport` 为
  `f203923a1508c9344f5fc6b17bd8822f011655c4`，上一轮十二格门禁均通过。
- `bcc-tools` 已形成仅修改 `packaging/bcc-tools.spec` 的本地候选；
  aarch64、x86_64 的 libc++/GCC 四格通过。
- armv7l 所需的 libc++ LLVM 候选已被后续 GCC 对照构建覆盖。尝试从已确认
  工作树重建时，旧 LLVM 根的标准 GBS 清理连续 60 分钟处于不可中断磁盘
  等待；为避免再次拖垮系统，已中止。armv7l 两格记 `NOT_OBSERVED`。
- 因 `bcc-tools` 六格未闭合，未提交、未推送；按依赖门禁未开始
  `bpftrace`，也未修改、未提交、未推送它。
- 两个包的远端目标分支均保持任务开始时的 SHA。

主要材料：

- `REPORT.md`：过程、结果、边界核查与未完成项；
- `MATRIX.tsv`：两个包的十二格矩阵；
- `PACKAGE_STATUS.tsv`：Base 侧 11 包的最新状态；
- `checks/`：通过格的产品核查；
- `raw/`：命令、标准输出、标准错误与退出码；
- `code/`：本次使用的日志包装器与产品核查脚本快照。

材料自检见 `raw/053_*`、`raw/054_*`。系统崩溃中断的 `raw/019_*` 没有
自动退出码；该缺口被明确保留，不作事后补写。

未向任何正式分支推送；未使用 force push。
