# R64：M2 armv7l 归因与 W3 分批数量确定

日期：2026-08-25
执行分支：`codex/r5-ehabi-diagnosis`

## 结论

1. M2 的 armv7l 结论为 **`FIXTURE_ISSUE_CONFIRMED`**。原探针使用异步取消并在 `WindowResult` 析构中无取消点忙等；挂起现场中工作线程已经退出，只剩主线程在 futex 等待 future。将探针改为延迟取消并在完全相同的“值已发布、执行未返回”窗口调用 `pthread_testcancel()` 后，armv7l 上修复版 libc++ 与 libstdc++ 参考侧的主窗口、`wait_for`、多等待者三格均 **20/20** 通过；x86_64 同矩阵也均 **20/20**，与 W1 一致。没有修改 M2 产品实现。
2. W3 的原始不确定变量是构建时 `%{build_type}` 的实际展开值；原构建的 CMakeCache、`compile_commands.json` 与日志均未保存。R64 从 Tizen 官方当前 armv7l 发布调试 RPM（与 W3 源 RPM NEVR 一致）补得实际 producer 记录：`app-installers` 与 `tpk-backend` 均带 `-flto`，并有 `-fltrans` 链接时优化记录。因此：
   - 任务要求的“只计 W3 三个确认例外”的保守下界仍为 **库内批 31 / 头文件批 19**；
   - 对当前官方发布的同 NEVR armv7l 制品，精确值可确定为 **30 / 20**；新增移动的是 `app-installers` 命中的 `S08`，`tpk-backend` 没有观察到受影响站点。
   - 与落章文档 `34 / 16` 相比，当前发布制品口径为库内批少 4、头文件批多 4。未修改落章文档。

## 纪律与执行环境

- 开工先调用 `tools/resource_gate.sh --level light`，退出 0；当时 `MemAvailable=17646448 KiB`，`load1=8.10`，阈值分别为 2 GiB 与 30。
- 只重编/重链探针，复用 W2 的 libc++/libc++abi；没有修改产品代码。
- armv7l 在 `192.168.108.26:26101` 真机原生执行；x86_64 在宿主原生执行；未使用模拟器执行测试。
- 交叉编译器本身由既有包装器借助 qemu 启动目标 clang，仅用于交叉编译，不用于行为验证。
- 共享盘曾出现严重 I/O wait；中止了本任务自身卡住的交叉编译与只读盘点进程，待退出后串行重跑。没有并发启动第二个构建。
- 板端部署目录、两个传输归档及测试进程均已清理，清理核验退出 0。

## 输入与 SHA256

完整清单见 `raw/025_input_sha256.stdout.txt`。关键项：

| 输入 | SHA256 |
|---|---|
| W2 原探针 `r61_m2_matrix.cpp` | `b3521c4435acd23c61d3497727446a6c1a7d809e07d49523adf697b8e8c2c820` |
| W2 报告 | `bbfceb6ce9bd3c918374d97af8bce502f9cc82c58e5c7d658eda2755e58ed440` |
| W3 报告 | `97d9d82a63c3ddc6cd9ae3ac8dbd31b2f9facde58e9b6226da59702b78c58505` |
| W3 `reviewed_exception_consumers.tsv` | `03effe8f5e349675c834fe10970d2f40a7208d54d89d6be75db2037a3adcb8af` |
| W2 armv7l libc++.so.1 | `22d9ee51df953c470587d10c785567635b3a719b1456529db9cba99da61e6fc9` |
| W2 armv7l libc++abi.so.1 | `f63b9604b9a9490d62e2b87b0a8c8fd84e1a5d8947f2594bff2a656b2b175c46` |
| 官方 app-installers armv7l debuginfo RPM | `430f915d5cddc247dd50bd6bff440043d50db60af318808d72c62bde8ce88290` |
| 官方 tpk-installer armv7l debuginfo RPM | `b728d515528a87d9ae57724998d107e70005010211b9de4aaf955985cf352d6b` |

## 第一部分：M2 armv7l 归因

### 1. 原探针与挂起现场

原探针有两个决定性形态：

- `WindowResult::~WindowResult()` 先置 `window_entered=1`，随后执行无限 `asm volatile` 忙等；循环中没有取消点。
- `make_window_result()` 将取消类型设置为 `PTHREAD_CANCEL_ASYNCHRONOUS`。

在 armv7l 修复版 libc++ 的原 `m2` 格启动后 3 秒读取 `/proc`：

- 进程仍存活，只有主 TID `12560`；工作线程已经不存在。
- 主线程状态 `S`，`wchan=__futex_wait`，系统调用为 futex，说明等待方仍在等共享状态。
- 运行时映射确认加载 W2 实验版 libc++.so.1/libc++abi.so.1。
- 板上无 gdb/gstack/pstack/eu-stack；`/proc/.../stack` 返回 `Permission denied`。因此主线程与已退出工作线程的完整用户态调用栈均为 **`NOT_AVAILABLE`**。
- 原探针在 `future.get()` 返回前不打印 `cancel_rc`，该次原格的返回值为 **`NOT_OBSERVED`**。延迟取消探针的 120 次 armv7l 执行中，`pthread_cancel` 均返回 0。

这组现场与“工作线程仍停在忙等，展开根本没有开始”不符：观测时工作线程已经退出。直接观测到的是“工作线程退出，但共享状态未使等待方返回”。不对已退出线程的未采集栈作推断补全。

原始证据见 `raw/009_capture_hang_proc_state.stdout`，汇总见 `tables/hang_observation.tsv`。

### 2. 对照探针

仅修改探针两点：

```diff
-    for (;;)
-      asm volatile("" ::: "memory");
+    for (;;)
+      pthread_testcancel();
...
-  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);
+  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type);
```

另在三个待测模式的 `pthread_cancel` 后增加结果打印，不改变控制流。完整 diff 为 `code/r64_m2_deferred.diff`。

窗口语义保持：`window_entered` 仍由同一个 `WindowResult` 析构在进入循环前发布，主线程仍在观察到该标志后才取消工作线程；改变的只是取消类型以及循环中是否有显式取消点。

### 3. 对照矩阵

分母：每格 20 次；通过口径为探针退出 0，且相应状态断言成立。

| 架构 | 运行时 | 主窗口 | wait_for | 多等待者 |
|---|---|---:|---:|---:|
| armv7l | 修复版 libc++ | 20/20 | 20/20 | 20/20 |
| armv7l | libstdc++ 参考 | 20/20 | 20/20 | 20/20 |
| x86_64 | 修复版 libc++ | 20/20 | 20/20 | 20/20 |
| x86_64 | libstdc++ 参考 | 20/20 | 20/20 | 20/20 |

逐格状态：

- 主窗口：`CANCEL_REQUEST_RC=0`，等待方得到 `broken_promise`。
- `wait_for`：首次等待超时，取消后第二次为 ready，随后得到 `broken_promise`。
- 多等待者：4 个等待者全部返回，4 个均得到 `broken_promise`。

armv7l/x86_64 完整 120 格原始输出分别在 `raw/arm_deferred_matrix/`、`raw/x86_deferred_matrix/`；汇总在 `tables/arm_deferred_matrix.tsv` 与 `tables/x86_deferred_matrix.tsv`。运行时身份佐证见 `raw/022_validate_runners_and_runtime_identity.stdout.txt`。

### 4. 归因

结论：**`FIXTURE_ISSUE_CONFIRMED`**。

依据是同一产品实现与同一参考实现都只在原来的“异步取消 + 任意 PC 忙等”夹具下挂起；改为延迟取消点后，两架构、两运行时均稳定取得目标语义。x86_64 修改后结果与 W1 三格结论一致。故 W2 的 armv7l 失败不能作为 M2 产品代码未取得目标语义的证据，M2 产品实现无需在本任务中改动。

证据边界：无法取得已退出工作线程的用户态栈，因此没有对“异步任意 PC 展开在 ARM 内部具体失败在哪一条指令/帧”作机制定位；本任务只确定失败由原夹具触发，且产品与参考两侧同现。

## 第二部分：W3 分批数量

### 5. 原不确定性的来源与补取

W3 的唯一影响精确站点数的变量是 `app-installers` 的 `%{build_type}`：spec 传入 `-DCMAKE_BUILD_TYPE=%{?build_type:%build_type}`，源码在 Release 或空值时启用 `-flto`；但 W3 留存材料没有该宏的实际展开，也没有对应 CMakeCache、`compile_commands.json` 或构建日志。`tpk-backend` 同样缺该变量，不过其源码没有观察到层 C 受影响站点，因而不改变批次计数。

R64 补取 Tizen 官方当前 armv7l 调试 RPM：

- `app-installers-debuginfo-1.37.2-1.armv7l.rpm`，Source RPM 与 W3 一致；DWARF producer 明列 `-flto`，GIMPLE producer 明列 `-fltrans`。
- `tpk-installer-debuginfo-1.11.4-1.armv7l.rpm`，Source RPM 为 `tpk-backend-1.11.4-1.src.rpm`；同样观察到 `-flto` 与 `-fltrans`。

下载命令与 URL 在 `commands/016_fetch_and_extract_published_debug.command.txt`；RPM 身份在 `raw/016_fetch_and_extract_published_debug.stdout`；producer 原文在 `raw/017_inspect_published_dwarf_flags.stdout`。

原始历史构建调用本身仍为 `NOT_AVAILABLE`；补得的是同 NEVR 当前官方发布制品的直接构建结果，不冒充未保存的历史日志。

### 6. 五个命中项复核

| 包 | 命中 | 状态 | 站点 | 批次影响 |
|---|---|---|---|---|
| capi-context-motion | LTO | 确认 | 未观察到 | 例外消费者，但不移动站点 |
| ncnn | LTO | 确认 | S16、S17 | 库内转头文件 2 处 |
| boost | 非标准字符特化 | 确认 | S33 | 库内转头文件 1 处 |
| app-installers | Release/空 build type 时 LTO | W3 条件命中；R64 官方发布 ARM 制品实证启用 | S08 | 库内转头文件 1 处 |
| tpk-backend | Release/空 build type 时 LTO | W3 条件命中；R64 官方发布 ARM 制品实证启用 | 未观察到 | 不移动站点 |

完整字段见 `tables/w3_hit_review.tsv`。

### 7. 数量

口径为 50 个层 C 站点，按站点去重，库内批与头文件批之和始终为 50。

| 口径 | 库内批 | 头文件批 | 说明 |
|---|---:|---:|---|
| 落章基线 | 34 | 16 | 已冻结文档值 |
| 当前可确定的最保守下界 | **31** | **19** | 只计 S16/S17/S33 三个确认例外 |
| 当前官方同 NEVR armv7l 发布制品 | **30** | **20** | 再计已实证 LTO 的 app-installers/S08 |

与落章 34/16 的差异来自四个站点转批：S16、S17、S33 由 W3 已确认例外触发，S08 由 R64 补得的官方发布 ARM LTO 证据触发。没有修改落章文档。

## 自行判断与尚存疑问

自行判断：

1. 使用同 Source RPM NEVR 的 Tizen 官方当前 armv7l debuginfo producer 信息作为实际发布构建是否启用 LTO的直接证据；同时保留“原 W3 历史构建调用未保存”的边界，不将下载制品称为原始日志。
2. 以同一析构中的 `window_entered` 发布点作为窗口不变的判据；只改变取消方式和取消点，不改变“值已发布、执行未返回”的排序关系。
3. 原格 `cancel_rc` 未打印，按纪律记 `NOT_OBSERVED`，不以修改后返回 0 反填原格。

尚存疑问/未取得：

1. 原异步取消格中工作线程退出前的完整用户态栈 `NOT_AVAILABLE`；无法进一步区分 ARM 任意 PC 展开内部的具体路径。
2. 原 W3 构建时 `%{build_type}` 的文本展开与原 CMakeCache 仍为 `NOT_AVAILABLE`；当前 30/20 由同 NEVR 官方发布 ARM 制品直接确定。

## 材料导航

- 探针与脚本：`code/`
- 命令原文与退出码：`commands/`
- 完整原始输出：`raw/`
- 矩阵与复核表：`tables/`
- 板端清理：`raw/026_board_cleanup.stdout.txt`
