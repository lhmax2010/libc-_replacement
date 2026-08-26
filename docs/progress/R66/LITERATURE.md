# R66 上游文献与公开迁移记录编目

## 范围与方法

检索范围限定为 libc++、GCC/libstdc++、Android NDK 的官方文档，LLVM
公开 issue，以及 Chromium 的公开源码/迁移提交。检索日期为
2026-08-26。逐项清单见 `tables/upstream_behavior_inventory.tsv`；URL
获取和退出码见 `commands/034_upstream_source_fetch.log`。GCC 页面直接
curl 返回 403、Chromium 某提交直取返回 504，未把空响应当证据；相应内容
使用浏览检索取得的官方页面和仓内 GCC 源码作交叉依据。

## 编目结果

14 项公开记录中，真正只在运行期或装载后暴露的类别包括：

- 混用两个 STL 或在多个 DSO 静态复制 runtime 后的异常、RTTI、分配释放
  与全局流状态分裂；链接器只能捕获其中一部分；
- libstdc++ dual ABI 中不进入 mangled name 的对象布局差异；
- libc++ ABI 配置宏、可见性和 runtime pairing 不一致造成的同名类型或
  状态形态差异；
- `std::atomic` 的布局/对齐取决于编译器与库组合；
- 标准允许的实现差异，如 deque 分块/保留策略、filebuf/stringbuf 具体
  行为；它们通常表现为运行时内存、性能或边界行为，而不是链接错误；
- 静态 ABI runtime 拓扑中的 iostream 初始化状态分裂。

编译/链接期为主的记录包括 clang 驱动搜索路径、GLIBCXX/CXXABI 版本节点
不满足、以及迁移项目直接依赖实现私有 API。Chromium 的公开迁移还显示，
一次标准库替换会同时触发编译失败、GPU/运行测试失败和静态初始化位置
改变；不能以“LLVM 自身常用 libc++ 构建”代替本平台运行验证。

## 本平台适用性

本平台保留 glibc，因而 Android/Bionic 的系统库和打包结论不能直接移植；
但 Android 官方所述“一进程一套 STL”、异常/RTTI/分配跨 DSO 不兼容是
C++ ABI 拓扑约束，仍适用。Linux/glibc 的 Chromium 记录更接近本平台，
但 Chromium 采用自带 libc++、特定 `-fno-exceptions/-fno-rtti` 与构建
封装，不能据此推断 Tizen 包已覆盖。

本项目的组合是 libc++ + libc++abi + libgcc_s + glibc，且有 ARM EHABI
先导架构和本地标准库补丁。libc++ 官方 vendor 文档允许多 ABI runtime
组合，只证明“可配置”，不证明本组合的取消、外来异常或 EHABI 行为已由
上游覆盖。线程取消是这一区别的已实测样本。

## 文献方法的盲区

- issue 检索不能证明“无 issue 即无差异”；关键词未命中的条目仍可能存在。
- 标准允许的行为差异数量没有有限全集，官方 implementation-defined 页面
  也不包含 unspecified/quality-of-implementation 的全部差异。
- 文献中的其他操作系统、编译器版本和 ABI 配置均需重新判断适用性；本表
  明确记录了适用条件，没有把公开案例记成平台实测。
- 运行时并发、取消、locale、动态加载与第三方插件组合仍需主动测试，文献
  只能提供风险类别。
