# QuickBuild #1194606 修复 R4（含人工裁决后的补验与推送）

## 最新结论：三个包均 PUSHED

人工修订门禁后，ICU 六份库符号检查通过；qemu 三变体均完成 x86_64 构建、清单自洽与逐项溯源验收。cereal 沿用本轮原验收，不重跑、不改提交。三包均已普通推送到各自 sandbox/lhmax2025/libcxx-migration 并回核。下方原“部分完成 / FAIL”章节完整保留，属于裁决前历史，不再代表当前门禁结论。

| 包 | 最新状态 | 远端 SHA | 验收依据 |
|---|---|---|---|
| cereal | PUSHED | a681c106a5866b6113b90a25307bf3abb3e76334 | 原性能链接、NEEDED、90 个头文件逐字节检查 PASS；d62ccf97 保留 |
| icu | PUSHED | b7ab6868d425e3190057e6d9f3c2b69646793df1 | ICU_ADJUDICATION_R4.json；raw/R4_058–060 |
| qemu-accel | PUSHED | 3b8d6274822cc2c2ce373eff7b921f5b3de3062e | QEMU_GATE_REVISED_R4.json；raw/R4_070–071 |

QUICKBUILD_BRANCHES_R5.tsv 更新为 **14 行**（原 12 行更新 ICU，再增加 cereal、qemu-accel 各一行）。**可作为第二轮 QuickBuild 开工清单**；逐行回核记录为 REMOTE_CHECK_R4_adjudicated.json、raw/R4_remote_adjudicated_01–14。三份 qemu spec 在同一源码仓、同一远端提交，不增加另外两行。本轮未启动 QuickBuild，下一轮仍由人工发起。

### 补验一：ICU 的实际符号条件

| 库 | `_ZNSt3__1` 未定义符号行数 | GLIBCXX/CXXABI_ 版本引用行数 | 直接 libc++ NEEDED | 新门禁 |
|---|---:|---:|---|---|
| libicudata.so.78.1 | 0 | 0 | 无 | PASS，批准的数据例外；亦无 `_ZSt` 开头符号 |
| libicui18n.so.78.1 | 0 | 0 | 无 | PASS，有 libc++abi NEEDED，满足批准的条件例外 |
| libicuio.so.78.1 | 8 | 0 | 有 | PASS |
| libicutest.so.78.1 | 0 | 0 | 有 | PASS |
| libicutu.so.78.1 | 24 | 0 | 有 | PASS |
| libicuuc.so.78.1 | 6 | 0 | 有 | PASS |

六份均无 libstdc++.so.6 NEEDED。另检查含 const 限定的 std::__1 自身符号前缀（`_ZNKSt3__1` 等），仍满足“引用该库自身符号者必须直接依赖 libc++”；不能把 ICU 函数参数里出现 std 类型名等同于 std 自身符号的欠链接。完整 nm -D --undefined-only 与 readelf 在 raw/adjudication_R4/libicu*-nm.stdout、libicu*-needed.stdout（文件名中的点转为下划线）；指定三条命令计数原文 raw/R4_045。计数为 0 时 grep 返回 1，未写成工具故障。

tz2icu 的 -iquote、完整 RPM、原版清单对照沿用前文实测；没有修改 ICU 链接参数。原 verify-icu_R4/result.json 的旧门禁 FAIL 保留，新结论独立记录。

### 补验二：qemu 三变体

| 变体（构建宿主均 x86_64） | GBS 退出码 / 秒 | 两条 .so.1 各一次 | unpackaged | 清单/溯源 | 结果 |
|---|---|---|---|---|---|
| armv7l | 0 / 305.551（沿用） | 是 | 无 | A/B/ARCH 逐项闭合 | PASS |
| aarch64 | 0 / 240.286（沿用） | 是 | 无 | 对旧版新增 8 项：A=2、B=6 | PASS |
| riscv64 | 0 / 265.404（补建） | 是 | 无 | A/B/ARCH 逐项闭合 | PASS |

三份 `%{_sourcedir}/clang-accel.list` 原文在 raw/adjudication_R4/<arch>-clang-accel-list.stdout；与 aarch64 的逐行 diff 在 <arch>-variant-list.diff。armv7l/riscv64 的差异仅两条带 target_arch 的 clang/clang++ 入口（换名前后排序位置也改变）；除这些精确入口替换后排序核对，未忽略其它路径、未丢弃重复项。两个 .so.1 各恰好一次。额外 .so.1.0 按后给出的 B 类溯源裁决验收，不再引用先前“没有其它 libc++* 条目”的限制。

主包与 python-accel：三变体的源清单原文、各 RPM 的 rpm -qlp 全文和交叉核对保存在 raw/qemu_gate_revised_R4/；源清单条目均被各自 RPM 覆盖，补丁新增的两条 SONAME 路径未进入这两个子包，完整 build.log 无 unpackaged。按新条款不再要求前版对照，也不把该核查扩大为所有二进制内容不变。全部 RPM 名、大小、SHA、清单在 QEMU_RPM_INVENTORY_REVISED_R4.json。

### 每一项清单差异的来源（适用于三个变体）

以下库路径均位于 `/emul/usr/lib64/`。逐变体、逐文件的完整记录另见 QEMU_PROVENANCE_REVISED_R4.tsv；其中省略目录的证据文件名均在 raw/adjudication_R4/。

| 文件 | 解释类型 | 实际证据 |
|---|---|---|
| libc++.so.1 | A：本补丁新增 | raw/066_R3_qemu_show.stdout.txt，模板与三 spec 的循环仅追加这两条；三源清单各一次 |
| libc++abi.so.1 | A：本补丁新增 | 同上 |
| libc++.so.1.0 | B：原失败轮已出现 | ../QB_1194606/excerpts/qemu-accel-<arch>-x86_64.txt:33，Missing build-id；三变体原文摘在 raw/R4_063 |
| libc++abi.so.1.0 | B：原失败轮已出现 | 上述各文件:34；raw/R4_063 |
| libLLVM.so.22.1 | B：输入与 ldd 链带入 | 前版 /emul Clang 的 readelf 与新 RPM 的 readelf；<arch>-input-rpm.stdout、<arch>-ldd-original-inputs.stdout |
| libclang-cpp.so.22.1 | B：输入与 ldd 链带入 | 同上；新 Clang 直接 NEEDED，此两库亦在原 QuickBuild 警告中 |
| libatomic.so.1 | B：ldd 链带入 | <arch>-ldd-original-inputs.stdout 中实际解析为 /lib64/libatomic.so.1；前后 Clang 依赖对照同上 |
| libatomic.so.1.2.0 | B：上述 SONAME 的真实文件 | <arch>-ldd-realpaths-v2.stdout：/lib64/libatomic.so.1 → /usr/lib64/libatomic.so.1.2.0；spec:314–320 把 readlink -f 的结果加入清单 |
| /emul/usr/bin/<target_arch>-clang、clang++ | ARCH：架构工具入口 | <arch>-variant-list.diff；spec:640–644 的目标前缀符号链接/清单生成；不计 FAIL |

双重证据的具体位置：旧 RPM 解出的 /emul/usr/bin/clang-22 的 readelf 为 raw/adjudication_R4/previous-clang-needed.stdout，NEEDED 含 libstdc++.so.6、不含 libLLVM/libclang-cpp；新 RPM 同路径的 readelf 为 raw/qemu_gate_revised_R4/new-rpm-clang-needed.stdout，直接含 libc++、libc++abi、libLLVM.so.22.1、libclang-cpp.so.22.1、libc。新输入 rpmdb 明确是 llvm/clang 22.1.8-3.4、VCS 5c169afc。旧 clang-accel RPM 的 requires 未声明精确 LLVM RPM 版本，不能据此臆造旧 release，采用人工允许的前后 ELF NEEDED 对照证据。

保留构建根中的 ldd 输出按 spec:335–345 的输入列表只读重放，未改清单、未重建、未更换输入；保存每个 executable 与完整依赖行。libatomic 的真实路径和 spec 原有的路径归一/去重规则与最终源清单对得上。此证据是补取时的实际输出，不冒充构建当时已记录的 ldd 输出。

### 收尾与限制

新门禁、工具采集修正及原始失败保留方式见 DECISIONS_R4.md。riscv64 仍使用 -j1、nice19、ionice idle、MemoryMax=16536461312 字节、w5.xzdio，RESOURCE 与执行快照落盘。三包源提交本轮续跑未变，不改 codes/、不上板、不启动 QuickBuild。R101 cereal 与 12/55/6/0 判定计数沿用上一段完成结果；ICU/qemu 是已有判定之外的本次构建修复，不另增“需要改”的计数。

最新下一步：人工使用 14 行 R5 起第二轮 QuickBuild，再与 #1194606 逐项对账。本轮验证只覆盖指定 x86_64 构建及 qemu 三个变体，不能代替下一轮三架构总部实测。项目仓补充材料普通提交推送，最新回执另记。

---

## 以下为门禁修订前的完整历史报告

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
