# Unified 全局注入选项探针

## 输入身份

本轮实际 QuickBuild 日志中的环境值为：

```text
CXXFLAGS += -stdlib=libc++
LDFLAGS  += -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed
```

工作区 `codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config` 是未应用该
变更的原件（SHA256
`a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086`），
所以本轮不把它冒充 QuickBuild 的实际配置。上述注入值由 QuickBuild 日志
的 `export CXXFLAGS` / `export LDFLAGS` 行直接确认，并与已经验证过的
`docs/progress/R84/unified_project_config.patch` 一致。

## 方法

对三架构的 GCC 14.2.0 与 Clang 22.1.8 C++ driver 分别使用 `-###`：

- 单独试 `-stdlib=libc++`；
- 单独试 `-Wl,--no-as-needed`、`-lc++abi`、`-Wl,--as-needed`；
- 最后合并全部链接选项。

`-###` 不执行编译与链接，但能区分 driver 直接拒绝、接受并传给 linker、
以及 Clang 是否选择 `-lc++`。完整命令与输出在 `raw/009_*`，结构化结果在
`tables/trace_option_probe.tsv`。

## 结果

| 架构 | driver | `-stdlib=libc++` | 三个其余链接选项（逐项） | 全组选项 |
| --- | --- | --- | --- | --- |
| armv7l | GCC 14.2.0 | 明确拒绝 | 均接受并转交 linker | 在 `-stdlib=libc++` 处拒绝 |
| armv7l | Clang 22.1.8 | 接受并选择 `-lc++` | 均接受并转交 linker | 全部接受并转交 |
| aarch64 | GCC | `NOT_OBSERVED` | `NOT_OBSERVED` | `NOT_OBSERVED` |
| aarch64 | Clang 22.1.8 | 接受并选择 `-lc++` | 均接受并转交 linker | 全部接受并转交 |
| x86_64 | GCC 14.2.0 | 明确拒绝 | 均接受并转交 linker | 在 `-stdlib=libc++` 处拒绝 |
| x86_64 | Clang 22.1.8 | 接受并选择 `-lc++` | 均接受并转交 linker | 全部接受并转交 |

aarch64 GCC 的复用 buildroot 中，driver 连 `--version` 都以 `139` 退出，故
不能把本地非零解释为拒绝选项。独立的 QuickBuild aarch64 日志则直接证明：
`gfortran` 或目标 `gcc` 在实际包命令中明确报告不认识
`-stdlib=libc++`。这补足了该选项在实际 aarch64 环境的证据，但不能补足
三个其余链接选项的“逐项”本地探针，后三项保持 `NOT_OBSERVED`。

## “静默忽略”核对

在已完成的 C++ driver 探针中，**没有发现静默忽略**：

- GCC 对 `-stdlib=libc++` 明确报错；
- Clang++ 不打印诊断但 `-###` 明确改为 `-lc++`，属于静默接受且生效，
  不是忽略；
- `-Wl,--no-as-needed`、`-lc++abi`、`-Wl,--as-needed` 均在 linker argv
  中出现，属于转交，不是忽略。

这里证明的是 driver 行为，不是每个软件包完整构建后的行为。
