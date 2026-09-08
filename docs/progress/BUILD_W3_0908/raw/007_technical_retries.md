# 技术性重试

一次调用校验脚本时，编排层输入误含 `olders?`，在 shell 命令执行前由
JavaScript 解析器报 `SyntaxError: Unexpected token 'const'`；没有运行任何
任务命令、没有产生文件改动。删除误输入后，以 `raw/006` 记录的同一校验
命令重跑，退出码为 0。该错误属于命令包装技术问题，不是判据失败。
