# P6：已发布提交改写需要非快进更新，暂停待裁决

状态：**BLOCKED_REQUIRES_HISTORY_REPLACEMENT**。只完成现状核查，没有重建实施提交、安装 hook、
改写实施分支或更新旧 SHA 引用。没有使用 force，也没有向 Gerrit 执行 push。

## 停止的依据

2026-09-11 的实际只读查询确认：实施仓 origin 为
`ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm`；本地及远端
`sandbox/lhmax2025/libcxx-noexcept-relief` 的 HEAD 都是
`f3c1af692b579add991861e1f7c4950f6af39932`。

基点 `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa` 之后恰好五个线性提交，全部没有 Change-Id，
且全部已在该远端分支中。完整原提交对象、消息、parent、tree 已保存。

**Git 提交图分析**：在保持基点、五提交数量及次序的条件下，给第一条消息增加 Change-Id 会改变
该提交 ID；后续提交的消息及 parent ID 也会变化。新链不包含原远端 HEAD，因此不是普通推送
允许的快进更新。tree 相同不能使提交祖先关系相同。保留原链再追加或合并，也不符合本任务
“重新生成这五个提交、没有多余提交”的要求，故没有自行采用这些方式。

这不是某次 push 的服务器拒绝原文：**本轮实施分支 push 为 NOT_OBSERVED，未尝试**。
遵循任务开头“若任一步需要 force 推送，停下报告”，在已查明这一必要条件时即停止，
没有先改写再等待拒绝。hook 获取与后续验证均未执行，不补写成功或失败结果。

## 五个当前提交与新 SHA 状态

| 顺序 | 旧 SHA | 新 SHA | 内容主题 |
|---|---|---|---|
| 1 | 753ed75f299eb7d693df490a7a36834599199de4 | NOT_GENERATED | 解除条件变量等待规格 |
| 2 | 2b94110f0ef5e036a17e8cfea80d0c69b0330935 | NOT_GENERATED | 写者状态回滚 |
| 3 | aafbe73ba928785fff19baf964b5a31c33be91b8 | NOT_GENERATED | 析构不再同步 |
| 4 | 72e7a0f21ceeb1a2a4de60fc5137be194c748b90 | NOT_GENERATED | 条件变量双版本 |
| 5 | f3c1af692b579add991861e1f7c4950f6af39932 | NOT_GENERATED | 定时入口的目标 intmax_t ABI 修正 |

逐提交旧 tree、parent、原主题见 [COMMITS_BEFORE.tsv](COMMITS_BEFORE.tsv)。
原消息字节与完整提交对象在 [original_commits/](original_commits/)，不是格式化摘录替代原文。
没有新提交，所以新旧 tree 一致性、Change-Id 唯一性及消息仅增一行检查均为 NOT_OBSERVED，不能标为通过。

## sandbox 命名空间的远端现状

| 分支（均在 sandbox/lhmax2025/ 下） | 远端 SHA |
|---|---|
| libcxx-ehabi-backport | c3f8578a4db871d9d6de96d751f4c2ea7b1638fa |
| libcxx-migration | 2d23367d74afbf2bb1e9e4013fce072b3a154109 |
| libcxx-noexcept-relief | f3c1af692b579add991861e1f7c4950f6af39932 |
| llvm_only_clang | f8277158cf44f872e2b2d62a2e839cb33bd0afea |
| mlgo | 08ffd8cd7c72f5dd6e612d8395362def96d84029 |
| mlgo_aot | 88ff57cadcff52ab50ca6865db7faed7388358fe |

第 1c 的完整对照表见 [NAMESPACE_BRANCHES_BEFORE.tsv](NAMESPACE_BRANCHES_BEFORE.tsv)；
另保留整个 `sandbox/` 集合 [SANDBOX_BRANCHES_BEFORE.tsv](SANDBOX_BRANCHES_BEFORE.tsv)，
全部远端 heads 原始输出在 raw/005。停止后的第二次只读比较见 raw/008 和 raw/009。
两次查询的 **133 个远端 heads 逐字节相同**（cmp 退出 0），包括本命名空间六分支及正式分支。
本轮未向任何实施仓分支写入，不存在正式分支或其他 sandbox 推送操作。

## 未执行与引用处理

| 任务阶段 | 实际状态 |
|---|---|
| 资源检查、当前提交／分支记录 | 完成；light 退出 0 |
| 从 Gerrit 获取／安装可执行 commit-msg hook | NOT_OBSERVED，未执行 |
| 重建五个提交并添加 Change-Id | 未执行 |
| 新旧 tree／消息／唯一 Change-Id 检查 | NOT_OBSERVED，无新提交可比 |
| 实施分支普通 push 或 force push | 均未执行 |
| 支持声明、交付材料等引用更新 | **0 文件修改**；没有已确认的新 SHA，保留当前有效引用 |
| 全材料旧 SHA 零残留检查 | NOT_OBSERVED，尚未进入引用替换阶段；不能声称旧 SHA 已清零 |

仅将本次阻塞记录提交到 GitHub 的材料分支 `codex/runtime-validation`，不改变既有报告结论。
提交使用任务指定身份 `hao.lin <hao.lin@samsung.com>`，通过单次提交配置传入，不更改全局身份。

## 自行判断与尚存疑问

1. 将“任一步需要 force 即停”应用在远端历史核查阶段，而不是等到改写后失败；依据是已发布的五提交线性图，不是猜测 SSH 或 Gerrit 权限。
2. 产物按任务末尾上传位置使用 `docs/progress/P6_0909/`，不另建一套重复的 progress 根目录。
3. **需要人工裁决：如何处理该已发布 sandbox 分支的历史替换？** 现有禁止 force 条件下，本任务无法完成原分支更新。没有擅自创建替代远端分支或提交 review。
4. 原五提交 author／committer 都是 `He Fangyu <fangyu.he@samsung.com>`，尚未改动。若恢复任务，需明确指定 Git 身份仅用于新 committer，还是也要求更改原 author；后者超出“只加 Change-Id”的通常改写范围。
5. 后续“全部材料旧 SHA 零残留”与保留原始证据、新旧对照表之间存在范围问题：是否仅更新现行说明中的引用，保留原始日志、历史快照、补丁原文及新旧对照的旧 SHA？本轮尚未修改任何这些材料。

没有改代码内容。已存在的 R115 两份未提交修改保持原样；本次只暂存 P6 材料。
最初试读不存在的 W4/REPORT_ZH.md 是定位失误，随后使用 rg 找到实际 DELIVERY_ZH.md；
没有据该不存在路径作任何结论。执行脚本快照和 SHA256 随包提供。

任务在上述阻塞点暂停，交人工审阅；不将此状态写成 Change-Id 已补齐。
