# spec 条件保护扫描

命令原文：

```bash
chmod +x docs/progress/BUILD_W3_0909/code/audit_libcxx_guards.sh
docs/progress/BUILD_W3_0909/code/audit_libcxx_guards.sh /home/toolchain/development/libc++_replacement
printf 'audit_exit=%s\n' "$?"
```

输出：

```text
package commit spec definition_guard libcxx_payload_guard libcxx_token_lines
abseil-cpp 7a4f04dad6d30ba09c406837cdd23ad779a69210 packaging/abseil-cpp.spec PASS PASS 33,34,58,59
boost 4168e873584ae50a12733262026c007dce18ec89 packaging/boost.spec PASS PASS 86,87,115,478,479
icu dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 packaging/icu.spec PASS PASS 34,35,88,89
jsoncpp 9cbab147f44097d5b8da2b3ef38f75376a282d01 packaging/jsoncpp.spec PASS PASS 22,23,47,48
libsigc++ 4f2f592d2f88eb6a3a949cec8970a4cd5d40165b packaging/libsigc++.spec PASS PASS 24,25,51,52
pcre 09a5bfc9a96c4235628ce800db3b95bf28ef2f23 packaging/pcre.spec PASS PASS 23,24,39,116,117
taglib 8b552bdf27c913a93bca664c41c3fafb214962a1 packaging/taglib.spec PASS PASS 17,18,41,42
tensorflow2 6d76139029ca4162249667f0db65a73387a94cfe packaging/tensorflow2.spec PASS PASS 85,86,180,181
audit_exit=0
```

输出列在终端中由制表符显示为间距；结构化结果已写入
`COMPATIBILITY_AUDIT.tsv`。扫描证明列出的 libc++ payload 行位于
`%if 0%{?build_with_libcxx}` 内，且该宏只在 `_toolchain` 已定义与
`toolchain_is clang` 嵌套条件内定义。它不证明整个包的运行时行为；运行时
兼容性引用此前的实际 GCC 构建证据。

LLVM 未提交候选另行只读检查：

```bash
nl -ba tmp/BUILD_W1_0908/llvm-worktree/packaging/llvm.spec | rg -C 4 'build_with_libcxx|LLVM_ENABLE_LIBCXX|mlgo_.*libcxx|libc\+\+-devel|libc\+\+abi-devel'
```

退出码 `0`。候选在第 18–22 行以同一 `_toolchain`/clang 条件定义宏，
BuildRequires、资产选择和 `LLVM_ENABLE_LIBCXX=ON` 均依赖该宏；但候选未
提交、完整兼容门禁未过，故不列为最终通过。
