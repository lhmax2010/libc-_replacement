# 疑问与执行边界

1. 开发板 SDB 当前为 owner 用户，无法列出 `/opt/usr/share/crash/dump`（Permission denied，远端退出 2；不是连接中断）。已询问是否可临时切换 SDB root 并恢复；答复前不运行会产生崩溃的 ARM EWK 格。普通测试及 Boost 同侧测试已清理上传件。
2. libc++ 构建的真实 EWK 仍未取得，GNU 调用方 → libc++ EWK 记 `NOT_AVAILABLE`，不能以模拟实现补格。
3. private 成员及位域不适合以可移植 `offsetof` 探针直接访问。本轮使用实际目标编译器布局输出，另实测 sizeof/alignof/内联数据地址；明确区分编译器静态结果与运行结果。
4. 真实包公开接口含标准库类型，并不保证可以无初始化调用。本轮不把构造的 provider 当真实包，不把已验证数据层当真实回调链。
5. 资源约束按单个构建驱动顺序执行。EWK native 最后两次运行尚未结束时启动 ARM EWK 编译驱动，有短暂两进程重叠；这是执行偏差，后续驱动等前一驱动退出再启动。
