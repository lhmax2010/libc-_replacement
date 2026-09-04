# 强制展开识别子句统计口径说明

这里的“识别子句”按源码语法定义为捕获
`abi::__forced_unwind` 或 `__cxxabiv1::__forced_unwind` 的
`catch`/`__catch` 子句；“传播站点”还要求 handler 通过裸 `throw;` 或
libstdc++ 的 `__throw_exception_again` 宏继续传播。站点以源码实例、包内相对
路径和行号共同标识。

## “50”最初怎样得出

原数字来自一轮 371 个已成功解包源码 RPM 的词法扫描。扫描器先找
`abi::__forced_unwind`/`__cxxabiv1::__forced_unwind` token，再把 token
所在行附近 16 行内、下一处 catch 之前出现裸 `throw;` 的行标为
`CONFIRMED_RETHROW`。从原表筛选该状态且排除文档路径，得到 50 行，分布于
10 个源码 RPM 实例、8 个去重包名。

逐行回看源码后，确认这个 50 **不是 50 个严格意义上的 catch 子句**：

- 46 行确实是 catch 子句；
- 4 行是 `key-manager` handler 内日志字符串
  `"abi::__forced_unwind caught..."`，因为紧邻真实 catch 和 `throw;` 而被启发式
  误标；
- libstdc++ 用 `__throw_exception_again` 表示重抛，原判定只认裸 `throw;`，因此
  漏掉 126 个实际传播子句；
- 原文件后缀白名单没有包含 `future`、`ostream`、`condition_variable` 等无扩展名
  C++ 头文件，这也是宏重抛漏项的一部分。

所以，“50 processing distinct sites”准确地说是“旧启发式筛出的 50 个 token
行”，不能称为 50 个独立识别子句。完整 50 行及四个误计行见
`REPORTED_50_AUDIT.md`。

## 按语法和传播行为重新计数

对原始冻结源码实例进行无扩展名限制的文本复核，结果为：

| 口径 | 数量 | 说明 |
|---|---:|---|
| 所有识别 catch 子句 | 175 | 11 个源码 RPM 实例、9 个去重包名；含 3 个不重抛的 Dali 子句 |
| 继续传播的识别子句 | 172 | 46 个裸 `throw;`，126 个 `__throw_exception_again` |
| 按包名、路径、行号和子句形态去除重复源码 RPM 变体 | 104 | 10 个源码 RPM 实例归并为 8 个包名；保留不同资产路径 |

`dali2-csharp-binder` 有 3 个语法上明确的识别子句，但 handler 返回而不重抛；
因此它属于“存在识别子句”，不属于“正确继续传播”。

## 为什么是 10 个源码 RPM、8 个包名

两个软件各有两个源码 RPM Release 变体，而每对变体来自同一 Git revision：

- `gcc-14.2.0-1.12.src.rpm` 与 `gcc-14.2.0-1.14.src.rpm` 均来自
  `3d5dd5bec16591dbf3277df37fb43e9cc5b890de`；
- `tensorflow2-2.18.0-1.2.src.rpm` 与 `tensorflow2-2.18.0-1.3.src.rpm` 均来自
  `1cdba73549f741720f11dd53da1bb516b03763a2`。

因此源码 RPM 实例数为 10，按软件包名归并后为 8。旧 50 行中，这两对重复实例
贡献 5 个重复行，按旧表自身去重后恰为 45；这也是早期资料中 45 与 50 并存的
来源。它们并非两个互相独立的软件实现。

## 当前复核

对上述八个历史阳性包的当前开发分支重新扫描，得到 140 个“识别并继续传播”
站点：Boost 22、GCC 66、key-manager 5、libcynara-commons 1、LLVM 42、
security-manager 1、tensorflow2 2、webauthn 1。

与原始源码按包名/逻辑路径去重后的 104 处相比，净增 36 处，全部位于 LLVM：
当前 `tizen_base` 已包含 libc++ 的强制展开防护，新增 34 个头文件本地 handler
和 2 个 `ios.cpp` 汇聚 handler；原有三架构 MLGO 资产中的 6 个 pybind11 副本
仍在。这里的 36 个 handler 对应更早分析中的 50 个 libc++ catch-all 处理位置：
其中 34 个需本地 handler，另 16 个调用点汇聚到 2 个 out-of-line handler，
所以“处理位置 50”不等于“新增 typed catch 子句 50”。

这次当前复核只覆盖历史已知阳性的八个包，用于核对站点漂移；没有对当前平台
全部源码包重新冻结和扫描，因此“其他包是否新出现识别子句”为
`NOT_OBSERVED`，不能把 140 表述为当前全平台总数。
