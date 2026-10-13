# QuickBuild #1194606 修复 R3：真实 ref 已确认，cereal 验收失败后停止

**未完成三项修复推送。** cereal 已用 manifest 精确 SHA 构建，批准的 CLANG_USE_LIBCPP=ON 生效，但 performance 又遇到 libc++ 的 uniform_int_distribution<char> 编译错误。按原 FIX 第 8 条“任一验收失败，保留现场、停下回报”停止；未替换修法、未跳过 performance，ICU/qemu 不启动后续构建，三个本地提交均未推送。R3 规定 cereal 源码不可得时继续其它包的分支未触发：本轮源码全部可得，停止原因是实际验收失败。

## 一、真实 ref 与候选

| 包 / ref | HEAD SHA / 采用基线 | 包含指定 revision | packaging spec |
|---|---|---|---|
| cereal / tizen | 896a21f0cbcb34835bba8d39f2a4254d691ea98f | 否，空树占位 | 不存在 |
| cereal / tizen_base | 28aedc95fdc600568c57babf28ff717e0b2bca7d | 是，同一提交 | 存在 |
| cereal / accepted/tizen_base、base_dev、base_toolchain、base_x（完整 ref 见 REFS_R3） | 28aedc95fdc600568c57babf28ff717e0b2bca7d | 是 | 存在 |
| icu / sandbox/lhmax2025/libcxx-migration | dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 | 是 | 存在 |
| qemu-accel / tizen_base | HEAD 419d8296b4073bbabab7123bec1abe4146087126；采用 manifest 2737f3bf78ed33be4333114c0e5c741a1deca467 | 是；不追较新 HEAD | 三个生成 spec 存在 |

完整 heads/tags/ls-remote/cat-file/contains 原文见 raw/040–052。manifest 的三行 revision 原文及 upstream 字段缺失说明见 REFS_R3.md；ICU manifest 旧 e4a4 与实际 checkout 不一致，按人工指定使用 dcbb。没有猜其它 ref。qemu HEAD 多的 419d829“Remove toybox binary from /emul to prevent host arch leaking”未纳入本轮精确基线。

| 包 | 仅本地候选提交 | 作者/提交人/签字 | Change-Id 与完整提交 |
|---|---|---|---|
| cereal | d62ccf97dc889b02ffbfd6c9630469f3d93fe495 | 均 Hao Lin <hao.lin@samsung.com> | Ie8f53373d7f06ce50ea0135950d5fa0913fc99df；raw/064_R3_cereal_show.stdout.txt |
| icu | b7ab6868d425e3190057e6d9f3c2b69646793df1 | 同上 | I7d9788029f712a53b8d822b070aec86e161711f4；raw/065_R3_icu_show.stdout.txt |
| qemu-accel | 3b8d6274822cc2c2ce373eff7b921f5b3de3062e | 同上 | I8b578a992218d1111569cf01fdb97977cbaabe4e；raw/066_R3_qemu_show.stdout.txt |

三仓工作树干净（raw/079）。cereal 仍保留旧 origin/tizen 跟踪关系，显示 ahead/behind 仅相对空占位，不代表有远端 sandbox；未修改跟踪分支冒充已推送。

## 二、补丁与四组宏门禁

| spec | default | override gcc | toolchain gcc | undefine toolchain |
|---|---|---|---|---|
| cereal | PASS，ON | PASS，OFF | PASS，OFF | PASS，OFF |
| icu | PASS | PASS | PASS | PASS |
| qemu-accel-armv7l | PASS | PASS | PASS | PASS |
| qemu-accel-aarch64 | PASS | PASS | PASS | PASS |
| qemu-accel-riscv64 | PASS | PASS | PASS | PASS |

比较器仅忽略纯空行，严格排除批准内容之外的差异；不是声称所有组合 baseline 与候选无差异。原始 candidate-diffs 的 apply --check 全通过（raw/054）。cereal/qemu 原补丁直接应用。ICU 新增 Patch0，采用现有 spec 的显式 patch 调用风格，包装补丁上下文保留 Source4 的 LINKFLAGS；先 PIE 后 -iquote 的应用实验退出 0（raw/071），不改变 PIE。源码本体不直接提交修改。

旧保留根门禁 raw/060 通过后，在本轮 RBS 实际安装根（rpm 4.14.1）重新跑 20 组也全部通过（raw/077，macro-check_R3/actual-RBS-root）。旧工具准备阶段的 bwrap /task 只读挂载错误及 %patch0 字面宏兼容性记录均保留，修正过程见 DECISIONS_R3.md；没有把失败记录删去。构建进程已确认 payload 为 w5.xzdio（raw/075），日志的 init_buildsystem/rpmbuild 命令同样带该 define。

## 三、本地 x86_64 验收

| 包 | GBS 退出码 / 耗时 | 实际结果 | 产物门禁 |
|---|---|---|---|
| cereal | 1 / 450.498 秒，包含输入下载初始化 | 98% performance 编译失败 | 未链接、未写 RPM；performance NEEDED 和头文件 RPM 对照 NOT_OBSERVED |
| icu | NOT_OBSERVED | cereal 失败后按原门禁停止，未启动 | tz2icu 完整构建及 libicu NEEDED NOT_OBSERVED；局部补丁顺序已验证 |
| qemu-accel-armv7l | NOT_OBSERVED | 同上，未启动 | clang-accel RPM 文件清单验收 NOT_OBSERVED |

首个真实错误（build-cereal-attempt2_R3/build.log:9700）：

```text
/usr/bin/../include/c++/v1/__random/uniform_int_distribution.h:138:17: error: static assertion failed due to requirement 'std::__libcpp_random_is_valid_inttype<char>::value': IntType must be a supported integer type
/home/abuild/rpmbuild/BUILD/cereal-1.3.2/sandbox/performance.cpp:261:9: note: in instantiation of template class 'std::uniform_int_distribution<char>' requested here
  261 |     c = std::uniform_int_distribution<char>(' ', '~')(gen);
```

performance.cpp:280 同样使用 `uniform_int_distribution<char>('0', '1')`。本轮归类为 libc++ 头文件/API 约束差异；不扩大成整个 cereal 安装头文件不兼容。实际 CMakeCache 显示 CLANG_USE_LIBCPP:BOOL=ON，performance 的 flags.make 含 -stdlib=libc++。失败发生在编译阶段，**尚未验证原 basic_binary_oprimitive 链接错误消失**。完整日志保留，首错前后各 40 行在 failure-cereal_R3/FIRST_ERROR.txt。

现场未清理：`tmp/QB_1194606_FIX/GBS-ROOT/local/BUILD-ROOTS/scratch.x86_64.0`。关键 CMakeCache、flags.make、link.txt、performance.cpp、spec/config/repomd 独立复制及 SHA/mtime 在 failure-cereal_R3/IDENTITIES.json。没有生成可用于替代失败验收的 RPM。前一发布版 cereal-devel 已从 20260924.094908 固定快照下载、包头读取，SHA 为 8b3b0b2555528178d5c8f33b0713861d63ecb0aae685db831c7e4397c73ca7e3，但新 RPM 未出，不能完成两者对照。

输入 URL：`https://download.tizen.org/RBS/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20261012.081855/repos/standard/packages/`；repomd SHA 为 e568cd09a390bf727d9800f80fc9757e9843fe66f45ecd2b875dbc7e51c2b754。实际 rpmdb：boost-serialization 1.83.0-2.1，llvm/clang 22.1.8-3.4，libc++/libc++abi 22.1.8-2.4（raw/076）。配置副本在 tmp/QB_1194606_FIX/gbs.conf；config/ 未改。第一次 GBS 因 cwd 不正确尚未找到源码即退出，入口修正后才进行上述实际构建；两份日志独立保留。

资源：medium 退出0，磁盘 226GiB，I/O 探测退出0；-j1、nice19、ionice idle。MemoryMax=16536461312 字节，本次实际 rpmbuild 进程 cgroup 与限制原文见 raw/074，输入安装进程见 raw/070。目标数不足500，周期性500目标探测未触发；不能写成已触发。小时记录由监控器保留，构建在一小时内结束。无其它构建并跑、无上板、无新 QuickBuild、无 codes/ 改动。

## 四、台账与 R5

依人工已有裁决，R101 仅 cereal 行改为“需要改（未推送；R3 验证失败）”，不虚报“已推送”。BASE_LEDGER 新增 CEREAL_CORRECTION_1194606_R3.md，说明当前判定为 **12/55/6/0**，其中已推仍 **11**，cereal 待完成；原历史固定快照表不覆写。

QUICKBUILD_BRANCHES_R5.tsv 使用本轮要求的 package/repo/branch/commit 列名，目前 **12 行**，内容仅沿用并重新 ls-remote 核实 R4 已推送分支（raw/R3_branch_01–12，全部 MATCH）。ICU 仍是 dcbb，cereal/qemu 不加入本地候选 SHA。**这是未完成修复的远端现状清单，不是可起第二轮 QuickBuild 的新验收清单。** 无包仓 push，没有任何推送回执可编造；项目材料另行普通推送。

## 五、判断、未决与结论

本轮自行技术判断仅涉及原补丁上下文接入、四组宏允许差异比较、GBS cwd 修正；详见 DECISIONS_R3.md。保留旧 FAIL 原文，不降低产物验收要求。停止规则采用原 FIX 第8条，未把 R3“失败包不推”解读为授权更换失败修法；后续是否先单独继续 ICU/qemu 等人工明确。

需要人工裁决：是否增加仅针对 performance 两处 char 随机分布的源码补丁，以及是否先继续另两包验证。**未实施该追加补丁、未跳过 performance、未重跑。**

| 包 | 最终状态 | 说明 |
|---|---|---|
| cereal | FAIL | 本地构建失败，不推 |
| icu | NOT_AVAILABLE | 源码可得、候选存在；此状态指本轮验收/推送结果不可得（构建 NOT_OBSERVED），不是仓库取不到 |
| qemu-accel | NOT_AVAILABLE | 同上 |

R5 总行数：**12**；本轮包仓推送数：**0**。
