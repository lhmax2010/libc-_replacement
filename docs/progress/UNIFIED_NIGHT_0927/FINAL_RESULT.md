# Unified 默认 libc++ 试编：输入门禁未满足

## 结论

**PARTIAL / NO_ELIGIBLE_ARCHITECTURE。本轮未启动任何构建。** 已固定运行线边表、Unified 快照与 12 个提供方 VCS，盘点本地 Base 输入并准备两份 project_config 副本。三架构均缺任务指定的 libc++ 版 Base RPM；按第 3 条“缺输入的架构今晚不跑”，停止在输入门禁。不能据此判断任何 Unified 包能否通过编译，也不能把旧轮通过记录写成本轮通过。

起始 2026-09-27 16:50 +08:00，第一阶段约 20 分钟完成；硬截止为 2026-09-28 08:30 +08:00，本轮因输入门禁提前停止，不是超时停止。精确命令起止时间见 raw/*.started.txt、finished.txt。

## 范围与输入身份

| 对象 | 固定身份与证据 |
|---|---|
| 运行线 | `72c0ad91858db03f84a653d029767efebde9dfa7` 的 `RUNTIME_PHASE_SUMMARY_0921/EDGES.tsv` 与 `NEXT_STAGE.tsv`；只读 git show，原文保留于本目录 |
| 边与提供方 | 23 条边、14 个提供方；其中 jsoncpp、abseil-cpp 在 Base，本轮 Unified 提供方 12 个 |
| Unified | `tizen-unified-toolchain_20260917.132101`，[固定快照](https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/tizen-unified-toolchain_20260917.132101/) |
| 关联 Base | build.xml 的 base_id 为 `tizen-base-toolchain_20260914.073422`；[固定快照](https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260914.073422/) |
| 元数据校验 | 下载 source/packages primary，以 repomd 指定的摘要校验；完整 URL、SHA 在 INPUT_IDENTITIES.json 与 BASE_METADATA_IDENTITY.json |
| 提供方源码 | PROVIDER_SOURCE_METADATA.json 记录每个 SOURCERPM URL、SHA、版本及 version.vcs；BUILD_SET.tsv 列 Gerrit 仓路径与完整提交。VCS 来自固定快照元数据，不是猜测当前分支 HEAD；本轮未 checkout 或构建这些源码 |

### 依赖集合与层次

按源码 RPM metadata 的 BuildRequires，经二进制 Provides → SOURCERPM 回溯，再用该源码产物的 C++ 运行时 Requires 作为阳性证据，发现 **12 + 39 = 51 个直接相关候选**。39 个额外包都是直接层，不是递归膨胀；超过 30 个的范围上限。已提问是否先试 12 个提供方、实际缺依赖时再补到不超过 30 个；**未获回复，不自行裁掉直接依赖**。输入门禁已独立阻止全部架构启动，因此未据未决范围开工。

`BUILD_SET.tsv` 是这 51 个包的**发现集合，非获批启动清单**；`DIRECT_BUILDREQUIRES.tsv` 给出每条依据。`SET_INTERNAL_EDGES.tsv` 仅对集合内再排依赖，不扩充集合。拓扑层次见 SCOPE_RESULT.json 的 layers（11 层，无集合内循环）；集合外依赖不因此视为已满足。仅 Unified 中未找到的 36 类 BuildRequires 已全部在关联 Base metadata 中定位，见 BASE_BUILDREQUIRES.tsv；这证明仓库能力名映射，不证明本地 libc++ 输入存在。

方法边界：运行时依赖是 C++ 阳性证据，不足以否定纯头文件/静态载体，也不证明该依赖向消费者暴露 C++ ABI。未命中写 CXX_NOT_OBSERVED，不写“不含 C++”。本轮未逐包展开所有 spec 条件，也未由 GBS 求解出实际必须先重建的最小闭包；该项尚未闭合。

## 本地 Base RPM 盘点与门禁

完整路径、SHA256、大小、包头 Requires/VCS 在 RPM_INVENTORY.tsv/json；保留候选在 RETAINED_LIBCXX_INPUTS.tsv。盘点包含 415 条记录、394 个绝对路径、253 个不同 SHA（包含 GCC 对照与重复副本，不是 415 个可用输入）。查询命令、输出及退出码在 inventory_commands_initial.json 与 inventory_commands.json。搜索范围在 INVENTORY_SEARCH*.json；补查原 scratch 写包目录，没有把解包目录重新打包成 RPM。

| 架构 | 仍可定位的 libc++ 输入 | 缺失的指定输入 | 决定 |
|---|---|---|---|
| armv7l | llvm、libcxx-runtimes、abseil-cpp、tensorflow2、bcc-tools、bpftrace | boost、icu、jsoncpp、libsigc++、pcre、taglib | NOT_OBSERVED：不启动该架构 |
| aarch64 | llvm、libcxx-runtimes、tensorflow2、bcc-tools、bpftrace | abseil-cpp、boost、icu、jsoncpp、libsigc++、pcre、taglib | NOT_OBSERVED：不启动该架构 |
| x86_64 | tensorflow2、bcc-tools | llvm、libcxx-runtimes、abseil-cpp、boost、icu、jsoncpp、libsigc++、pcre、taglib | NOT_OBSERVED：不启动该架构；bpftrace 因 ExclusiveArch 不适用，不作为缺失项 |

“缺失”限定为记录的本地输出/保留路径中 **未取得符合要求的 libc++ RPM**，不宣称整个磁盘不存在。历史源码配方一致性沿用 LLVM_W4_0923/spec-audit_R3 与 R4 已关闭的最终裁决；本轮 raw/062 重算所涉 spec SHA，与审计表一致。llvm/runtime 的获准差异已由 5c169afc 纳入，R3 旧“未推送”标记不再作当前结论；ARM 与 aarch64 输入来源分别为 ARM_W5_0921 的整轮产物及 aarch64_complete_1417，runtime 为 c5358237…配方产物。复用具体输入名单为 BPF_W1_0921/installed-{arch}-inputs.json，并重新读取文件/摘要；这些旧 JSON 本身只证明拟安装清单，不拿它们证明本轮安装。

关键反例：R105 历史 libc++ Boost RPM Release 是 ARM 105.10.3、aarch64 105.10.5、x86_64 105.10.7；当前输出目录为 GCC 的 105.10.4/6/8，boost-thread 包头要求 libstdc++.so.6（raw/032、040、RPM_INVENTORY）。旧 libc++ 解包 payload 仍可见，但不是可安装 RPM。本轮不替换成 GCC RPM、不擅自重建 Base、不从旧解包目录拼装包。其它 R104 目录同样不能凭目录名含 libcxx 就认定当前 RPM 为 libc++。

## 逐包逐架构结果

| 提供方 | armv7l | aarch64 | x86_64 |
|---|---|---|---|
| app-core | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| bundle | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| cert-svc | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| dali2 | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| dali2-adaptor | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| dali2-toolkit | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| gtest | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| inference-engine-interface | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| isf | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| libzypp | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| manifest-parser | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |
| notification | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED |

全部 36 格原因是上表架构输入门禁。退出码与构建耗时均 NOT_OBSERVED，不填 0；机读结果为 RESULTS.tsv。额外 39 个直接依赖全部未跑，列表为 SCOPE_RESULT.json 的 direct_CXX_dependencies。

| 统计项 | 本轮结果 |
|---|---|
| 实际启动构建 | 0 |
| 头文件/API、硬编码标准库、GCC 参数、测试、其它编译失败 | 均 NOT_OBSERVED；没有构建样本 |
| 依赖缺失连锁编译失败 | NOT_OBSERVED；只有开工前输入缺口，未触发包求解/构建失败 |
| C++ ELF 标准库与 GLIBCXX 检查 | NOT_OBSERVED；没有本轮产物 |
| 纯 C ELF 被引入 libc++abi 的数量 | NOT_OBSERVED，不能写 0；也不能仅凭无 C++ 动态符号就断言纯 C |

## 配置副本与资源

Unified 前置 SHA `a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086` 匹配；R84 两份 patch 均直接应用成功，无需内容重写。临时配置在 tmp/UNIFIED_NIGHT_0927/configs/；完整 diff 与前后 SHA 在 raw/037、038、054。原 codes/ 配置 SHA 前后相同。尚未创建可启动的优先 RPM 仓/完整 gbs 配置，未执行 payload 宏展开或 gbs build；后续必须显式用 w5.xzdio，禁止 T<数字>。

开工 resource_gate medium 退出 0，磁盘约 257 GiB；两次 I/O 探测 2.146 s / 2.020 s，均小于 30 s，无需暂停重试。输入盘点串行，nice19/ionice idle，cgroup 实测 memory.max=**16536453120 字节**（总内存 33072910336 字节的 50% 按 4096 字节页向下取整；严格不超过 50%）。实际 PID、cgroup、nice、ionice 在 RESOURCE_inventory*.json。本轮无构建进程，-j1 与每 500 目标探测未触发，不声称已验证构建约束。

未改 spec/codes、未上板、未推包仓/sandbox、未启动 QuickBuild。项目材料单独提交推送；仓内原有及运行线的其它变更不纳入。

## 判断、修正与未决

详见 DECISIONS.md。当前不具备“哪些提供方可直接编过”的实测答案。下一步需提供缺失的已验证 libc++ RPM，或由人工另行授权 Base 输入补建；同时确定超过 30 个直接相关候选的裁剪方式。不能把此前 QuickBuild 配方已齐备，等同于本地所有架构的 RPM 输入仍保留齐备。
