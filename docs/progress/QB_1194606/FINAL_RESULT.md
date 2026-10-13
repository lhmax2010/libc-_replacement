# QuickBuild #1194606：逐包失败分析

结论：本次 Base-Toolchain 全量构建未通过。报告覆盖 258 个构建包、774 个架构格：752 succeeded、9 failed、13 excluded，无 unresolvable/blocked。失败涉及 5 个构建包、3 个源码仓，分为 cereal 构建期 Boost ABI 不匹配、ICU 同名头文件碰撞、qemu-accel 三个变体 RPM 文件清单漏项。12 个指定构建包的实际 checkout/export revision 与预期一致；发布 manifest 的 ICU revision 却不一致，必须单列。本轮只分析，候选 diff 未应用或构建，包仓未推送。

## 1. 数据身份与范围

| 项目 | 观测 |
|---|---|
| 主构建 | [QuickBuild #1194606](https://quickbuild.tizen.org/build/1194606/gbs_reports)，Failed；GBS 子构建 #1194608 |
| 标签 | TIZEN_Tizen_Tizen-Base-Toolchain_RBS_TRIGGER_20261012.081855 |
| 页面时间 | 开始 2026-10-12 08:18:55，时长14h1m4s；页面时区未单独确认 |
| reference | tizen-base-toolchain_20260924.094908，不是旧 R101 快照 |
| 产物 | [本次 RBS 目录](https://download.tizen.org/RBS/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20261012.081855/) |
| 状态原文 | evidence/report_index_response.html、report_index.txt；HTML 与文本逐格交叉核对 |
| 完整逐包表 | [BUILD_STATUS.md](BUILD_STATUS.md)，258行；[BUILD_STATUS.tsv](BUILD_STATUS.tsv)，774行含日志链接 |
| 完整日志 | 9份失败日志、llvm/runtime各3份、bpftrace2份，17/17 HTTP 200，原始字节 SHA 复核一致 |
| 大小与存放 | 合计13,267,844字节；按整批超过10MB保守处理，原文留 tmp/QB_1194606/downloads/logs/，不提交全文 |
| 日志身份 | [DOWNLOADS.tsv](DOWNLOADS.tsv)：URL、大小、SHA、原文路径；其它下载身份见 LOCAL_DOWNLOAD_IDENTITIES.tsv |
| 登录 | 人工在 headed Chromium 登录；同一 context 下载，不读取密码、不导出 cookie/认证头 |
| profile | tmp/QB_1194606/browser-profile/，保留且不入库 |

认证 UI 包装页的原文留 tmp；仓内保存静态报告原文与必要的构建日志摘录。DOM 捕获与 HTTP 原始响应明确区分。浏览器只读命令/结果见 BROWSER_ACTIONS.json；命令行与退出码见 raw/。没有构建、上板、包仓/sandbox/Gerrit 推送或新 QuickBuild。

## 2. 输入核对

完整 SHA、Gerrit 仓、预期分支、实际执行证据行与 manifest 对照见 [INPUT_COMPARISON.tsv](INPUT_COMPARISON.tsv)。MATCH 指 revision；执行方式为 checkout SHA，实际分支名本身为 NOT_OBSERVED，不由 SHA 反推分支。

| 构建包 | 实际 revision（缩写） | 对预期 | armv7l / aarch64 / x86_64 |
|---|---|---|---|
| abseil-cpp | 7a4f04da | MATCH | succeeded / succeeded / succeeded |
| boost | 4168e873 | MATCH | succeeded / succeeded / succeeded |
| icu | dcbb3fb4 | MATCH；manifest 不同 | failed / failed / failed |
| jsoncpp | 9cbab147 | MATCH | succeeded / succeeded / succeeded |
| libsigc++ | 4f2f592d | MATCH | succeeded / succeeded / succeeded |
| pcre | 09a5bfc9 | MATCH | succeeded / succeeded / succeeded |
| taglib | 8b552bdf | MATCH | succeeded / succeeded / succeeded |
| tensorflow2 | 6d761390 | MATCH | succeeded / succeeded / succeeded |
| llvm | 5c169afc | MATCH | succeeded / succeeded / succeeded |
| bcc-tools | 5bd0654e | MATCH | succeeded / succeeded / succeeded |
| bpftrace | 7831fb34 | MATCH | succeeded / succeeded / excluded |
| libcxx-runtimes | 5c169afc（llvm仓） | MATCH | succeeded / succeeded / succeeded |

llvm/runtime 共仓，故是12个构建包、11个仓库。父日志2150、2442行确认LLVM checkout，2504–2510行单独导出/解析 packaging/libcxx-runtimes.spec。证据 evidence/input-execution.numbered.txt 首列保留父日志原行号。成功日志的 Wrote/结束摘录见 evidence/supporting-log-lines.txt。不把构建成功外推为所有产物 ABI、安装或设备运行均通过。

**输入元数据矛盾：** [公开 manifest](https://download.tizen.org/RBS/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20261012.081855/builddata/manifest/tizen-base-toolchain_20261012.081855_standard.xml) 中 ICU 为 e4a4d7411b0f288581cf020a176b5dddbb504ae7；实际父日志2152行 checkout dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330，2343–2344行明确从e4a4切到dcbb。其它11个构建包的manifest SHA匹配。只登记，不修manifest，不推断具体生成环节的原因。

### project_config

从父日志1064–1421行 PROJECT_CONFIG 取回完整配置。evidence/project_config.numbered-raw.txt 保留原文/原行号；project_config.from-log 移除日志前缀、将DOM不换行空格还原普通空格，SHA256为42ed43f8e4973b549393ccf599e97db2ef4dcdcd8d74b6936baf7127d55645a6。这是派生文本，不冒充远端文件字节身份。

| 检查 | 结果 |
|---|---|
| Support: libc++-devel libc++abi-devel | 存在，1行 |
| Keep: libc++-devel libc++abi-devel | 存在，1行 |
| Keep: libc++ libc++abi | 存在，1行 |
| -fuse-ld=lld / --undefined-version | 配置字段内均零命中 |
| build_ldflags | -Wl,-z,relro,--as-needed%{?_ldflags_arch_extra} |
| Gerrit 356690 | 要求的三行及撤lld内容已体现；实际采用该Gerrit提交的SHA/合入来源 NOT_OBSERVED，不能由内容相同推定 |

检查结果 PROJECT_CONFIG_CHECK.json。runtime三格成功，其它包日志实际安装了libc++/libc++abi；没有“libc++尚未发布导致unresolvable”的格。

## 3. 状态与失败分类

| 架构 | succeeded | failed | excluded | unresolvable | blocked | 合计 |
|---|---:|---:|---:|---:|---:|---:|
| armv7l | 250 | 2 | 6 | 0 | 0 | 258 |
| aarch64 | 250 | 2 | 6 | 0 | 0 | 258 |
| x86_64 | 252 | 5 | 1 | 0 | 0 | 258 |
| 合计 | 752 | 9 | 13 | 0 | 0 | 774 |

0仅指这份完整报告中未出现，不外推其它构建。excluded不算成功。所有258包逐项表见BUILD_STATUS.md，不用旧254包全集替代。

| 失败包 | armv7l | aarch64 | x86_64 | 首个因果错误 | 原日志行号 |
|---|---|---|---|---|---|
| cereal | failed | failed | failed | performance 的 Boost archive undefined reference | 9531 / 9533 / 9517 |
| icu | failed | failed | failed | ./version:1:1: error: expected unqualified-id | 1973 / 1975 / 2054 |
| qemu-accel-armv7l | excluded | excluded | failed | Installed (but unpackaged) file(s) found | x86_64:3175 |
| qemu-accel-aarch64 | excluded | excluded | failed | 同上，两条libc++ SONAME路径 | x86_64:3162 |
| qemu-accel-riscv64 | excluded | excluded | failed | 同上，两条libc++ SONAME路径 | x86_64:3156 |

每格独立阅读，首个因果错误及前后各40行原文存 excerpts/<包>-<架构>.txt，附原日志行号；索引 [FAILURES.tsv](FAILURES.tsv)。不取尾部make Error 1/2作为根因。

| 分类 | 构建包数 | 失败格数 | 对历史基线 |
|---|---:|---:|---|
| 其它：构建期跨标准库Boost ABI不匹配 | 1 | 3 | NEW |
| libc++头文件/API差异：同名数据文件遮蔽标准头 | 1 | 3 | NEW |
| RPM打包/文件清单 | 3（同一源码仓） | 3 | NEW |
| 依赖缺失/无法解析、测试运行失败、GCC专用参数、环境/超时 | 0 | 0 | 本次未观测到这类致命错误 |

对照 ../QB_PRECHECK_0922/EXPECTED_FAILURES.md：唯一可复用的完整构建已知失败是tensorflow2 armv7l/GCC XNNPACK条件，本次tensorflow2三格succeeded，不可用该例外豁免新失败。六个全局GCC flag风险是driver探针而非逐包必败，也不适用本次九格。九格均为该基线之外的新失败；没有依赖缺失型连锁失败，不把每条undefined reference重复计数。

## 4. 根因与候选修法（均未实施）

### cereal：交付物纯头文件，构建过程仍有Boost运行时边

实际源码28aedc95fdc600568c57babf28ff717e0b2bca7d。sources/cereal/cereal.spec:11,37–39要求boost-devel并构建默认目标；CMakeLists.txt:16,44–49,109–112默认CLANG_USE_LIBCPP=OFF、未跳过performance；sandbox-CMakeLists.txt:15–21将performance链到Boost serialization。

三份日志的CXXFLAGS无-stdlib=libc++，使用Clang22.1.8；performance对象请求std::__cxx11 / std::char_traits形式的接口。首个undefined reference是basic_binary_oprimitive<..., std::char_traits<char>>析构函数，不是缺boost-devel。

额外只读下载本次RBS x86_64 boost-serialization-1.83.0-2.1，版本与cereal日志安装条目相同，包头VCS为4168e873…。解出库实际NEEDED含libc++abi/libc++/libgcc_s/libc，导出对应std::__1::char_traits接口，直接支撑ABI不匹配。RPM/ELF SHA及完整检查输出见raw/033_boost_header.stdout.txt、raw/035_boost_elf.stdout.txt。未取远端根内ELF的SHA，不把同NVRA发布件冒充根内字节级核验；arm/aarch错误与consumer痕迹相同，但provider ELF未另逐架构提取。

主候选 [cereal-libcpp-build-only.patch](candidate-diffs/cereal-libcpp-build-only.patch)：仅定义_toolchain且clang时传现成CLANG_USE_LIBCPP=ON，其余OFF。上游CMake将选项用于本包编译/链接，不向cereal INTERFACE导出选项。需重验benchmark、其余测试及GCC/未定义路径，未宣布修复通过。

人工可选 [cereal-skip-performance-ALTERNATIVE.patch](candidate-diffs/cereal-skip-performance-ALTERNATIVE.patch)：只跳过Boost性能比较，代价为减少该目标的构建覆盖；不得默认两方案都用，也不自动删除BuildRequires。

旧清单说头文件由消费方实例化、交付物无需迁移，不覆盖混合标准库下构建benchmark。本次暴露此边界，且旧记录为不同cereal revision；不自行改旧判定、也不把“11包”自动改成12，由人工决定构建期适配还是不构建该benchmark。

### ICU：-I.让tzdata/version遮蔽<version>

sources/icu/Makefile.in:133–134带-I$(srcdir)；142–144行将tzdata解入该目录，仅排除Makefile。日志实测命令含-stdlib=libc++、-std=c++17、-I. -I../../common。随后include链由libc++ limits:118读到./version，首行2025b被当C++解析。是头搜索路径碰撞，不是时区数据文本错误或缺libc++。

候选 [icu-tz2icu-quote-search.patch](candidate-diffs/icu-tz2icu-quote-search.patch)：只在tz2icu规则将-I$(srcdir)改-iquote $(srcdir)，双引号本地头仍可找，尖括号标准头不落到该目录。tz2icu.cpp:13的tzfile.h使用双引号；不删除或改名tzdata/version。此处给核心源码diff，实施时须通过打包patch/源码评审接入。副作用：此规则中若有尖括号本地头将改变搜索范围，需回归；未扩大修改其它编译规则、未验证修复成功。

### qemu-accel：已复制SONAME文件未收入RPM

三个包均为x86_64宿主加速器包，不是arm/aarch/riscv目标上执行失败。源码2737f3bf…，ExclusiveArch x86_64。最终三个日志均报：

```text
error: Installed (but unpackaged) file(s) found:
   /emul/usr/lib64/libc++.so.1
   /emul/usr/lib64/libc++abi.so.1
```

模板sources/qemu-accel/qemu-accel.spec.in:300–327通过ldd/readlink建清单，含/../的路径不写原始列表但真实路径写入；380行cp -aL；664–666行生成clang-accel差集清单。**确定的是两条SONAME路径已在BUILDROOT、却不归任何包**。远端set +x阶段的ldd原文/中间.list未取得，不能把“必因某条/../输出漏记”写成已确认原因。

候选 [qemu-accel-libcpp-file-list.patch](candidate-diffs/qemu-accel-libcpp-file-list.patch)：最终clang清单差集之前，将BUILDROOT中存在的两条SONAME路径加入clang.list，继续既有sort/comm去重归属。给出模板和三个生成spec的同步diff；不删除库、不关闭未打包检查，原无这些库的根不强加不存在文件。需验证三个变体、旧标准库根、文件归属及/emul工具运行。不是让所有qemu源码切换标准库。

附带线索保留：其它spec的Unable to open之后仍进入Processing files，Missing build-id是警告；%fdupes展开还有printf %sn拼接痕迹（evidence/supporting-log-lines.txt）。未证明这些造成漏项，不顺便归因或修改。

## 5. 建议顺序、可选复现与边界

1. 人工先核对/说明ICU manifest元数据，实际分析以checkout/export及包日志为准。
2. 优先评审qemu-accel清单修正，关系后续交叉构建加速工具；本次大量arm/aarch已成功，不声称它导致cereal/ICU失败。
3. 人工决定cereal构建策略；保留benchmark则consumer与已发布libc++ Boost对齐，无需先回退Boost。
4. ICU局部头搜索路径可独立验证，无需先修cereal/qemu才能判断本错误。
5. 授权后再定点复现/候选验证，之后人工决定新QuickBuild；本轮不执行。

| 可选复现 | 已取得 | 当前相同GBS根与完整输入身份 | 本轮 |
|---|---|---|---|
| cereal x86_64 performance | 精确源码、日志、本轮Boost RPM/ELF | NOT_OBSERVED，不声称环境齐备 | 未构建 |
| ICU tz2icu | 精确sandbox源码、命令、三架构错误链 | NOT_OBSERVED | 未构建 |
| qemu三个变体 | 精确源码、模板/生成spec、完整日志 | 远端BUILDROOT/.list NOT_AVAILABLE；已查页面链接与下载材料，未提供这些中间件 | 未构建 |

没有读取凭据文件、复制cookie或改变安全策略。成功状态不等于安装/运行验收：bpftrace仅确认两架构构建成功，尚未对本次全部RPM重新做static ELF/设备门禁，旧板测和旧RPM身份不冒充本轮。完整链接命令未被日志展开的部分记NOT_OBSERVED，不用候选命令冒充。

## 6. 自行判断与交接

三类归并、候选修法与优先顺序均为本次分析判断；qemu三个构建包与一个源码仓分开计。cereal两方案留人工裁决；不改既有判定。输入参数/实际checkout/发布manifest三层区分，配置内容与Gerrit来源区分。原日志总量>10MB留tmp，仓内每格前后40行、SHA/URL齐备，没有静默省略失败格。

项目提交推送后回执见PUSH_RECEIPT.md；包仓始终不推。完成后停止，交人工审阅并决定修复/复现范围。
