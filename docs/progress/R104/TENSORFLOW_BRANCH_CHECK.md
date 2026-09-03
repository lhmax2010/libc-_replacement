# tensorflow2 分支身份核查

## 结论摘要

1. `platform/upstream/tensorflow2` 当前**不存在** `refs/heads/tizen_base`。
2. 对 R104 所讨论的 `Tizen-Base-Toolchain` 包，当前可核实的源码基线是
   `1cdba73549f741720f11dd53da1bb516b03763a2`；该 SHA 位于
   `refs/heads/accepted/tizen_base_toolchain`，并被 Base Toolchain 的实际
   manifest 明确钉住。
3. R103 用来证明三个消费方“实际链入”的现存 Unified 产物，实际安装的是
   `tensorflow2-lite-devel-2.18.0-1`，其源码 revision 是另一棵树
   `ea3f134d35fc64667c2eebd72992bbb3c8e0069d`；该 SHA 位于
   `refs/heads/accepted/tizen_unified_toolchain`，也同时位于若干其他 Unified
   refs。它不是 Base Toolchain 的 `1cdba735…`。
4. 因而，“平台实际使用哪个分支”必须按项目区分：Base Toolchain 使用
   `1cdba735…`；R103 的现存 Unified 消费产物使用 `ea3f134d…`。

若 R104 的范围保持为任务书所述的 **Base 层 / Tizen-Base-Toolchain**，则
建议在 `tensorflow2` 仓以 `1cdba735…` 为基线创建
`sandbox/lhmax2025/libcxx-migration`。这不是因为 `accepted/*` 是开发主线，
而是因为该仓没有 `tizen_base`，且实际 Base Toolchain manifest 正在使用
这个 accepted revision。

若人工意图是直接改变 R103 那批 Unified 消费方下一次实际取得的
TensorFlow，则还需要处理/验证 `ea3f134d…` 这条 Unified 基线；不能把
Base 的 `1cdba735…` 无依据地当成相同源码。R103 只证明两版归档对现存消费
ELF 的命中符号集合相同，并明确记录两版归档 SHA256 不同。

在人工确认上述范围区分之前，本轮没有在 `tensorflow2` 仓创建本地或远端
sandbox 分支。

## 1. 是否存在 tizen_base

不存在。两份独立记录一致：

- R103 的 `git clone --branch tizen_base` 返回
  `fatal: Remote branch tizen_base not found in upstream origin`；
- R104 捕获的 49 条当前远端 heads 中没有 `refs/heads/tizen_base`，但有
  `refs/heads/accepted/tizen_base_toolchain`，其值为 `1cdba735…`。

因此无法给出 `tizen_base` 的 HEAD 或最近三条提交；应记为
`NOT_AVAILABLE (REMOTE_BRANCH_ABSENT)`，不是未执行查询。

## 2. 两套 Toolchain 项目的实际源码身份

### Base Toolchain

`codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/manifest.xml`：

```xml
<default revision="tizen_base" remote="tizen" sync-j="8"/>
...
<project name="platform/upstream/tensorflow2"
         path="platform/upstream/tensorflow2"
         revision="1cdba73549f741720f11dd53da1bb516b03763a2"/>
```

同目录 `project_meta` 将项目候选分支定义为 `tizen_base`；Gerrit 实际可见的
验收结果 ref 是 `accepted/tizen_base_toolchain`，指向同一 SHA。快照
`tizen-base-toolchain_20260828.101647` 中的包版本为
`tensorflow2-2.18.0-1.1`。

该 SHA 最近三条提交为：

1. `1cdba73549f741720f11dd53da1bb516b03763a2 fix: Add include directories and EIGEN_NEON_GEBP_NR=4 for XLA AOT runtime`
2. `a5e093af9470d15b3673dc7650cb09ca636a7f35 fix: install AOT include/ to %{_datadir} for LLVM TENSORFLOW_AOT_PATH`
3. `36fb24a9d288f515c53eacb10f2e23f8d3489d78 Add XLA AOT compilation support for LLVM MLGO`

### Unified Toolchain 与 R103 的现存消费产物

`codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/manifest.xml` 钉住：

```xml
<project name="platform/upstream/tensorflow2"
         path="platform/upstream/tensorflow2"
         revision="ea3f134d35fc64667c2eebd72992bbb3c8e0069d"/>
```

R103 保存的 `inference-engine-tflite`、`nnstreamer`、`nntrainer` 三架构实际
成功构建日志均显示，构建根安装的是
`tensorflow2-lite-devel-2.18.0-1`。R103 据此将这批现存消费 ELF 的实际
提供方严格限定为 Unified `2.18.0-1` / `ea3f134d…`。

Gerrit refs 捕获时，`accepted/tizen_unified_toolchain` 指向
`ea3f134d…`；`accepted/tizen_unified`、`tizen` 等多个 ref 也指向相同
SHA。因此从 manifest 可以严格证明**源码 revision**，并可证明
`accepted/tizen_unified_toolchain` 承载该 revision；manifest 本身按 SHA
固定内容，不单独记录“构建器最终以哪个 ref 名解析”的运行日志。

## 3. R103 结论为何仍能支撑 Base 包判定

R103 没有把两版包说成字节相同。它对 Base `2.18.0-1.1` 和 Unified
`2.18.0-1` 的 `libtensorflow2-lite.a` 分别做精确符号交集检查：20/20 个
消费 ELF 分组的命中集合均为 `base_only=0`、`unified_only=0`。这说明 Base
候选提供了同一批被消费的 C++ 符号，从而足以支持包级 `NEED_LIBCXX`
判定；但它不等于证明 Base `1.1` 的具体字节进入了那些既有 Unified ELF。

## 4. 尚需人工确认的范围问题

- 若本阶段只为 `Tizen-Base-Toolchain` 的 11 包建迁移分支：采用
  `1cdba735…` 是与当前项目 manifest 一致的基线。
- 若还要求 Unified 项目重建 `tensorflow2` 本身并让三个消费方使用迁移后
  静态归档：需另行明确是否还要以 `ea3f134d…` 为基线准备对应改动。

本文件不自行扩大 R104 的分支创建范围。

