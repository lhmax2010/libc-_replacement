# 编译适配线待确认问题（2026-09-05）

## W1：MLGO / XLA AOT 资产

1. **libc++ 生成机制缺失**：`mlgo_pack.py`/`fetch_sysroot.py` 明确使用 libstdc++ 头文件，AOT spec 固定 GCC，且没有 `-stdlib=libc++` 或等价开关。请确认应使用哪个 revision、补丁或另一个工具版本来生成 `std::__1` 资产。
2. **交叉支持材料缺失**：指南要求 spec 带 `--define=with_cross_compiler_support=true` 并引用 `../mlgo-tf-aot-cross.diff`；实际 spec 未传该 define，引用的 diff 也不存在。该文件是否漏交？若不是，交叉支持应如何启用？
3. **sysroot 缓存缺失**：指南称随目录提供约 75 MiB 的 `build/sysroot-cache/`，实际没有且 `/build/` 被忽略。应补齐固定缓存，还是允许从脚本中的 Tizen `reference` URL在线下载？
4. **LLVM 消费接口不一致**：指南产出 9 个 `mlgo_{runtime,inliner,regalloc}_<arch>.tar.gz` 并称对应 Source1002–1010；当前 `codes/R104/llvm/packaging/llvm.spec` 仍只消费 3 个 `mlgo_{arm,aarch,x86}_model.tar.gz`。请确认目标 LLVM revision/spec 以及 9 包到 3 包的导入方式。
5. **验证配置与资源纪律**：`verify-gbs.sh` 固定使用相邻 `../llvm`、`../gbs.conf`；当前相邻 `gbs.conf` 不存在，且脚本会选择 5/6 并行、`nice -n 10`、无 ionice，与本任务 job=2、`nice -n 15`、`ionice -c 3` 冲突。请提供应使用的 `gbs.conf`，并确认允许采用何种方式满足资源上限。

在以上问题确认前，W1 资产生成未启动；W2 因依赖 W1 未启动。
