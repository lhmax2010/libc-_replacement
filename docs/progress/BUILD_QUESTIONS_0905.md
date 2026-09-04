# 编译适配线待确认问题（2026-09-05）

## W1：MLGO / XLA AOT 资产

1. **libc++ 生成机制缺失**：`mlgo_pack.py`/`fetch_sysroot.py` 明确使用 libstdc++ 头文件，AOT spec 固定 GCC，且没有 `-stdlib=libc++` 或等价开关。请确认应使用哪个 revision、补丁或另一个工具版本来生成 `std::__1` 资产。
2. **交叉支持材料缺失**：指南要求 spec 带 `--define=with_cross_compiler_support=true` 并引用 `../mlgo-tf-aot-cross.diff`；实际 spec 未传该 define，引用的 diff 也不存在。该文件是否漏交？若不是，交叉支持应如何启用？
3. **sysroot 缓存缺失**：指南称随目录提供约 75 MiB 的 `build/sysroot-cache/`，实际没有且 `/build/` 被忽略。应补齐固定缓存，还是允许从脚本中的 Tizen `reference` URL在线下载？
4. **LLVM 消费接口不一致**：指南产出 9 个 `mlgo_{runtime,inliner,regalloc}_<arch>.tar.gz` 并称对应 Source1002–1010；当前 `codes/R104/llvm/packaging/llvm.spec` 仍只消费 3 个 `mlgo_{arm,aarch,x86}_model.tar.gz`。请确认目标 LLVM revision/spec 以及 9 包到 3 包的导入方式。
5. **验证配置与资源纪律**：`verify-gbs.sh` 固定使用相邻 `../llvm`、`../gbs.conf`；当前相邻 `gbs.conf` 不存在，且脚本会选择 5/6 并行、`nice -n 10`、无 ionice，与本任务 job=2、`nice -n 15`、`ionice -c 3` 冲突。请提供应使用的 `gbs.conf`，并确认允许采用何种方式满足资源上限。

## 人工答复后的当前问题

前述第 1、3、5 项已获人工授权处理；第 2 项续查发现 `.bazelrc` 有等价 `cpu_cross` 配置，但 AOT spec 未消费；第 4 项被进一步定位为当前阻断的核心。

1. **inliner 与 runtime 的范围矛盾**：模型对象不含 `std::__cxx11`/`std::__1`，错误 ABI 符号全部来自 XLA runtime。若仍限定只生成 inliner，无法解除 LLVM 链接阻断。需确认是否允许同步重生 runtime。
2. **默认输出的等价口径**：manifest 时间戳和 tar 元数据使字节级复现不稳定。需确认要求逐字节一致，还是有效载荷、接口与 ABI 一致。
3. **开发流程确认**：AOT spec 应如何正式启用已有 `cpu_cross`；9 包到 3 个合并包的正式消费接口；指南是否应记录在线 sysroot 的固定快照/校验；验证脚本是否应暴露资源参数。
4. **W3 日志缺失**：要把 17 包候选归类收口为确切原因，需要总部本轮 449/1034 构建的逐包完整日志及任务身份元数据。

W1 因第 1 项停止，资产生成未启动；W2 记 `NOT_AVAILABLE`；W3 已完成探针与候选归类，结论为 `PARTIAL`。
