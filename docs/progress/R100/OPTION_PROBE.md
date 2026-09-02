# 全局选项驱动器探针说明

完整原始输出位于 `raw/040_probe_global_flags_all_drivers_retry.*`，摘要位于
`tables/driver_option_probe.tsv`。

判读时必须区分三种状态：

- `REJECTED`：驱动器明确非零并报不识别选项；
- `ACCEPTED_SILENT`：没有诊断，但不代表“忽略”。Clang++ 的 `-###`
  已显示选择 `-lc++`，所以这里是静默接受且生效；
- `ACCEPTED_WITH_DIAGNOSTIC`：Clang C 编译明确警告 `-stdlib` 未用于 C
  编译，链接 trace 仍显示链接参数被转交。

aarch64 GCC 的 139 在 `--version` 即出现，属于复用 buildroot 的运行故障，
不是对选项的判定证据，已标成 `NOT_OBSERVED`。
