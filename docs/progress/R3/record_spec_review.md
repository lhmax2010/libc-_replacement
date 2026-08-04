# libc++/libc++abi 三架构 spec 复核

复核对象为 commit
`67e4b73a5f6a40545555ba6b1318807967f86678` 的
`packaging/libcxx-runtimes.spec`。复核时工作树干净，基线 spec SHA256
在 `spec_baseline.sha256`。

## 数据结论

- spec 没有硬编码 armv7l triple。C、C++、ASM compiler target 和
  `LLVM_DEFAULT_TARGET_TRIPLE` 都使用目标 buildconf 展开的 `%{_host}`。
- 安装目录由 `%{_lib}`/`%{_libdir}` 配对控制；aarch64/x86_64 不需要在
  spec 中硬编码 `/usr/lib64`。
- libc++abi 的 ARM EHABI、LP64 以及体系结构 builtins 由上游源码根据
  clang target 宏选择。aarch64/x86_64 都走 LP64/DWARF EH，不应继承
  armv7l 的 `_LIBCXXABI_ARM_EHABI`。
- `--rtlib=libgcc` 与 `LIBCXXABI_USE_LLVM_UNWINDER=OFF` 是三架构共同
  约束；是否真正闭合到各架构 `libgcc_s.so.1` 必须由产物门确认，不能仅
  从 spec 推断。
- Toolchain 的 Base/Unified 两个 repo 对三种架构目录均返回 HTTP 200，
  原始响应见 `raw/repo_arch_probe.log`。

## 预先冻结的初始最小改动

仅提议三项元数据改动：增加 `ExclusiveArch`、Release 1→2、补多架构
changelog。编译 target、汇编 flags、libdir、unwinder 和版本脚本均不改。
逐行提案见 `spec_diff_proposal.patch`；该文件在任何 spec 修改和构建之前
生成，满足“先列 diff，不边改边编”的约束。

## 实构建后的最终处置

64 位首次构建证明 LLVM runtimes 忽略了原有通用 libdir 设置；v2
尝试把 `CMAKE_INSTALL_LIBDIR` 改为绝对 `%{_libdir}` 后仍被忽略。
两次失败均完整保留，v2 已标为 superseded。

最终 v3 在修改前先冻结 `spec_diff_proposal_v3.patch` 并通过
`git apply --check`，然后显式设置 LLVM 21 使用的两个专用开关：
`LIBCXX_INSTALL_LIBRARY_DIR=%{_lib}` 和
`LIBCXXABI_INSTALL_LIBRARY_DIR=%{_lib}`。最终实测安装位置为：

- armv7l：`/usr/lib`；
- aarch64、x86_64：`/usr/lib64`。

最终实际 diff 为 `spec_actual_v3.diff`。它只包含 Release/架构范围、
两个安装目录接线项及 changelog；编译 triple、汇编选择、unwinder、
版本脚本和代码生成选项均未改变。
