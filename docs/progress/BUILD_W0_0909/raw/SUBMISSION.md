# 提交、推送与远端核对记录

## 主材料提交

命令原文：

```bash
git commit -m 'docs: record QuickBuild access verification'
```

标准输出：

```text
[codex/r5-ehabi-diagnosis 2720a0d] docs: record QuickBuild access verification
 13 files changed, 847 insertions(+)
 create mode 100644 docs/progress/BUILD_W0_0909/ACCESS_RESULTS.tsv
 create mode 100644 docs/progress/BUILD_W0_0909/README.md
 create mode 100644 docs/progress/BUILD_W0_0909/REPORT.md
 create mode 100644 docs/progress/BUILD_W0_0909/raw/COMMANDS_AND_OUTPUT.md
 create mode 100644 docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_form_excerpt.html
 create mode 100644 docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
 create mode 100644 docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
 create mode 100644 docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
 create mode 100644 docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
 create mode 100644 docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
 create mode 100644 docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
 create mode 100644 docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
 create mode 100644 docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
```

退出码：`0`

## 主材料推送

命令原文：

```bash
git push origin codex/r5-ehabi-diagnosis
```

标准输出：

```text
To github.com:lhmax2010/libc-_replacement.git
   4455af0..2720a0d  codex/r5-ehabi-diagnosis -> codex/r5-ehabi-diagnosis
```

退出码：`0`

## 主材料远端核对

命令原文：

```bash
local_sha=$(git rev-parse HEAD)
remote_sha=$(git ls-remote --heads origin refs/heads/codex/r5-ehabi-diagnosis | awk '{print $1}')
printf 'local_sha=%s\nremote_sha=%s\n' "$local_sha" "$remote_sha"
test "$local_sha" = "$remote_sha"
printf 'sha_match=yes\n'
git status --short --branch | sed -n '1,60p'
```

与 SHA 核对有关的标准输出：

```text
local_sha=2720a0df3e4dec97ab1b9e964b14c126313793eb
remote_sha=2720a0df3e4dec97ab1b9e964b14c126313793eb
sha_match=yes
```

退出码：`0`

`git status` 同时显示工作区存在本任务开始前已有的其他修改和未跟踪文件；它们没有被暂存或提交。完整列表未重复纳入本文件，以免把其他任务材料混入本任务交付。
