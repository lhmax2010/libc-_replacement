# R4 决定与实施口径

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
