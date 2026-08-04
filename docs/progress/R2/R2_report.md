# R2：NO_OP 矛盾日志核查报告

状态：`DONE`。三选一裁决为 `INJECTION_EFFECTIVE_CONFIRMED`，仅针对有 21.1.1 candidate 实际日志的 `libcynara-commons`。没有重建、没有修改源码或 spec，也没有把该包证据外推成其他四包的实际构建结论。

## 冻结输入

唯一 candidate 日志为 `/home/toolchain/development/libc++_replacement_bak/wave1_execution/stageA/candidate_build/logs/01_libcynara-commons.raw.log`（`LOCAL_ONLY`，548,118 字节，SHA256 `07c898f78a5c415aacef0705736384d944f7f8983b3b066deeeaf4892414242e`）。candidate MANIFEST 也登记同一 SHA。所有日志、正式 buildconf、退役根 RPM 宏、spec 和宏求值文件的路径、大小与 SHA 见 `progress/R2/input_identity.tsv:1`。

正式 buildconf SHA256 为 `7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`；日志首部冻结命令明确以 `-D` 使用该文件，命令原文见 `progress/R2/commands/07_frozen_command_and_result.log:1`。

## 实际编译与链接

计数与逐字 cmp 见 `progress/R2/evidence_counts_and_cmp.tsv:1`：

- `-stdlib=libc++` token 总计 252 次；其中实际 clang++ 编译命令严格为 239 条。五条完整样例见 `progress/R2/evidence/compile_samples.raw.log:1`。
- `-lc++ -lc++abi` token 总计 6 次；去除首次 LDFLAGS 赋值后，实际链接命令严格为 5 条，五条完整原文见 `progress/R2/evidence/link_commands_all.raw.log:1`。

一条日志物理行同时串接了链接与下一条编译命令，因此 token 总数不能直接当作命令行数；本次分别按 clang++ `-c` 与链接 token 做命令分类，并对 239/5 设硬断言。

## 参数进入路径

完整链见 `progress/R2/provenance_chain.tsv:1`，关键原文摘录见 `progress/R2/evidence/macro_chain.raw.log:1`，`%build` 起始 shell trace 见 `progress/R2/evidence/build_prologue.raw.log:1`：

1. 正式 buildconf 的 allowlist 对 `libcynara-commons` 把 ` -stdlib=libc++` 接入 `%optflags`，把 ` -lc++ -lc++abi` 接入 `%build_ldflags`。
2. 退役根 `tizen_macros` 的 `%__spec_build_pre` 在 spec `%build` 正文前调用 `%set_build_flags`。
3. RPM `%set_build_flags` 在变量为空时令 `CXXFLAGS=%build_cxxflags=%optflags`，并令 `LDFLAGS=%build_ldflags`；日志中首次出现的 CXXFLAGS/LDFLAGS 就是这两条赋值和紧随其后的 export，没有更早的独立环境预置赋值。
4. 日志首次 CXXFLAGS 与正式 `%optflags` 宏求值逐字 `cmp=0`；首次 LDFLAGS 与正式 `%build_ldflags` 逐字 `cmp=0`。
5. spec 随后才执行 `CXXFLAGS+=" -Wl,-z,relro,-z,now"` 和 `LDFLAGS+=" -Wl,--rpath=/usr/lib"`。两次追加保留了已注入值；之后 CMake 生成的 239 条编译命令与 5 条链接命令携带相应 token。

所以实际路径分别是 `%optflags → %build_cxxflags → %set_build_flags → CXXFLAGS → CMake 编译命令` 与 `%build_ldflags → %set_build_flags → LDFLAGS → CMake 链接命令`。它们不是由 `%cmake` 再次直接展开宏，也不是 spec 文本直接引用 `%optflags/%build_ldflags`。

## 对照组与裁决边界

candidate 批次在 ordinal 1 的 libcynara-commons forced_unwind 编译失败后 fail-fast；security-manager 状态为 `UNDETERMINED_NOT_RUN_AFTER_RED`，没有 candidate 日志，见 `progress/R2/control_group.tsv:1`。因此 security-manager 对照为 `NOT_AVAILABLE`，没有伪造对照。

最终三选一记录见 `progress/R2/adjudication.tsv:1`：libcynara-commons 的 optflags 与 build_ldflags 均为 `INJECTION_EFFECTIVE_CONFIRMED`。原 T2 静态 NO_OP 疑点只在该包范围内被日志裁决取代，原文件保持不变，边界登记见 `progress/R2/supersession_registry.tsv:1`。本结论不改变 forced_unwind 编译失败事实。

## 只读闭环

提取与裁决命令原文、输出和退出码见 `progress/R2/commands/13_extract_and_adjudicate.log:1`；结束时日志、buildconf、spec 与原 T2 表 SHA 均未变化，源码仓的工作树和 index 都 clean，见 `progress/R2/commands/14_readonly_postcheck.log:1`。
