# 补充数据包的核查

在固定运行库集合之外，另从同一快照取得四个 x86_64 RPM：manifest-parser-examples、manifest-parser-tests、cert-svc-test、cert-svc-test-binaries。URL、包版本和 SHA256 在 [清单](EXTRA_DATA_RPMS.json)，下载/校验/解包记录在 [staging/test-data.json](staging/test-data.json)。只解到 `tmp/EDGE_FIXTURE_0927/test-data/`；未运行测试包二进制或安装脚本，未改主运行树和 RPMS.json。

- Delta：已枚举两个 manifest 包内的文件，看到普通应用清单、语言配置、有效/无效应用签名样本；未识别可与 DeltaInfo 的 added/modified/removed 列表对应的有效 Delta 文档。此处不是“平台不存在样本”的全局零命中结论。错误路径夹具五轮真实调用已留存，但缺成功输入的正向对照，仍不关闭。
- cert-svc：已取得 `apps/wgt/`、`unit_test_data/wgt_valid/` 等真实签名样本及相关证书材料，不再称“缺签名样本”。尚未建立可独立断言的信任环境。提供库字符串中有 `/usr/share/cert-svc/schema.xsd`、`/usr/share/ca-certificates/tizen`、`/usr/share/ca-certificates/fingerprint/fingerprint_list.xml`、`/usr/lib64/libcert-svc-validator-plugin.so` 等绝对路径，实际本机这些路径均不存在。字符串是静态线索，不是动态调用链证明。已读安装的 SignatureValidator/SignatureFinder/SignatureData/Certificate 公共接口，未确认可用的根目录重定向方式；不把输出结构的 setStorageType 当作全局信任库配置。没有通过改宿主路径、模拟验证器或空 URI 列表补结果。

原始命令、退出码及完整输出可按 raw 中标签 `testdata_inventory`、`extra_data_scan`、`cert_public_contract`、`cert_public_headers`、`cert_trust_contract`、`cert_runtime_paths`、`cert_host_prerequisites` 定位。限定本地源码路径检查未找到 cert-svc/manifest-parser；它不证明其他仓库没有源码。

重启条件：Delta 的有来源有效文档及其预期列表；cert-svc 的可隔离信任环境、插件/配置与确定的有效/无效签名判定契约。当前均为 NOT_AVAILABLE，不算 GNU 成功基线。
