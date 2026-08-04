# R4：LLVM 22.1.8 packaging spec 迁移改版

状态：`DONE_SECOND_SEGMENT_NO_BUILD`，任务状态行见 `progress/R4/state.tsv:1`。本轮仅迁移 packaging 输入并做静态、解析级机械校验；没有运行构建、没有产生 RPM、没有重打包平台 tarball，也没有修改 `plan_evaluation` 或备份目录。

## 基线入仓

迁移基线采用人工裁决指定的 21.1.1 dirty 工作树 spec，而不是该参考仓 HEAD。输入冻结时，参考仓 HEAD 为 `67e4b73a5f6a40545555ba6b1318807967f86678`，工作树恰有 `packaging/libcxx-runtimes.spec` 一项修改；spec SHA256 为 `25db13b28204be28d21bab4a54cfbfc96b2abfc9e3f114f1041175a967f13f4f`。全部输入路径、大小、SHA256 和只读属性见 `progress/R4/input_identity.tsv:1`，冻结命令与退出码见 `progress/R4/commands/00_freeze_inputs.log:1`。

工作树版 spec 已逐字复制为 `packaging/libcxx-runtimes.spec.baseline-21.1.1:1`；`libcxx-llvm21.map`、`libcxx-runtimes.manifest` 和原始 `SOURCE_PROVENANCE` 同批导入。四项逐一 `cmp=0`，哈希及末尾 16 字节见 `progress/R4/baseline_import.tsv:1` 和 `progress/R4/commands/01_import_baseline.log:1`。该基线形成独立提交 `4d04d307e6d7b8da6c5f0ea1ea5168be941b3207`；其来源、dirty 性质和裁决留痕见 `progress/R4/baseline_commit.tsv:1`。

## 迁移结果

生产 spec 为 `packaging/libcxx-runtimes.spec:1`。V01--V19 与 C01--C13 的逐项“原文 / 新文 / 依据 / 证据”完整表见 `progress/R4/change_items.tsv:1`。本次实际变化仅为：

- `llvm_major` 保留并由 21 改为 22，`llvm_version`、RPM Version 改为 22.1.8，Release 从 1 起；依赖宏文本不变、展开值随之更新。
- Source0 改为 `llvm-%{version}.tar.gz`，展开后精确对应平台原名 `llvm-22.1.8.tar.gz`；`%setup` 使用 `-n llvm-%{version}` 对应顶层 `llvm-22.1.8`。没有复制或重打包该 tarball，三锚 SHA 未变。
- Source1 改为 `libcxx-llvm22.map`。新 map 与 `libcxx-llvm21.map` 的逐行 diff 只有节点名 `LLVM_21` → `LLVM_22`，见 `progress/R4/evidence/libcxx-llvm21_to_llvm22.map.diff:1`；spec 描述和 Source1 间接 version-script 引用同步对齐。
- 新增 22.1.8-1 changelog；21.1.1-2 与 21.1.1-1 历史块逐字保持。历史说明中的 `LLVM_21` 仍只存在于 changelog，当前条目使用 `LLVM_22`，从而同时满足历史保留与“非 changelog 零命中”的明确断言。

spec 完整基线到新版 diff 见 `progress/R4/evidence/baseline_to_22_spec.diff:1`。新旧 SHA 汇总见 `progress/R4/new_old_sha256.tsv:1`：新版 spec SHA256 为 `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74`，新版 map SHA256 为 `e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d`。

## 关联输入

`packaging/libcxx-runtimes.manifest:1` 沿用名称和内容，与 21.1.1 输入逐字 `cmp=0`。实测内容只有通用的 Tizen `<domain name="_"/>` 请求，不含版本、源码名、顶层目录或安装路径，因此 C03 选择“沿用”，未作推断性改写。各输入处置和哈希见 `progress/R4/auxiliary_inputs.tsv:1`。

`packaging/SOURCE_PROVENANCE:1` 已改为 22.1.8 三锚：

- tarball SHA256：`16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e`；
- 平台 spec VCS：`platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146`；
- `llvmorg-22.1.8^{}`：`ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。

同时保留 `stdlib_abi=libcxxabi`、`unwinder=libgcc_s`、`compiler_rt=disabled`、`llvm_unwinder=disabled` 四项打包策略字段。完整来源文件 diff 见 `progress/R4/evidence/SOURCE_PROVENANCE_21_to_22.diff:1`。

## 不变的打包决策

四个子包及 Requires 形态、40 个 `-D` 参数和值、两个多库安装目录参数、ABI v1、libgcc_s unwinder、static/new-delete 策略、三个 build target、`LLVM_ENABLE_RUNTIMES="libcxxabi;libcxx"` 全部保持 21.1.1 决策。虽然 22.1.8 源码新增完整 `libunwind` 目录，spec 仍明确不启用它。

40 个参数区块与基线逐字 `cmp=0`。其中 26 个 LLVM runtime 项目选项已逐项在 22.1.8 源码重新定位，14 个登记为标准 CMake 变量；未发现任何项目选项消失或改名。逐项结果和三个 build target 原文见 `progress/R4/option_presence_22_1_8.tsv:1`、`progress/R4/commands/06_option_and_policy_precheck.log:1`。

符号节点与 T5 一致性也已核对：`gates/tools/run_static_gates.sh` SHA256 仍为冻结值 `d24f7c234d266bf970dbfd9c24939052681eba2105c186d0002009891c119885`，三处 `@@LLVM_22` 命中见 `progress/R4/evidence/T5_LLVM_22_gate_consistency.log:1`，`@@LLVM_21` 为零。

## 机械自检

全部可静态执行的门项通过，汇总见 `progress/R4/self_check.tsv:1`：

- 按 `progress/R4/reverse_normalization_rules.tsv:1` 将新版 spec 的全部申报变化反向归一化后，与 dirty 基线 SHA256 同为 `25db13b2…`，逐字 `cmp=0`、逐行 diff 退出 0；因此没有未申报差异。完整命令、退出码及各不变区块的 `cmp` 结果见 `progress/R4/commands/08b_static_invariants.log:1`。
- map 将 `LLVM_22` 反向归一化为 `LLVM_21` 后，与旧 map 逐字 `cmp=0`。
- `rpmspec -P packaging/libcxx-runtimes.spec` 退出 0；Version、Release、Source0、Source1、BuildRequires、setup 顶层和 version-script 展开断言全部通过，完整原文见 `progress/R4/commands/07_rpmspec_parse.log:1`。
- `c++/v1`、`%{_datadir}/libc++/v1`、`libc++.so.1*`、`libc++abi.so.1*` 原样存在；非 changelog 的 `LLVM_21` 命中数为 0。
- 子包区块、CMake/targets 区块、四个 `%files` 区块以及既有历史 changelog 分别与基线逐字 `cmp=0`。

首次静态自检因零命中计数命令返回空串而退出 2；迁移对象的反向归一化在该次已实际 `cmp=0`。该命令表达错误没有被隐藏，原日志保留；改为显式输出 0 后，同一判据由 08b 通过。首次交付就绪脚本又因占位符扫描包含脚本自身而自匹配退出 1；收敛到交付数据文件后由 09b 按原判据通过。两项及调试记录均见 `progress/R4/execution_incidents.tsv:1`，最终只读边界和交付就绪原文见 `progress/R4/commands/09b_readonly_and_delivery_readiness.log:1`。

## 已知缺口与停止边界

C10 登记为 `PENDING_FIRST_BUILD_VERIFICATION`，见 `progress/R4/known_gaps.tsv:1`。四个 `%files` 区块虽与基线逐字不变，但静态检查不能证明 22.1.8 的实际安装清单；本报告不把它伪装成 PASS，须在人工重开 T3 的首次构建后逐路径核对。

第二段到此停止：没有进入配置、编译、安装或打包，不进入 T3 构建。
