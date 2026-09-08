# 支持范围草稿审阅清单 / Draft review checklist

此表用于逐项核对材料，**不是审批通过记录**。The checklist is not an approval record.

| 核对事项 / Check | 材料中的限定 / Required qualification |
|---|---|
| 四项处理 / Four changes | 等待与回滚、旧/新版本、现代析构不同步、Boost 两端 ABI；Boost 计划不等于全量部署完成。 / Waiting and rollback, versioned contracts, modern destructor behavior, and both sides of Boost; plans are not deployment proof. |
| 干净退出 / Clean exit | 展开穿过、清理执行、线程回收均有断言，不能只看退出码。 / Asserted unwinding, cleanup, and joining, not exit status alone. |
| 共享锁 / Shared locks | 清写者位、保留读者、排队者进展、正常值对照；不外推任意模板与调度。 / Writer rollback, reader preservation, queued progress, and normal values; no arbitrary-template or scheduling claim. |
| 四种绑定 / Four bindings | 旧已链接终止；旧对象重链布局依赖；新头新链接干净；新头先链旧库仍终止。 / Old linked: termination; old relinked: layout-dependent; new/new: clean; new headers linked to old library: termination. |
| “布局”含义 / Meaning of layout | 旧对象为调用点/清理表覆盖，不与 Boost 对象内存布局混用。 / Call-site/cleanup-table coverage for old objects, not Boost object representation. |
| 三处头内实现 / Three header implementations | 两种头内定时等待和转换缓冲析构须重编；定时写者回滚也须重编。 / Rebuild both header timed waits and the buffer destructor, as well as timed-writer rollback. |
| 排除范围 / Exclusions | 异步取消、async 析构、用户锁抛出、无异常信息、额外不抛帧、任意 Clock/回调、call_once 既有风险。 / Async cancellation, async destruction, throwing user locks, missing exception support, extra barriers, arbitrary Clocks/callbacks, and existing call_once risk. |
| 普通错误 / Ordinary errors | 新入口可抛；无人捕获仍终止，既有原生样本清理为零，不能许诺普遍改善。 / New entry can throw; uncaught still terminates, with zero cleanup in prior native samples; no general improvement claim. |
| 输出变化 / Output change | 六字节样本新析构只留一字节；显式同步完整。损失量不是普适常数。 / One of six bytes without explicit synchronization in the new sample; explicit synchronization completes output; loss is sample-specific. |
| 验证覆盖 / Coverage | 两架构各 235 次定向；原生官方 11,402，ARM 有效 7,848、缺口 3,554；aarch64 未测。 / 235 directed runs per architecture; native official 11,402, ARM valid 7,848 and gap 3,554; aarch64 unmeasured. |
| 验收与收尾 / Acceptance and cleanup | 新增官方失败不豁免；ARM 失联清理未完成；文稿保持草稿。 / New official failure not exempted; disconnected-board cleanup incomplete; document remains a draft. |
| 外部与部署 / External scope and deployment | 外部分母不可得；新头先于重编、双版本库先于链接，匹配运行库与 Boost ABI；产品发布未认证。 / External denominator unavailable; headers before rebuild, dual-version library before link, matched runtimes/Boost ABI; no product-release certification. |

中英文正文的仓内证据链接及 SHA256 见 [EVIDENCE_INDEX.tsv](EVIDENCE_INDEX.tsv)。
Repository evidence links and SHA256 values for both drafts are indexed there.
