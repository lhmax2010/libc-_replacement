# W2 M2 armv7l 三格勘误

- 勘误日期：2026-08-25
- 涉及原报告：`docs/progress/W2/REPORT.md`
- 涉及格：M2 主窗口、`wait_for`、async 多等待者

## 原记述

W2 将上述三格记为“未取得目标语义”：M2 主窗口与 `wait_for` 在实验版 libc++ 和同板 libstdc++ 参考侧均超时，async 多等待者在两侧均为 0/20。原报告仅陈述 ARM 与 x86_64 的观测差异，没有将其归为产品补丁特有回归。

## 勘误

上述失败由探针夹具造成，不是 M2 产品实现缺陷。原探针采用异步取消，并让 `WindowResult` 析构函数停在不含取消点的无限忙等中，要求 ARM EHABI 从任意 PC 正确遍历到 `std::async` 的共享状态处理帧。

R64 将夹具改为延迟取消并在同一“值已发布、执行未返回”窗口调用 `pthread_testcancel()`。主窗口、`wait_for`、多等待者在 armv7l 与 x86_64、实验版 libc++ 与 libstdc++ 参考侧均为 20/20 通过，且 x86_64 结果与原有 W1 结论一致。

R65 的调试器补证进一步显示：

- 取消前，原探针工作线程确实停在 `WindowResult::~WindowResult()` 的忙等循环；
- `pthread_cancel` 返回 0；异步信号进入 `__pthread_unwind`，并调用 `_Unwind_ForcedUnwind`；
- 原探针的 forced unwind 期间没有命中 libc++abi 的 `__gxx_personality_v0`，所以没有进入 `__async_assoc_state::__execute()` 的 forced-unwind handler，也没有调用共享状态完成接口；工作线程退出后，主线程仍阻塞在 future 条件变量；
- 延迟取消对照命中 personality、进入 `future` 的 forced-unwind handler、执行共享状态完成接口并重抛，等待方随后取得 `broken_promise`。

因此，“展开根本没有开始”不是准确描述；准确观测是：原夹具中展开已经启动，但未正常遍历到 `async` 的共享状态捕获/完成路径。

M2 的产品实现未作任何改动。

## 证据

- R64：`docs/progress/R64/REPORT.md`，以及其 `tables/arm_deferred_matrix.tsv`、`tables/x86_deferred_matrix.tsv`。
- R65：`docs/progress/R65/raw/008_trace_original_m2_v2.stdout.txt` 与 `docs/progress/R65/raw/009_trace_deferred_m2.stdout.txt`。
