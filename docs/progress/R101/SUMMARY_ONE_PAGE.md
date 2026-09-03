# Tizen-Base-Toolchain libc++ 逐包方案：一页汇总

## 范围与数字

输入是 `tizen-base-toolchain_20260828.101647`，不是 R95 的 GCC 标准
`Tizen-Base`。292 条 source 记录按源码包名去重后是 **254 包**。

| 项目 | 数量 |
|---|---:|
| 全部源码包 | 254 |
| R100 四路证据确认含 C++ | 73 |
| 需要改 | 10 |
| 不需要改 | 236（181 个非 C++ + 55 个 C++） |
| 当前不能改 | 6 |
| 未定 | 2 |

需要改的 10 包：`abseil-cpp`、`bcc-tools`、`boost`、`bpftrace`、`icu`、
`jsoncpp`、`libsigc++`、`llvm`、`pcre`、`taglib`。

## 为什么采用逐包方式

- 逐包方式只触及 10 个已由实际符号边证实需要迁移的 spec。
- 全局方式会把定义暴露给全部 254 包，并额外触及 55 个已判定不需要迁移的
  C++ 包、6 个当前 GCC 包和 2 个未定包。
- GCC 14.2.0 在 armv7l/x86_64 实测明确拒绝 `-stdlib=libc++`，不是静默忽略；
  这 6 个当前 GCC 包会形成明确失败暴露面。aarch64 GCC driver 环境自身退出
  139，故该架构的同项实测是 `NOT_OBSERVED`。
- Clang 22.1.8 会无诊断接受该选项，但 `-###` 证明它实际选择 `-lc++`，不是
  “编过但选项没作用”。对 55 个不需要迁移的 C++ 包，全局注入会真实改变标准库。

## Base 内部边补查

121 条布局敏感 ELF 边归并为 4 个源码包对：`bcc-tools→llvm`、
`boost→boost`、`bpftrace→llvm`、`llvm→llvm`。四个 provider 全在 10 包集合内；
**`NEED_LIBCXX → NO_LIBCXX_NEEDED` 的布局敏感边为 0**。

## 实施与验证要点

每个目标 spec 显式声明 libc++ devel 依赖，只对 C++/链接加入
`-stdlib=libc++` 和 libc++abi 链接参数。Boost/PCRE 还需更新 devel 子包中现有的
`libstdc++-devel` Requires；LLVM 要把 LDFLAGS 合入其显式 CMake linker flags。

构建顺序：先发布 `libcxx-runtimes`，再构建 LLVM，之后构建 BCC/bpftrace 及其余
目标包。逐架构验证编译器、实际命令、`DT_NEEDED`、`GLIBCXX_*`、精确边界和
运行 smoke test。

## 尚未覆盖

- 精确符号边只在 armv7l 执行；
- 镜像归属只核对六份标准镜像清单；
- `gmp`、`tensorflow2` 是否被 consumer 实际静态链接仍未定；
- 四个目标仓已较 R100 快照前移，新实施 revision 需重新核对；
- 本任务未修改 spec、未构建、未推送 Gerrit。
