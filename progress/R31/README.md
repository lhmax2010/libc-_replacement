# R31 evidence bundle

主报告：`R31_report.md`。

- `commands/`：命令原文、输出与退出码；失败尝试也保留。
- `inputs/`：R30 初始用例和 R31 构建 spec。
- `src/`：R31 受控实验完整源码。
- `results/matrix/`：逐格原生运行记录。
- `tables/`：执行方式、矩阵、回溯与 personality 观测汇总。
- `tools/`：运行矩阵和 GDB personality 观测器。

本任务没有修改目标源码、候选补丁或冻结配置；仅构造诊断实验件。

