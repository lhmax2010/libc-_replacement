# R74 补全说明

原始报告代码块一字未改，保存在 `progress/R74/extracted/`。

- `06_problem2_cout.cpp`：仅补入原代码使用但未包含的 `<iostream>`、`<pthread.h>`、`<unistd.h>`。
- `09_problem3_waiter_cancel.cpp`：补入 `<atomic>`、`<future>`、`<pthread.h>`、`<unistd.h>`，定义报告中使用但未给出的 `waiter_started` 与 `some_work`。`some_work` 等待 500 ms 后返回 42，使等待方在取消请求到达时仍阻塞；这是为实现报告文字所述场景而作的补全，不是报告原文。
