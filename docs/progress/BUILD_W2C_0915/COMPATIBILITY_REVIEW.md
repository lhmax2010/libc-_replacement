# 11 包的标准库选项隔离核对

本表针对各包已验证提交中的 spec；bcc-tools 使用本轮已推送提交
`5bd0654e24bb231035ea684d278f674aa95a2cd2` 的同一候选内容，
bpftrace 使用尚未提交的候选。其余九包未重新构建，spec 与上一轮钉住
revision 的快照逐字节一致。输入清单及完整内容见 spec_audit_before/。

共同启用判据的实际代码如下（bpftrace 候选第1–5行；其他包的具体行号
与原文逐项保存在 guard_context_audit.json）：

```spec
%if %{defined _toolchain}
%if %{toolchain_is clang}
%define build_with_libcxx 1
%endif
%endif
```

新增标准库依赖、编译/链接选项位于 `%if 0%{?build_with_libcxx}` 的
正分支中；tensorflow2 还要求自己的 USE_CLANG 开关为 ON。

| 包 | 条件与参数传播核对 | GCC 实测证据的范围 |
| --- | --- | --- |
| abseil-cpp | 条件内添加开发依赖、CXXFLAGS、LDFLAGS | 沿用原三架构记录，本轮未重跑 |
| boost | 条件内追加选项；首次构建与安装使用保存/读回的同一组参数 | 沿用原三架构和子包一致性检查 |
| icu | 条件内追加，保留原有其余参数 | 沿用原三架构记录 |
| jsoncpp | 条件内添加依赖与 flags | 沿用原三架构记录 |
| libsigc++ | 条件内追加，随后交给 Meson | 沿用原三架构记录；libc++ 不以强留 DT_NEEDED 为判据 |
| pcre | 编译 flags 与 devel 标准库 Requires 均受同一条件控制 | 沿用原三架构记录 |
| taglib | 条件内添加依赖与 flags | 沿用原三架构记录 |
| tensorflow2 | 另要求 USE_CLANG=ON；仅条件内向 CLANG_CXXFLAGS、LDFLAGS 追加 | 沿用原真实 GCC 分支记录；armv7l 是与未改基线同点同因失败，不是完整构建成功 |
| llvm | 条件内启用 LLVM_ENABLE_LIBCXX、libc++abi 链接选项及新 AOT 资产选择 | 沿用已闭合的十二格；本轮只补建 armv7l libc++ 输入 |
| bcc-tools | 新增15行中的开发依赖与 flags 均受共同条件控制 | 本轮 armv7l 真实 GNU 14.2.0 完整构建、五次核查；aarch64/x86_64 沿用已通过记录 |
| bpftrace | 新增15行同样隔离；Source1002 及其安装逻辑不改 | 本轮四格结果见各 checks/bpftrace_*；最终状态见 STATUS.md，不由本表预判 |

## 核查方法与边界

1. 逐个保留完整 spec、revision 与 SHA256，并检查所有直接写入的
   `-stdlib=libc++`、`-lc++abi`、`LLVM_ENABLE_LIBCXX=ON` 和 libc++ 依赖声明
   所在条件。机器清单包含每处的行号、实际原文和外围条件栈。
2. 条件枚举工具使用四个小样本校准：正确的双条件、无条件选项、
   else 分支中的选项、缺少 Clang 判据的定义。后三者被识别为不满足条件。
   结果为11包字面选项条件检查通过（raw/136）。这不是 RPM 宏求值器，
   也不替代人工阅读或真实构建。
3. bcc-tools 本轮 GCC 完整构建日志中 `-stdlib=libc++`、`-lc++abi` 为0，
   两个运行时 ELF 依赖 libstdc++、不依赖 libc++，五次检查一致。
   bpftrace 各格完整日志和逐程序结果另列；未完成格不得借用别格结论。
4. “选项隔离”不等于“所有历史改动均在条件内”。例如 Boost 保存/读取参数
   的语句在条件外，但不会自行生成 libc++ 选项。不能据本表保证任意外部
   CXXFLAGS/LDFLAGS 注入或任意工具链宏覆写组合都被自动过滤。
5. GCC 构建配置包含历史使用的 libc++ devel 支持仓，以处理依赖预解析器
   对条件依赖的处理；构建根里出现该开发包，不等于 GCC 命令使用它。
   判定依据是实际编译器、完整选项、最终 ELF，而非仅看安装包列表。
6. bpftrace 的预编译 static 子包不会被这些 flags 重编。其原资产与已检查
   RPM 内二进制逐字节一致，但内嵌 C++ 标准库未观测，单列待确认，
   不纳入“源码程序已改用 libc++”的结论。

历史实测索引：../BUILD_W2_0912/PACKAGE_STATUS.tsv、
../BUILD_W3_0909/COMPATIBILITY_AUDIT.tsv、../BUILD_W1B_0909/MATRIX.tsv。
本轮新证据见 bcc_gate.json、checks/ 与 raw/096–105、131、137、147 等。
