# R18 实现逐项说明

## 实现位置与提交

- 仓：`codes/llvm`
- 分支：`tizen_base`（未新建分支）
- 基线提交：`3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`
- 本地实现提交：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`
- 提交 Change-Id：`I8f5edc6ae952097166d2c3cdf1bbe8210fb6535b`
- Gerrit/sandbox 状态：未推送；远端既有 sandbox 仍为 `3d3e3da…`

## 新增文件

| 新增文件 | SHA256 | 作用 | gcc 对应事实 |
|---|---|---|---|
| `packaging/libcxx-runtimes.spec` | `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74` | 独立的 libc++/libc++abi 源码包打包描述 | 与 gcc 的多个完整 spec 并存形态对应，但不是宏前缀派生 |
| `packaging/libcxx-llvm22.map` | `e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d` | LLVM_22 导出符号版本脚本 | 对应 gcc 将包专属源码输入与 spec 同置 packaging 的组织事实 |
| `packaging/libcxx-runtimes.manifest` | `db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3` | libcxx-runtimes 安全 manifest | 对应 `gcc.manifest` 的同目录组织方式 |
| `packaging/SOURCE_PROVENANCE` | `1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37` | R9-C 三架构构建输入身份及六个导出 patch 锚 | gcc 没有同名输入；仅复用其辅助输入同目录组织方式 |

四个 SHA256 均与 R9-C seg2 已验证输入逐字一致，证据见 `commands/07_r9c_input_identity.txt` 与 `commands/09_llvm_implementation_identity.txt`。

## 未新增和未修改项

- 未新增生成脚本：两份 spec 内容独立，不存在 gcc 的机械前缀变换。
- 未新增 `_multibuild`、`_service` 或 XML：gcc packaging 内未见对应物。
- `packaging/llvm.spec` 未修改：提交前后 SHA256 均为 `7962d176484920de6138d70c7e8a8e1764dfada6655abf0332e9748167e889fd`；`HEAD^..HEAD` 对该文件 diff 退出 0。
- 未修改 gcc 源码树。
- 未向 `tizen_base`、sandbox 或任何 Gerrit 分支推送。
