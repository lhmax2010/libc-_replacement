# 逐包兼容性核对（进行中）

核对对象为 `spec_audit/inputs.tsv` 中钉住 revision 和 SHA256 的完整 spec
快照。当前取回 11 包，逐项阅读新增标准库选项与周边条件。

| 包 | 条件与传播方式 | 本轮核查结果 |
| --- | --- | --- |
| abseil-cpp | `_toolchain` 已定义且 clang；条件内 BuildRequires、CXXFLAGS、LDFLAGS | 结构符合；三架构 GCC 实测沿用原记录 |
| boost | 同上；CXXFLAGS/LDFLAGS 在条件内追加，构建与安装保存/读回同一参数 | 结构符合；保存参数的语句不在条件内，但不会自行引入 libc++ 选项 |
| icu | 同上；条件内追加，现有其他参数保留 | 结构符合；三架构 GCC 实测沿用原记录 |
| jsoncpp | 同上；条件内追加 | 结构符合；三架构 GCC 实测沿用原记录 |
| libsigc++ | 同上；条件内追加，再交给 Meson | 结构符合；三架构 GCC 实测沿用原记录 |
| pcre | 同上；devel 的标准库 Requires 也按同一条件二选一 | 结构符合；三架构 GCC 实测沿用原记录 |
| taglib | 同上；条件内追加 | 结构符合；三架构 GCC 实测沿用原记录 |
| tensorflow2 | 另要求 USE_CLANG=ON；仅条件内给 CLANG_CXXFLAGS 与 LDFLAGS 追加 | 结构符合；armv7l GCC 沿用基线同点同因失败判据，不写成完整构建成功 |
| llvm | 条件内启用 LLVM_ENABLE_LIBCXX 和 libc++abi 链接选项，并选择新 AOT 资产 | 结构符合；三架构 GCC/非 libc++ 对照沿用已闭合十二格 |
| bcc-tools | 同一条件控制新增 15 行中的依赖与 flags | 结构符合；aarch64/x86_64 GCC 实测沿用，armv7l 本轮待补 |
| bpftrace | 尚无适配改动 | NOT_OBSERVED：待前置闭合后实施和核查 |

以上“结构符合”仅指本次适配引入的选项；不等同于任意外部环境注入均能
被这些 spec 过滤。最终是否泄漏以对应实际 GCC 构建日志补证。

历史实测索引：`docs/progress/BUILD_W2_0912/PACKAGE_STATUS.tsv`、
`docs/progress/BUILD_W3_0909/COMPATIBILITY_AUDIT.tsv`、
`docs/progress/BUILD_W1B_0909/MATRIX.tsv`。本轮未重跑已闭合的格。
