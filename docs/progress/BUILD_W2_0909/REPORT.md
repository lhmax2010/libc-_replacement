# W2 bcc-tools 与 bpftrace 适配报告

## 结论

结论为 **`NOT_AVAILABLE`**。W2 的明确前置条件是 W1 的 LLVM 候选已经
通过全部门禁并推送；实际 W1 结论为 `GATE_NOT_CLOSED`，LLVM package
仓远端仍停留在 `2d23367d74afbf2bb1e9e4013fce072b3a154109`。因此没有
修改、构建、提交或推送 `bcc-tools`、`bpftrace`，直接进入独立的 W3。

## 前置条件证据

- W1 矩阵中 aarch64、x86_64 的 GCC/不启用 libc++ 完整构建在时间上限
  处中止，未形成可核查产物；
- armv7l 的不启用 libc++ 资产来源已证明一致，但跨日期 RPM 对照存在
  动态符号与依赖差异，因此整包等价仍为 PARTIAL；
- LLVM 远端 `sandbox/lhmax2025/libcxx-migration` 未更新。

W1 的完整依据见 `docs/progress/BUILD_W1_0909/REPORT.md` 和
`MATRIX.tsv`。

## 资源门禁

执行 medium 门禁，退出码 0。该任务未发起构建。

## 未观测与自行判断

- `NOT_OBSERVED`：bcc-tools、bpftrace 的本轮改动、六格构建、边界两侧
  标准库检查；原因是前置 LLVM 未推送，而不是资源不足。
- 自行判断：无。跳过行为直接按任务包“若 W1 未完成，本任务记
  NOT_AVAILABLE”执行。
- 尚存疑问：无新增疑问；W1 的闭合口径问题已记录在共同问题文件中。
