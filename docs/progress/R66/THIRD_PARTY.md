# R66 第三方库、测试框架与预编译组件

## 二进制 ABI 耦合面

本节复用 R22 的完整 ELF `DT_NEEDED + UND/DEF` 相交证据，不把单纯
`DT_NEEDED` 当 C++ ABI 耦合。`code/summarize_cpp_provider_edges.py` 对
`TRUE_CPP_ABI_COUPLING` 按 provider source RPM 聚合，结果见
`tables/cpp_provider_edges.tsv`，命令/退出码见 `commands/036`。

R22 迁移范围内共出现 82 个有真实 C++ 符号相交的 provider source RPM
身份、383 个 consumer source RPM 身份。这里既包含 libstdc++ 自身，也
包含平台库和第三方库，不能把 82 全称为“第三方”。剔除两个 GCC runtime
身份后，机械清单中显著的跨包 C++ ABI provider 包括：

| provider | consumer 源码身份数 | 不同 C++ 符号数 | 形态 |
|---|---:|---:|---|
| gtest 1.15.2 | 125 | 125 | gtest/gmock 共享库 |
| Boost 1.83 | 13 | 104 | filesystem/iostreams/program-options/test/thread 共享库 |
| ICU（按三架构 release 身份） | 10 | 1,021/1,032 | C++ 库 ABI |
| Abseil | 8 | 299 | 多组件 C++ ABI |
| Protobuf | 7 | 257 | protobuf/compiler/lite |
| LLVM/Clang | 6 | 6,388 | 大型 C++ ABI 面 |
| gRPC | 2 | 28 | C++ shared provider |
| TBB | 2 | 54 | C++ threading ABI |

平台自身的 `bundle`、`aul`、`app-core`、`rpc-port` 等也在清单中出现，
原因是 provider DSO 的导出和 consumer UND 有真实 C++ 符号相交；这类
“表面 C API、provider 内部泄漏 C++ ABI”的边界会在重建/换 runtime 时
形成与第三方库同类的二进制约束。

## 测试框架

R23 的 source-RPM metadata 口径识别出 gtest/gmock 源码框架 1/1，gmock
由 gtest 同源产出。三架构 RPM 均为共享 `libgtest/libgmock`，devel 包提供
头、CMake/pkgconfig，不提供静态库或源码 payload。1,450 个源码包实例中
153 个 BuildRequires gtest/gmock；这是构建依赖口径。R22 符号相交的 125
个 consumer 是运行二进制 ABI 口径。两个数字分母和判据不同，不互相替代。

已知 Boost.Test 首批失败正属于这种共享/编译产物 ABI 不一致：头与库或
消费者与 provider 不是同一 STL 形态时，会在链接期暴露缺符号，也可能在
测试运行时暴露对象/异常不一致。

## 预编译组件

厂商四包的源码可重建性另见 `VENDOR_AUDIT.md`。唯一由本次提供的 source
RPM 技术上无法重编的是 `pen-wave`（1 个源码身份、2 个二进制包名）；其
五架构 DSO 明确依赖 `libstdc++.so.6`、GLIBCXX/CXXABI 和大量 std/RTTI
符号。该对象不能靠重新编译消费者消除依赖，属于运行/装载边界。

R23 还在冻结平台二进制中识别出 1,289 个带 GCC `.comment` 的 ELF、490
个二进制包实例、160 个源码 RPM 身份。`.comment` 只证明编译器生产者，
不能证明所有权、是否闭源或不可重编；本报告没有把它们追加成 160 个
“预编译永久约束”。

## 暴露阶段和后果

- provider mangled name/版本节点不满足：链接期或 loader 阶段；
- consumer 与共享测试/第三方库用不同 STL 对象模型：链接期可见一部分，
  layout、RTTI、exception_ptr、allocator ownership 在运行期才可见；
- C API provider 自身携带旧 libstdc++：调用边界可能是 C-safe，但同一
  进程仍加载两 runtime；是否产生跨 runtime 对象/异常必须逐边确认；
- 仅测试包受影响会阻断构建验证而非产品运行，但会使迁移无法按原测试门禁
  收官。

## 限制

- 82/383 来自 R22 的 483 包迁移/发布 ELF 图，不是本轮 371 解包源码扫描
  的分母；清单复用时保留了这一口径。
- 符号相交能证明 ABI 使用，但不能仅凭 provider 名判断该符号是否跨公开
  API 传对象；运行影响需要场景。
- 未重新构建第三方库，新的 libc++ 产物结果 `NOT_OBSERVED`。
