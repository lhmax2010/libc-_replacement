# R65：M2 armv7l 调用栈补证与两处勘误

## 结论

调用栈证据支持 R64 的 **`FIXTURE_ISSUE_CONFIRMED`** 判定，可以进入第二部分。M2 产品实现未作任何修改。

需要修正 R64 中一句机制层措辞：原夹具下并非“展开根本没有开始”。本次直接观测到异步取消经 SIG32、`__pthread_unwind` 进入 `_Unwind_ForcedUnwind`；但 ARM forced unwind 全程没有调用 libc++abi personality，因此没有执行 `std::async` 的 forced-unwind handler 和共享状态完成接口。工作线程最终经 glibc cancellation stop 的 end-of-stack 路径退出，主线程继续阻塞在 future 条件变量。

延迟取消对照从 `pthread_testcancel()` 发起后，personality 被调用，`future` 的 forced-unwind handler 命中，`__complete_forced_unwind()` 执行，随后 `__cxa_rethrow()` 继续取消；等待方取得 `broken_promise`。这与 R64 两架构、两标准库三场景各 20/20 的结果一致。

第二部分已完成：W2 新增勘误并在原报告开头加入指向；决定文档新增 2026-08-25 数量勘误，README 摘要改为库内批 30 / 头文件批 20；`superseded/` 内容未改。

## 一、环境与方法

- 开发板：`192.168.108.26:26101`，armv7l，Tizen 11.0，build id `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`。
- 调试器：`/usr/bin/gdb` 16.3、`/usr/bin/gdbserver` 16.3 可用；`lldb` 与 `lldb-server` 实测不存在。
- 附加权限：GDB 成功附加到活跃的 `sleep` 进程、列出线程并 detach，退出 0。
- 符号：glibc 与实验版 libc++abi 的调试符号可读；板上 `/lib/libgcc_s.so.1` 缺少调试信息，因此不能在 libgcc 内部逐语句单步。
- 复用对象：W2 原探针与 R64 延迟取消探针，未重新编译。原探针 fixed SHA256 为 `f82f58dce381689de4d490d167e0e63a52ff693ee135988931006aea1e0fee9a`，延迟探针 fixed 为 `a0d59266ccab77604fe96de16011db10b9b0e8dbc757e770b9e25bcc1bda5605`。
- 运行时：两探针均从 `/var/tmp/r65_20260825/lib/` 加载 W2 实验库；`libc++.so.1` SHA256 为 `22d9ee51df953c470587d10c785567635b3a719b1456529db9cba99da61e6fc9`，`libc++abi.so.1` 为 `f63b9604b9a9490d62e2b87b0a8c8fd84e1a5d8947f2594bff2a656b2b175c46`。板上与宿主 SHA 一致，`ldd` 原文在 `raw/005_prepare_and_deploy_payload.stdout.txt`。
- 抓取方法：先在 `main` 停住以加载共享库并解析真实符号地址，再分别对 `pthread_cancel`、`__pthread_unwind`、两份 `_Unwind_ForcedUnwind`、`unwind_stop`、`__gxx_personality_v0`、`__cxa_rethrow` 和 future handler 源码行设置断点。取消前与取消后均对所有线程执行 `bt 40`。

资源检查 `tools/resource_gate.sh --level light` 退出 0：MemAvailable 20,569,516 KiB，load1 9.68，阈值分别为 2,097,152 KiB 与 30.0。

## 二、原探针调用栈

### 取消前

工作线程停在 `WindowResult::~WindowResult()` 第 31 行的无限忙等。完整栈为：

`WindowResult::~WindowResult` → `__async_assoc_state<WindowResult>::__execute`（`future:916`）→ `__thread_execute` → `__thread_proxy` → `start_thread`。

主线程在 `pthread_cancel` 调用点。取消返回后，GDB 直接读取局部变量得到 `cancel_rc = 0`。

### 取消后与退出

实际路径为：

`sigcancel_handler` → `__do_cancel` → `__pthread_unwind` → glibc `_Unwind_ForcedUnwind` → libgcc_s `_Unwind_ForcedUnwind` → `unwind_stop`。

`unwind_stop` 共命中四次，前三次 `actions=9`，末次 `actions=25`（包含 end-of-stack）。随后 GDB 报工作线程退出。

已在实验版 libc++abi 的真实地址 `0xb6f07172` 安装 `__gxx_personality_v0` 断点，但从 forced unwind 开始到工作线程退出没有一次命中；`__cxa_rethrow` 同样未命中。故本次直接观测到的是：展开已经开始，但没有遍历执行 `__async_assoc_state::__execute()` 的 C++ handler。

同时，主线程栈为：

`futex` → `pthread_cond_wait` → `condition_variable::wait` → `__assoc_sub_state::__sub_wait` → `__assoc_state<WindowResult>::move` → `future<WindowResult>::get`。

### 四问回答

1. **取消前是否在忙等循环：是。** GDB 的当前帧和源码行均为原探针析构第 31 行。
2. **取消后展开是否开始：是。** 直接命中 `__pthread_unwind` 及 glibc/libgcc_s 的 `_Unwind_ForcedUnwind`。它没有停在某一 C++ handler；相反，未调用 personality，直接运行到 cancellation stop 的 end-of-stack。
3. **线程退出路径：** 异步 SIG32 触发 glibc cancellation forced unwind，libgcc_s 最终以 `actions=25` 回调 glibc stop，随后工作线程退出。这不是“未展开退出”，但也不是正常遍历并执行 future C++ handler 的展开完成。
4. **共享状态为何未 ready：** `__async_assoc_state::__execute()` 在 `__construct_value_unready()` 后、`__make_ready()` 前遭取消；正常补偿位于 forced-unwind handler 的 `__complete_forced_unwind()`。本次没有 personality 调用、没有 handler 命中、没有完成接口调用，因此状态没有收到 ready 位与通知；主线程栈仍停在 `__sub_wait()` 的条件变量即为运行时佐证。

完整未经整理输出为 `raw/008_trace_original_m2_v2.stdout.txt`，定位索引见 `tables/call_stack_evidence.tsv`。

## 三、延迟取消对照

R64 探针在同一“值已发布、执行未返回”窗口中将忙等体改为 `pthread_testcancel()`，取消类型改为 deferred。本次附加观测显示：

- 取消前工作线程位于 `pthread_testcancel()`；
- forced unwind 开始后命中 `__gxx_personality_v0`，首次 forced state 为 9；
- 命中 `future:926` 的 `__complete_forced_unwind()` 调用点；
- 随后命中 `__cxa_rethrow()`；
- 等待方输出 `M2_RESULT=FUTURE_ERROR BROKEN_PROMISE=1 CANCEL_RC=0`。

该次 GDB batch 最终为 1，是因为主线程已经打印结果并退出后，仍在收尾的 detached 工作线程再次命中 personality 断点，GDB 报目标已不可写并无法读取断点地址。决定性输出和完成路径均在该错误之前取得；此调试器收尾错误未作为测试通过码使用。完整原文在 `raw/009_trace_deferred_m2.stdout.txt`。

对照支持 R64 判定：同一产品库下，只改变取消夹具后，ARM 能遍历到共享状态 handler 并发布完成结果。产品实现无需修改。

## 四、两处勘误

### W2

- 新增 `docs/progress/W2/ERRATUM-M2-ARMV7L-FIXTURE.md`，说明原三格记述、R64 的 20/20 对照和本次调用栈补证，并明确 M2 产品实现未改。
- `docs/progress/W2/REPORT.md:3` 只增加一行显著勘误指向；原有结论文字未改。

### 决定文档

- `docs/decisions/20260815-thread-cancellation-scope.md:15` 起，在裁决结论表后加入数量勘误：由 34 / 16 修正为 **30 / 20**，分批框架不变。
- `docs/decisions/README.md:18-23` 的摘要数字和复核状态同步为 **30 / 20**。
- `docs/decisions/superseded/` 九个文件修改前后 SHA256 清单逐字节相同，见 `tables/superseded_before.sha256` 与 `tables/superseded_after.sha256`。

## 五、其他 34/16 引用

除本次修改的 README 摘要与决定文档勘误外，仍有以下引用；均按任务要求未修改：

- 现行决定文档被冻结正文中的原始口径：`docs/decisions/20260815-thread-cancellation-scope.md:12,22,247,248,302,303,310,390`；新勘误位于第 15 行起。
- 活跃进展报告：`docs/progress/R50/REPORT.md:8-9`、`docs/progress/R52/REPORT.md:1`、`docs/progress/R53/REPORT.md:1`、`docs/progress/W3/REPORT.md:9,15`、`docs/progress/R64/REPORT.md:12,141`。
- 原始/策展证据副本：`docs/progress/R50/zero/commands/001_pre_annotation.log:14`、`docs/progress/R52/raw/004_prior_tables_and_reports.log:297-298`、`docs/progress/W3/tables/input_sha256.tsv:5`、`docs/progress/R64/code/evidence/raw/004_w3_primary_evidence.stdout:9,15,112`、`docs/progress/R54/review_package/raw/R52/004_prior_tables_and_reports.log:297-298`、`docs/progress/R58/review_package/raw/prior/R52/004_prior_tables_and_reports.log:297-298`。
- 历史版本：`solution_definition_v5.md` 至 `v9.md` 各六处；逐文件行号见 `tables/other_document_references.tsv`。这些文件受 `superseded/` 保护，未改。

全仓语义检索的 58 条原文在 `raw/010c_find_all_semantic_34_16_references.stdout.txt`；分组清单在 `tables/other_document_references.tsv`。

## 六、异常、判断与疑问

### 本任务中的自行判断

1. 板上实测无 lldb，因此选用可用且已验证能 attach 的 GDB 16.3。
2. 为避免启动前 pending breakpoint 只绑定到测试件 PLT，先 `start` 加载共享库，再对真实 libc++abi/libgcc/glibc 符号设断点。
3. 执行了任务列为可选的延迟取消调用栈对照，因为原探针“没有 personality 命中”需要正面对照才能排除断点或符号搭建错误。
4. 没有沿用“展开根本没有开始”的旧措辞；实测要求将其修正为“展开开始，但未遍历到 C++ personality/handler”。

### 尚存疑问与限制

- 板上 libgcc_s 无调试信息，未能定位其 ARM unwinder 为何从原探针任意 PC 启动时跳过 personality；本任务已取得区分夹具路径与产品 handler 的充分调用序列，但没有给出 libgcc 内部逐指令根因。
- 原探针的工作线程退出非常快，未取得“退出之后”的用户态栈；取得了退出前的 `unwind_stop(actions=25)` 栈与 GDB thread-exited 事件。
- 延迟对照的 GDB 在主线程正常取得 `broken_promise` 后因 detached 工作线程收尾报错，已完整保留，未把 GDB 退出 1冒充为成功。

## 七、收尾与材料

板上 `/var/tmp/r65_20260825`、五个传输归档均已删除；最终进程核验无 `original_fixed`、`deferred_fixed` 或本任务 GDB 进程，清理命令退出 0。原文见 `raw/012_board_cleanup.stdout.txt`。

命令原文及退出记录在 `commands/`，未整理输出在 `raw/`，GDB 脚本、复用探针源码及既有编译命令在 `code/`，索引在 `INDEX.tsv`。
