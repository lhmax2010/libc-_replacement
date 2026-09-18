# 本轮临时候选 spec 副本

这些文件从 `tmp/WEEKEND_0918/specs/` 复制，仅用于审阅，不是平台源码树修改或包仓提交。

- `bpftrace.spec`：与先前保存的 `bpftrace-pathfixed.spec` SHA256 一致，见 raw/378；四个回退格审计结果中记录同一候选 SHA256。
- `bpftrace-undefined-harness.spec`：仅用于测试未定义 `_toolchain` 的入口，包含原样候选；不是拟提交的平台 spec。
- `libcxx-runtimes.spec`：本轮 ARM 运行库实际调用路径所指向的候选；本副本保存于该构建完成后，实际展开内容另见 runtime-arm-expanded.spec 与 raw/379，不冒称本次副本在启动时自动保存。
- `llvm.spec`：W2 尚未启动；在重试前保存。准备阶段要求它与已批准的旧临时 spec 字节相同。

复制命令与四份文件 SHA256 见本轮 raw 日志。这里不包含任何源码 tar 或 RPM 二进制。
