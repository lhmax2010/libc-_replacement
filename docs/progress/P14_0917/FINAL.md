# 审阅交接

**当前 adaptor 状态：PARTIAL。** 按人工裁决，三项原阻断分别归为行为变化、provider 外部缺陷及语言固有限制。原始证据没有改写，生产方向等尚未验证，不等于生产发布验收通过。

| 项目 | 结果 | 证据 |
| --- | --- | --- |
| GNU-only 四格 | 每格 5 次；展开中第二异常逸出 5/5 SIGABRT，与既测 adaptor 路径一致；另三格全部存活 | [对照](W1/REPORT.md) |
| 正常退出时析构抛出 | GNU 外层接到 Custom{314}，未终止；夹具显式 noexcept(false)，不支持“此格 adaptor 比 GNU 更宽容”的说法 | W1 原始输出与夹具 SHA |
| provider 泄漏定位 | 首个名称 257 字节、值 513 字节、vector 存储 64 字节；第 4 次请求扩容失败后遗留 834 字节；连续 10 次为 8340 字节，各 5 次稳定 | [定位](W2/REPORT.md)、ALLOCATION_OWNERSHIP.json |
| 独立复现 | 原始真实 GNU EWK，无 adaptor；正常分配/释放对照归零，原始 12 场景各 5 次通过 | W2/ASSERTED_ORIGINAL_RESULTS.json、NORMAL_ORIGINAL_RESULTS.json |
| 候选 patch | 真实 SRPM 基线、局部 GN 编译单元清理配置；应用检查及副本 hash 比对通过 | [候选](W2/provider-exception-cleanup.CANDIDATE.patch)、PATCH_IDENTITY.json |
| 修后验证 | **NOT_OBSERVED**：真实源码编译停在缺产品生成头，未取得重建 provider；不能宣称泄漏=0 或修后正常路径通过 | W2/raw/compile_source_*.stderr |
| 文档 | 已更新 P13 当前结论、错误契约、部署说明；状态索引及历史问题增加当前归类说明 | W3/DOCUMENT_CHANGES.json |

## 仍需什么

1. provider 修复：确认局部 GN 候选方向；提供该快照的产品 GN 参数、生成头及构建依赖闭包，重建真实库后执行已备好的失败归零与 12 场景脚本。候选草案的构建/运行有效性尚未确认。
2. adaptor 验收：生产方向真实 libc++ EWK、armv7l（NOT_OBSERVED_BOARD_OFFLINE）、并发/取消、真实回调、产品 linker/GN/RPM/升级回退。

## 自行判断与范围

沿用完全相同的析构夹具，不将显式 noexcept(false) 偷换为隐式 noexcept；将观察器扩展为固定表调用栈/对象字节定位；源码已有 RAII，故提出局部启用清理的构建层草案，而非增加重复 delete/catch。该方案方向已提问，尚未收到人工回复；仅形成候选副本，没有平台实施。

W2 **部分完成**：定位、可执行复现、候选草案完成；修后验证因真实构建前置缺失未完成。没有因为状态改为 PARTIAL 就把这些缺口写成通过。周报可用稿见 [资源遗留说明](W2/WEEKLY.md)。完成交接后停，交人工审阅。
