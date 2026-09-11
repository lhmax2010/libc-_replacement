# 范围说明的辅助公开资料

这些页面只用于说明公开文档如何区分 Native/.NET/Web 与 NUI/DALi，不参与冻结 RPM 或 API 数量计算。
本地 SDK 配方和已发布头文件优先；在线文档版本不等于本次仓快照对应的产品支持合同。

| 用途 | 可点击的原始出处 | 本地记录 |
|---|---|---|
| NUI 为 C# 层、下层 DALi 为 C++，说明需要分层盘点 | [Samsung tizen-docs：NUI overview](https://github.com/Samsung/tizen-docs/blob/master/docs/application/dotnet/guides/user-interface/nui/overview.md) | raw/015_official_nui.stdout；实际取回的 Markdown，不依赖搜索摘要 |
| Native 应用文档的范围入口 | [Samsung tizen-docs：Native application overview](https://github.com/Samsung/tizen-docs/blob/master/docs/application/native/index.md) | raw/016_official_native.stdout |

文档原站的 NUI overview 和 Web guides 路径在本次浏览时跳转到新版文档首页，
不能把首页 HTTP 可达写成原章节仍完整可用，因此使用官方文档仓的具体文件作为引用。
上述文件的原字节与 SHA256 随整体证据封存；本阶段没有据此核查实际 P/Invoke/绑定签名。
