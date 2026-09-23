# LLVM W4 R4：推送登记与配方审计关闭

**QuickBuild 所需的全部本地配方改动均已进入 sandbox。** 按本轮人工裁决关闭历史/测试配方差异，LLVM W4及当前验收配方审计关闭；QuickBuild的配方与分支前置已满足。清单共12个构建包、11个Gerrit仓（llvm与libcxx-runtimes同仓），逐行远端查询全部MATCH。本轮没有构建、上板、包仓推送或启动QuickBuild。

注意范围：这是配方准备完成，不是服务端构建已通过。实时OBS包定义的_link/_multibuild/_service未取得；当前公开reference也没有列出libcxx-runtimes。下文保留这一观测，不将历史元数据冒充当前服务端配置。

## 一、推送登记

| 项 | 实测值 | 依据 |
| --- | --- | --- |
| 远端HEAD | 5c169afc97945bfb88786d9f38477f6973fe5285 | raw/R4_004_llvm_remote；branch_commands_R4.json |
| 父提交 | da14498e07fa4c841b5a6bc7f88766d993982a16 | raw/R4_010_source_identity末尾 |
| 再上级 | f203923a1508c9344f5fc6b17bd8822f011655c4 | 同上 |
| 两笔Author/Committer/Signed-off-by | 均为Hao Lin <hao.lin@samsung.com> | raw/R4_005_llvm_identity |
| 远端与本地db5b49af候选 | 完整树零差异；diff --stat退出0、输出空 | raw/R4_006_tree_diff |
| 两边tree | b9c8c2b276f55ca8e2cb6bd490e9d5cc11c56f8f | raw/R4_010_source_identity |
| 本轮包仓动作 | 仅fetch、ls-remote、log/diff/show/rev-parse等读取；不amend、不签字、不push | raw/R4_*；branch_commands_R4.json |

## 二、26项人工处置

完整路径与逐项登记见`LOCAL_SPECS_R4.md`。下面是人工裁决，不冒充本轮重新构建的结论。R3 diff保持原样，实际头为sandbox/reference在---侧、历史used在+++侧，因此`-`行表示sandbox有而旧副本无。

| ID | 包 | 关闭依据 |
| --- | --- | --- |
| S003 | bpftrace | 旧版本，已由7831fb34取代 |
| S110 | bpftrace | 旧版本，已由7831fb34取代 |
| S019 | bpftrace | static新配方前旧版本，已取代 |
| S020 | bpftrace | static新配方前旧版本，已取代 |
| S021 | bpftrace | static新配方前旧版本，已取代 |
| S022 | bpftrace | static新配方前旧版本，已取代 |
| S008 | llvm | 缺f203923a已有MLGO/libc++条件，旧版本已取代 |
| S026 | llvm | 缺f203923a已有MLGO/libc++条件，旧版本已取代 |
| S029 | boost | 人工裁决R104 flags位置差异；R105验收版99cdc598与sandbox一致 |
| S035 | boost | 同上 |
| S051 | tensorflow2 | 验证专用GCC开关，按设计不推送 |
| S053 | tensorflow2 | 验证专用GCC开关，按设计不推送 |
| S054 | tensorflow2 | 验证专用GCC开关，按设计不推送 |
| S056 | tensorflow2 | 人工将该验证/基线对照组排除于发布配方 |
| S023 | libcxx-runtimes | R91旧迭代；最终采用5c169afc |
| S097 | libcxx-runtimes | R94旧迭代/实验，已取代 |
| S098 | libcxx-runtimes | R94旧迭代/实验，已取代 |
| S099 | libcxx-runtimes | R94旧迭代/实验，已取代 |
| S100 | libcxx-runtimes | R96旧迭代，已取代 |
| S101 | libcxx-runtimes | R96旧迭代，已取代 |
| S102 | libcxx-runtimes | R96旧迭代，已取代 |
| S103 | libcxx-runtimes | R96旧迭代，已取代 |
| S104 | libcxx-runtimes | R96旧迭代，已取代 |
| S069 | pcre | R29早期实验；R104验收版与sandbox一致 |
| S070 | pcre | 同上 |
| S071 | pcre | 同上 |

runtime旧迭代中Patch0..9为GBS导出生成，R94 override为当时实验；最终sandbox含更新condvar LLVM_22_TIZEN_1及本次静态配方。另27项NOT_AVAILABLE按人工裁决属测试夹具/R29 Unified实验包，排除Base-Toolchain范围；其历史标签不改。6个获准LLVM/runtime副本现已进入5c169afc。R2全根溯源、R3原始表/diff及111条未定位历史命令继续保留，不继续扩展调查，也不改写为已核实。

## 三、libcxx-runtimes构建方式

| 观测 | 身份与结果 | 能证明什么/边界 |
| --- | --- | --- |
| 固定历史Base-Toolchain二进制primary | tizen-base-toolchain_20260828.101647；libc++、libc++-devel、libc++abi、libc++abi-devel × 3架构，共12项，SOURCERPM全为libcxx-runtimes-22.1.8-1.1.src.rpm | 独立源码构建单元，不是llvm.spec一次写包的子包 |
| 同快照源码primary | 单独列出libcxx-runtimes-22.1.8-1.1.src.rpm | 与上述来源互证 |
| 远端spec | packaging/libcxx-runtimes.spec:16为Name，41/48/57/63为四个子包；llvm.spec:38为Name llvm | 同一Gerrit仓内两份独立spec；raw/R4_014原文行号 |
| 本地项目manifest | codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/manifest.xml:109指向platform/upstream/llvm | 仓映射；不是实时OBS服务定义 |
| 本地GBS配置 | config/gbs_llvm.conf指向Base-Toolchain与Unified-Toolchain仓，未定义独立_link/_service | 不从GBS仓URL推断OBS内部接入形式 |
| 当前reference | build_R4.xml:3为tizen-base-toolchain_20260914.073422；下载该固定快照packages/source primary，未列出libcxx-runtimes及上述四类二进制 | 仅本快照公开元数据缺项；原因NOT_OBSERVED，不据此认定OBS包已删除 |
| OBS实时_link/_multibuild/_service | NOT_OBSERVED | api.tizen.org DNS失败；build.tizen.org source路径重定向QuickBuild后404；本地指定仓目录未找到这三类文件，不猜具体机制 |

历史两份primary校验与repomd SHA一致，完整URL/输入SHA/所选原文见`HISTORICAL_METADATA_R4.json`；现有快照下载URL、SHA与原文见`METADATA_IDENTITIES_R4.json`、`RUNTIME_METADATA_R4.json`及两份primary压缩文件。只使用Tizen-Base-Toolchain，不混用GCC Tizen-Base。

**QuickBuild需要同时覆盖llvm与libcxx-runtimes两个源码构建单元**，两者均指向`platform/upstream/llvm`的`sandbox/lhmax2025/libcxx-ehabi-backport`，SHA `5c169afc97945bfb88786d9f38477f6973fe5285`，分别选择`packaging/llvm.spec`与`packaging/libcxx-runtimes.spec`。因此清单增加libcxx-runtimes一行；该构建单元身份来自已发布固定快照，不声称本轮读到了实时OBS配置。人工起QuickBuild时须确保两份spec都被选中，不能只设置llvm而遗漏runtime。

## 四、QuickBuild分支清单

交付`QUICKBUILD_BRANCHES_R4.tsv`全文12行：11个适配源码包加独立runtime构建单元。`branch_commands_R4.json`逐行记录ls-remote argv、stdout/stderr、退出码，12/12 MATCH；llvm/runtime更新为5c169afc，bpftrace保持7831fb34，其它沿用前轮登记SHA且重新核验。表不是构建成功证明。

| OBS构建包 | Gerrit仓 | 分支 | 远端SHA |
| --- | --- | --- | --- |
| abseil-cpp | platform/upstream/abseil-cpp | sandbox/lhmax2025/libcxx-migration | 7a4f04dad6d30ba09c406837cdd23ad779a69210 |
| boost | platform/upstream/boost | sandbox/lhmax2025/libcxx-migration | 4168e873584ae50a12733262026c007dce18ec89 |
| icu | platform/upstream/icu | sandbox/lhmax2025/libcxx-migration | dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 |
| jsoncpp | platform/upstream/jsoncpp | sandbox/lhmax2025/libcxx-migration | 9cbab147f44097d5b8da2b3ef38f75376a282d01 |
| libsigc++ | platform/upstream/libsigc++ | sandbox/lhmax2025/libcxx-migration | 4f2f592d2f88eb6a3a949cec8970a4cd5d40165b |
| pcre | platform/upstream/pcre | sandbox/lhmax2025/libcxx-migration | 09a5bfc9a96c4235628ce800db3b95bf28ef2f23 |
| taglib | platform/upstream/taglib | sandbox/lhmax2025/libcxx-migration | 8b552bdf27c913a93bca664c41c3fafb214962a1 |
| tensorflow2 | platform/upstream/tensorflow2 | sandbox/lhmax2025/libcxx-migration | 6d76139029ca4162249667f0db65a73387a94cfe |
| llvm | platform/upstream/llvm | sandbox/lhmax2025/libcxx-ehabi-backport | 5c169afc97945bfb88786d9f38477f6973fe5285 |
| bcc-tools | platform/upstream/bcc | sandbox/lhmax2025/libcxx-migration | 5bd0654e24bb231035ea684d278f674aa95a2cd2 |
| bpftrace | platform/upstream/bpftrace | sandbox/lhmax2025/libcxx-migration | 7831fb34c6881b5cff0b8bdd0dac27a1829290fa |
| libcxx-runtimes | platform/upstream/llvm | sandbox/lhmax2025/libcxx-ehabi-backport | 5c169afc97945bfb88786d9f38477f6973fe5285 |

## 五、备案与自行判断

- `PACKAGE_STATUS.md`只更新llvm行的SHA和备注，其他10包行不变。旧表序言仍是历史状态，当前结论以本报告与LINE_STATUS为准，不擅改其它文字。
- `LINE_STATUS.md`关闭LLVM W4和配方审计，下一步：**人工起Tizen-Base-Toolchain QuickBuild → 结果与QB_PRECHECK_0922/EXPECTED_FAILURES.md对账**。正常RPM安装验证仍留待镜像阶段。
- 额外判断仅为把历史独立SRPM身份映射到同仓两份spec，且明确标注时间范围；没有推断OBS究竟使用_link、_multibuild还是独立_service。当前reference缺项原样申报，不扩展为原因分析。
- 技术记录：初版采集脚本假设当前reference仍含runtime，断言失败退出1；证据保留，未改数据凑通过。后续改用已固定历史元数据完成构建单元查证。raw/R4_014末尾rg无文件匹配退出1，不是构建失败。只读查询的错误/无结果均保留。
- 全部包仓不推；未修改平台源码、spec、项目配置。项目材料独立提交推送，远端SHA见交付回执与最终回复。
- 提交前安全自检已执行：无凭据命中；本次暂存范围、12行远端MATCH、26/27/6处置计数、候选树一致性、PACKAGE_STATUS仅llvm行变化、旧R2/R3证据不变均PASS。原始日志含CRLF、行号制表符与尾空行，完整`git diff --check`退出2；原样保留，不为格式检查改证据，另对非raw交付文件检查。
