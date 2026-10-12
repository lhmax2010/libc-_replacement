# 两项评审合并为一个 change

**完成。** 356690 已更新到 patchset 3，包含撤销 lld 默认链接器与三行 libc++ Support/Keep；356689 已作废。没有新建 change，没有直接推 master 或使用 force。新提交的 tree 与原第二笔 f385421f 完全相同。

| 项 | 核验结果 |
|---|---|
| 保留的评审 | https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690/3 |
| 新提交 | e13ce91df74fe79237a3b6afe2e1443b8a8e8d00 |
| Change-Id | I0403eb61b5c412c7ed53dc6f862c92765ead121f，正文中仅此一个 |
| patchset | 3；refs/changes/90/356690/3 与本地 HEAD 一致 |
| 作者 / 提交人 / Signed-off-by | 均为 Hao Lin <hao.lin@samsung.com> |
| 新旧 tree | 均 04f46636709cd42b7967e3c7bd40cb6098d61cfa，git diff f385421f HEAD 为空 |
| master | fetch 后与推后均为 0bb6ff66506574606cf4d636aaa8974ff4c779b8，未前进 |
| 新提交父节点 | 直接为上述 master，仅一笔提交，不再依赖 356689 |
| 356689 | ABANDONED；作废目标为 patchset 2 的 2c5b5af89ce1a001f7acea9d8355fe5bc1e91137 |
| 356690 状态 | NEW / open，尚未合入 |
| 工作树 / 分支 | 干净 / prjconf-libcxx |

证据：`VERIFY_RESULT_R3.json`、`VERIFY_COMMANDS_R3.json`、`raw/010_remote_refs_R3.*`、`raw/011_review_status_R3.*`。Gerrit 查询里的帐号显示名为 Lin Hao；实际 Git 提交对象中的作者/提交人和 Signed-off-by 均是人工要求的 Hao Lin，两者不混淆。

## 执行过程

开工确认 clone 为 `tmp/PRJCONF_REVIEW_1012/repo`、分支正确、工作树干净、HEAD 为 f385421f。fetch 退出 0，master 未前进，故无需先做移植 rebase。按人工指定执行 `git reset --soft origin/master`，再使用 `git commit -s -F SQUASH_MESSAGE_R3.txt`。用户正文原样保存，文件尾显式提供 356690 的 Change-Id，hook 未新生成 Change-Id。

严格核验：tree 相同、diff 仅一文件、字节级等于 master 去掉唯一 `-fuse-ld=lld ` 后在指定三处各增一行 libc++；提交全文与指定正文及自动签字一致。脚本快照 `verify_R3.py`，新提交的 `git show HEAD` 全文为 `GIT_SHOW_R3.txt`。

## 完整配置 diff

```diff
diff --git a/TIZEN/Tizen/Tizen-Base-Toolchain/project_config b/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
index 333eabeb6..7f1b8c811 100644
--- a/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
+++ b/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
@@ -293,6 +293,7 @@ Support: less
 Support: which  elfutils
 Support: update-alternatives
 Support: libstdc++-devel
+Support: libc++-devel libc++abi-devel
 Support: cpp
 Support: libatomic
 Support: libgomp
@@ -301,7 +302,9 @@ Support: hal-rootstrap-checker
 Support: app-rootstrap-checker
 
 Keep: libstdc++-devel
+Keep: libc++-devel libc++abi-devel
 Keep: cpp gcc libstdc++
+Keep: libc++ libc++abi
 Keep: pam
 Keep: binutils cpp  libmagic-data file findutils gawk gcc  gcc-c++
 Keep: gdbm gzip libada libunwind  glibc-devel libpcre xz-lzma-compat
@@ -350,5 +353,5 @@ Macros:
 :Macros
 %endif
 Macros:
-%build_ldflags -fuse-ld=lld -Wl,-z,relro,--as-needed%{?_ldflags_arch_extra}
+%build_ldflags -Wl,-z,relro,--as-needed%{?_ldflags_arch_extra}
 :Macros
```

同一 diff 原始文件：`FINAL_DIFF_R3.patch`。

## push 与 abandon 原始回执

push：`git -C tmp/PRJCONF_REVIEW_1012/repo push origin HEAD:refs/for/master`，退出 **0**。stdout 为空；stderr 原始字节保留于 `raw/008_review_push_R3.stderr.txt`。以下转录内容（终端进度行控制效果及行尾空白以原文件为准）：

```text
remote:
remote: Processing changes: refs: 1, updated: 1
remote: Processing changes: refs: 1, updated: 1, done
remote: warning: e13ce91: no files changed, message updated, was rebased
remote:
remote: SUCCESS
remote:
remote:   https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690 Prepare Tizen-Base-Toolchain build root for libc++
remote:
To ssh://review.tizen.org:29418/scm/meta/qb
 * [new reference]       HEAD -> refs/for/master
```

上传成功后作废：

```sh
ssh -p 29418 lhmax2025@review.tizen.org gerrit review --abandon --message '"Squashed into change 356690"' 2c5b5af89ce1a001f7acea9d8355fe5bc1e91137
```

退出 **0**；stdout **空**；stderr **空**。双层引用用于 SSH 远端重新解析后把整句保留为一个参数，消息内容仍为 `Squashed into change 356690`。精确 argv、命令与空输出保留于 `raw/009_abandon_R3.*`。随后只读 Gerrit query 确认 356689 为 `ABANDONED` / `open:false`，356690 为 `NEW` / patchset 3，无凭命令成功猜测最终状态。

## 边界、前提及项目回执

本轮不构建、不上板、不改 codes/ 或配置文件内容、不推包仓/sandbox、不起 QuickBuild；只改提交组织、说明、获准作废旧评审及项目材料。历史 R1/R2 材料保留。后续只需人工审阅 356690 patchset 3，不再合入 356689；libcxx-runtimes 出包并发布后，libc++ Support 才能生效的前提不变。lld 待验收后另提。

项目仓只提交本轮 _R3 材料和 LINE_STATUS.md；原有无关工作区修改不纳入。推送与远端 SHA 回执见 `PROJECT_RECEIPT_R3.md` 和本轮项目命令日志。没有新增需裁决的配置差异。
