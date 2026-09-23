# 11 包最终状态

11/11 已发布源码适配；本轮未推包仓。历史验证沿用，不重计为本轮新构建。bpftrace static 候选仍不可作为已验收替换件。

| package | branch | commit | published_status | prior_validation_reused | spec_guard_check | remote_head_match | candidate_notes |
| --- | --- | --- | --- | --- | --- | --- | --- |
| abseil-cpp | sandbox/lhmax2025/libcxx-migration | 7a4f04dad6d30ba09c406837cdd23ad779a69210 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| boost | sandbox/lhmax2025/libcxx-migration | 4168e873584ae50a12733262026c007dce18ec89 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过；全部子包一致 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| icu | sandbox/lhmax2025/libcxx-migration | dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| jsoncpp | sandbox/lhmax2025/libcxx-migration | 9cbab147f44097d5b8da2b3ef38f75376a282d01 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| libsigc++ | sandbox/lhmax2025/libcxx-migration | 4f2f592d2f88eb6a3a949cec8970a4cd5d40165b | 已完成并推送 | 三架构×libc++/GCC 六格完整通过；libc++ 以编译命令和 std::__1 痕迹确认 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| pcre | sandbox/lhmax2025/libcxx-migration | 09a5bfc9a96c4235628ce800db3b95bf28ef2f23 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| taglib | sandbox/lhmax2025/libcxx-migration | 8b552bdf27c913a93bca664c41c3fafb214962a1 | 已完成并推送 | 三架构×libc++/GCC 六格完整通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| tensorflow2 | sandbox/lhmax2025/libcxx-migration | 6d76139029ca4162249667f0db65a73387a94cfe | 已完成并推送 | libc++ 三格及 aarch64/x86_64 GCC 两格通过；armv7l GCC 与未改基线同点同因失败，按相对基线无回归判据通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| llvm | sandbox/lhmax2025/libcxx-ehabi-backport | 5c169afc97945bfb88786d9f38477f6973fe5285 | 已完成并推送 | 三架构 libc++、MLGO 执行、GCC、非 libc++ 同环境对照十二格通过 | PASS（字面条件审计） | MATCH | 新增 libclang.a 与 libc++/libc++abi 静态库，配合 bpftrace static；LLVM_W4_0923 R4已核实人工签字推送与候选树一致；历史构建记录保留 |
| bcc-tools | sandbox/lhmax2025/libcxx-migration | 5bd0654e24bb231035ea684d278f674aa95a2cd2 | 已完成并推送 | aarch64/x86_64 的 libc++/GCC 四格通过，沿用；本轮armv7l libc++/GCC完整构建及各五次核查通过 | PASS（字面条件审计） | MATCH | 不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编 |
| bpftrace | sandbox/lhmax2025/libcxx-migration | 7831fb34c6881b5cff0b8bdd0dac27a1829290fa | 已完成并推送（源码适配）；static 配方已由人工推送 | armv7l/aarch64 × libc++/GCC 源码四格完整通过；历史 static 单列状态见旧提交；新版两架构 RPM/ELF 与 ARM 板测见 BPF_STATIC_0922、BPF_W3R_0922；x86_64 不在 ExclusiveArch | PASS（沿用字面条件审计） | MATCH（QB_PRECHECK_0922 实查） | static 子包改从源码构建，配方见 BPF_STATIC_0922/recipe；GCC/未定义路径保留 Source1002；正常 RPM 安装验证留待镜像阶段 |
