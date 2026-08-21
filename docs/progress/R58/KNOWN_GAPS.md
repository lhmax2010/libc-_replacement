# 已知未覆盖与残余边界

- aarch64 未验证。
- 混合 libc++/libstdc++ 栈帧穿越未验证。
- LTO 未验证。
- `dlopen` 和插件边界未验证。
- 超过 4 个工作线程及长时间压力未验证。
- armv7l 上未执行 libc++ iostream/future/string 源码测试同构集合。
- `basic_string::shrink_to_fit` 与 `basic_ostream<char>::sentry::~sentry()` 因 `noexcept` 不可达，记 `NOT_OBSERVED`。
- 标准库并发输出在全补丁与 libstdc++ 参考两侧均不稳定；本任务只保留事实，不定位该已登记边界。
- 层 C 条件编译改造为已登记待办。R56 已证明仅前置声明不成立；仓内未找到 R57 交付材料，记 `NOT_AVAILABLE`。
