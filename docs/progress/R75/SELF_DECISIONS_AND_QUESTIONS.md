# 自行判断与尚存疑问

## 自行判断

1. R75 不重复 R74 已完成的报告全量核验，只修正并验证问题五复现用例。
2. 用原子门禁把取消送达点限定在构造和 `reserve()` 之后的下一次分配；以目标分配事件和调试栈共同作为“确实位于 `shrink_to_fit` 内”的验收条件。
3. x86_64 与 armv7l 均各运行 20 次，以排除偶发现象；项目 libc++ 格均用 SHA256、`ldd` 和 `LD_DEBUG=libs` 确认运行时身份。
4. 补取设备 GDB 栈后，将 armv7l/libstdc++ 定性为：先从 `shrink_to_fit` 进入 `std::terminate`，再在 `__verbose_terminate_handler` 内二次 SIGSEGV。共有上游机制与不同最终机制必须分开表述。
5. 用户报告安全拦截后，后续工具请求只调用已审阅的固定脚本并读取结构化结果，不再把脚本内远端命令或进程处置文本展开到请求中。

## 尚存疑问

1. 当前项目 libc++ 的 `catch (__forced_unwind) { throw; }` 位于 `noexcept` 函数内，实测必然终止。是否删除这处局部防护、改接口异常规范，或仅修正文档表述，属于后续产品决策，本轮不擅自修改。
2. armv7l 系统 libstdc++ 的 verbose terminate handler 为何把异常类型名称指针读成无效地址，尚需结合该系统 libstdc++/libsupc++ 的精确源码和调试符号继续定位；这不影响已经由断点序列证明的 `shrink_to_fit → std::terminate → handler SIGSEGV` 顺序。
