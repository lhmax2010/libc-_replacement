# LLVM 配方 R2：PARTIAL，等待展开空行口径确认

没有构建、上板、包仓推送或 QuickBuild。第一笔本地提交仅正文已修改；第二笔 runtime 提交尚未创建。不能把本报告理解为已完成两笔提交或全量输入门禁。

## 一、等价性与本地提交

真实 ARM 根、原宏参数及逐命令退出码见 `equivalence_R2.py`、`prep-check/commands_R2.json`；基线均为 f203923a。展开命令均退出 0，判据另行检查。

| 配方 | 默认 | _toolchain_override gcc | _toolchain gcc | --undefine _toolchain |
| --- | --- | --- | --- | --- |
| llvm.spec | PASS：只加 LIBCLANG_BUILD_STATIC | PASS：只加 LIBCLANG_BUILD_STATIC | PASS：完全相同 | PASS：完全相同 |
| libcxx-runtimes.spec | 严格字节判据未通过：获准六项之外多 12 个开头空行 | NOT_OBSERVED，已暂停 | NOT_OBSERVED，已暂停 | NOT_OBSERVED，已暂停 |

runtime 原始 diff：`prep-check/runtime_R2/default.diff`。四个 OFF→ON 开关与两条 .a 文件记录均在授权范围；另外 12 个开头空行对应新增条件宏定义的展开。未发现其他非空内容差异，但未自行豁免空行。已询问是否允许单列为非功能差异；确认前不修改比较器、不创建第二笔、不执行 runtime %prep。原始 FAIL 保留。

| 项 | 结果 | 证据 |
| --- | --- | --- |
| fetch 远端 | f203923a1508c9344f5fc6b17bd8822f011655c4 | raw/R2_002、R2_003 |
| 第一笔 | 617a210064c4559fe1152728f7c9cf5e1ab99a9f；替代 2cba97e5，仅正文变，tree 相同 | raw/R2_014、R2_015、LOCAL_COMMITS_R2.json |
| 作者/签字 | Hao Lin <hao.lin@samsung.com>；无 Signed-off-by；committer 保留现有配置 | SANDBOX_COMMITS_R2.diff |
| 第二笔 | NOT_CREATED；包仓 runtime spec 未改 | LOCAL_COMMITS_R2.json |
| 本地状态 | 干净，ahead 1 / behind 0，不是目标 ahead 2 | export_commands_R2.json |
| runtime 候选 | c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe，身份符合 | raw/R2_003 |

`SANDBOX_COMMITS_R2.diff` 当前仅第一笔全文，format-patch 也仅 `0001-Build-static-libclang_R2.patch` 一份。旧 R1 文件保留，当前身份以 R2 为准。

## 二、输入溯源：追加事务不等于全根

BPF_STATIC 实际绑定 ARM `input-armv7l-origin`、aarch64 `input-aarch64-v3`；主包与 static 共用同一输入根。原19份清单在事务前写出，本身不是安装成功证据；实际安装成功记录沿用 `BPF_W1_0921/INPUT_STATUS.md`。本轮又逐份重算38个现存RPM的SHA256，并与当前rpmdb的 name/version/arch/包头SHA1核对，均匹配。

| 追加输入（每架构19份） | ARM | aarch64 | 依据 |
| --- | --- | --- | --- |
| libbpf-static / liblzma-static / zlib-devel-static / libffi-devel-static / libffi / libffi-devel | 6 A | 6 A | 原公开下载URL及SHA与现存RPM重算SHA一致 |
| bcc-tools-static | 1 B | 1 B | sandbox 5bd0654e…；两架构导出spec相同，仅比sandbox多VCS |
| LLVM八个子包 | 8 C | 8 C | 获准libclang配方；待推送后转B |
| libc++/libc++abi主包及devel | 4 C | 4 C | 获准runtime配方；待推送后转B |

逐份名称、版本、SHA、路径与分类：`inputs_R2/introduced_R2.json`。授权 C 类完整差异：`BASELINE_TO_VERIFIED_LLVM.diff`、`UNPUSHED_LIBCXX_RUNTIME.diff`。BCC两架构导出spec均为8b290fb0…，相对sandbox只差VCS：raw/R2_022。旧LLVM aarch64 108.2输入spec相对f203923a也只差VCS（raw/R2_012），但完整patch/资产链尚未重核，不据单份spec把全根判为B。

### 全根当前保留状态：未闭合

| 根 | rpmdb记录 | A | B | C | 尚未能归类 |
| --- | ---: | ---: | ---: | ---: | ---: |
| bpftrace armv7l（含主包输入） | 133 | 117 | 1 | 12 | 3 |
| bpftrace aarch64（含主包输入） | 134 | 118 | 1 | 12 | 3 |
| LLVM armv7l | 126 | 122 | 0 | 0 | 4 |
| LLVM aarch64 | 127 | 0 | 0 | 0 | 127 |

完整表：`inputs_R2/ALL_INPUTS_R2.tsv`。空分类字段是 **NOT_OBSERVED：证据未闭合**，不是新增第四类来源，更不是 C=0 就证明无本地候选输入。

A类中，38份追加输入以外的SHA来自保留Base-Toolchain仓primary.xml，按确切name/version-release/arch匹配；`sha_source`区分“仓元数据SHA”和“现存RPM重算SHA”。前者不是现存RPM重新校验，也不是根内文件逐项验证。原ARM下载URL在保留.build.log中，元数据身份见`metadata_identity_R2.json`。当前保留根查询不证明历史构建瞬间所有文件未变。

未归类的BPF项为原有BCC主包/devel与lldb；LLVM ARM为四个既有runtime包。LLVM aarch64原根路径/缓存当前不可得（raw/R2_018），私有根仍可查询127项；其八个本地追加输入历史清单为`BUILD_STATIC_0917B/private_root_rpm_inputs.json`。这些输入逐份RPM身份、完整配方/资产与当前sandbox对应尚未核完。没有把 `(none)` / `-dirty` VCS直接判C，也不把历史已提交但版本不同的输入直接判“未进任何sandbox”。

已试方法：raw/R2_007–013、R2_016–022；`inputs_R2/commands_R2.json`记录四根查询均退出0。跨全部巨大GBS树的索引主动停止，随后按历史GBS命令定位精确路径；此为查询调整，不是构建失败或缺包证据。

## 三、裁决与下一步

| 项 | 结论 |
| --- | --- |
| 旧GCC override门禁 | 已撤销；LLVM新四组全部PASS |
| runtime | 功能范围已授权；12个展开空行是否可作为非功能差异待确认 |
| 全量输入 | 未完成；不能宣布仅有获准C类或全量无C类 |
| QuickBuild前置 | LLVM仓两个提交推送 + 输入溯源无C类，并闭合未归类项；当前不满足 |
| 执行边界 | 仅第一笔本地amend；无包仓推送、build或板操作 |

自行判断：保留根不冒充历史快照；来源证据不足保留空分类与NOT_OBSERVED；RPM重算SHA和仓元数据SHA分列；空行不自行豁免。技术性路径查询非零保留，不能当作判据失败。资源light门禁退出0。

停下交人工确认。确认后继续runtime其余三组、第二笔提交、%prep及全量溯源；本轮没有裁决任何缺口可忽略。
