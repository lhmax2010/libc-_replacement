# T-1 发车前置报告

状态：`DONE`。

## 结论

磁盘守卫通过，任务开始时工作根所在文件系统可用 `1187468193792` 字节，80 GiB 断言退出 0（`progress/T-1/00_disk_guard.log:4`）。

staging 漂移恰为 `d5/d5_mechanism.md` 与 `d5/macro_definition_snippet.conf` 两项，路径集合与行数断言均退出 0（`progress/T-1/01_status_porcelain_and_assert.log:1`）。两文件 HEAD 与工作树版 SHA256 已逐一冻结（`progress/T-1/02_head_worktree_sha256.log:1`）。

正式 buildconf 的实测 SHA256 为 `7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`；剔除注释行与 allowlist 取值行后，它与工作树宏片段的机制块逐字 diff 退出 0（`progress/T-1/03_macro_mechanism_compare.log:1`）。`d5_mechanism.md` 的 HEAD→工作树完整 diff 保存在 `progress/T-1/04_d5_mechanism_head_to_worktree.diff.log:1`。

限定提交成功，提交 SHA 为 `6b7bc9e5d9c61fe8397ac3bf28aabeef4ab79e7a`（`progress/T-1/06_commit.log:1`）。注解 tag `line-21.1.1-final` 指向该提交，tag message 已按规格记录（`progress/T-1/07_tag.log:8`）。提交与 tag 均推送成功，退出码为 0（`progress/T-1/08_push_commit_and_tag.log:5`）。

目标根在建仓前无 `.git`，顶层内容符合允许集合；预置任务规格与附件逐字节相同（`progress/T-1/09_workspace_precheck.log:4`）。`git init`、fetch 与从 `origin/main` 建立本地 `main` 全部退出 0（`progress/T-1/10_init_fetch_checkout.log:1`）。

目录契约与 `.gitignore` 已建立，任务规格已随提交 `35cadd29c3a5df8edc22edb4e876aa112973b936` 入仓并推送成功（`progress/T-1/12_workspace_commit.log:1`；`progress/T-1/13_workspace_push.log:1`）。规格文件 SHA256 为 `7958b8c7d6a9158da723ba321b36b66eede2205f0ddfbc0d67beff9e4c12539c`（`progress/T-1/14_final_git_identity.log:26`）。工作树下 `gbs_llvm.conf` 保持为未跟踪的新平台配置原件，未被越界加入本次提交（`progress/T-1/14_final_git_identity.log:13`）。

`wave1_repo_staging` 最终干净，自 `2026-07-31T18:59:11+08:00` 起作为只读参考，不再写入（`progress/T-1/16_staging_readonly_boundary.log:1`）。

## 身份摘要

- D5 修复提交：`6b7bc9e5d9c61fe8397ac3bf28aabeef4ab79e7a`
- 线界 tag：`line-21.1.1-final`
- 唯一工作树 HEAD：`35cadd29c3a5df8edc22edb4e876aa112973b936`
- 冻结规格 SHA256：`7958b8c7d6a9158da723ba321b36b66eede2205f0ddfbc0d67beff9e4c12539c`
