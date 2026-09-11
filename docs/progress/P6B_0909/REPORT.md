# 分支归并与旧分支删除核验

状态：`COMPLETE_PENDING_HUMAN_REVIEW`。已按顺序完成普通快进、材料更新、删除前确认及指定远端分支删除；全程没有 force。

## 合并前后状态

实施仓为 `platform/upstream/llvm`，只操作既有命名空间中的指定分支。
远端归并前完整 133 个 heads 见 [原始列表](raw/003_heads_before.stdout)，
其中本用户命名空间六个分支见 [命名空间快照](NAMESPACE_BEFORE.tsv)。

| 分支 | 归并前 HEAD | 归并后 HEAD |
|---|---|---|
| `sandbox/lhmax2025/libcxx-ehabi-backport` | `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa` | `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13` |
| `sandbox/lhmax2025/libcxx-noexcept-relief` | `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13` | `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`（此表为删除前状态） |

五提交首提交的直接父提交就是归并前目标 HEAD；merge-base 完全相同。
分支没有分叉；相对该基线恰好增加既定五个提交，既有 565,564 个祖先提交逐个保留，
合并后共 565,569 个可达提交。这是包含 LLVM 历史的祖先提交计数，不是新增实施提交数量。
完整旧提交清单、合并后逐项保留结果分别在 EXISTING_COMMITS_BEFORE.tsv、POST_MERGE_EXISTING_COMMITS.tsv。
清单为未压缩原始文本，文件摘要见 SHA256SUMS。

[逐文件检查](FILE_CONFLICT_CHECK.tsv)列出全部 10 个变化文件的旧／新 blob ID。
由于目标就是五提交的基线，没有目标侧分叉改动，没有需要选择的冲突解决结果。
没有执行 rebase、cherry-pick 或新增 merge commit；也没有修改任何实施代码。
在本地为远端既有分支建立同名引用并切换到相同 tree 后，使用普通、单目标 push 快进。
[推送原始 stderr](raw/009_merge_push.stderr)；没有 force、通配更新、refs/for 或正式分支推送。

## 五提交内容和身份

| 顺序 | 原样保留的 SHA | Change-Id |
|---|---|---|
| 1 | `16a73b0af4d7f9194ae3a12d4934965d2f4005b7` | `I2040e52eacc5c998257115519a520fcdd7ca2c1a` |
| 2 | `72e893320c94db78b8661441628dfe9ba1e2cb02` | `I687773d43ca05c7cc76afb6905250bdf592d7a2e` |
| 3 | `8e8335839aad10eb1ac876c49afe32355ff51cf5` | `Id76cca985495072519e680a283525feceb639cc3` |
| 4 | `4c5ba1fa3a40340a8687b0f4a76326ca41826d7f` | `Ie75fbec36a85a613cc1d1a6edf5827c3148f9192` |
| 5 | `c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13` | `I39ff3d53d2af599841a72d66a8fb23f498c599b2` |

[合并前身份表](FIVE_COMMITS_BEFORE.tsv)与[合并后身份表](POST_MERGE_FIVE_COMMITS.tsv)记录完整 tree ID。
每一条从新分支 HEAD 的对应祖先位置重新解析，SHA 与原始记录相同；tree ID 相等；
五次 `git diff --exit-code <原 tree> <合并后提交 tree>` 均退出 0、无输出。
提交对象原字节也逐一与 commit_objects/ 中的快照比较相等，包括消息、父关系、作者和 Change-Id。
五个 Change-Id 各出现一次且互不相同。

本地目标分支和远端 HEAD 一致；[合并前后全分支对照](POST_MERGE_BRANCH_COMPARISON.tsv)确认
除目标快进外，其他 132 个远端 heads 逐项未变，无新增／删除分支。

## 材料引用更新

对全部 54,983 个跟踪文本文件扫描，初始 99 个文件、330 行、334 次旧名命中；
原始结果见 [初始清单](BRANCH_REFERENCES_before.tsv)。不是仅扫描支持声明。
8 处现行分支引用改成目标分支；12 份现行说明增加日期和统一映射入口；加交付证据索引共修改 13 个现行材料文件。
早期实施的两段建立／推送旧分支操作原文保留，并明确标记为历史记录。
89 个历史日志、旧快照或元数据审计文件逐文件核对 SHA256 保持不变。
统一解释见 [分支映射](BRANCH_MAPPING.md)，现行交付证据摘要索引按更新后文档重新计算。

逐处替换原文、替换后文本及行号见 [REFERENCE_EDITS.tsv](REFERENCE_EDITS.tsv)；
全部改动文件和前后摘要见 [CHANGED_FILES.tsv](CHANGED_FILES.tsv)，完整逐文件 diff 在 diffs/。
历史例外逐处清单为 [HISTORICAL_BRANCH_REFERENCES.tsv](HISTORICAL_BRANCH_REFERENCES.tsv)。
五提交 SHA 不变；[现行 SHA 引用核验](CURRENT_COMMIT_REFERENCES.tsv)检查它们在目标分支上可达。
没有把历史旧名命中伪称全仓零命中；要求为“现行有效依赖零残留”，有初始八处实际命中和构造正向对照。

## 删除前的必要确认

第一步已通过：五个提交全部在目标远端及本地分支上，tree 和提交对象原字节一致，旧提交无丢失。
第二步已通过：独立材料检查确认有效旧名依赖为 0，正向对照通过；89 个历史文件和两个原操作段落保全；
35 处现行提交引用在目标分支上可达，中英交付文档 92 个本地引用及既有数字检查通过。
结果先写入 [删除前最终确认](PRE_DELETE_CONFIRMATION.md)，raw/019 退出 0，之后才执行 raw/020 的删除命令。
完整即时删除前列表 raw/018 与合并后列表逐项一致，目标及旧分支 HEAD 仍相同。

## 删除后核对

只执行 `git push origin --delete sandbox/lhmax2025/libcxx-noexcept-relief`，退出 0。
[完整删除 stderr](raw/020_delete_remote.stderr)明确显示该单分支被删除。
随后的 [远端完整 heads](raw/021_heads_after_delete.stdout)共 132 条，旧分支不存在；
[删除前后逐项对照](POST_DELETE_BRANCH_COMPARISON.tsv)确认只删除该一条，其他 132 条逐项不变。
这 132 条包括已快进的目标分支；以整个任务最初状态为准，变化只有目标快进和旧分支删除两项。

目标分支远端、本地分支、远端跟踪引用与工作仓 HEAD 均为
`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`，源码工作树干净。
再核全量 565,564 个旧祖先提交一个未丢，只增加既定五个；
[删除后的五提交复核](POST_DELETE_FIVE_COMMITS.tsv)再次通过 tree ID、逐次 diff、消息和提交原字节检查。
全量逐提交保留表为 POST_DELETE_EXISTING_COMMITS.tsv。
删除后的材料复查也通过（raw/023；MATERIAL_CHECK_post_delete.json），没有因删除出现失效的现行提交引用。

删除对象仅为远端分支引用；提交仍由目标分支保留。本地旧分支引用和完整旧状态快照保留，
未删除任何代码或历史证据，未尝试自动恢复或重建旧远端分支。

## 封存与交付

材料提交仅包含本目录证据和上述 13 个现行材料文件，发往 GitHub `codex/runtime-validation`，普通推送。
命令、完整 stdout/stderr、退出码与 UTC 起止时间在 raw/；每个 Git 核验子命令也独立记录。
审计脚本原样落盘，脚本 SHA256 在 SCRIPT_SHA256SUMS，完整证据摘要在 SHA256SUMS。
材料提交自身的提交／推送回执保留于本地 delivery/，不把尚未结束的日志封进其自身提交。
本次没有出现基线分叉、冲突、推送拒绝或待人工裁决的问题。

## 自行判断与限制

- “不新建 sandbox 分支”落实为不新增远端名称；本地同名引用只用于跟踪既有目标分支。
- 早期报告的两处原操作叙述属于历史证据，保留原文并加映射；不把当时创建／推送的目标伪改为另一分支。
- 历史证据保留的授权已由本次任务书明确；现行材料只改分支定位与对应索引，不改变测试数据或支持结论。
- 源码树未改、五提交未重建，因此没有新的构建或运行验证。旧的构建／测试证据身份未被重写。
- 本线已有 R115 两份未提交修改不在任务范围，不触碰、不提交。
- 本地旧分支引用不是远端分支；本任务只删除用户指定的远端引用，不扩大为删除本地备份、文件或提交对象。

完成后交人工审阅，不继续实施或安排新任务。
