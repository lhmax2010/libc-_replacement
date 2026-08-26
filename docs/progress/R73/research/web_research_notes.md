# R73 联网调研记录

本文件是检索结果的事实摘录，不是处置建议。精确检索未命中时以 `NOT_FOUND` 记录，并保留检索范围，不把它外推为“互联网不存在”。

## libc++

- 官方 `noexcept` policy 自 libc++ 13 起使用的是 “may mark”，并明示强化异常规格可能导致用户回调抛出的异常不再传播。
- D95821 的评审明确把 “will” 改为 “may”：目的是允许实现使用这项权利，但不要求逐项机械应用。因此政策本身容许例外，但文档没有给出 pthread cancellation 专用例外表或宏。
- 对 llvm-project GitHub issues 的三个精确查询及本地历史 `git log -S pthread_cancel -- libcxx` 均未命中本场景。没有找到“提出后被维护者拒绝”的直接材料，结论为 `NOT_FOUND`，不能写成“上游拒绝过”。

## GCC PR103382

- 初版补丁直接说明：NPTL cancellation 使用的 `__forced_unwind` 穿过 `condition_variable::wait noexcept` 会终止进程，因此 wait 应允许异常穿过。
- 讨论确认存在 ABI 风险：旧头文件已把调用记录为不抛异常；仅替换共享库定义可能与旧调用方的异常元数据不一致。
- 最终 commit `9e18a25331fa25c3907249fede65a02c6817b06e` 使用新版本 `@@GLIBCXX_3.4.30` 提供可取消行为，并为旧 `@GLIBCXX_3.4.11` 保留 `noexcept` wrapper。也就是说，GCC 同时处理了语义和旧 ABI，而不是静默覆盖旧符号。
- 变更覆盖 public `condition_variable::wait`、内部普通/定时/clock wait；测试覆盖 wait、predicate wait、wait_for 和 wait_until。

## 其他项目与平台

- Android/Bionic 官方状态页明确不提供 `pthread_cancel`。这是平台级取消能力取舍，不是修补 libc++ 边界。
- Chromium 的公开 `base::Thread` 以投递退出任务和 join 实现停止；公开条件变量是自有 pthread wrapper。限定检索中未找到 `pthread_cancel` 或该 libc++ 边界的本地补丁。这只能支撑“未命中”和“常用封装采用协作退出”，不能证明 Chromium 全仓永久禁用取消。
- 对 Debian、Fedora、FreeBSD、Alpine 的 libc++ packaging 做了定向检索，未找到该精确冲突的本地补丁，记 `NOT_FOUND`。
- WG21 P3085R0 明确提到 C++ 视角不能抛出的 `fclose` 在 POSIX 线程取消中可参与展开，说明这一冲突已进入一般 `noexcept` 政策讨论；该文档是 R0 提案，不冒充已采纳标准政策。
