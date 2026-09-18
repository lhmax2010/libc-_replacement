# 周末编译线开工

开工观测：2026-09-18 12:28:14 +08。W1 截止 18:28:14；总窗口最晚 2026-09-19 09:28:14。W2/W3/W4 各自起点在进入任务时记录，分别不超过 6/6/3 小时，且不超过总窗口。

采用人工已批准的 cp -a + 逐文件 SHA256 校验。armv7l LLVM 18 个单独核验的 RPM 标记 USABLE_BUT_BUILD_INCOMPLETE；aarch64 使用已完整通过的 12 个 RPM。输入身份仍须核验，不因历史 PASS 而跳过。

codes/ 只读；候选留在 tmp/；不推包仓、不覆盖 Source1002。只在 codex/r5-ehabi-diagnosis 提交本轮证据。并行 1、nice19、ionice idle、实际 cgroup 内存上限为本次 MemTotal 的 50%。

引导命令与工具返回恢复记录：`pwd; date -Is; git branch --show-current; ls -l AGENTS.md; ls progress/BUILD_STATIC_0917B/resume_0917`，组合退出 0；其中 ls AGENTS.md 报不存在，组合末项成功不表示该文件存在。引导命令输出来自本轮工具返回，非本轮包装器自动记录。
