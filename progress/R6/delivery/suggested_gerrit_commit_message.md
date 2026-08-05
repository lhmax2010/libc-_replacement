# 建议的 Gerrit 提交说明

中文说明（供上传者/Reviewer 阅读，不建议放入英文提交正文）：

LLVM 22.1.8 的 ARM EHABI + libgcc unwinder 组合因上游回归缺少 `__gnu_unwind_frame` 可见声明，导致 `libcxxabi/src/cxa_personality.cpp` 编译失败。本变更逐字回移 LLVM 22.x Release milestone 下的上游修复 8119f1854948b50358bbfaea08f207f51970f06c；不改变 Tizen 的 libc++ 打包策略、unwinder 选择或任何 spec。armv7l 已以全新单用根完成 7 binary RPM + 1 SRPM 构建，`ldd -r` 强未解析符号为 0，符号绑定到 `libgcc_s.so.1`。

建议英文提交正文（保留上游正文并补充回移追踪字段）：

```text
libcxxabi: declare __gnu_unwind_frame in cxa_personality (#189787)

ARM EHABI builds of libcxxabi fail with clang-22+ because
cxa_personality.cpp calls __gnu_unwind_frame without a visible
declaration, triggering:

  error: use of undeclared identifier '__gnu_unwind_frame'

Add an extern "C" forward declaration before the EHABI unwind helper so
the source compiles correctly.

Signed-off-by: Khem Raj <khem.raj@oss.qualcomm.com>
Upstream-Status: Backport [8119f1854948b50358bbfaea08f207f51970f06c]
Upstream-Commit: https://github.com/llvm/llvm-project/commit/8119f1854948b50358bbfaea08f207f51970f06c
Link: https://github.com/llvm/llvm-project/pull/189787
Issue: https://github.com/llvm/llvm-project/issues/194232
Introduced-by: 3c100d5d548d511d09746dd5a2fee29bdd00805c
Introduced-PR: https://github.com/llvm/llvm-project/pull/116261
Change-Id: I9ad60d66cbb643ffddb516be0839a3b43a931aa4
```

说明：实际本地 backport commit 已保留上游正文，并含 `Upstream-Status`、PR #189787 链接与 Gerrit `Change-Id`；上面的 `Issue`、引入提交/PR 和 `Upstream-Commit` 是供人工上传前补齐追踪信息的建议字段。
