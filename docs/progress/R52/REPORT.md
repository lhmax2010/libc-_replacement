# R52：层 C 头文件批（16 处）实现与验证

## 结论

结论为 `PARTIAL`。

16 处目标站点已全部实现，实际改动为 3 个头文件、47 行新增：6 个
`istream` 自由函数站点、9 个 `future` 模板站点、1 个
`basic_string::shrink_to_fit` 站点。`getline` 与 `packaged_task` 的正向
场景在 x86_64 原生和 armv7l 真机上均由退出 134 / SIGABRT 修复为退出
0，`pthread_join` 均返回 `PTHREAD_CANCELED`；只换入重编后的 libc++、
不重编消费者时仍为 134，实测确认消费者必须重编。

`string::shrink_to_fit` 的强制取消正向效果记 `NOT_OBSERVED`：两套标准
库的该成员均为 `noexcept`，构造出的取消会穿过 `noexcept` 帧；探索格
中 libc++ 与 libstdc++ 对照都退出 134，违反了本任务“调用链无
noexcept 帧”的用例前提，不能用来判定该站点的防护效果。因为 16 处
中尚有这一处缺少有效的强制取消正向实测，所以不报
`HEADER_BATCH_COMPLETE`。

## 1. 输入、基线与口径

- llvm 源码基线：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`；
  `codes/llvm` 在任务前后均无改动。
- R50 分类输入：`progress/R50/tables/armv7l_site_classification_34.tsv`，
  SHA256 `1312f266…c78c04d`。
- R46 站点输入：`progress/R46/tables/unaggregated_34.tsv`，SHA256
  `cf29138b…f203fa`。
- R51 回归基线：`progress/R51/tables/regression_matrix.tsv`，SHA256
  `45835fc8…aff9f`。
- R51 产品版 libc++abi：x86_64 SHA256 `f4f1c05a…81b591`；armv7l
  SHA256 `44672c2c…8c04d`。本任务未重建 libc++abi。
- 本任务重链的 libc++：x86_64 SHA256 `e600fc2e…1de0f`；armv7l
  SHA256 `5bffa634…43c69`。只重编 `ios.instantiations.cpp.o` 并必要重链；
  16 个目标定义本身仍由消费者头文件实例化。

完整 SHA、CMakeCache SHA 和仓状态见
`commands/047_baseline_and_input_identity.log`。

## 2. 类型可用性前置矩阵

两套标准库逐格一致，故继续实施：

- `const&` 与非 const 引用捕获均通过；
- 按值捕获、派生并实例化、`new`、`delete` 均编译失败；
- `sizeof` 在两侧都编译通过，与任务书给出的预期不同，但不存在两套
  标准库行为差异。抽象类和私有析构并不使类型不完整，因此 `sizeof`
  本身是合法的；原始编译输出未作推断补全。

详见 `tables/type_availability.tsv` 和 `raw/type_matrix/`。

## 3. 参照、设计与实现

libstdc++ 参照原文完整落于 `raw/reference_and_baseline_contexts.log`：

- `bits/istream.tcc` 在 forced handler 中先置 `badbit` 再重抛；
- `std/future` 的 task setter / async state 在 forced handler 中重抛，
  不把外来强制展开写入 `exception_ptr`；
- `bits/basic_string.tcc` 在 `shrink_to_fit` 中先捕获 forced 类型并重抛，
  普通异常仍由 catch-all 吞掉。

本实现与其语义对齐：`istream` 的 6 处先设置相同的 `badbit` 状态再
`throw;`；`future` 的 9 处在写 `exception_ptr` 前直接 `throw;`；
`string` 的 1 处在普通异常吞掉分支前直接 `throw;`。为使类型声明可见，
三个目标头文件各显式包含 `<cxxabi.h>`。

不使用统一宏。16 处虽然都有“forced handler 先于 catch-all”的共同
骨架，但 `istream` 需要保留状态更新，`future` 分为 value、void、
at-thread-exit 和 policy fallback，多处后续动作不同，`string` 又是
吞掉分配异常。以宏合并会隐藏这些不同的既有语义；本实现保持逐站点
显式写法。完整清单见 `tables/header_sites_16.tsv`。

补丁为 `patches/0001-libcxx-preserve-forced-unwind-across-header-catch-al.patch`，
SHA256 `8beb1bb0…dfec`。人工未指定 name/email，补丁暂以
`NOT_AVAILABLE <not-available@invalid>` 生成，提交外部源码仓前须由人工
`--amend`。

## 4. 场景矩阵

有效场景 `getline` 与 `packaged_task` 在两架构上结果完全一致：

1. 未打任何补丁：134 / SIGABRT；
2. 仅层 A+B、旧头文件消费者：134 / SIGABRT；
3. 层 A+B + 新头文件重编消费者：0，join 为 `PTHREAD_CANCELED`；
4. 只换入重编 libc++、消费者不重编：134 / SIGABRT；
5. libstdc++ 对照：0，join 为 `PTHREAD_CANCELED`。

完整逐格表见 `tables/scenario_matrix.tsv`，原始输出见
`raw/x86_64_scenario_matrix.log` 与 `raw/armv7l_scenario_matrix.log`。
两份原文均含 SHA256、`ldd` / `LD_DEBUG=libs` 身份佐证；armv7l 为板上
原生执行，未使用模拟器。

第 4 格证明头文件批确需重编消费者：新库不能改变已经内联或本地实例化
在旧消费者中的 catch 代码。

## 5. 回归与非取消行为

- R51 的 9 类场景 × 2 架构，共 18 个场景-架构行；每行的 libc++ 与
  libstdc++ 两侧均退出 0，join、析构和 handler 顺序一致。详见
  `tables/r51_regression.tsv`。
- 非取消路径在两架构上逐项对照：正常 `getline`、失败 `getline`、
  `packaged_task` 保存真实 `runtime_error`、正常 `shrink_to_fit`，改动
  前后均退出 0且观测值一致。详见 `tables/noncancel_matrix.tsv`。
- 选取 6 个上游 libc++ 测试（istream 2、future 3、string 1），在
  x86_64 上均编译退出 0、运行退出 0。命令、范围和原始输出见
  `raw/x86_64_selected_upstream_tests.log` 与 `tables/upstream_tests.tsv`。

## 6. 板端纪律与清理

每次部署前均确认目标目录不存在且相关进程数为 0。三轮板端载荷均部署
到独立的 `/var/tmp/r52_*_20260820`，结束后同时删除远端目录、共享区
tar 和相关进程；每轮清理核验均为文件不存在、进程数 0。未安装 RPM、
未修改系统库或配置、未重启或启停服务。

## 7. 已验证与未覆盖

已验证：x86_64 原生、armv7l 真机；动态 libc++ 场景；默认 extern
template、无 LTO；`getline` 与 `packaged_task` 的强制取消；R51 机制
回归；部分上游 iostream/future/string 测试；非取消行为。

未覆盖：aarch64、LTO、并发取消压力、更多自定义字符/traits/allocator
实例化、混合运行时帧穿越扩展矩阵；`string::shrink_to_fit` 的无
`noexcept` 干扰强制取消正向场景；armv7l 上游 libc++ 测试套件（该架构
由场景矩阵、R51 回归及非取消对照覆盖）。
