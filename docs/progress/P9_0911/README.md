# 对象大小与对齐的成员级配套说明

- [中文报告](MEMBER_LAYOUT_ZH.md)
- [English report](MEMBER_LAYOUT_EN.md)
- [原表原样副本](baseline/abi_layout_comparison.tsv)
- [GNU 13.3／14.2 声明对照](GNU_REVISION_CROSSCHECK.md)
- [源码身份及哈希](SOURCE_MANIFEST.tsv)、[LLVM 探针缓存对照](LLVM_CACHE_CROSSCHECK.json)
- [公开设计依据与检索缺口](PUBLIC_REFERENCES.md)
- [文档核查结果](VALIDATION.json)
- [执行与交付记录](EXECUTION.md)

只分析原表中 10 个不同类型；其余 23 个类型没有扩展成员分析。原件 TSV 未修改。对象总大小沿用旧实测；成员字节账目来自固定源码与基线 ABI 的静态核算，未构建、未运行新探针、未使用开发板。

Only the ten differing types are analyzed. The other 23 types have no new member analysis. The original TSV is unchanged. Object sizes reuse existing measurements; member byte accounting is static source/ABI analysis. No build, new probe execution or board use occurred.

报告清楚列出无法追溯的设计原因、未实测的偏移／节点／控制块与行为组合。材料完成后交人工审阅；本说明不批准任何布局改造或生产部署。
