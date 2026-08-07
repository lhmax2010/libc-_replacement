# R18：参照 gcc 实现 llvm 多 spec（人工审阅前段）

## 状态

`PASS_LOCAL_IMPLEMENTATION_AWAITING_HUMAN_REVIEW`

已完成 gcc 机制取证、差异分析、`codes/llvm` 本地实现、两份 spec 解析自检和 GitHub 审阅材料准备。未向 Gerrit 推送，既有 `sandbox/lhmax2025/libcxx-ehabi-backport` 远端仍指向 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`。

## gcc 机制结论

- `pre_checkin.sh` 从一个完整 `gcc.spec` 生成四个完整派生 spec。
- 每个派生 spec 只增加 `%define cross <arch>`、对应架构宏和空行，剩余正文与基准逐字一致。
- `Name: gcc%{?cross:-%{cross}}` 使五份 spec 分别产生 `gcc` 与四个 `gcc-<arch>` 源码包名。
- packaging 内未见 `_multibuild`、`_service` 或其他 OBS 配套文件。

完整材料见 `GCC_MULTI_SPEC_MECHANISM.md`。

## 差异裁决

`llvm.spec` 与 `libcxx-runtimes.spec` 是两份独立完整内容，不具备 gcc 的“宏前缀 + 公共正文”生成关系。因此本次不新增生成脚本，只让两个完整 spec 并存，并为 libcxx 增加三个专属输入。Name 分别为 `llvm` 与 `libcxx-runtimes`，Version 均为 `22.1.8`。详见 `DIFFERENCE_ANALYSIS.md`。

## 本地实现

`codes/llvm` 的 `tizen_base` 从 `3d3e3da…` 前进到本地提交 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`，只新增：

- `packaging/libcxx-runtimes.spec`
- `packaging/libcxx-llvm22.map`
- `packaging/libcxx-runtimes.manifest`
- `packaging/SOURCE_PROVENANCE`

四个文件 SHA256 与 R9-C seg2 构建输入一致。`llvm.spec` 没有任何字节变化。详见 `IMPLEMENTATION_NOTES.md`。

## 自检

- `rpmspec -P llvm.spec`：退出 0。
- `rpmspec -P libcxx-runtimes.spec`：退出 0。
- Name 不同、Version 相同：PASS。
- `llvm.spec` 提交前后 diff：退出 0。
- LLVM 工作树提交后干净：PASS。
- sandbox 远端未变化：PASS。

完整原文见 `commands/` 与 `SELFCHECK_SUMMARY.md`。

## 未验证边界

本地实现尚未在 QuickBuild 构建；QuickBuild 是否支持 llvm packaging 内两份独立 spec 仍未观测。本轮按任务第 4 步在 GitHub 交付审阅材料后停止，不执行第 5 步 Gerrit sandbox 推送。
