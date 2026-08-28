# R76：四补丁追加至既有 Gerrit sandbox / QuickBuild 验证

日期：2026-08-28  
最终状态：`SUBMITTED_BUILD_PENDING`

## 1. 结论

四个定稿补丁已按 A、B、0003（头文件批）、0004（库内批）追加到既有
`refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport`。目标分支从
`5ed6c77278dfa7a470667cf1a137723d3c96fe60` 普通快进至
`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`；既有 HEAD 是新 HEAD 的祖先，
既有提交没有丢失。

四补丁在既有 HEAD 上依次执行 `git apply --check` 和索引应用均退出 0，
包括与既有 ARM EHABI 改动相邻的层 A 第二处修改，没有冲突。四个补丁文件
及 R69 `SHA256SUMS` 未改，提交源码 patch-id 与定稿原件逐项一致。

推送后，除目标分支按预期快进外，其余 sandbox 分支及正式 `tizen_base`
均未变化。误建的 `sandbox/lhmax2025/libcxx-forced-unwind` 经四提交逐项
patch-id、完整 message 和 author 等价门禁后删除；没有观察到 R76 创建的
其他分支。未推送正式分支，未向 LLVM 上游提交。

本次沿用 R7 的 QuickBuild 触发形态：直推既有 sandbox ref 后由平台侧流程
构建并人工观察。当前环境没有 QuickBuild 日志读取入口，故构建结果、产物、
其他包影响和新增编译告警均为 `NOT_OBSERVED`，需要人工在平台侧查看；不能
判定构建通过，状态为 `SUBMITTED_BUILD_PENDING`。

## 2. 资源与补丁身份

续作资源命令 `tools/resource_gate.sh --level light` 退出 0，见
`commands/101_resource_gate_continuation.log`。

| 顺序 | 补丁 | R69 记录 / 当前 SHA256 | 结果 |
|---|---|---|---|
| 1 | `0001-Layer-A-fix-foreign-forced-unwind-resume.patch` | `af41f2989efcc28405bd26e3db24fb8e26c1fcca89f7d6f22c9140ccdb5beebd` | 一致 |
| 2 | `0002-Layer-B-add-GNU-forced-unwind-marker.patch` | `a121ce3bb7b95d3170a5ae0acd885557a5c7c0ab100b360a469d129c9c985b6e` | 一致 |
| 3 | `0003-Layer-C-header-batch-20-sites.patch` | `0a32dc44e0e57ed524283bc416bdc5c4f6a00b131de5e421485b6e7bcb4b82e9` | 一致 |
| 4 | `0004-Layer-C-library-batch-30-sites.patch` | `88228328db96b38ca139ec1fa3b04ddd82c25f17b97e8a90c674feb2a654d90e` | 一致 |

复核原文见 `raw/107_patch_identity_recheck_corrected.stdout` 和
`raw/108_patch_sha256_current.stdout`。补丁内容改动：**无**；
`SHA256SUMS` 改动：**无**；平台源码人工修改：**无**。

## 3. 目标分支推送前现状

目标分支推送前 HEAD 为 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
它相对共同基线 `fd7a4929c0e424d564cdd1f0c55af9153dcea321` 有两个既有提交：

1. `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`：
   `libcxxabi: declare __gnu_unwind_frame in cxa_personality (#189787)`；
2. `5ed6c77278dfa7a470667cf1a137723d3c96fe60`：
   `packaging: add libc++ runtimes spec`。

完整 message 见 `raw/110_target_existing_commits_full.stdout`。

推送前 `sandbox/lhmax2025/*` 共五个分支，见
`raw/103_sandbox_namespace_before.stdout`：

| 分支 | 推送前 SHA |
|---|---|
| `libcxx-ehabi-backport` | `5ed6c77278dfa7a470667cf1a137723d3c96fe60` |
| `libcxx-forced-unwind`（误建） | `74900f3fbb2969f7f127449f9b1d20df72b9eefa` |
| `llvm_only_clang` | `f8277158cf44f872e2b2d62a2e839cb33bd0afea` |
| `mlgo` | `08ffd8cd7c72f5dd6e612d8395362def96d84029` |
| `mlgo_aot` | `88ff57cadcff52ab50ca6865db7faed7388358fe` |

正式 `refs/heads/tizen_base` 推送前为
`8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`。

## 4. 试应用与提交序列

以目标分支现有 HEAD 建立独立 worktree，四补丁按定稿顺序逐个执行
`git apply --check`、`git apply --index`，八条命令全部退出 0；最终
`git diff --cached --check` 退出 0，补丁应用前后身份复核均为 4/4 `OK`。
结果为 `R76_EXISTING_BRANCH_APPLY_PREFLIGHT=PASS`，原始日志位于
`raw/apply_existing_branch/`。试应用结论：**无冲突**。

正确目标分支上的新增提交为：

| 顺序 | 层 | 提交 SHA | parent | author | committer |
|---|---|---|---|---|---|
| 1 | A | `77e0f73ba61ef3e865f65eaf089bd1eafd9bfdea` | `5ed6c77278dfa7a470667cf1a137723d3c96fe60` | `R51 Evidence <r51@example.invalid>` | `hao.lin <hao.lin@samsung.com>` |
| 2 | B | `82afebabd7782ed25017e248f319c10284f7bec7` | `77e0f73ba61ef3e865f65eaf089bd1eafd9bfdea` | `R51 Evidence <r51@example.invalid>` | `hao.lin <hao.lin@samsung.com>` |
| 3 | C 头文件批 | `9039afa48d5bdbd30f63589b0ca68e3c564f45ab` | `82afebabd7782ed25017e248f319c10284f7bec7` | `NOT_AVAILABLE <not-available@invalid>` | `hao.lin <hao.lin@samsung.com>` |
| 4 | C 库内批 | `08a64b6eb95ed74339f1d862a6c22a2f93fa78f9` | `9039afa48d5bdbd30f63589b0ca68e3c564f45ab` | `He Fangyu <fangyu.he@samsung.com>` | `hao.lin <hao.lin@samsung.com>` |

`raw/existing_branch_series_validation/commits.tsv` 证明四提交线性相接、
committer 正确、author 保持占位/原值、每提交 patch-id 与原补丁相同，且
总源码 diff 与试应用结果逐字节一致。Layer A 保留此前批准的中英文完整依赖
说明和 Change-Id；其余三个 message 与定稿提交逐字一致。

依赖说明明确：四者是一套，顺序为 A → B → 0003 头文件批 → 0004 库内批；
两个层 C 批次依赖层 B，并在 ARM 上依赖层 A；只应用或验证子集可能失败或
引入回归。

## 5. 推送与分支对照

实际推送命令：

```text
git -C tmp/R76/ehabi-backport-series push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

退出码 0，且未使用 `--force`、`--force-with-lease`、`+refs/...` 或
`refs/for/...`。服务端结果为
`5ed6c77278df..08a64b6eb95e` 普通快进；完整原文见
`raw/127_push_existing_sandbox_branch.stderr`。

服务端报告五条 commit-message 风格 warning（subject 长度或超过 72 字符的
message 行）。它们不是编译告警，也不是 QuickBuild 结果；按“不修改定稿
message”约束未处理。

推送后：

- 目标 HEAD 为 `08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`；
- `git merge-base --is-ancestor 5ed6c772… 08a64b6e…` 退出 0，既有提交未丢；
- 除目标外的四个 sandbox 分支与推送前逐字一致；
- 正式 `tizen_base` 仍为 `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`。

原始对照见 `raw/128_target_branch_after_push.stdout`、
`raw/129_sandbox_namespace_after_push.stdout`、
`raw/130_formal_branch_after_push.stdout` 和 commands 131–134。

## 6. 误建分支清理

删除前门禁确认：误建分支从仍由正式 `tizen_base` 保留的
`8dfebafe…` 起恰好追加四个任务提交，没有第五个提交；这四个提交与正确
分支新增四提交逐项 stable patch-id 相同、完整 commit message 相同、author
相同。完整表见 `raw/mistaken_branch_disposal_gate/equivalence.tsv`，门禁结果为
`R76_MISTAKEN_BRANCH_DISPOSAL_GATE=PASS`。

在目标推送及全部推后核对通过后，执行：

```text
git -C codes/llvm push origin --delete sandbox/lhmax2025/libcxx-forced-unwind
```

退出码 0。删除后精确查询误建 ref 为零输出且退出 0；sandbox 命名空间只剩
正确目标及原有 `llvm_only_clang`、`mlgo`、`mlgo_aot`，目标与正式分支均未
变化。原文见 commands/raw 137–145。除此之外没有观察到 R76 新建的其他
分支。

## 7. QuickBuild

- 触发方式：沿用 R7，向 `platform/upstream/llvm` 的既有 sandbox ref 直推；
- 构建目标：仓库/包 `platform/upstream/llvm`，ref
  `sandbox/lhmax2025/libcxx-ehabi-backport`；平台 profile/架构矩阵
  `NOT_OBSERVED`；
- 触发点：成功的普通快进推送（command 127）；
- 完整平台日志：`NOT_OBSERVED`，当前环境无法读取，需人工平台侧查看；
- 构建是否成功：`NOT_OBSERVED`；
- 构建产物是否正常生成：`NOT_OBSERVED`；
- 与本地产物是否一致：`NOT_OBSERVED`，无法比对；
- 是否导致平台其他包失败：`NOT_OBSERVED`；
- 是否出现指向本次改动的新增编译告警：`NOT_OBSERVED`；
- 构建失败重试次数：0（尚未观察到失败，未重试）。

因此结论只能是 `SUBMITTED_BUILD_PENDING`，不能写
`SUBMITTED_AND_BUILT`。人工需重点回传完整日志、产物状态、其他包结果及
指向本次改动的新增告警。

## 8. 验证范围与未覆盖范围

既有状态：x86_64 与 armv7l 全量验证完成；三方独立评审提出的四项缺陷已
修复并复验。本轮新增覆盖：补丁字节身份、既有目标 HEAD、命名空间快照、
目标 HEAD 上顺序试应用、提交链/身份/message/patch-id/总 diff、普通快进
推送、既有提交保留、其他分支不变及误建分支安全清理。

未覆盖：aarch64、LLVM 官方测试套件全量运行、共存期/混合运行时场景，
以及本次 QuickBuild 的实际构建、产物、其他包和告警结果。

## 9. 异常、判断与疑问

续作中记录三项技术性命令/脚本异常，均未改变任何判据对象：

1. command 105 的精确 fetch 遇 SSH 连接超时；包装进程被终止，退出码未能
   观测，按纪律记 `NOT_OBSERVED`。目标 SHA 已由 command 102 的远端查询
   取得且对象本地存在；推送前 command 125 再次远端确认目标未变化。
2. command 106 错在 `SHA256SUMS` 已含工作区相对路径却从补丁目录执行；
   原样留证后只改调用工作目录，command 107 复核 4/4 `OK`。
3. 首轮新 series 验证器把实际的中英文两个标题误写成一个不存在的合并标题；
   只修正两条标题匹配，diff 为 `raw/123_validator_title_match_fix.diff`，重跑
   通过。

自行判断：删除门禁中的“无未迁移内容”按任务产生的四提交内容解释，同时
要求误建分支基线仍由正式 ref 保留；据此检查提交数量、parent、patch-id、
完整 message 和 author。两分支基线不同，因此不把 commit SHA/committer
时间不同误判为补丁内容不同。该判断及其证据已明确记录。

尚存疑问仅为平台侧不可见项：QuickBuild 的 profile、架构矩阵、完整日志、
产物、其他包结果及新增编译告警，需人工查看后回传。
