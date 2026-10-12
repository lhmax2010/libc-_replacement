# Base-Toolchain project_config：两项评审已上传

## 结论与评审

**UPLOADED_FOR_REVIEW**。基于人工确认地址的最新 master，撤销当前 lld 默认链接器改动，再叠加三行 libc++ Support/Keep。唯一 qb push 为 `HEAD:refs/for/master`，退出 0、新增两个 NEW change；未直接推 master、未合入。初次地址门禁记录保留于 `INITIAL_BLOCKER.md`，该阻断已由人工确认解除。

| 顺序 | Gerrit 评审 | 提交 SHA | Change-Id |
|---|---|---|---|
| 1：撤 lld | https://review.tizen.org/gerrit/c/scm/meta/qb/+/356689 | db671becfe0f2826dac7f479d091d5a23feccb47 | I0b3f8e75e3664d9e044eeba972d7d4669c59a342 |
| 2：加 libc++ | https://review.tizen.org/gerrit/c/scm/meta/qb/+/356690 | 0e9fb65ce82b6ca54a5b80a93aa6bcd26c40db2a | I0403eb61b5c412c7ed53dc6f862c92765ead121f |

两笔作者、提交人、Signed-off-by 均为 `Hao Lin <hao.lin@samsung.com>`，各有独立 Change-Id，第二笔父提交为第一笔。完整元数据及字节级验证见 `FINAL_VALIDATION.json`、`FINAL_commands.json`；提交全文见 `FINAL_REVERT_COMMIT.diff`、`FINAL_LIBCXX_COMMIT.diff`；合并 diff 全文为 `FINAL_FINAL_DIFF.patch`。

## 远端、基线与旧副本

| 项 | 实测 |
|---|---|
| codes/qb origin（未改） | git://review.tizen.org/git/scm/meta/qb |
| 人工授权 SSH origin | ssh://lhmax2025@review.tizen.org:29418/scm/meta/qb |
| 两协议 master 查询 | 均退出 0，均为 0bb6ff66506574606cf4d636aaa8974ff4c779b8 |
| 最新 master 作者/日期 | Tizen Build Automation <tizenbuild@samsung.com>；2026-10-12 01:50:53 +0000 |
| codes/qb HEAD | 63d0d4ace65a45ac6da21b1577346c1d6cace7a0 |
| 新克隆/本地分支 | tmp/PRJCONF_REVIEW_1012/repo；prjconf-libcxx |
| 旧 HEAD..origin/master 范围 | 503 笔，完整清单 raw/030*；范围计数不是祖先关系证明 |
| 四份配置净差异 | 仅 Base-Toolchain：17 行变动（11 增、6 删），另三份无净差异；raw/031* |
| 推前再次 fetch | 退出 0，master 仍为 0bb6ff…，无需 rebase；重新严格核验 PASS |
| 推后远端 master | 仍为 0bb6ff…，raw/056*；评审上传没有更新 master |

最新 master 提交信息原文（raw/022*）：

```text
Update candidate_snapshot : TIZEN/Tizen/Tizen-Base-Toolchain/project_meta,TIZEN/Tizen/Tizen-Unified-Toolchain/project_meta,TIZEN/Tizen/Tizen-Base/project_meta,
```

新克隆起初缺旧 codes/qb 对象，两次比较退出 128（raw/023、024）。通过只读本地 fetch 将旧 HEAD 导入新克隆独立 `refs/remotes/local-mirror/inspection-head` 后重做成功，未替换 origin/master。查询、克隆与修正命令均保留于 raw/016–031。

## 四份配置与 lld 历史

最新 master 的完整匹配见 raw/028*；Base-Toolchain 的完整 `git log -p --follow` 见 raw/029*。

| 文件（TIZEN/Tizen/ 下） | 命中行原文 | 本轮处理 |
|---|---|---|
| Tizen-Base/project_config | 39: `Prefer: lldb` | lldb 调试器不是 lld；不改 |
| Tizen-Unified/project_config | 无匹配 | 不改 |
| Tizen-Base-Toolchain/project_config | 39: `Prefer: lldb`；353: `%build_ldflags -fuse-ld=lld -Wl,-z,relro,--as-needed%{?_ldflags_arch_extra}` | 只撤 fuse-ld；保留 lldb |
| Tizen-Unified-Toolchain/project_config | 无匹配 | 不改 |

**最新 master 没有 undefined-version 或 Support/Keep lld。** 当前 fuse-ld 来自独立提交 bae4ebdd，只改本文件一行一个标志，按授权完整 `git revert --no-commit`，再按指定正文 `git commit -s`，不是裁剪混合提交。

| 历史提交 | 作者、日期 | 内容与处置 |
|---|---|---|
| eaf00da271d3d70affafb0e388a4a2332304347a | Shanshan Liu <ss0217.liu@samsung.com>；2026-09-29 17:22:09 +0800 | 首次加 Prefer/Support/Keep lld 与 fuse-ld；原文 raw/033* |
| 79580aa642d57881db6541df99cf04316699b393 | fangyu.he <fangyu.he@samsung.com>；2026-09-30 14:13:59 +0800 | 删除 Prefer/Support/Keep lld；不反撤 |
| d14ae536384bcfda6b3499fed501fd3e47600cd7 | Dongkyun Son <dongkyun.s@samsung.com>；2026-10-01 16:51:57 +0900 | 删除 fuse-ld；原文 raw/033* |
| bae4ebdd732e3c01611de2414434d4c5c162763c | fangyu.he <fangyu.he@samsung.com>；2026-10-08 11:45:43 +0800 | 独立重新加 fuse-ld；本轮撤销对象，原文 raw/032* |

bae4ebdd 提交信息原文：

```text
Modify codes to support llvm-lld

Change-Id: I68facad05efa7e0430b7a79697ebcfabc8325958
```

其余引入/删除提交的完整 SHA、作者/提交人、日期、提交信息和 diff 均保留于 raw/033*，不以摘要代替原文。两份 lldb 行 blame 为 391390ff…（latest snapshot 边界），见 raw/037*，未当作 lld 删除。

## 修改与门禁

| 核查 | 结果 |
|---|---|
| commit-msg hook | 指定主机 hooks/commit-msg，scp -O 退出 0；SHA256 3c7e9b5fbe0b7ed945abd74248913c912ee0464abb416c18278bc5811dbb6f50 |
| 撤销范围 | 与 master 逐字节比对恰好删除唯一 `-fuse-ld=lld `；raw/039–041 |
| 已有 libc++ Support 检查 | 原文件未有，raw/042* |
| R84 上下文 | `git apply --check -p2` 退出 0；-p2 去掉 codes/qb/ 前缀；raw/043* |
| 三行写入 | 上下文通过后 apply_patch 写入同样三行，严格比对通过；并非上下文不匹配后的语义替代 |
| 最终文件范围 | 只改 Base-Toolchain/project_config；另三份 blob 不变；工作树干净 |
| 最终 grep | 296 Support、305/307 Keep；无 fuse-ld=lld、undefined-version、Support/Keep lld；raw/053* |
| 最终文件 SHA256 | 22bb5009c002a65fedffe888aeeb60f0bba9bd213b646902cfe6652aa290c097 |

撤销提交正文按用户指定全文保留，包括 undefined-version 与 Support/Keep lld 的背景描述；**不据正文声称最新 master 或本次 diff 实际含有这些行**。三行 libc++ 的位置、提交信息逐字核验均见 FINAL_VALIDATION/commands。

## 上传、发布前提及边界

实际上传命令 `git -C tmp/PRJCONF_REVIEW_1012/repo push origin HEAD:refs/for/master`，退出 **0**；原始命令、argv、完整 stdout/stderr、退出码保留于 `raw/054_gerrit_review_push.*`，返回 `Processing changes: refs: 1, new: 2`、`SUCCESS` 和两个 NEW 链接。没有 force、没有重试其它路径或直接推 master。

本轮没有构建、上板、改 codes/、推 sandbox/包仓分支或起 QuickBuild。**上传不等于合入。** 按人工要求先撤 lld、再加 libc++，两项一起合入；**libcxx-runtimes 已构建并发布后，libc++ Support 才能生效**，否则新增 Support 包不可解析。lld 待验收后重提；Unified 全局 libc++ 本轮不提。当前发布时机由人工确认，本轮未重新检查发布仓；没有以配置核查冒充构建验收。

项目仓材料收尾回执见 PROJECT_RECEIPT.md 与项目 push/ls-remote 原始日志；该 SHA 不代表 qb master。凭据文件未读取或提交，公开邮箱、用户名和仓库 URL 作为身份与来源证据保留。

提交前暂存范围核验 PASS：仅本目录与 LINE_STATUS.md；355 个暂存文件、403890 字节，私钥/密码令牌赋值及 token 形态扫描无命中（raw/061）。这是一项模式扫描，不声称能识别任意秘密；未读取凭据文件进行比对。脚本快照 SHA256 见 raw/062。原始 git show/patch 的上下文与空白保留，不重排原始证据；人工编写 Markdown 的 diff --check 退出 0。
