# 首次 MLGO 运行观测

默认启发式退出码 0；release 模式已产生多轮 `[MLInlineAdvisor]` 输出，
随后附加的 `print<inline-advisor>` pass 在访问已删除函数时段错误，release
子进程退出码 139，脚本内部退出码 11。栈顶为 `llvm::Value::getName()`，
调用点 `MLInlineAdvisor.cpp:566`。完整 stderr 位于
`runtime_checks/x86_64/print_pass_attempt/release.stderr`。

该故障属于观测 pass，不是 AOT runner 的 `Run()`；改用 LLVM 原生的
`inline-ml` YAML optimization remarks 后独立探针退出码 0，产生 6 条
带 `ShouldInline` 的记录。正式脚本据此更新并重跑。
