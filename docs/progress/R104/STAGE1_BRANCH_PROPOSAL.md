# R104 建分支前阶段记录

状态：`AWAITING_BRANCH_NAME_CONFIRMATION`

## 已完成的门禁与拉取核对

- `tools/resource_gate.sh --level light`：退出码 0。
- 拉取前磁盘空间满足要求；11 个仓库隔离放在 `codes/R104/<包名>/`，未触碰既有 `codes/llvm` 工作树。
- 拉取采用账号 `lhmax2025`，串行、低优先级执行。`tensorflow2` 首次拉取遇到 SSH 连接超时，原始非零结果已归档；确认失败目录未残留后重试一次成功。这是传输命令的技术性失败，不是源码身份判据失败。
- 11 个仓库当前工作树全部干净，最近 3 条提交均已记录。
- 与 R101（`tensorflow2` 对照 R103）记录的当前 revision 比对：11/11 完全一致，无新增漂移。
- 11/11 当前 spec 的 SHA256 与 R101/R103 归档副本一致；因此未出现由后续 spec 漂移导致原建议失效的包。
- 已保存建分支前全部远端 heads，共 747 条，供推送后逐仓逐条对照。
- 未创建本地或远端迁移分支，未修改任何 spec，未提交代码改动。

详细逐仓信息见 `tables/git_state_before_branch.tsv`；全部远端分支基线见 `tables/remote_heads_before_branch.tsv`；命令、标准输出、标准错误及退出码见 `raw/`。

## 拟用分支名

建议 11 个仓库统一使用：

`sandbox/lhmax2025/libcxx-migration`

理由：

1. 位于获准账号的 sandbox 命名空间，不涉及任何正式分支；
2. 直接表达这些仓库承载的是包侧 libc++ 迁移；
3. 与 LLVM 运行时补丁分支 `sandbox/lhmax2025/libcxx-ehabi-backport` 区分清楚；
4. 11 个仓库使用同名分支，便于 QuickBuild 配置、批量核对和人工审阅；
5. 已逐仓检查，`refs/heads/sandbox/lhmax2025/libcxx-migration` 在 11 个远端均不存在，不会覆盖既有分支。

## 等待确认

收到人工对上述分支名的确认后，才会基于各仓当前 HEAD 创建并以普通 push 推送；不会使用 force，也不会推送正式分支。
