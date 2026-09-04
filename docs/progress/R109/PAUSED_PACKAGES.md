# `llvm`、`bcc-tools`、`bpftrace` 暂停说明

## 阻断点

LLVM spec 引入的三份 `mlgo_{arm,aarch,x86}_model.tar.gz` 是源码包随附的
预编译输入，不是本次 LLVM 构建现场生成。每份资产含 MLGO 模型对象、
TensorFlow/XLA runtime 对象、sysroot 和预编译归档；spec 在 `%prep` 解包后
直接把这些对象嵌入 LLVM。

现有对象按 libstdc++ ABI 构建，C++ 签名中的字符串等实体编码为
`std::__cxx11`。LLVM 改用 libc++ 后，新目标代码请求 `std::__1` 版本的符号。
两者 Itanium mangled name 不同，链接器因找不到定义而失败。这是“预编译 C++
二进制资产 + 迁移后代码”的内联命名空间不兼容，不是普通缺库，也不是可以由
链接器自动桥接的同名接口。

## 为什么三包必须协调

已保存的真实符号边显示：

- `bcc-tools → llvm`：584 个布局敏感 C++ 符号；
- `bpftrace → llvm`：201 个布局敏感 C++ 符号；
- LLVM 包内部还有 17,589 个布局敏感符号。

只迁移 LLVM 会先与旧 MLGO/XLA 对象发生链接错误；只迁移 `bcc-tools` 或
`bpftrace`，又会让其与仍使用 libstdc++ 的 LLVM 在同一进程跨边界交换布局敏感
C++ 类型。因而三包的实施提交可以独立，但生效和验证必须按同一 ABI 状态协调，
改一半比三者暂留原状态风险更大。

## 解锁条件

暂停可在以下事实闭合后解除：

1. 用与目标 LLVM 一致的 libc++ 工具链/ABI 重新生成三架构所需的 MLGO/XLA
   预编译 C++ 对象和归档；禁用 MLGO 不在既定方案范围内。
2. 对重建资产做架构、符号定义/引用和 `std::__1` / `std::__cxx11` 身份核验，
   并证明它们能被迁移后的 LLVM 完整链接和运行。
3. 按 `libcxx-runtimes → llvm → bcc-tools → bpftrace` 的依赖顺序完成构建，
   再核对 584/201 条边的两端使用同一标准库和 ABI。

现有报告还记录了一种需要新设计的纯 C opaque-handle 桥接可能性，但当前代码
没有该接口，且涉及所有权、异常、错误传递、性能和双运行时；它不是本轮可直接
采用的现成功能。全平台预编译资产的系统性扫描也尚未完成，候选全集记为
`NOT_OBSERVED`。

