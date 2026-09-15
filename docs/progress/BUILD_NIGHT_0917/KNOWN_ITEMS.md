# 单列事项登记（不改既有发布判定）

| 项目 | 已知事实 | 当前未闭合部分 |
| --- | --- | --- |
| tensorflow2 / XNNPACK | 沿用 ../R108/ 与 ../BUILD_NIGHT_0916/KNOWN_ITEMS.md：armv7l GCC 原基线与候选同点同因失败，按人工批准的相对基线判据交付 | 不是这一格完整构建通过；原汇编检测问题仍单列 |
| LLVM / MLGO XLA | 沿用 ../BUILD_W1B_0909/：TF 2.18 双标准库资产及消费适配已完成，LLVM f203923a… 已推；不写成仍阻断当前 LLVM | 保留生成机制、版本、ABI 追踪；本夜未重做 MLGO 验证 |
| 已发布 bpftrace-static | Source1002 tar SHA256 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123；原两程序版本 0.24.2，含 Clang/LLVM 21.1.1 字串；来源未确认，内嵌标准库 NOT_OBSERVED | 不因本夜找到旧 libclang 分支就声称完整程序来源已确认；原资产未替换 |
| 旧 libclang.a.21.1.1 | bpftrace 引入提交 84440faa…，GCC 14.2.0 注记与 libstdc++ ABI 均直接确认；匹配的 LLVM 静态配方候选为 46df8cd… | 配方到归档 SHA256 的生成批次链仍缺；不匹配当前 22.1.8 + libc++，本夜没有使用它 |
| 本夜新 bpftrace-static 候选 | x86_64、aarch64 均源码构建、ABI/系统依赖核查通过；GCC/未定义路径实跑与原资产安装字节对照通过 | 功能等价、完整 RPM、空环境复建、新静态展开器行为未闭合；不推包仓 |

旧 libclang 两个精确对象：

- AArch64：`cb5958fa3c575430a9e033d9d24dc49311836693f6f9f295c93b5a18963fa9b8`。
- ARM：`bceccaf5ef0d5fd44478b93888dd5630f9389408474651159d114a2fccb317d5`。
- tar：`0ac7bac842a0cbfa64a8227bd554a0a9c3d85d54d3795a44791603917359264a`。
- 引入提交：`84440faa935cc2650bf84e3204550016d7d21db8`。
- 消费分支：`sandbox/hoegeun/static-build @ 624f68d5f67a673083e5ad08ea205742613d6e53`。
- 生产配方候选：`sandbox/hoegeun/llvm-static @ 46df8cd0d7cb564af6e1e482f3c48d04dba3079b`。

原完整程序的镜像结论仅沿用固定
Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247 的六份清单，
不含 bpftrace-static；不排除测试加装、其它镜像或后续快照。
`%post static` 仍将 `/usr/bin/bpftrace-static` 复制覆盖 `/usr/bin/bpftrace`，
所以安装 static 后不能拿普通主包的验证替代该程序的验证。

上述登记不表示 Clang driver 只能用某套标准库；这里区分编译器工具自身携带
的 C++ ABI 与它为用户代码选择的标准库。生产者 GCC 身份来自对象注记，
而非单凭 std::__cxx11。完整证据见 W4_REPORT.md 与 old_libclang_result.json。
