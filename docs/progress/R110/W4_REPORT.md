# 四项待决事项背景材料汇编报告

## 产物

- `DECISION_BACKGROUND.md`：面向人工裁决者和多方评审的完整背景材料。四个裁决面
  均包含要决定的内容、实测与静态事实、libstdc++ 对照、各选项后果和未查清事项。
- `REVIEW_FACT_CHECKLIST.md`：供评审逐条核对事实、分母、架构和证据措辞。

材料没有使用内部站点编号组织正文；技术术语在首次出现处解释；所有推断、未观测
与不可得项均显式标注。单列了头文件内联无法由库更新修正、异步取消正交性、
libstdc++ 未持锁返回后的所有权不一致，以及共存不等于 ABI 安全等共同事实。

## 结论边界

本项只完成材料汇编，不作推荐、排序或最终决定。外部产品/应用分母、aarch64
目标内核取消行为、41 条未测 Boost 边和真实 compatibility wrapper 等缺口保持
原状态，没有推断补全。

平台源码、既有四个补丁、`codes/` 与 `config/` 均未修改。

## 自校验与脚本快照

`code/w4_validate.py` 检查四个裁决节、共同事实、架构覆盖、证据标签、Boost 计数、
65 条评审核对项和全部 Markdown 证据链接。最终输出：

```text
ASSERTIONS=PASS decision_sections=4 checklist_items=65 links=PASS arithmetic=PASS scope_labels=PASS
```

脚本 SHA256：
`5c27a7bd67cbb42f9f1b1888601ad774ec3ee3bfad5c594ed9ff6d0a08c5a5f2`。
命令、stdout、stderr 和退出码见 `raw/163_validate_w4.*`，哈希命令见
`raw/165_hash_w4_script.*`。
