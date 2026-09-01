Addressed by adding `%define _toolchain_override clang` near the existing LLVM
version macros, following the convention already used by the platform
`llvm.spec`.

This package is intentionally Clang-only because its runtime configuration
passes the Clang driver option `--rtlib=libgcc`. The existing CMake compiler
macros, target triples, and cross-compilation settings remain unchanged.

The spec-local definition overrides a project- or command-line-provided
`_toolchain_override gcc`; it does not conflict or report an error, and
`%{__cc}` / `%{__cxx}` still expand to the target-prefixed Clang drivers. This
was verified by preprocessing the spec with an external GCC override.

An x86_64 GBS build completed successfully with Clang 22.1.8. The resulting
`libc++.so.1.0` and `libc++abi.so.1.0` both have `DT_NEEDED` for
`libgcc_s.so.1`, and the five patched development headers are byte-identical to
the previously validated QuickBuild package.

中文说明：已按平台 `llvm.spec` 的惯例在 spec 顶部定义
`_toolchain_override clang`。外部即使设置为 GCC，也会被本 spec 的定义覆盖，
最终仍使用目标前缀的 Clang，不会发生宏冲突。交叉编译的 target 设置未改；
x86_64 实际构建、libgcc_s 展开器依赖以及五处头文件均已核验通过。
