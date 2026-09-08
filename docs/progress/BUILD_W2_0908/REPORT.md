# W2：bcc-tools 与 bpftrace 适配

## 结论

结论为 `NOT_AVAILABLE`。

W2 的明示前置条件是 W1 的 LLVM 改动已经完成验证并推送。W1 最终为
`PARTIAL`：aarch64 全量构建、两种兼容路径和两个非 x86_64 的模型运行格
没有闭合，因此 LLVM 候选未提交、未推送。按任务书第 7 条，本任务没有
修改、构建或推送 `bcc-tools` 与 `bpftrace`，直接进入 W3。

## 资源门禁

`tools/resource_gate.sh --level medium` 退出码为 0；门禁通过时
`mem_available_kib=22244204`、`load1=13.99`、
`disk_available_kib=69712376`。资源不是本任务停止原因。

## 未观测与未改动

- `bcc-tools` 改动与六格验证：`NOT_AVAILABLE`（缺少已推送的新 LLVM）。
- `bpftrace` 改动与六格验证：`NOT_AVAILABLE`（同上，且依赖新的 bcc-tools）。
- 两条布局敏感边的双侧 libc++ 产物核查：`NOT_AVAILABLE`。
- 两个包均未产生本地改动，两个 sandbox 分支均未推送。

## 自行判断与疑问

没有自行扩大范围，也没有需要人工裁决的设计岔路。停止依据为任务书明确
给定的前置条件，而非推断。
