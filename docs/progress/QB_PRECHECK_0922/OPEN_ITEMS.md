# 已知挂账对 QuickBuild 的影响

| 挂账 | 影响判断（不处理） | 依据 |
| --- | --- | --- |
| 5 份 libclang.a 身份不同 | 影响输入追溯/字节可复现性；差异未归因，不足以预报构建失败或 ABI 错误，QuickBuild 新产物须另登记身份，不能强称等于第五份 | `docs/progress/ARM_W5_0921/FINAL_RESULT.md` 第五份身份表；第五份 `5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a` |
| gmp 判定依据 | 不构成已知构建阻断；无需适配结论仅覆盖已核消费方/快照，不能外推所有未来消费者，QuickBuild 若改变消费关系需重核 | `docs/progress/R103/REPORT.md:9,73`：eigen 为头文件，python-pycrypto 实际 `-lgmp`，无 libgmpxx/libstdc++ 依赖、强符号交集 0 |
| -lstdc++fs | 仍是配方链接输入与 ABI 审阅项，QuickBuild 需提供该归档并核最终依赖/符号；不能以其名称推导已动态链接 libstdc++，也不能证明所有归档成员被拉入 | `docs/progress/BPF_STATIC_0922/DECISIONS.md:6,12–13`：来自 src/CMakeLists.txt:188–191，两架构实测解析 GCC14.2 目录下 libstdc++fs.a |
| RPM 根修复未进入平台包 | 根缺陷仍待修，但本地项目/保留根w5T及三份现有RPM的5T不触发；失败来自本地T1覆盖，与libc++无关、不在sandbox；QuickBuild无需payload对策，本地继续w5且禁止T<数字>，OBS当前宏仍未观测 | PAYLOAD.md；RPM_PAYLOAD_HEADERS.json；upstream-rpmio.c:765–777 |
| ARM static-libgcc 崩溃未复现 | 不构成编译阻断，也不能套用 x86_64 崩溃预期；ARM 物理板共享/静态对照各20/20通过，候选共享 libgcc 的主证据有效，但跨环境通用结论仍受限 | `docs/progress/BPF_W3R_0922/FINAL_RESULT.md`、`BOARD_RESULT.md` |

附加前置边界：本地 rpmdb 能证明四个 BuildRequires 的源码提供方与已使用版本，不能证明 OBS 当前仓一定有这些静态子包，也不能证明已发布 LLVM 分支会产出本地候选 spec 打开的 libclang.a；此项服务端输入可用性 NOT_OBSERVED，不能用 11 个远端分支存在替代依赖解析。
