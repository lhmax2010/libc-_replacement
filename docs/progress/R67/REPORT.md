# R67：补丁集重建、两项代码缺陷修复与全量重验

## 结论

结论为 **`PATCHSET_VERIFIED`**。

- M0 已关闭：最终四补丁从干净基线重放，得到的 9 个变更文件与修复后的实验树逐文件 SHA256、逐字节完全一致。
- 分批已真正调整为 **头文件批 20 处 / 库内批 30 处**。S08、S16、S17、S33 位于头文件批；特别核验 S33 的 sentry 析构防护由补丁 3 加入，补丁 4 不再回删。
- M1 已关闭：引用结果共享状态补齐未发布构造与强制完成路径；两架构编译运行通过，上游 `async.pass.cpp`（含引用返回用例）两架构均通过。
- M2 已关闭：值类型析构移到共享状态互斥锁之外；专用探针两架构各 20/20 通过，并直接观测到析构内 `wait_for` 返回 `TIMEOUT`，证明析构期间可取得状态锁。
- F1、F2、M1、M2 定向格在 x86_64 与 armv7l 上均与 libstdc++ 参考一致；F2 直接断言 `RDSTATE=1 BAD=1 FAIL=1`，M1 直接断言 callable 执行两次并返回 42。
- 最终补丁树的构建、运行时支持测试、所选 libc++ future/iostream/string 上游测试、设施矩阵、R51 18 格回归、正常异常传递与并发矩阵均未发现本补丁新增回归。
- 已知的 `cout`/标准库并发输出超时仍在两侧出现；它不是最终补丁独有。本任务不改变这一已登记边界。

## 1. 资源与执行纪律

`tools/resource_gate.sh --level medium` 返回 0 后才开始盘点与构建。最终构建均使用：

- `CMAKE_BUILD_PARALLEL_LEVEL=2`
- `LLVM_PARALLEL_LINK_JOBS=1`
- `nice -n 15`
- `ionice -c 3`
- `cmake --build ... -- -j2`

开发板为 `192.168.108.26:26101`，`armv7l`，Tizen 11.0，build id `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`。首次连接失败，第二次显式重试成功，未超过三次。最终清理核验：远端部署目录不存在、共享归档不存在、目标进程数为 0。

本轮有三次执行工具问题，均在继续前保留原文并纠正：

1. 首次资源门禁包装命令在创建 raw 目录前重定向，门禁本身未执行；补建目录后重新调用并取得 0。
2. 首次板端清单把 `MANIFEST.sha256` 自身纳入清单；实际文件均通过但自哈希失败，因此未运行测试，先清理后修正并重新部署。
3. 首次/最终板端矩阵的主机重定向目录遗漏；脚本未启动，补建目录后重跑。

## 2. M1：引用结果支持

### 实现

在 `__assoc_state<_Rp&>` 中补齐与值类型主模板对应的两个内部成员：

- `__construct_value_unready(_Rp&)`：在锁内记录被引用对象的地址并置 `__constructed`，不发布 `ready`。
- `__complete_forced_unwind(exception_ptr)`：在锁内清除 `__constructed` 与指针，写入 `broken_promise`，置 `ready` 并通知等待者。

引用特化不拥有所指对象，故强制完成时不执行析构；值类型主模板拥有 placement-new 构造的对象，故必须销毁。这个差异由两种特化分别实现。

### 直接判据

| 架构 | 自建 `std::async` 返回 `T&` | 上游 `async.pass.cpp` | 相关竞态测试 |
|---|---:|---:|---:|
| x86_64 | 0 | 0 | `async_race`、`async_race.38682` 均 0 |
| armv7l | 0 | 0 | `async_race`、`async_race.38682` 均 0 |

## 3. M2：用户析构移出持锁区

### 实现与原子性

值类型 `__complete_forced_unwind` 分三段：

1. 持锁：检查尚未 ready；记录是否存在已构造值；清除 `__constructed`；写入最终 `exception_ptr`。
2. 释放锁：若原值存在，调用用户类型析构。
3. 再持锁：当仍未 ready 且异常已存在时置 `ready` 并通知。

在第 1、3 段之间，等待者只能看到 `ready == false`，不能读取已被移出状态机的对象；async 状态的执行线程仍持有共享状态引用，状态不会在析构期间被释放。async 状态只有该执行线程是结果写入者，因此没有第二个合法结果提交者可越过这一状态转移。

用户析构外层的内层 `catch (...) {}` 保留，用于阻止用户析构抛出的异常替换正在传播的强制展开。整个 `_NOEXCEPT` 完成函数外层的 `catch (...) {}` 也保留，覆盖互斥锁/内部状态操作的异常边界，维持“不抛异常的完成接口”契约；它不再包住持锁状态下的用户代码。

### 直接判据

专用探针把一个 `shared_future` 句柄提供给待销毁结果对象；结果析构中调用同一共享状态的 `wait_for(0)`：

```text
STORED_DTOR_WAIT_STATUS=TIMEOUT
FUTURE_ERROR=... BROKEN=1
CANCEL_RC=0 STORED_DTOR_ENTERED=1 STORED_DTOR_COMPLETED=1
```

如果析构仍持有状态互斥锁，`wait_for(0)` 无法取得该锁并返回。最终结果：

| 架构 | 修复版 | libstdc++ 参考 |
|---|---:|---:|
| x86_64 | 20/20 | 20/20 |
| armv7l | 20/20 | 20/20 |

参考实现中结果对象的实际销毁时点不同，但等待方同样取得 `broken_promise`，没有永久等待。

## 4. M0：唯一补丁集与身份核验

### 上游基线

- commit：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`
- subject：`packaging: add libc++ runtimes spec`
- time：`2026-08-07T11:07:46+08:00`

原始与最终文件 SHA256 全列于 `raw/065_source_and_patch_inventory.output.txt`。

### 最终补丁

| 顺序 | 补丁 | SHA256 |
|---:|---|---|
| 1 | `0001-Layer-A-fix-foreign-forced-unwind-resume.patch` | `09065e3b8490daf23e7e6c7eb1a50aa1b2bdb6797f9a9ac4873964200b7f2ff2` |
| 2 | `0002-Layer-B-add-GNU-forced-unwind-marker.patch` | `b718648618b7e7617421dc95d70cc6cc9b03bd72a419d907b63fd16d465c4509` |
| 3 | `0003-Layer-C-header-batch-20-sites.patch` | `982e61c4ebe24261479c231f22ac6ed97f1074bbfa7e8f7e61114c90246284e7` |
| 4 | `0004-Layer-C-library-batch-30-sites.patch` | `9b6832724559f5ce20660803b83a93aab2c0fcf890fb15e2a88214ae667d3db7` |

补丁 3 有 20 个 typed forced-unwind handler；补丁 4 有 17 个直接 handler，并由两个 `ios.cpp` 汇聚点覆盖其余库内站点，按站点口径为 30。

### 双重身份核验

从干净基线顺序 `git am` 四补丁成功，无冲突。最终有 9 个变更文件；实验树、补丁生成树、干净重放树的对应文件：

- SHA256 三方一致；
- `cmp` 三方退出 0；
- S33 最终源码存在 typed handler；库内补丁中不存在 sentry 析构 hunk。

发现并纠正了一次重要的自检盲区：初版实验树与补丁树虽字节一致，但补丁 4 把补丁 3 的 S33 回删，属于“两棵树同错”。增加“分批语义审计”后修正，所有最终构建和测试均在修正后的干净重放树上重新执行，修正前结果不用于本结论。

## 5. 构建

| 架构 | libc++.so.1.0 SHA256 | libc++abi.so.1.0 SHA256 | 结果 |
|---|---|---|---|
| x86_64 | `2e1a750732f5615cac3293cf82d5edc0792255730aebdcdbdd966df11bc1163c` | `335ecd9ce36a71b3f9e01a70e468602086dcbcf8038b9a1040cd2bafdc92fb06` | 成功 |
| armv7l | `7c62ffed71d33f80ee03dd37c14e2c8e057412e2f14bc0a60dadb9f160fc6824` | `1f58fafb1598945ed0f5966178af64f5905e7ee1c5b6dba8a2c93b27fa098666` | 成功 |

最终构建日志有 x86_64 36 行、armv7l 72 行既有驱动选项 warning（`--rtlib=libgcc`，ARM 另含 `--ld-path`）；无 error，无指向 9 个被改源码文件的 warning。四个运行库中没有 `R40/R41/R42/R45` 诊断字符串。

## 6. 定向、设施与回归

### F1/F2/M1

两架构修复版与 libstdc++ 参考均退出 0：

- F1：顶层真实异常 handler 被进入，`STATE_BAD=1`。
- F2：取消完成，`JOIN_CANCELED=1`，流状态 `RDSTATE=1 BAD=1 FAIL=1`。
- M1：第一次执行被取消，第二个等待者触发第二次 callable，`INVOCATIONS=2 VALUE=42`。

### 设施矩阵

- `getline` 自由函数/成员、`read`、`packaged_task`、`async`：两架构最终修复版与参考侧退出 0；未打补丁基线按场景复现 134。
- 算术输入：本轮具体夹具三侧退出 0，故该基线故障未由这一格复现；F2 专项已直接覆盖异常掩码与状态位提交。
- `cout`：armv7l 修复版/参考均 0；x86_64 两侧均 124，属于已登记的共同退出阶段超时。
- 正常 `async` 返回、真实异常经 `future.get()` 传递，`packaged_task` 正常返回/真实异常，iostream 格式失败均与基线及参考一致。
- R51 的 9 场景 × 修复版/参考 = 18 格，两架构均 18/18 退出 0。

完整逐格结果见 `tables/all_matrix_cells.tsv`，关键定向/设施结果见 `tables/directed_facility_regression.tsv`。

## 7. 上游测试

两架构均执行：

- libc++abi：`forced_unwind{1,2,3,4}`、`unwind_0{1..6}`；修复版与 libstdc++ 参考全部退出 0。
- libc++ future：`async.pass.cpp`、`async_race.pass.cpp`、`async_race.38682.pass.cpp`、`wait_on_destruct.pass.cpp`、`thread_create_failure.pass.cpp`、packaged task 的 `get_future` 与 `operator()`；全部退出 0。
- libc++ iostream：getline、read、ws 相关用例；全部退出 0。
- libc++ string：`shrink_to_fit.pass.cpp`；正常路径退出 0。

共每架构 31 个运行格，全部通过。ARM 测试采用真机，不使用模拟器。

## 8. 并发验证

| 场景 | x86_64 修复/参考 | armv7l 修复/参考 |
|---|---|---|
| M1 同一 deferred future 多等待者 | 20/20，20/20 | 20/20，20/20 |
| M2 锁外析构与完成竞态探针 | 20/20，20/20 | 20/20，20/20 |
| 多线程同时取消 | 20/20，20/20 | 20/20，20/20 |
| 取消与真实异常并发 | 20/20，20/20 | 20/20，20/20 |
| 重复取消 | 20/20，20/20 | 20/20，20/20 |

标准库并发输出仍不稳定：x86_64 该组共 25 个 124，armv7l 共 33 个 143，修复版和参考侧都有成功与超时样本。完整失败样本未删除。

## 9. 已验证与未覆盖范围

已验证：x86_64 Itanium/DWARF 与 armv7l ARM EHABI；动态 libc++；默认 extern-template；无 LTO；单次/并发取消；deferred/async future；引用/值/void 结果；异常掩码与流状态；上游所列测试。

未覆盖：

- aarch64；
- LTO 下的最终产品构建；
- 混合运行时帧穿越；
- `dlopen`/插件边界与复杂加载顺序；
- 高负载长时压力、取消风暴；
- 已知 `noexcept` 不可达的 `basic_string::shrink_to_fit` forced-unwind 站点与 `basic_ostream::sentry` 析构站点的正向取消行为（维持 `NOT_OBSERVED`）。

## 10. 自行判断与尚存疑问

本任务做出的实现判断：

1. 引用特化仅记录指针，不取得所有权、不析构引用对象；这与 `future<T&>` 的既有语义一致。
2. 值类型强制完成采用“锁内撤销可见值并保存错误—锁外析构—锁内发布 ready”，而不是移动构造临时值；后者仍会执行用户移动构造，扩大锁外用户代码与失败面。
3. 保留析构周围的局部 catch 以保护原强制展开；保留完成接口最外层 catch 以满足 `_NOEXCEPT` 内部接口约束。
4. 修正前测试结果全部作废并从最终补丁重放树重跑，因为仅比较两棵同源树不能关闭 M0。

尚存疑问：无阻断性问题。标准库并发输出的双侧不稳定与 x86_64 `cout` 双侧退出超时仍是已登记边界，不在本任务修复范围。
