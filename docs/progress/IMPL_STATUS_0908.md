# 四项决策实施总览

实施源分支：`sandbox/lhmax2025/libcxx-noexcept-relief`；基线：`c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`（已核实远端）。材料分支：`codex/runtime-validation`。

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
|---|---|---|---|---|
| 等待规格、写者回滚、析构补丁 | PATCHES_READY | 约 18 分钟 | `IMPL_0908/W1/` | 三提交已推 sandbox，严格原生匹配构建通过、25 次轻量值断言通过 |
| 双版本符号 | NOT_STARTED | — | `IMPL_0908/W2/` | 顺序执行 |
| 两架构完整验证 | NOT_STARTED | — | `IMPL_0908/W3/` | 依赖前两项，ARM 要求物理板 |
| 支持范围声明 | NOT_STARTED | — | `IMPL_0908/W4/` | 声明任务自身写明不需要开发板 |

全部疑问与口径见 [IMPL_QUESTIONS_0908.md](IMPL_QUESTIONS_0908.md)。未修改既有四补丁；不推正式平台分支或 LLVM 上游，不使用 force。
