# QuickBuild #1194606 修复 R4：cereal 已推送，其余两包产物门禁未通过

**部分完成；不可作为第二轮 QuickBuild 开工清单。** cereal 追加批准的源码补丁，完成编译、链接、RPM 与头文件对照并推送。ICU、qemu 已执行的构建均退出 0、原 QuickBuild 错误已实测消失，但分别未满足本轮明确的 NEEDED / 文件清单门禁；不自行降低门禁，未推这两包。qemu 在 aarch64 验收 FAIL 后停止该包，riscv64 未启动。现场全部保留，等待人工审阅。

## 一、三包结论与提交

| 包 | 最终状态 | 本地候选 / 推送 SHA | 说明 |
|---|---|---|---|
| cereal | PUSHED | a681c106a5866b6113b90a25307bf3abb3e76334 | 父提交 d62ccf97dc889b02ffbfd6c9630469f3d93fe495 不动，追加第二笔；远端已回核 |
| icu | FAIL | b7ab6868d425e3190057e6d9f3c2b69646793df1（未推） | 构建成功，但两份库不含直接 libc++.so.1 NEEDED；远端仍 dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 |
| qemu-accel | FAIL | 3b8d6274822cc2c2ce373eff7b921f5b3de3062e（未推） | aarch64 同变体清单新增 8 项，而不是仅 2 项；riscv64 停止未跑 |

cereal 完整提交在 raw/R4_004_cereal_commit.stdout.txt、raw/R4_025_identity_final.stdout.txt；ICU/qemu 候选与 R3 相同，完整提交仍为 raw/065_R3_icu_show.stdout.txt、raw/066_R3_qemu_show.stdout.txt。作者、提交人、Signed-off-by 均 Hao Lin <hao.lin@samsung.com>，各有 Change-Id。没有 squash、没有变更其它两个候选、没有改 codes/。

cereal 源码补丁只有指定的两处 int 分布再转 char。rpm 4.14.1 的 `%patch -P 0 -p1` 在实测宏展开中保留字面，依批准使用 `%{__patch} -p1 < %{PATCH0}`；详见 DECISIONS_R4.md 与 raw/R4_002。补丁无工具链条件。四组宏检查仅允许已批准 CLANG_USE_LIBCPP 与 Patch0/应用两行差异，忽略纯空行后全 PASS：

| default | _toolchain_override gcc | _toolchain gcc | --undefine _toolchain |
|---|---|---|---|
| PASS | PASS | PASS | PASS |

原始展开、diff 与判定在 macro-check_R4/。ICU/qemu 的 16 组检查沿用 R3 实际 RBS 根结果，候选未变化，不冒充本轮重跑。

## 二、本地 x86_64 构建与逐项验收

| 构建格 | GBS 退出码 | 耗时（秒，含初始化） | 原错误与产物验收 |
|---|---:|---:|---|
| cereal | 0 | 240.255 | performance 完成链接、产出 cereal-devel RPM；全部验收 PASS |
| icu | 0 | 305.568 | tz2icu 编译 -iquote 实测；NEEDED 门禁 FAIL |
| qemu-accel-armv7l | 0 | 305.551 | 包含两条 .so.1、无 unpackaged；同变体旧 RPM 清单对照 NOT_OBSERVED |
| qemu-accel-aarch64 | 0 | 240.286 | 包含两条 .so.1、无 unpackaged；清单仅两条新增门禁 FAIL |
| qemu-accel-riscv64 | NOT_OBSERVED | NOT_OBSERVED | 同包 aarch64 验收失败后依要求停止；未运行，不记 0/N |

各格完整日志、退出码、耗时在 build-*_R4/；RPM 名、路径、大小、SHA256、完整文件清单在 verify-*_R4/RPM_INVENTORY.json。所有已生成 RPM 的摘要检查通过，未清理失败现场或改用其它修法。

### cereal

| 要求 | 结果 | 证据 |
|---|---|---|
| uniform_int_distribution 错误消失 | PASS | build-cereal_R4/build.log；verify-cereal_R4/result.json |
| performance 链接完成；basic_binary_oprimitive / basic_binary_iprimitive 未定义引用 | **已实测消失** | build.log:9545–9546，Linking / Built target performance；无对应 undefined 错误 |
| NEEDED libc++.so.1、无 libstdc++.so.6 | PASS | raw/R4_010；verify-cereal_R4 内 readelf 原文 |
| cereal-devel 完整 rpm -qlp 清单相同 | PASS | verify-cereal_R4/file-list.diff 为空 |
| 安装头文件逐字节相同 | PASS，90 个 | headers-content.diff 为空；headers-sha256.json |

前版 RPM SHA：8b3b0b2555528178d5c8f33b0713861d63ecb0aae685db831c7e4397c73ca7e3。performance SHA：f24dc8ab237950d6f052a460bf6005da4d95bc5d91213159c9756e14280ffa3f。结果不扩大成跨架构构建验证。

### ICU

tz2icu 实际编译命令见 build-icu_R4/build.log:2105，包含 `-pie -iquote . -I../../common ./tz2icu.cpp`。完整构建成功，无原 version 遮蔽错误。

| ELF | 直接 NEEDED libc++.so.1 | 直接 NEEDED libstdc++.so.6 |
|---|---|---|
| libicudata.so.78.1 | 无，FAIL | 无 |
| libicui18n.so.78.1 | 无，FAIL | 无 |
| libicuio.so.78.1 | 有，PASS | 无 |
| libicutest.so.78.1 | 有，PASS | 无 |
| libicutu.so.78.1 | 有，PASS | 无 |
| libicuuc.so.78.1 | 有，PASS | 无 |

libicui18n 依赖 libicuuc 不等于它有直接 libc++ NEEDED。未自行将直接门禁改成间接门禁。原文在 raw/R4_018、verify-icu_R4/，首个未满足项在 ICU_FIRST_ERROR_R4.md。

产出 7 个二进制 RPM 的完整清单全部登记；已取得前版的 libicu、libicu-devel、libicu-tzdata 三包各自 rpm -qlp 清单完全一致。其它四包的前版对照 NOT_OBSERVED，不把三包对照写成七包全量对照。前版来源为 20260924.094908 固定发布快照，实际 VCS e4a4d741，原 SHA 在 raw/R4_006；构建输入不是该旧快照，仍是指定 RBS。

### qemu-accel

两格原 unpackaged 问题均已实测消失。aarch64 同变体前版对照新增 8 项、删除 0 项：要求的 libc++.so.1 / libc++abi.so.1，以及 libc++.so.1.0 / libc++abi.so.1.0、libLLVM.so.22.1、libclang-cpp.so.22.1、libatomic.so.1 / libatomic.so.1.2.0。没有把额外 6 项静默忽略或归因为已确认原因。

前版 clang-accel SHA 为 fd0ddae1d9cde87a07a3e3993768de668c3d8e698e2fd0ebc5d0f54ed3c99c45，VCS 2737f3bf，清单含 aarch64 编译器入口。armv7l 的跨变体 diff 保留但不能证明同变体“只多两条”，记 NOT_OBSERVED；尚未取得 armv7l/riscv64 对应旧 RPM。完整原文见 QEMU_FIRST_ERROR_R4.md 与 verify-qemu-accel-*/clang-accel-file-list.diff。riscv64 因确定的产物门禁 FAIL 停止，而非环境、源码或时间不可得。

## 三、输入、资源与边界

固定 RBS：`https://download.tizen.org/RBS/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20261012.081855/repos/standard/packages/`。repomd SHA：e568cd09a390bf727d9800f80fc9757e9843fe66f45ecd2b875dbc7e51c2b754。各格独立 tmp/QB_1194606_FIX/GBS-R4/<cell>/，只复制下载缓存，不复用编译对象。配置副本 tmp/QB_1194606_FIX/gbs.conf，config/ 原件不改。

四个实际构建串行，medium 门禁、磁盘与初始 I/O 记录逐格保存；磁盘均高于 20GiB。GBS --threads=1，_smp_mflags=-j1，_smp_build_ncpus=1，payload w5.xzdio。cgroup memory.max=16536461312 字节（总内存 50%）、nice19、ionice idle；实际 rpmbuild 进程检查 raw/R4_007、R4_020。框架命令仍显示 --jobs40，但实际覆写宏如上，未隐瞒默认参数。

监控器与脚本快照/已知改进见 DECISIONS_R4；ICU I/O 每 500 目标补充监控的实测探测在 raw/R4_icu_io_502，未触发退化暂停。各格不足一小时；小时状态机制保留，不编造整小时巡检。没有上板、没有启动 QuickBuild、没有 push Gerrit master、没有 force。

## 四、推送、台账与下一步

cereal 普通推送原文 raw/R4_012_cereal_push.*；推前分支不存在、推后远端 a681c106 回核在 raw/R4_011、R4_013。ICU/qemu 未执行推送。

QUICKBUILD_BRANCHES_R5.tsv 当前 **13 行**：原 12 行 + cereal，ICU 保持现有远端旧 SHA，不加入未通过的 qemu 候选。每行 ls-remote 均 MATCH，见 REMOTE_CHECK_R4.json、raw/R4_remote_01–13。**不可作为第二轮 QuickBuild 开工清单**：三项修复尚未全量验收推送；不能为了达到 14 行填入未推提交。

R101 只更新 cereal 行为“需要改，已推送”。判定计数继续 **12/55/6/0**，其中需要改集合 12 个均已有已推分支；此数不表示本轮 ICU/qemu 两项后续修复通过。历史 BASE_LEDGER 不覆写，追加 CEREAL_CORRECTION_1194606_R4.md。

自行判断限于明确批准的 rpm 宏兼容写法、独立输入根、匹配监控日志格式、将跨变体基线不可比标 NOT_OBSERVED；没有改变修法或门禁。待人工裁决：ICU 两份库直接 NEEDED 的例外是否允许；qemu 前版清单额外新增项与缺失变体基线如何验收。问题已提出，未获许可前不推这两包。项目材料按授权普通提交推送，回执单列；不触碰工作树中无关的已有修改。

提交前检查已执行：STAGED_CHECK_R4.json / raw/R4_032。仅扫描暂存的本任务文件，未读取任何凭据存储；常见凭据模式命中 0、超范围 0、单文件超过 10MB 为 0。公开作者邮箱与 SSH 仓 URL 不作为凭据。模式扫描并不是任意秘密不存在的数学证明。
