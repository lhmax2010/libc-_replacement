# 投影结果审计状态

已处理第 1–60 个 / 65 个 COMPILE_TIME_ONLY 候选；未修改原重判记录或 plan.json。

- 候选汇总：{"RESULT_UNRESOLVED": 60}。
- 逐处结果：{"no_object": 15671, "stdlib_object": 13, "unresolved": 15743}。
- 含 std 对象投影的候选：2；包边审查尚未结束，不能把对象投影计为新包边。
- 全量范围与数量：COVERAGE_INVENTORY.tsv；逐处实际实参、来源及结果：occurrences/ 与 results/。
- 编译测量使用从平台原文提取的封闭表达式；约束投影若固定结果类型，单列存在性未观测。对依赖名称、using 导入、模板模板实参等保留具体原因，不填任意 int 代入。
- 两库结构对照、五次输出一致性、源文件 SHA 核验保持；没有重判 BUILTIN_ALIAS / EMPTY_TYPE 的既有 24 项。
- 资源 light、串行 nice 19 / ionice 3、RLIMIT_AS 30%；不用板子，不构建平台包。
