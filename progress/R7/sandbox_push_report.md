# R7 上游 backport sandbox 分支推送报告

## 结论

状态：`PASS`。

R6 已验证提交 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` 已在未修改源码、提交内容和历史的前提下，一次成功直推至：

```text
refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

未使用 `refs/for/`，未使用 `--force`，分支名不含 `+`，未推送或改动远端 `tizen_base`。QuickBuild 结果由人工观察后回传。

## 推送前核对

| 核对项 | 实测值 | 结果 |
|---|---|---|
| 源码目录 | `/home/toolchain/development/libc++_replacement/codes/llvm` | PASS |
| 当前分支 | `tizen_base` | PASS |
| HEAD | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` | PASS |
| 工作树 | `git status --porcelain=v1` 零输出 | PASS |
| origin URL | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm` | PASS |
| `origin/tizen_base` | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | RECORDED |
| ahead/behind | 1 / 0 | PASS |
| patch-id | `ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479` | PASS |
| `cxa_personality.cpp` SHA256 | `55e136184adbaeee078a339afde2d6da103f4af7cb7c5c31926f2911127eff03` | PASS，等于 R6 |
| sandbox 推送前 | `NOT_FOUND`（`ls-remote` 零输出、退出 0） | RECORDED |

提交说明原文已落盘，并确认含有：

```text
Change-Id: I9ad60d66cbb643ffddb516be0839a3b43a931aa4
Upstream-Status: Backport [8119f1854948b50358bbfaea08f207f51970f06c]
Link: https://github.com/llvm/llvm-project/pull/189787
```

## 推送执行

执行工作目录：

```text
/home/toolchain/development/libc++_replacement/codes/llvm
```

命令原文：

```text
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

退出码：`0`；尝试次数：`1`。

远端原文：

```text
remote: Processing changes: refs: 1
remote: Processing changes: refs: 1, done
remote: commit 3d3e3da: warning: subject >50 characters; use shorter first paragraph
To ssh://review.tizen.org:29418/platform/upstream/llvm
 * [new branch]                HEAD -> sandbox/lhmax2025/libcxx-ehabi-backport
```

其中标题长度 warning 是服务端提示，不是拒绝；push 退出码为 0，提交内容未改写。

## 推送后核对

| 项目 | 推送前 | 推送后 | 结果 |
|---|---|---|---|
| sandbox 分支 | 不存在 | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` | PASS |
| `tizen_base` | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` | UNCHANGED |
| 本地分支/HEAD | `tizen_base` / `3d3e3da…` | `tizen_base` / `3d3e3da…` | UNCHANGED |
| 本地工作树 | clean | clean | UNCHANGED |

`git ls-remote origin sandbox/lhmax2025/libcxx-ehabi-backport` 与精确 ref 查询结果逐字相同：

```text
3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b	refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

## 证据导航

- 全部命令原文与退出码：`commands/`
- 推送前提交说明：`raw/commit_message.stdout.txt`
- 推送原始输出：`raw/push_attempt_1.combined.txt`
- 远端核验摘要：`remote_verification.tsv`
- 推送前后 `tizen_base` 原文：`raw/remote_tizen_base_before.stdout.txt`、`raw/remote_tizen_base_after.stdout.txt`
- sandbox 远端原文：`raw/remote_sandbox_after_exact_ref.stdout.txt`
