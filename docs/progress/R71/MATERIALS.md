# R71 材料说明

## 基线身份

- llvm-project：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`
- GCC：`49c5060508ce8a6ed6beca375233688a6bdd9140`
- libstdc++ 取消兼容历史提交：`9e18a25331fa25c3907249fede65a02c6817b06e`

## 测试与观测代码

- `code/source/r69_new_cases.cpp`：逐字复制自 `progress/R69/src/r69_new_cases.cpp`，SHA256 为 `1c4f1966c1410f1617924d6a0dc4d7b8e13208e678f0ab7f5ac7200d9b174ade`。
- `code/gdb_waiter_trace.gdb`：本任务新增的只读 GDB 观测脚本；不修改 inferior 的内存或控制流，只设置断点、打印线程栈并继续。

## 编译、运行和身份材料

- `code/commands/compile_libcxx_baseline.log`、`compile_libstdcxx_reference.log`：原始逐选项编译链接命令与退出码，复制自 R69 的已归档构建记录。
- `code/commands/017_runtime_identity.log`：二进制 SHA256 与 `ldd` 原文。
- `code/commands/019_direct_runtime.log`：脱离调试器的两侧完整输出与退出码。
- `code/raw/x86_64/gdb_*.log`：GDB 完整原始输出。

## 静态与上游材料

- `code/tables/call_path_comparison.tsv`：逐层调用路径、异常规格、catch/清理构造对照。
- `code/tables/source_identity.tsv`：本次引用的 libc++/libstdc++ 文件、源码仓 commit 与 SHA256。
- `code/static/`：本任务读取源码、精确历史提交和检索口径的原始命令输出。
- `code/research/upstream_search.tsv`：LLVM/GCC/标准与设计文档的检索范围、命中状态和 URL。

`INDEX.tsv` 覆盖上述上传材料；索引自身不递归登记自身 SHA256。
