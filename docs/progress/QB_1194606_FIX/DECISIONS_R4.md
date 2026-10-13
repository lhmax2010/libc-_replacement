# R4 决定与实施口径

## 人工裁决后追加（以前的 FAIL 保留为历史）

- 使用人工随后修订的 ICU 门禁：六库均无 libstdc++ NEEDED、无 GLIBCXX/CXXABI_ 版本引用；引用 std::__1 自身符号者须直接依赖 libc++。libicudata 的指定符号为 0；libicui18n 的两项 grep 计数均为 0 且 NEEDED 含 libc++abi，按授权登记例外。grep 无匹配退出 1 是计数 0 的正常语义，原文保留。
- qemu 两条答复中，后给出的 A/B 逐项溯源裁决明确覆盖三变体，并说“不再引用只多两条”；据此不再把旧“无其它 libc++* 条目”用于拒绝已获 B 类证据的 .so.1.0。仍保留每条 .so.1 恰一次、原始列表、无 unpackaged、跨变体仅目标工具入口差异、主包/python 清单交叉核查。
- 不再查找不存在的 armv7l/riscv64 前版同名 RPM。保留前版 aarch64 的完整原始 diff；架构入口变化标 ARCH、不计 FAIL，其余 8 项逐一分为 A（2 条补丁新增）和 B（6 条原失败日志或现有 ldd 链带入）。旧 RPM 与新 RPM 的 /emul/usr/bin/clang-22 分别解到新的 tmp 目录，只读 readelf，不改既有文件。
- ldd 证据是在保留的同一构建根中，以只读 bwrap 重放 spec:335–345 的全部输入 glob，保存每个输入的 ldd 原文；不把它冒充构建当时原始输出，也不重新执行会修改清单的 prepare_package_list。rpmdb 的输入版本与 SHA 所属 VCS一并记录。
- 技术性采集问题均留痕后修正，未更改包方案：R4_049 的日志命名 with_suffix 冲突导致拒绝覆盖，改为标签点号转下划线后 R4_052 成功；R4_051 的旧 readlink 不接受多参数，R4_055 改为逐个路径并使用 ldd 实际 /lib64/libatomic 路径；R4_062 的目标三元组替换改变了排序位置，原 diff 只有两条工具入口变化，改为替换后排序比较（保留重复项计数，不用集合丢计数）后 R4_065 成功。各旧记录不删。
- ICU 复用 R4 构建产物，仅补只读符号门禁；qemu armv7l/aarch64 复用已成功构建，riscv64 补一次完整构建，退出 0。没有新增源码或 spec 改动。

- 保留 cereal d62ccf97 不动；追加第二笔 a681c106a5866b6113b90a25307bf3abb3e76334。新增 packaging 补丁仅替换 performance.cpp 两个 char 分布表达式，与人工文字完全一致。所有工具链都应用，不使用 SKIP_PERFORMANCE_COMPARISON。
- 实际 rpm 4.14.1 的 rpmspec -P 对 `%patch -P 0 -p1` 保持字面形式（raw/R4_002）；依授权改为 `%{__patch} -p1 < %{PATCH0}`。四组宏严格比较忽略纯空行后，差异只含既有 libc++ 选项与新增 Patch0/调用，全部 PASS。
- ICU b7ab6868、qemu-accel 3b8d6274 内容保持不变。R4 授权某包失败后继续其它包，取代 R3 的整轮停止方式；不自行改失败修法。
- 每格使用独立 GBS 根 `tmp/QB_1194606_FIX/GBS-R4/<cell>/`，保留 R3 失败现场及其它格现场。仅用 cp -a 复制旧根下载缓存，不复用对象文件；固定 RBS 20261012.081855、payload w5.xzdio。五格串行、-j1、nice19、ionice idle，实际 cgroup MemoryMax=16536461312 字节；各格 RESOURCE-*_R4.json、raw 与 build-*_R4/ 留证。
- 前一发布版统一取 20260924.094908 固定快照。cereal 沿用 R3 下载且 SHA 为 8b3b0b25…；ICU 三份及 clang-accel RPM 下载 URL、SHA 在 raw/R4_006。clang-accel VCS 确为 qemu-accel#2737f3bf；旧 ICU VCS 为 e4a4d741，不冒充已迁移的 dcbb 基线。旧 RPM 只用于清单/内容对照，构建输入仍来自指定 RBS。
- R5 按本轮明确要求，qemu-accel 只列一个源码仓级入口（三份 spec 在该仓同一提交），不再按三变体增加三行；全部成功应为 14 行。三变体验收结果分别列出，不能用一格替代其它两格。
- ICU 默认简略命令输出不匹配最初监控器的编译目标正则；运行中新增独立 quiet-output I/O 看护（watch_icu_R4.py），不另起构建，按同一 cgroup 的 freeze/恢复规则处理超时。后续启动脚本加入该正则，并逐格自动保存执行脚本与 SHA。cereal/ICU 已启动版根据唯一正则变更反向恢复保存在 build_R4_cereal_icu_snapshot.py（明确为事后恢复，不冒充启动时自动快照）；启动 argv 和资源记录原样保留。
- ICU 构建成功不替代产物门禁。libicudata 与 libicui18n 缺少直接 libc++.so.1 NEEDED，原始严格 FAIL 保留；未获人工例外裁决不推送，也不通过新增链接参数规避。
- 已取到的前版 clang-accel 清单包含 aarch64 的编译器入口，不能用它证明 armv7l/riscv64 同变体“只多两条”。比较器保留跨变体完整 diff，将此项记 NOT_OBSERVED 而非 PASS 或确定的回归；aarch64 同变体仍按“仅两条新增”严格比较。另有 .so.1.0、LLVM/Clang/atomic 条目的差异也完整保留，不擅自忽略。已向人工提出前版输入与验收口径问题。
- 构建框架进程命令中可见 /usr/bin/build --jobs 40；本轮实际覆盖 _smp_mflags=-j1、_smp_build_ncpus=1，GBS --threads=1 且串行启动各格。没有把框架默认值写成已修改；实际 rpmbuild 资源、参数原文见 raw/R4_019–020。
