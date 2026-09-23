# 11 包已验证配方与 sandbox 对账

| 包 | sandbox SHA | 最近验收依据中选取的配方记录 | 结论 |
| --- | --- | --- | --- |
| abseil-cpp | `7a4f04dad6d30ba09c406837cdd23ad779a69210` | `docs/progress/R104/raw/178_build_abseil_x86_64_libcxx.stdout:30` | 仅非功能差异 |
| boost | `4168e873584ae50a12733262026c007dce18ec89` | `docs/progress/R105/raw/067_build_boost_x86_64_libcxx.stdout.txt:30` | 仅非功能差异 |
| icu | `dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330` | `docs/progress/R104/raw/308_build_icu_x86_64_libcxx.stdout:30` | 仅非功能差异 |
| jsoncpp | `9cbab147f44097d5b8da2b3ef38f75376a282d01` | `docs/progress/R104/raw/208_build_jsoncpp_x86_64_libcxx.stdout:30` | 仅非功能差异 |
| libsigc++ | `4f2f592d2f88eb6a3a949cec8970a4cd5d40165b` | `docs/progress/R104/raw/366_build_libsigc_x86_64_libcxx_retry.stdout:30` | 仅非功能差异 |
| pcre | `09a5bfc9a96c4235628ce800db3b95bf28ef2f23` | `docs/progress/R104/raw/288_build_pcre_x86_64_libcxx.stdout:30` | 仅非功能差异 |
| taglib | `8b552bdf27c913a93bca664c41c3fafb214962a1` | `docs/progress/R104/raw/268_build_taglib_x86_64_libcxx.stdout:30` | 仅非功能差异 |
| tensorflow2 | `6d76139029ca4162249667f0db65a73387a94cfe` | `docs/progress/R105/raw/098_build_tensorflow2_x86_64_libcxx.stdout.txt:31` | 仅非功能差异 |
| llvm | `f203923a1508c9344f5fc6b17bd8822f011655c4` | `docs/progress/ARM_W5_0921/cell_commands/w5-original.json` | 有功能差异未推送 |
| bcc-tools | `5bd0654e24bb231035ea684d278f674aa95a2cd2` | `docs/progress/BUILD_W2C_0915/build-w2c-bcc-tools-armv7l-libcxx.build.log:29` | 仅非功能差异 |
| bpftrace | `7831fb34c6881b5cff0b8bdd0dac27a1829290fa` | `docs/progress/BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json` | 一致 |

九包的唯一spec差异均是GBS导出的VCS字段；原文逐包见recipe-audit/<包>/SPEC.diff，行内容同时收录RESULTS.json。此类字段记录源码身份，不改变构建命令；不纳入LLVM提交。bpftrace spec与patch完全一致。LLVM主spec唯一功能差异为LIBCLANG_BUILD_STATIC=ON，已创建本地未推提交。

共核对9份packaging中的.patch/.diff文件（boost 1、icu 6、pcre 1、bpftrace 1），与保存的实际导出输入SHA完全一致；未发现导出目录额外patch。其余包packaging下没有独立.patch/.diff文件，不代表源码tar内没有已合入补丁。全目录各文件哈希和缺项见各result.json；LLVM的附带libcxx-runtimes.spec与LLVM构建SOURCES中的那份仅差VCS（raw/035），但那份不是bpftrace静态运行库输入实际用的候选！后者另见UNPUSHED_LIBCXX_RUNTIME.diff。

范围：按最近作为验收依据的轮次选取其保留构建根的导出spec/patch（通常最后x86_64 libc++格；bcc取最后ARM libc++格；LLVM与bpftrace取最新完整RPM轮）。R105沿用R104 libsigc++构建，R108的tensorflow2判据沿用R105 libc++候选、不拿GCC基线夹具冒充候选。不是本轮重建，也不是逐个源码tar全部成员与Git树相等的证明；导出路径由历史日志定位，本轮SHA为保留文件现值。Release在所选副本中未产生差异。

本轮11个远端均重新ls-remote确认，远端未变；全部只读git show，不fetch到codes。当前不能据此关闭QuickBuild前置：LLVM本地主spec补齐未推，指定GCC override检查失败，libcxx-runtimes静态配方还未进入该sandbox。
