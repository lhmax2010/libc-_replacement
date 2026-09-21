# 输入状态：申报名单与实际安装严格区分

`installed-<arch>-inputs.json` 由准备脚本在事务开始前写出，实际含义是本轮拟安装的输入清单；**该文件存在不构成安装成功证据**。实际结果必须同时看 `prepare-<arch>-result.json`、准备事务命令输出和退出码。

输入集合与安装事务不是同一个计数：ARM 的 22 份、aarch64 的 12 份 LLVM RPM 全部复制保留并逐份校验；实际构建根分别安装其中 8 份（llvm、llvm-devel、llvm-static-devel、libllvm、clang、clang-devel、libomp、libomp-devel），加上既有的 11 份 runtime/BCC/其他依赖，共 19 份安装输入。没有把未安装的调试等 RPM 写成“已安装”。完整选单、源路径、原始 SHA256 均在两份 installed-*-inputs.json。

- armv7l（已解决输入接入，2026-09-21 22:55）：22 个 LLVM RPM 已复制、逐文件 SHA256 一致且摘要校验通过。第一次在旧私有根上的 `--test` 返回 19，211 条冲突来自新旧同名 `llvm-static-devel-22.1.8-1.armv7l`；该根未进行正式安装，现场保留。
- 随后只读核对得知原始根的 LLVM Release 为 115.1，而旧私有根为 1。只复制原始 RPM 数据库、文件树只读的预检返回 30，全部报根文件系统空间不足；**这次不是成功预检，也没有安装**。记录在 `original-arm-transaction-test.json`。
- 为真实准备输入，改从原始根做一份新的、当前用户所有的独立文件副本，复制范围 1,312,280,576 字节，21,242 条记录核验通过（普通文件逐项 SHA256、符号链接与类型/权限按记录核对）。该副本的**普通** `rpm -Uvh --ignorearch --oldpackage --replacepkgs --test` 与实际安装均退出 0；没有使用 `--replacefiles`、`--nodeps`、空间检查豁免或 sudo。证据：`origin-copy/copy-armv7l-result.json`、`origin-install-commands.json`、`prepare-armv7l-result.json`。
- 已安装新 ARM `libclang.a` 的完整 SHA256 为 `5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a`。构建 argv 只把四个私有输入绑定改为新副本，其他参数与 spec 不变（`origin-command-diff.json`）。此前关于 `--replacefiles` 的询问不再是继续执行的前提，实际未使用它；不据此归因旧根冲突或新旧归档差异。
- aarch64：12 个 LLVM RPM 已复制校验；事务预检与正式安装均退出 0，`prepare-aarch64-result.json` 为 PASS，私有环境 `libclang.a` 摘要为 `84aef989b7fad82ba205a99943430f37a1e33e84e8098857d197e8588b288f87`。
- 六格 payload 已在各自隔离环境中实际执行 `rpm --eval`：均为 `w5.xzdio`。aarch64 输入 LLVM RPM 本身仍为原 zstd 载荷，未重打包；输入 RPM 的压缩格式与本轮 bpftrace 输出压缩格式是两个对象。
