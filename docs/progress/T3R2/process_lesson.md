# 构建调用上下文契约追加

构建类任务书必须同时申报执行工作目录（`pwd`）和全部路径参数（至少包括输入位置、`--path`/位置参数、`-B`、`--style` 与 `--spec`）。只回链命令行不足以唯一确定 GBS/depanneur 的扫描起点；workspace 布局变化时，相同位置参数可能因启动工作目录命中仓库元数据而改为全仓扫描。

本次固定做法：调用记录首行写入 `WORKING_DIRECTORY=<pwd>`，随后逐字写入完整命令；构建日志同时保留 depanneur 的实际 `--path`、`start building packages from:` 与单包依赖排序结果。
