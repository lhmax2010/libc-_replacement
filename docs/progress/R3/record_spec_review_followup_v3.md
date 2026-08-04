# Spec 复核补充 v3：LLVM runtimes 专用安装目录

## 触发证据

aarch64 的 v2 配置命令确实传入
`-DCMAKE_INSTALL_LIBDIR=/usr/lib64`，但 LLVM 21 runtimes 的安装日志仍把
`libc++.so.1.0` 和 `libc++abi.so.1.0` 写入 `%{buildroot}/usr/lib`，随后
RPM `%files %{_libdir}` 在 `%{buildroot}/usr/lib64` 找不到文件。

原文分别见：

- `../aarch64/build/gbs_build_v2.log:6050`（CMake 入参）；
- `../aarch64/build/gbs_build_v2.log:9648`（实际安装目录）；
- `../aarch64/build/gbs_build_v2.log:9837`（RPM 文件收集失败）。

源码侧，LLVM 21 的 libc++ 与 libc++abi 不以
`CMAKE_INSTALL_LIBDIR` 作为最终库目录开关，而分别使用
`LIBCXX_INSTALL_LIBRARY_DIR` 与 `LIBCXXABI_INSTALL_LIBRARY_DIR`：

- `libcxx/CMakeLists.txt:419,435`；
- `libcxxabi/CMakeLists.txt:193,206`。

## v3 diff 提案

把通用 CMake 变量恢复为相对目录 `%{_lib}`，并显式给两个 LLVM
runtimes 专用变量传同一个相对目录。这样 armv7l 为 `lib`，
aarch64/x86_64 为 `lib64`；不改变编译选项、ABI、版本脚本或依赖策略。

提案全文见 `spec_diff_proposal_v3.patch`。只有该提案先完成落盘和
`git apply --check` 后，才允许应用并重建。
