# 实现定义类型差异扫描

第一节完成：对冻结的 31,586 处结果重扫，旧输出未覆盖。

| 结果 | 改前 | 改后 |
| --- | ---: | ---: |
| RESULT_NO_OBJECT | 15,780 | 15,770 |
| RESULT_TYPE_DIVERGENT | 0 | 10 |
| RESULT_STDLIB_OBJECT | 13 | 13 |
| RESULT_UNRESOLVED | 15,793 | 15,793 |

10 处逐条见 `RECLASSIFIED_CHANGES.tsv`，65 候选全量计数见 `RECLASSIFIED_CANDIDATES.tsv`。本节重新核验 6,952 个平台来源路径 SHA；复用原有两库五次测量，不重新选择实参。静态 const bool 定义链沿用受保护的实际两侧声明证据；具体类型必须相同才允许 RESULT_NO_OBJECT。

与人工给出的数量一致：numeric_limits 1、decay 9。未继续推进 65 项关闭。

第二节清单已建立：585 个固定表达式、20 族；`CATALOG.tsv`、`CATALOG_SOURCE_EVIDENCE.json`。有限实例范围与类型编码判据见 `SCOPE.md`。

第三节测量完成（x86_64）：585 个表达式，IDENTICAL 283、DIVERGENT 242、NOT_AVAILABLE 60；成功实例两库各五轮一致。

242 个不同分为：内建底层类型 13；仅命名空间/ABI 标签 114；枚举/内建或枚举身份 4；实现类/模板实参/组合类型 111。完整两侧展开、编码与原始证据见 TYPE_RESULTS.tsv / DIVERGENT_TYPES.tsv。不能把242个均称为本次新的标量类型问题。完整包装编码的解码修正和重测见 SCOPE.md。

第四节完成本轮扫描并保留缺口：242 项的逐项暴露表已出。4 项有闭合跨包结构链，15 项有公开声明但消费路径未闭合，223 项未闭合具体实例的公开签名（NOT_OBSERVED，不是阴性）。新增 2 包对 / 2 符号边，已确认范围由 16/21 更新为 18/23。Dali ReadFile 另有消费者匹配，但同 SONAME 的 provider 配置未唯一绑定，未混入已确认数字。

总体 PARTIAL：有限的 585 表达式测量和旧结果重判完成；不声称已经枚举所有可能实现定义类型、所有模板实例或闭合全部暴露路径。armv7l 全部 NOT_OBSERVED，未使用开发板。最终证据审计和交接见 FINAL.md / AUDIT.json。
