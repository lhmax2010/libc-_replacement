# 输入状态：申报名单与实际安装严格区分

`installed-<arch>-inputs.json` 由准备脚本在事务开始前写出，实际含义是本轮拟安装的输入清单；**该文件存在不构成安装成功证据**。实际结果必须同时看 `prepare-<arch>-result.json`、准备事务命令输出和退出码。

- armv7l：22 个 LLVM RPM 已复制、逐文件 SHA256 一致且摘要校验通过。安装 `--test` 返回 19；211 条冲突均来自新旧同名 `llvm-static-devel-22.1.8-1.armv7l`。没有运行后续正式安装，因此不能声称新 ARM 输入已被消费。等待人工确认仅在私有副本替换同包文件。
- aarch64：12 个 LLVM RPM 已复制校验；事务预检与正式安装均退出 0，`prepare-aarch64-result.json` 为 PASS，私有环境 `libclang.a` 摘要为 `84aef989b7fad82ba205a99943430f37a1e33e84e8098857d197e8588b288f87`。
- 六格 payload 已在各自隔离环境中实际执行 `rpm --eval`：均为 `w5.xzdio`。aarch64 输入 LLVM RPM 本身仍为原 zstd 载荷，未重打包；输入 RPM 的压缩格式与本轮 bpftrace 输出压缩格式是两个对象。
