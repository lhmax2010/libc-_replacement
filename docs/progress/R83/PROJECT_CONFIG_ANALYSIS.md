# project_config 分析与建议

## 两份文件的作用和差异

输入身份：

- Base：349 行，SHA256 `aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209`
- Unified：161 行，SHA256 `a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086`

Base 是工具链基础配置：定义 target/host、clang/gcc 驱动、optflags、
Preinstall/Required/Support/Keep 以及架构设置。关键现状是第 133 行默认
`%_toolchain` 已是 clang，第 290/298/299 行只保证 libstdc++ 开发和运行包。

Unified 是产品 overlay：定义 standard/emulator 条件、产品 feature 与最终
`%build` 宏。第 151–157 行当前只导出 AR/RANLIB/CC/CXX 和 `MAKEFLAGS=-k`；
它没有选择 C++ 标准库。

两文件均无 Git URL、source package branch、sandbox ref 或 Gerrit 设置。
**所以“使用 sandbox LLVM”不能靠修改 project_config 实现，相关行数为 0。**
源码身份必须由 checkout SHA 决定，二进制身份必须由优先本地 RPM repo
决定。

## 建议改动（只改人工服务器上的副本）

完整可应用 diff 在 `artifacts/PROJECT_CONFIG_CHANGES.diff`，已对给定 SHA 的
两文件以 `patch --fuzz=0 --dry-run` 验证通过。

| 文件 | 原行 | 原值 | 建议 |
|---|---:|---|---|
| Base | 290 后 | `Support: libstdc++-devel` | 新增 `Support: libc++-devel libc++abi-devel` |
| Base | 298 后 | `Keep: libstdc++-devel` | 新增 `Keep: libc++-devel libc++abi-devel` |
| Base | 299 后 | `Keep: cpp gcc libstdc++` | 新增 `Keep: libc++ libc++abi` |
| Unified | 155 后 | `export MAKEFLAGS+=" -k";\` | 新增 `CXXFLAGS="${CXXFLAGS} -stdlib=libc++"` |
| Unified | 同处 | 无 | 新增 `LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed"` |

Base 第 133 行保持不动，因为默认已经是 clang。第 37 行的 libstdc++ Prefer
也保持：迁移验证期间仍有构建工具和未迁移产物需要 libstdc++，而 Prefer
不能区分同 NEVRA 的 sandbox/远端包。

R29 原型还注入了三个样本 provider 的 `PKG_CONFIG_PATH` 和
`CMAKE_PREFIX_PATH` 私有 overlay。那是 pcre/flatbuffers/lottie-player 的
双运行时实验路径，不是当前平台通用目录，本次不建议照搬。

## 风险边界

全局 `LDFLAGS` 会被部分纯 C 目标使用，可能使本来不需要 C++ ABI 的链接也
看到 `-lc++abi`；部分包也会覆盖 CXXFLAGS/LDFLAGS。这份改动是用于发现全量
迁移编译问题的验证配置，不应未经全量结果审阅直接作为 release 配置。

必须先让优先本地仓包含 sandbox 的 `libc++-devel`/`libc++abi-devel`，再使用
上述配置。否则头文件选择被切到 libc++，buildroot 却没有新头文件，失败
没有判读价值。
