# 不可交付候选资产

本目录保存 2026-09-07 使用 TensorFlow 2.18.0 材料生成的三架构 libc++
runtime 与 inliner 拆分包，供人工复核。本批对象均通过架构检查，runtime
全局符号中存在 `std::__1` 且没有 `std::__cxx11`。

这些文件**不可导入 LLVM、不可用于发布**：未启用 libc++ 的对照路径也无法
复现现有 TensorFlow 2.15.1 资产，文件集合、runtime 归档成员、inliner 入口
符号和全局符号集合均有实质差异。完整证据见上两级目录的 `REPORT.md` 与
`asset_verification/`。
