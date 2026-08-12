# R29 双运行时 provider 打包形态设计与打通报告

日期：2026-08-12  
执行分支：`codex/r5-ehabi-diagnosis`  
范围：x86_64 本地 GBS 形态验证；未向任何外部仓推送

## 结论

规定的“单 spec 内两次构建”形态已在三个第二阶段层 1 provider 上打通：autotools 的 pcre、CMake 的 flatbuffers、meson 的 lottie-player。三包均同时产出默认 libstdc++ 子包和独立 `-libcxx` 子包；最终 GBS 退出码均为 0。

三组默认/变体 devel 包在临时 RPM 数据库中同时执行 `rpm --test --nodeps -ivh`，三次均退出 0，普通文件非预期同路径交集为 0。这里的 `--nodeps` 只隔离验证文件冲突；依赖闭包由实际 GBS buildroot 同时安装两组 BuildRequires 并完成消费者构建来验证，不能把前者单独解释为完整可安装性验证。

同一份最小消费者 spec（SHA256 `9bf1c6e056fa5e383c1f933647338207f695aa0696d9aea65df0c1387fa165ee`）在两根全新 buildroot 中构建：默认中央配置解析默认库，libc++ 中央配置通过 `CXXFLAGS`/`LDFLAGS`、`PKG_CONFIG_PATH`、`CMAKE_PREFIX_PATH` 解析三套变体。两次均退出 0；六个消费者 ELF 的 `ldd -r` 均退出 0。消费者 spec 与源码未因模式切换而改变。

本任务证明的是三类代表样包的可实现形态，不等于其余 73 包已经构建通过。逐包定制项和 76 包形态分布见“模板与适用边界”。

## 1. 输入、选样与口径

闭包输入为 `progress/R27/tables/closure_members.tsv`，SHA256 `025b334882d8936cdeb84908508b16b82a1a25532d7fb19da712e4dd911112e8`；迁移层输入为 `progress/R24/tables/migration_packages_483.tsv`，SHA256 `3d56ac7be96467394989a3b98a3738459d92185659a8bf8369b43d8c8fde7688`。

三个样包均属于 `PHASE2_LAYER1`，并分别覆盖：

- pcre 8.45-1.13：autotools；原有 6 个非 debug 二进制子包；含静态库、3 个 pkg-config 模块，无 CMake config。
- flatbuffers 24.3.25-1：CMake；原有 runtime/devel/python；含共享库、静态库、pkg-config 与 CMake config。
- lottie-player 0.2.5-1：meson；原有 runtime/devel；含共享模块与 pkg-config，无静态库；平台 spec 未启用其 CMake 构建入口。

完整选择依据和初始子包形态在 `tables/sample_selection.tsv`。

## 2. 冻结形态与理由

### 子包命名

变体运行时采用原运行时原子名加 `-libcxx`，开发包采用 `<源码包>-libcxx-devel`，原包已拆静态开发包时再用 `<源码包>-libcxx-devel-static`。这样默认包名和既有 Requires 不变，变体可被显式选择。

### 文件路径与 SONAME

共享库仍位于 `%{_libdir}`，但文件名与 `DT_SONAME` 同时增加 `-libcxx`。仅更换目录不能改变消费者记录的 DT_NEEDED，也不能保证 dlopen 共载，因此 SONAME 必须不同。最终实测：

- `libpcrecpp.so.0` / `libpcrecpp-libcxx.so.0`
- `libflatbuffers.so.24.3.25` / `libflatbuffers-libcxx.so.24.3.25`
- `librlottie.so.0` / `librlottie-libcxx.so.0`

头文件置于 `%{_includedir}/<包>-libcxx/`；静态库重命名为 `*-libcxx.a`；lottie image-loader 变体置于 `%{_libdir}/rlottie-libcxx/`。最终包清单未发现两套包拥有同一路径普通文件。

### pkg-config

变体 `.pc` 保留逻辑模块文件名，置于 `%{_libdir}/libcxx-pkgconfig/<包>/`，由中央 `PKG_CONFIG_PATH` 优先选择。私有目录被 `%__provides_exclude_from` 排除自动 Provides；变体只显式提供 `pkgconfig(<module>-libcxx)`。实测默认与变体没有同名 `pkgconfig(...)` Provides：pcre 为 `libpcrecpp`/`libpcrecpp-libcxx`，flatbuffers 为 `flatbuffers`/`flatbuffers-libcxx`，lottie 为 `rlottie`/`rlottie-libcxx`。

### CMake config

变体 config 位于 `%{_libdir}/libcxx-prefix/flatbuffers/lib/cmake/flatbuffers/`，中央 `CMAKE_PREFIX_PATH` 选择它。最终 imported shared target 指向 `/usr/lib64/libflatbuffers-libcxx.so.24.3.25`，static target 指向 `/usr/lib64/libflatbuffers-libcxx.a`，include 指向 `/usr/include/flatbuffers-libcxx`。

一次中间验证发现，仅改库名会遗漏导出 target 中硬编码的 `/usr/include` 与 `/usr/lib64/libflatbuffers.a`；该产物被保存为 `artifacts/R29/flatbuffers-superseded-cmake-overlay/`，修正后用全新根重建并重跑消费者。该实测证明只设置 `-I/-L` 或只复制 CMake config 不足以完成重定向。

### 构建选项

provider 的第二遍构建由各 spec 显式使用 `clang++ -stdlib=libc++`，并以 `-Wl,--no-as-needed -lc++abi -Wl,--as-needed` 保留直接 libc++abi 依赖。这是变体产出的包内职责。消费者选择则完全由中央 buildconf 注入，不修改消费者 spec。

## 3. 逐包实现与构建

### pcre / autotools

默认构建保持原路径。第二遍从隔离源码副本重新 configure，避免复用第一遍生成的缓存；显式复用平台 prefix/libdir 选项。只复制 C++ wrapper `libpcrecpp` 的变体，C ABI `libpcre.so.1` 继续共享。变体新增 3 个非 debug 子包：`libpcrecpp-libcxx`、`pcre-libcxx-devel`、`pcre-libcxx-devel-static`。最终根 `R29-pcre-x86_64-20260812-04`，GBS 退出 0。

### flatbuffers / CMake

第二遍使用独立源码副本和独立 `build-libcxx`，关闭第二遍 `flatc` 构建，安装到 staging 后重命名共享/静态库，并重写 pkg-config 和全部 CMake imported targets。变体新增 `flatbuffers-libcxx`、`flatbuffers-libcxx-devel`。最终根 `R29-flatbuffers-x86_64-20260812-04`，GBS 退出 0。

### lottie-player / meson

首次第二遍编译失败：Meson 生成 `-I..`，源码根下名为 `format` 的开发辅助脚本遮蔽 libc++ 标准头 `<format>`，clang 将脚本首行解析为 C++ 并报错。错误原文在 `commands/022_build_lottie.full.log`。

默认源码树未改。第二遍使用隔离副本，仅在该副本中把非头文件 `format` 改名为 `format.command`，随后用独立 `builddir-libcxx` 构建。此动作是 lottie 特化，不能机械套入通用 meson 模板。变体新增 `lottie-player-libcxx`、`lottie-player-libcxx-devel`。最终根 `R29-lottie-x86_64-20260812-02`，GBS 退出 0。

所有实际创建的 buildroot 均单次使用，并以 `RETIRED_SINGLE_USE` 标记；失败根未复用。尝试、红项分类和处置见 `tables/attempts_and_dispositions.tsv`。

## 4. Provider 验证

最终六个主要共享库的判据全部满足：默认侧含 `DT_NEEDED libstdc++.so.6`；libc++ 侧含 `libc++.so.1` 和 `libc++abi.so.1`，不含 `libstdc++.so.6`；两侧 SONAME 不同。

导出符号实测：pcre/flatbuffers/lottie 默认侧分别有 22/226/21 个 `std::__cxx11` 命中、0 个 `std::__1`；libc++ 侧分别有 28/233/69 个 `std::__1` 命中、0 个 `std::__cxx11`。逐 ELF 数据见 `tables/provider_elf_results.tsv`，原文见 `commands/030_final_elf_validation.log`。

共装测试与路径交集原文见 `commands/051_final_coinstall_and_path_overlap.log`；开发路径与静态库清单见 `commands/053_static_and_devel_path_validation.log`。

## 5. 消费者侧验证

消费者包同时 BuildRequires 两套 devel 包，保证选择不是靠卸载另一套完成。A/B 使用同一个 spec 和同一源码归档；仅 buildconf 不同。

默认模式解析：

- pcre include `/usr/include`，链接 `libpcrecpp.so.0`；
- rlottie include `/usr/include`，链接 `librlottie.so.0`；
- flatbuffers CMake target `/usr/lib64/libflatbuffers.so.24.3.25`，include `/usr/include`；
- 三个消费者 ELF 均含 `libstdc++.so.6`，不含 libc++ provider SONAME。

中央 libc++ 注入模式解析：

- pcre include `/usr/include/pcre-libcxx`，链接 `libpcrecpp-libcxx.so.0`；
- rlottie include `/usr/include/lottie-player-libcxx`，链接 `librlottie-libcxx.so.0`；
- flatbuffers CMake target `/usr/lib64/libflatbuffers-libcxx.so.24.3.25`，include `/usr/include/flatbuffers-libcxx`；
- 三个消费者 ELF 均含 `libc++.so.1`、`libc++abi.so.1`，不含 `libstdc++.so.6`。

六次 `ldd -r` 均退出 0。执行方式为宿主 x86_64 原生 `ldd -r`，`LD_LIBRARY_PATH` 指向各自退役 buildroot 的只读 `/usr/lib64`。输出中的 `/bin/bash` 对目标根 `libtinfo.so.6` 的版本提示来自该执行载体；目标 ELF 没有未解析符号且退出码为 0。完整原文在 `commands/049_consumer_elf_and_ldd_validation.log`。

解析路径、spec/buildconf SHA 和结果见 `tables/consumer_results.tsv`。这验证了消费者 spec 无需修改；切换由中央配置完成。

## 6. 模板与对其余 73 包的适用边界

对 76 个 R27 闭包源码身份的现有 spec/RPM 形态实测：CMake 52/76、autotools 13/76、meson 7/76、other 5/76；标签可重叠，所以构建系统分子合计可能大于 76。静态库 15/76、pkg-config 70/76、CMake config 7/76。二进制 RPM 文件清单口径为 1077 条，`rpm -qlp` 失败 0/1077。

`templates/` 给出三类模板。通用部分是双构建骨架、隔离 build/staging、SONAME/文件名隔离、私有开发接口和中央选择。必须逐包定制的部分包括 ABI major、原构建选项、内部 DSO 的 `--replace-needed`、模块目录、所有 `.pc`、所有 CMake imported targets、静态库拆包、代码生成器和源树重名文件。因此未对其余 73 包申报构建 PASS。

## 7. 产物与证据

- 最终 RPM/SRPM、构建日志和配置副本：`artifacts/R29/{pcre,flatbuffers,lottie-player,consumer}/`
- 被 CMake 路径验证淘汰但保留的中间产物：`artifacts/R29/flatbuffers-superseded-cmake-overlay/`
- 实际改造 spec、消费者源码与中央配置：`progress/R29/proposed/`
- 构建/验证汇总：`progress/R29/tables/`
- 可复用模板：`progress/R29/templates/`
- 命令原文、标准输出/错误与退出码：`progress/R29/commands/`

本任务未修改既有制品、冻结配置或外部源码仓；构建用配置均为 `tmp/R29/` 中的副本。未向 Gerrit 或任何外部源码仓推送。
