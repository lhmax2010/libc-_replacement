# R66 运行时差异主动发现手段评估

## 工具链自举作为发现手段

以本平台配置的 patched libc++ 构建 LLVM/Clang在技术上可行，且能把大量
真实 C++ 消费代码压过编译器、链接器、文件系统、线程与插件边界。其新增
价值不是“libc++ 能编译 LLVM”——上游已经持续覆盖这一点——而是固定为
本项目组合：glibc、libgcc_s、共享 libc++abi、平台编译选项，以及本项目
修改后的 libc++。

它主要暴露：实现私有 API、缺失扩展、错误 include/search path、符号版本
和链接闭包、构建配置分叉，以及启动/运行工具时触发的部分 ABI 问题。其
局限同样明确：标准库的 catch、失败回退、取消、future 状态机等路径只有
在故障被触发时才执行；“整套 LLVM 构建成功”不能替代这些运行判据。ARM
交叉构建只能提供编译/链接覆盖，不能替代真机 EHABI 运行。

本任务只评估，没有启动构建，因此没有调用资源门禁。

## 上游测试套件

冻结源码中静态计数为：libc++ 10,028 个测试文件，其中 8,620 个
`*.pass.cpp`；libc++abi 98/71；libunwind 30/17。数量是源码文件数，不是
当前配置的可运行分母，因为 lit 会按语言版本、feature、平台能力和
`UNSUPPORTED` 条件选择。

本机当前 PATH 没有 `llvm-lit`/`lit`，但有 CMake/Ninja，LLVM 源码带测试
基础设施。x86_64 可以通过 runtimes 配置生成 lit 环境并原生运行；全量
涉及数千编译和运行，成本显著高于项目此前的选择性测试。armv7l 需要宿主
交叉编译、把每个测试及依赖复制到板端并执行。上游提供 SSH/ADB executor
脚本，而本板使用 SDB，因此需要适配 executor；板上无编译器/lit。

当前实测下界来自 R58：两架构各跑通 10 个展开相关 libc++abi 测试；
x86_64 跑通 7 个选定 libc++ iostream/future/string 测试；armv7l 对应
libc++ 源码测试集合因未生成同构预编译集合而 `NOT_AVAILABLE`。**全量在
两架构各能跑多少目前是 `NOT_DETERMINED`**；不以 8,620 当作可运行数。
确定该数需要先完成每架构 lit 配置/feature probe，再以 PASS/FAIL/
UNSUPPORTED/UNRESOLVED 汇总。

可能被排除或需适配的类别包括：依赖宿主工具或 shell 的测试、locale/tzdb
数据、终端、文件系统权限、长时间/大内存测试、目标未提供的功能，以及
只能在编译宿主执行的生成步骤。哪些具体测试落入各类必须由 lit 实跑决定，
本任务未推断补全。

## 其他主动手段

`tables/runtime_discovery_methods.tsv` 列出九种机制的发现能力、成本形态、
平台适用性与盲区。它们提供不同观察面，不构成优先级或方案推荐：

- ELF/版本/RTTI 扫描适合在运行前堵住错误 runtime 和预制边界，但不看
  同名语义；
- 差分设施探针与 fault injection 能直接压运行时分支，但需可靠夹具和
  标准允许差异的归一化；
- differential fuzzing 扩大输入空间，但必须区分合规的 unspecified 差异；
- loader 绑定观测适合插件和重复 typeinfo；
- sanitizer/压力测试找后果，不等同语义 oracle；
- 371 包真实重建可最大化编译/链接发现率，仍不运行产品业务路径。

## 自行判断与疑问

- 自行判断：把“可运行测试数”定义为 lit feature probe 后实际进入执行的
  分母，而不是仓内 `*.pass.cpp` 文件数；否则数字会虚高。
- 自行判断：工具链自举只作为发现机制，不把成功结果当运行时兼容证明。
- 尚存疑问：板端能否稳定承受上千次 SDB 部署/执行、允许哪些临时路径与
  进程数，本任务没有连接开发板，记 `NOT_OBSERVED`。
- 尚存疑问：全量测试期望覆盖哪些 C++ 标准档位和 libc++ ABI/hardening
  配置；不同矩阵会改变分母和成本，需要人工定义后才能给精确数字。
