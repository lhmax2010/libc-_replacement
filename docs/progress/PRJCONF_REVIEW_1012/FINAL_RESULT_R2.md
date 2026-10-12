# 提交说明修正：两个评审均已更新到 patchset 2

**完成，未修改任何配置文件内容。** 只将 356689 的提交正文改为人工指定的准确表述；356690 因父提交改变随 rebase 更新 SHA，正文原样保留。Gerrit 返回 `updated: 2`，不是新 change；远端两条 patchset 2 引用均与本地新 SHA 一致。

| 评审 | 旧 SHA（patchset 1） | 新 SHA（patchset 2） | tree 对照 |
|---|---|---|---|
| [356689](https://review.tizen.org/gerrit/c/scm/meta/qb/+/356689/2) | db671becfe0f2826dac7f479d091d5a23feccb47 | 2c5b5af89ce1a001f7acea9d8355fe5bc1e91137 | 相同，git diff 为空 |
| [356690](https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690/2) | 0e9fb65ce82b6ca54a5b80a93aa6bcd26c40db2a | f385421f72c3daf28ddb60b5b04d051cbd2fdbad | 相同，git diff 为空 |

## 基线与修改方式

- clone：`tmp/PRJCONF_REVIEW_1012/repo`，分支 `prjconf-libcxx`，开工与收尾工作树干净。
- `git fetch origin` 退出 0；origin/master 仍为 `0bb6ff66506574606cf4d636aaa8974ff4c779b8`，不存在 master 前进后的移植或冲突处理。
- 使用 `git rebase -i`，脚本仅将两笔中的第一笔改为 edit；在该停点 `git commit --amend -F REVERT_MESSAGE_R2.txt`，再 `git rebase --continue`。未编辑工作树配置文件。
- 第一笔的全文严格等于 `REVERT_MESSAGE_R2.txt`，保留原 Change-Id 与 Signed-off-by；第二笔完整提交消息逐字比较相同。
- 未重新执行旧版 `verify_review.py`（它绑定旧正文）；本轮 `verify_R2.py` 针对新正文、新旧 tree、身份、图关系及第二笔完整正文作严格比较。

## 核验

| 项 | 结果 |
|---|---|
| 第一笔 tree | c8cec2a0689ed99b61e9bbb75763b2395aef7d8b，新旧相同 |
| 第二笔 tree | 04f46636709cd42b7967e3c7bd40cb6098d61cfa，新旧相同 |
| 两笔 git diff 旧SHA 新SHA | 分别为空且退出 0 |
| 第一笔 Change-Id | I0b3f8e75e3664d9e044eeba972d7d4669c59a342，不变 |
| 第二笔 Change-Id | I0403eb61b5c412c7ed53dc6f862c92765ead121f，不变 |
| 作者 / 提交人 / Signed-off-by | 两笔均 Hao Lin <hao.lin@samsung.com> |
| 第二笔正文 | 与旧提交逐字相同 |
| 第一笔父 / 第二笔父 | 原 master / 新第一笔 |
| 最终工作树 | 干净，分支 prjconf-libcxx |
| 推后 master | 仍为 0bb6ff…，未直接更新 master |

完整比对值及嵌套命令输出：`VERIFY_RESULT_R2.json`、`VERIFY_COMMANDS_R2.json`；两笔提交全文：`COMMITS_R2.diff`。脚本快照：`sequence_editor_R2.py`、`verify_R2.py`。原 patchset 1 报告保留，不改写历史结论。

## push 原始输出

命令：`git -C tmp/PRJCONF_REVIEW_1012/repo push origin HEAD:refs/for/master`，退出 **0**。原始逐字输出及命令在 `raw/008_review_push_R2.*`；以下转录 stderr（保留内容，去除终端进度用的行尾控制效果）：

```text
remote:
remote: Processing changes: refs: 1, updated: 2
remote: Processing changes: refs: 1, updated: 2, done
remote: warning: 2c5b5af: no files changed, message updated
remote: warning: f385421: no files changed, was rebased
remote:
remote: SUCCESS
remote:
remote:   https://review.tizen.org/gerrit/c/scm/meta/qb/+/356689 Revert lld default linker in Tizen-Base-Toolchain
remote:   https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690 Add libc++ to Tizen-Base-Toolchain build root
remote:
To ssh://review.tizen.org:29418/scm/meta/qb
 * [new reference]       HEAD -> refs/for/master
```

`git ls-remote` 实际确认（raw/009，退出 0）：

```text
2c5b5af89ce1a001f7acea9d8355fe5bc1e91137 refs/changes/89/356689/2
f385421f72c3daf28ddb60b5b04d051cbd2fdbad refs/changes/90/356690/2
0bb6ff66506574606cf4d636aaa8974ff4c779b8 refs/heads/master
```

## 边界与后续

本轮仅修改提交说明和项目材料；不构建、不上板、不改 codes/ 或配置内容、不推包仓/sandbox、不起 QuickBuild。没有 force，没有直接推 master。评审未由本轮合入；libcxx-runtimes 先出包发布、再让 libc++ Support 生效的前提不变。

项目材料提交推送及远端 SHA 回执见 `PROJECT_RECEIPT_R2.md` 和本轮 raw 中项目命令；仅纳入本轮材料与 LINE_STATUS.md，不纳入原有无关修改。无新增配置判断或待裁决分歧。
