# 实施分支映射（2026-09-11）

当前 LLVM 实施取件分支：`sandbox/lhmax2025/libcxx-ehabi-backport`。
代码仓：`ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm`。

| 历史分支 | 当前分支 | 提交身份 |
|---|---|---|
| `sandbox/lhmax2025/libcxx-noexcept-relief` | `sandbox/lhmax2025/libcxx-ehabi-backport` | 五个提交原样快进；SHA、tree、消息及 Change-Id 不变 |

当前 HEAD：`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`。
五提交原始基线：`c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`；它仍在当前分支祖先链上。
完整提交身份见 [五提交原始身份](FIVE_COMMITS_BEFORE.tsv)、
[快进后逐提交核验](POST_MERGE_FIVE_COMMITS.tsv)和[执行报告](REPORT.md)。

原始命令、stdout/stderr、旧快照、既往分支列表与元数据重建记录保持原文。
这些证据中的旧分支名不是当前下载或部署依赖；不可把它们改写成当时未执行过的命令。
早期实施阶段报告中“建立旧分支”和“推送到旧分支”的两个历史段落也保留，并在原报告顶端注明映射。
其余现行支持声明、交付说明和当前状态引用统一使用当前分支。
此前 P6 提出的历史证据保留范围问题，已由本次任务书明确授权解决。

SHA 引用不需要再次换号：五个提交未重建，在新分支上仍有效；
P6 更早的旧 SHA 如出现在原始证据中，仍按 [P6 新旧 SHA 映射](../P6_0909/resume/SHA_MAPPING.tsv)解释。
本次不改变任何实施代码、测试结果、支持边界或既有补丁。

## English summary

The current LLVM implementation branch is `sandbox/lhmax2025/libcxx-ehabi-backport`,
at `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`.
It contains the same five commits previously on `sandbox/lhmax2025/libcxx-noexcept-relief`;
their commit IDs, trees, messages and Change-Ids are unchanged.
Historical command logs and snapshots retain the old branch name as evidence of actions at that time.
They are not a dependency on that branch remaining available. No tests were rerun in this consolidation.
