# tensorflow2 sandbox 分支创建结果

状态：`CREATED_AND_VERIFIED`

## 创建结果

- 仓库：`ssh://lhmax2025@review.tizen.org:29418/platform/upstream/tensorflow2`
- 基线 ref：`refs/heads/accepted/tizen_base_toolchain`
- 基线 SHA：`1cdba73549f741720f11dd53da1bb516b03763a2`
- 新分支：`refs/heads/sandbox/lhmax2025/libcxx-migration`
- 新分支 SHA：`1cdba73549f741720f11dd53da1bb516b03763a2`
- 推送方式：普通 `git push`；未使用 force、未触碰正式分支。

创建前远端有 49 个 heads，创建后有 50 个。逐项去掉新分支后，创建前后
远端 heads 完全一致；本地分支也只有该新分支这一项新增。工作树仍停在
`accepted/tizen_base_toolchain`，HEAD 未变，工作树干净。

至此 11 个包的同名 sandbox 分支均已存在；再次逐仓读取远端 ref，11/11
均指向已批准的各自基线。

## Unified 那棵的单独登记

`accepted/tizen_unified_toolchain` 当前指向
`ea3f134d35fc64667c2eebd72992bbb3c8e0069d`。它属于
`Tizen-Unified-Toolchain` 项目，不属于本轮以
`Tizen-Base-Toolchain` manifest 为边界的实施范围。

R103 的实际构建日志证明，`inference-engine-tflite`、`nnstreamer`、
`nntrainer` 三个消费方使用的是该 Unified revision 产出的
`tensorflow2-lite-devel-2.18.0-1`，并将 `libtensorflow2-lite.a` 的 C++
成员实际链入最终 ELF。因此该 revision 同样需要迁移；本轮未处理是项目范围
不同，不是遗漏。后续应在 Unified 包集迁移环节，以 `ea3f134d…` 为基线建立
对应分支、应用经本轮验证的等价 spec 改动，并重建上述三个消费方。

Base `1cdba735…` 与 Unified `ea3f134d…` 的归档并非逐字节相同；R103 证明
的是它们对既有消费 ELF 的命中符号集合 20/20 组一致，不能据此省略
Unified revision 的后续实施与验证。

## 证据位置

- `tables/tensorflow_branch_result.tsv`
- `branch_ops/tensorflow2/remote_before.heads`
- `branch_ops/tensorflow2/remote_after.heads`
- `branch_ops/tensorflow2/local_before.tsv`
- `branch_ops/tensorflow2/local_after.tsv`
- `raw/048_create_tensorflow_sandbox_branch.*`
- `raw/065_summarize_tensorflow_branch_evidence.*`
- `raw/067_verify_all_eleven_target_heads.*`

