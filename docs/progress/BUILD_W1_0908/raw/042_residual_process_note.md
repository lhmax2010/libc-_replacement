# 中止后的进程核验

顶层 GBS 进程和当前用户拥有的 Ninja/QEMU 进程先停止，编译负载归零。
构建工具通过 sudo 启动的三个 root 所有包装/日志进程曾短暂处于休眠且 CPU
为 0.0%：`build` PID 1760311、`perl` PID 1760342、`qemu-aarch64` PID
1762189。普通用户 `kill` 返回 `Operation not permitted`，非交互 sudo 也因
无凭据而失败，故没有扩大权限或做破坏性处理。稍后再次核验时三个 PID 均已
自行退出，最终没有遗留构建进程。
