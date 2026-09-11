# P6 续执行：Change-Id 已补齐并更新原 sandbox

实施元数据状态：**已完成并推送；代码 tree 不变。现行材料引用已同步，历史证据保留并列明，待人工审阅。**

用户已明确授权本次仅对 `sandbox/lhmax2025/libcxx-noexcept-relief` force 推送。
实际使用的是带完整旧 HEAD 的 `--force-with-lease`，没有使用通配 refspec、`--mirror` 或其他分支的强制更新。
旧基点不变，五提交次序不变；原作者与作者时间不变，新 committer 为 `hao.lin <hao.lin@samsung.com>`。

## 实施提交与核对

| 顺序 | 旧 SHA | 新 SHA | Change-Id |
|---|---|---|---|
| 1 | 753ed75f299eb7d693df490a7a36834599199de4 | 16a73b0af4d7f9194ae3a12d4934965d2f4005b7 | I2040e52eacc5c998257115519a520fcdd7ca2c1a |
| 2 | 2b94110f0ef5e036a17e8cfea80d0c69b0330935 | 72e893320c94db78b8661441628dfe9ba1e2cb02 | I687773d43ca05c7cc76afb6905250bdf592d7a2e |
| 3 | aafbe73ba928785fff19baf964b5a31c33be91b8 | 8e8335839aad10eb1ac876c49afe32355ff51cf5 | Id76cca985495072519e680a283525feceb639cc3 |
| 4 | 72e7a0f21ceeb1a2a4de60fc5137be194c748b90 | 4c5ba1fa3a40340a8687b0f4a76326ca41826d7f | Ie75fbec36a85a613cc1d1a6edf5827c3148f9192 |
| 5 | f3c1af692b579add991861e1f7c4950f6af39932 | c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13 | I39ff3d53d2af599841a72d66a8fb23f498c599b2 |

完整新旧 tree ID、parent 及逐项判定见 [SHA_MAPPING.tsv](SHA_MAPPING.tsv)；
旧／新提交对象的原字节在 [commit_objects/](commit_objects/)。五个 tree ID 逐一相同，
五次 `git diff --exit-code <旧> <新>` 均退出 0、输出为空；新链相对原基点恰好五提交。
每个消息只有一个 Change-Id，五个互不相同。原消息字节作为完整前缀保留，
只由标准 hook 增加一个必要分隔空行与 Change-Id 行，没有修改原主题或正文。
因此“仅加 Change-Id”的准确字节含义是新增 footer 及其分隔空行，而非声称总字节数只增一行。

## 标准 hook 与实际方式

使用 `scp -O -p -P 29418 -o BatchMode=yes -o ConnectTimeout=10` 从
`lhmax2025@review.tizen.org:hooks/commit-msg` 下载到隔离仓的 `.git/hooks/commit-msg`。
`-O` 使用 Gerrit 支持的 SCP 传输；`chmod u+x` 后 `test -x` 通过，实际逐条运行该 hook。
未改写 hook 算法，未手工生成 Change-Id。版本头为 Gerrit 3.10.5；
SHA256 为 `3c7e9b5fbe0b7ed945abd74248913c912ee0464abb416c18278bc5811dbb6f50`。
下载、权限、全文记录分别见 raw/006、007，[hook 快照](commit-msg)。

在 `tmp/P6_0909.7w4ZI8/repo` 的隔离无 checkout 仓中，逐提交取原 tree 和消息，运行 hook，
使用 commit-tree 复用原 tree 并串接新 parent。每条生成后即检查，不通过则停止。
推送后原实施工作仓只导入新提交对象并更新本地分支引用；最终工作树干净，没有写入平台代码。
脚本和全部子命令在 [rebuild.py](rebuild.py)、raw/rebuild/。

## 推送与全部分支对照

1. raw/003 在推送前重新记录了全部 **133** 个远端 heads，其中包含目标分支；不是“另有133个”。
2. raw/011 普通推送退出 1，服务器输出 `non-fast-forward`；原文完整保留。
3. raw/013 仅指定该一个 refspec，使用 `--force-with-lease=refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief:<完整旧HEAD>` 推送成功。
4. raw/014 再查全部 heads；[ALL_BRANCH_COMPARISON.tsv](ALL_BRANCH_COMPARISON.tsv)逐项比较：目标分支变化，**其他132个未变，无新增或删除分支**。
5. raw/017 确认原实施工作仓、隔离仓所建新链与远端最终 HEAD 一致：`c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13`；树／消息／五条链再次通过核验。

没有推正式分支、refs/for 或 LLVM 上游。Gerrit 对五个原主题给出了超过50字符的 warning，
但推送成功；按“只加 Change-Id”要求未顺手缩写主题。sandbox 推送成功不是正式 review 已建立或全部接收条件通过。

## 材料引用

全材料仓跟踪文本扫描覆盖54,533个文件，初次找到102个文件、306次旧SHA引用，
包括完整SHA和7至40位缩写，检索有正向对照。没有只查支持声明而漏查其他材料。
[初次逐处清单](REFERENCES_before.tsv)、[非文本等排除项](SCAN_EXCLUSIONS_before.tsv)。

现行说明文档的机械更新记录见 [REFERENCE_EDITS.tsv](REFERENCE_EDITS.tsv)：12个Markdown文件、33处SHA。
保留原始字节在 before_current/；正文增加日期及等树映射说明，防止误读为历史实验曾用新ID重跑。
五份当前交付 patch 快照、COMMIT_INDEX 和当前验证脚本的HEAD也同步更新；
原四个既有补丁不在更改范围，实施代码 tree 不变。

历史原始命令／stdout／stderr、旧快照及其校验清单、新旧映射必须与当前引用区别。
不能把 raw 中当时实际命令替换成从未执行过的新 SHA，也不能把旧补丁快照改名后继续使用旧摘要。
已向用户询问“更新现行引用、保留历史证据并列例外”的范围，截至封存尚未收到该异步问题的答复。
本轮没有改写历史证据：依照原任务的原始命令／退出码逐字落盘要求，保持其事实身份；
“全仓旧 SHA 字节为零”也与任务必须提供新旧映射、完整旧状态相冲突，不能据此删除旧列。

现行引用再次扫描为 **0 文件、0 处残留**（raw/028，有正向对照）；
既有跟踪材料中保留的历史／执行快照为 **83 文件、262 处**，逐处见
[REFERENCES_after.tsv](REFERENCES_after.tsv)。这些位置没有充当当前实施 HEAD；
本次新增审计材料还包含原提交、旧状态和新旧对照，其旧 SHA 同样属于明确的审计例外。
若人工要求另行处理历史证据，应另定保全与版本化方式；本轮不篡改它们，也不将这项范围确认写成已获答复。

现行12份Markdown更新33处SHA，加当前交付5份patch、COMMIT_INDEX与验证脚本，共44处原SHA引用同步。
另更新交付阶段状态／问题说明、当前EVIDENCE_INDEX，避免“已补齐但说明仍说缺失”。
合计修改23个现行材料文件，完整文件清单及每处差异见 [CHANGED_FILES.tsv](CHANGED_FILES.tsv)、diffs/；
旧版HASH清单与原始patch快照保留，当前交付索引已重新按当前文件计算，不冒充旧快照。
五份交付patch的稳定patch-id与旧版相同，`diff --git` 开始的代码diff字节也逐份完全相同（raw/029）。

当前中英文交付文档的90个本地引用、既有ARM统计与R81逐项来源重核通过（raw/027），
没有借元数据修订重新宣称运行验证，也没有改变已有测试数字或支持边界。

## 自行判断与限制

- 精确 lease 防止覆盖核查后他人推进的目标 HEAD；授权只适用于这一次这一分支。
- 保留原作者和作者时间；指定身份用作本次 committer，避免无授权改写署名。
- 标准 hook 自动添加 footer 分隔空行；原消息其他字节严格不动。
- 当前补丁快照只改变提交元数据，不视作新增代码实施或新的构建／运行验证。
- 现行材料的“仍缺 Change-Id”状态同步为已补齐；历史阶段记录明确标注其发生时间，不伪造既往操作。
- 初次读取引用清单遇到 CSV 字段超过默认大小，raw/019 退出1；提升读取字段上限后 raw/020 成功。
  初版扫描脚本快照单列 inventory_refs_v1.py，后续版本仅优化预筛和缩短上下文、细分校验清单类别。
- 不把历史证据中的旧 SHA、新旧对照表的旧列误判为“现行引用仍未更新”，也不声称全仓所有字节中旧 SHA 消失。

本线既有 R115 两份未提交修改未触碰。材料提交仅发往 GitHub `codex/runtime-validation`，不用 force。
