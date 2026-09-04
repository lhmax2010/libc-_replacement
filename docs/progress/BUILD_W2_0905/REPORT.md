# W2：llvm / bcc-tools / bpftrace 适配

## 结论

`NOT_AVAILABLE`。

W2 的硬前置是 W1 产出经核验可用的 libc++ MLGO/XLA AOT 资产。W1 续查确认，现有错误 ABI 符号位于 XLA runtime，而人工限定本轮只生成 inliner；只生成 inliner 不能解除 LLVM 链接阻断。因此 W1 没有生成可用资产，W2 按任务书第 5 条没有实施，直接进入 W3。

## 资源门禁

任务开始前执行 `tools/resource_gate.sh --level medium`，退出码为 0。证据见 `raw/001_resource_gate.*`。

## 未执行项

- LLVM spec 改动：`NOT_OBSERVED`；
- llvm 三架构 × libc++/GCC 六格：`NOT_OBSERVED`；
- 新 clang 的两种 `-###` 检查：`NOT_OBSERVED`；
- bcc-tools、bpftrace 改动与六格：`NOT_OBSERVED`；
- Gerrit sandbox 推送：`NOT_OBSERVED`。

## 边界确认

- 未修改任何平台源码、spec、配置或补丁；
- 未生成、替换或导入资产；
- 未向 Gerrit 或其他外部源码仓推送；
- 未切换或修改 `codex/runtime-validation`。
