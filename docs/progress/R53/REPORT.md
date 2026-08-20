# R53：层 C 库内批（34 处）实现与验证

## 1. 结论

本任务结论为 **`PARTIAL`**。

- 34 个逻辑站点已全部实现。实现形态为 20 个代码改动点：`ios.cpp`
  两个 out-of-line helper 覆盖 16 个汇聚站点，`istream` 的 17 个库内
  载体站点覆盖 17 个逻辑站点，`basic_ostream<char>::sentry` 析构
  覆盖 1 个站点。库内批补丁与 R52 头文件批补丁分开交付。
- x86_64 原生与 armv7l 真机的四个代表场景（`cout`、failbit helper、
  `basic_istream::getline`、`basic_istream::read`）均实测：未打补丁及
  仅层 A+B 为 134；换入本任务 libc++ 后，无论消费者是否重编均为
  0，且 `pthread_join` 报告 `PTHREAD_CANCELED`。这给出“只换库即
  生效”的正向证据。
- 可达性静态复核为 `REACHABLE 33/34`、
  `UNREACHABLE_NOEXCEPT 1/34`、`UNDETERMINED 0/34`。唯一不可达项是
  `basic_ostream<char>::sentry::~sentry()`：析构函数隐式 `noexcept`，
  若 `pubsync` 内发生强制展开，重抛越出该帧即终止。因此该站点按
  任务要求实现了形式对齐，但功能性效果为 `NOT_OBSERVED`，不能申报
  全量完成。
- x86_64 的 `cout` libstdc++ 对照在打印
  `JOIN_IS_CANCELED=1` 后由外层 harness 以 143/SIGTERM 结束；相同现象
  已在既有对照中出现。本报告保留该偏差，不把它改写为退出 0。

## 2. 输入、基线与不变项

源码基线为 `codes/llvm` 提交
`5ed6c77278dfa7a470667cf1a137723d3c96fe60`（`upstream/22.1.8-9-g5ed6c77278df`）。
三个本批文件的原始 SHA256 见 `raw/input_sha256.txt`：

- `libcxx/src/ios.cpp`：`1c26a8fe…2ed0`；
- `libcxx/include/istream`：`17e8ec74…582c7`；
- `libcxx/include/__ostream/basic_ostream.h`：`af677742…71b0`。

实验副本以 R52 头文件批为前置，前置补丁 SHA256 为
`8beb1bb0…dfec`。R51 产品版 libc++abi 复用而未重建：x86_64 为
`f4f1c05a…b591`，armv7l 为 `44672c2c…c04d`。`codes/llvm` 的任务后
状态为空，未修改平台源码；原文见 `commands/043_source_and_project_status.log`。

## 3. 参照与设计

libstdc++ 对应实现逐段原文与行号保存在
`raw/libstdcxx_references.txt`。采用的共同规则是：在既有
`catch (...)` 前捕获 `const __cxxabiv1::__forced_unwind&`，完成与原
catch-all 相同的状态更新后立即重抛。

### 3.1 两个汇聚 helper

`ios_base::__set_badbit_and_consider_rethrow` 与
`__set_failbit_and_consider_rethrow` 被头文件 catch 块调用，自身不在
catch 子句词法体内。实现保留既有状态位设置，随后以 `throw;` 取得
当前异常，在内层 handler 中识别 forced unwind 并重抛；普通异常被
内层 catch-all 吸收后继续执行既有 `exceptions()` 掩码判断。两架构的
badbit 与 failbit 路径均取得正向结果。

### 3.2 库内符号站点

`basic_istream<char>` 的显式实例化成员及其 arithmetic carrier 在
`libc++.so` 内生成；在原 catch-all 前加入相同状态变更与重抛。
`basic_ostream<char>::sentry::~sentry()` 同样由库中显式实例化承载，
但受隐式 `noexcept` 限制，功能性结果没有被观测到。

本批只改变强制展开被捕获后的分支。普通异常继续进入原 catch-all；
非异常路径不进入新增 handler。非取消对照在两架构各 8/8 退出 0。

## 4. 可达性与交付性质

逐站点结果见 `tables/library_sites_34.tsv`，计数见
`tables/reachability_summary.tsv`。口径：

- `REACHABLE`：标准 I/O 路径存在阻塞 read/write 取消点，或站点调用
  可由用户 streambuf/facet 实现的虚函数，后者可合法执行
  `pthread_testcancel`；
- `UNREACHABLE_NOEXCEPT`：强制展开若离开该帧会先触发 noexcept
  终止，无法由站点 handler 恢复；
- `UNDETERMINED`：现有源码不足以建立上述任一事实。本次没有此类。

34 个站点在 R47/R50 的 `-O0/-O2`、两架构分类中均由 out-of-line
符号、显式实例化成员或其 carrier 承载于动态 libc++。本任务重编
`ios.cpp.o` 与 `ios.instantiations.cpp.o` 后重链 libc++；消费者旧
二进制未重编的关键格仍生效。结构上 34/34 都只需交付新库；功能性
正向实测覆盖 4 条代表路径，不能把未逐站点触发的站点推断为已实测。

## 5. 实现与补丁

本批共 3 个文件、90 行新增：

- `libcxx/src/ios.cpp`：13 行；
- `libcxx/include/istream`：74 行；
- `libcxx/include/__ostream/basic_ostream.h`：3 行。

完整 diff 在 `raw/*_library_batch.diff`，完整当前文件在 `code/libcxx/`。
格式补丁为
`patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch`，
对 R52 实验基线执行 `git apply --check` 为 0。R52 头文件批以独立文件
`patches/0001-prerequisite-header-batch-from-R52.patch` 随附，没有合并。

任务未提供专用署名。格式补丁继承本仓配置的
`He Fangyu <fangyu.he@samsung.com>`；提交外部仓前由人工核对，若不符
需 `--amend --author`。

## 6. 构建与产物

只重编受影响的两个 libc++ 对象并重链，未重建 libc++abi。完整命令
逐项见：

- x86_64：`commands/007_build_x86_64_retry.log`；
- armv7l：`commands/008_build_armv7l.log`。

两者均退出 0，未检出新增 warning/error。产物：

| 架构 | `libc++.so.1.0` SHA256 |
|---|---|
| x86_64 | `705cbb1932039df6508e33ea39007056a820bf3706eee56d6998ce0ec625565b` |
| armv7l | `513c6d883c92ad4f848f0c1168997ecd118f8be1c4d1f7f55fa8a2a1094b8b49` |

初次 x86_64 编译因覆盖完整 include 树改变头文件查找而失败，随后把
overlay 收窄到三个实际改动头后成功；失败原文保存在
`commands/006_build_x86_64.log`，未从证据中删除。

## 7. 场景矩阵

逐格汇总见 `tables/core_scenario_matrix.tsv`，完整原始输出见
`raw/x86_64_scenario_matrix.log` 与
`raw/armv7l_scenario_matrix_final.log`。有效矩阵为 2 架构 × 4 场景 ×
5 组合，共 40 格。

- 两架构、四场景的未打补丁与仅 A+B 共 16 格均为 134，并出现
  `FATAL: exception not rethrown`；
- 本批库 + 旧消费者及本批库 + 重编消费者共 16 格均为 0，join 为
  `PTHREAD_CANCELED`；
- libstdc++ 对照 8 格中 7 格退出 0；x86_64 `cout` 格在 join 已确认
  canceled 后为 143，armv7l 对应格为 0。

armv7l 的早期 DSO carrier 对 `cout`/istream 连 libstdc++ 对照也产生
139，不能表达目标语义；其原文保存在 `raw/armv7l_scenario_matrix.log`。
后续改用直接可执行文件取得最终有效矩阵。此前的链接失败与无效格均
保留，未纳入结论。

## 8. 回归与上游测试

汇总见 `tables/validation_summary.tsv`。

- R51 的 9 类场景 × libc++/libstdc++：x86_64 18/18、armv7l
  18/18 退出 0；
- R52：`getline` 与 `packaged_task` 在两架构保持“补头且重编消费者
  为 0，旧头消费者为 134”；x86_64 `shrink_to_fit` 的五格均为 134，
  与 R52 的 `noexcept` 限制一致，继续记 `NOT_OBSERVED`；armv7l 沿用
  R52 范围，未构造该格；
- 选取 7 个上游 libc++ 测试（istream 3、future 3、string 1），
  x86_64 编译和运行 7/7 通过；armv7l 未执行上游测试 harness，记
  `NOT_AVAILABLE`；
- 非取消行为（getline 成功/失败、packaged exception、string normal）
  的改前/改后对照：x86_64 8/8、armv7l 8/8 退出 0。armv7l 首次调用
  因补充包实际尚未部署而得到 127，该无效尝试原样保存在
  `commands/037_noncancel_armv7l.log`；部署后有效结果为
  `commands/040_noncancel_armv7l_valid.log`。

## 9. 板端纪律与清理

所有 armv7l 运行均在 `192.168.108.26:26101` 真机原生执行，不使用
模拟器，不安装 RPM、不改系统库或配置、不重启。只使用
`/var/tmp/r53_20260820` 及三个共享区传输归档。收尾核验：部署目录与
三个归档均不存在，目标进程数为 0，见
`commands/041_board_cleanup.log`。已登记的
`/home/owner/r44b_push_probe` 未访问、未删除。

## 10. 未覆盖面

- aarch64；
- LTO、非默认 extern-template、静态 libc++、同名符号插入；
- 并发取消与混合运行时帧穿越；
- armv7l 上游 libc++ 测试 harness；
- 33 个可达站点中的逐站点动态触发（本任务动态覆盖 4 条代表路径）；
- `sentry` 析构站点的功能性正向结果（`UNREACHABLE_NOEXCEPT`）。

本报告不提出上传建议；未推送 Gerrit 或其他外部源码仓，也未修改
落章文档。
