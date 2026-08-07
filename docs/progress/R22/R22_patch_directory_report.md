# R22 第三部分：补丁目录建立

已建立 `docs/patches/INDEX.tsv`、`docs/patches/libcynara-commons/{README.md,patch/}` 和 `docs/patches/security-manager/{README.md,patch/}`。两项状态均为“待开发”；目标仓由本地代码仓 remote 原文确认。没有编写补丁代码，也没有修改任何源码。

两份 README 均按固定模板填写。真实 armv7l libc++ 编译诊断、触发配置、源码上下文和 44/44 条传播记录均来自既有 T2/bak 证据。传播分布是：libcynara-commons 公开头 14 条（本包 13、libcynara-creds-dbus 1），security-manager 内部头 30 条（全部本包）。

缺项全部显式登记在 `patches/missing_fields.tsv`：具体修复方案、修改后双标准库构建兼容性、线程取消运行时验证。因为补丁尚未开发，这些字段均为 NOT_AVAILABLE，而不是推断补全。
