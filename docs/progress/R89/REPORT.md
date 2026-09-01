# R89：修正 SOURCE_PROVENANCE 并推送 sandbox

日期：2026-09-01（Asia/Shanghai）

## 1. 结论

已仅修改 `packaging/SOURCE_PROVENANCE`，以普通快进方式推送到既有分支：

```text
refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
08a64b6eb95ed74339f1d862a6c22a2f93fa78f9
  -> 48fc3a299ba20dd321a7d4a37c4e13ce095176cd
```

提交作者与提交者均为 `hao.lin <hao.lin@samsung.com>`，Change-Id 为
`Ie00b27c05f4e04bac524ff9d13b40e358e4304f2`。提交只包含一个文件，既有
HEAD 是新提交的直接父节点，未丢失任何已有提交。

推送后逐项核对通过：另外 3 个 `sandbox/lhmax2025/*` 分支 SHA 全部未变，
`tizen_base` 与 `tizen_base_next` 未变，没有新建分支，没有使用 force，也
没有向 LLVM 上游推送。

## 2. 自指矛盾的处理

采用人工确认的第 3 种方案：**不在安装文件中记录当前 commit、分支组织
或下游补丁拓扑。**

具体分工如下：

- `SOURCE_PROVENANCE` 记录不受 metadata commit、squash、rebase 或
  cherry-pick 影响的内容身份：
  - GBS Source0 文件名与 SHA256；
  - GBS export tag 及 peeled commit；
  - LLVM 官方 release tag、peeled commit、archive 与 SHA256；
  - 最终 `libcxx` 与 `libcxxabi` Git 子树对象；
  - ABI、unwinder、compiler-rt 和 symbol version 配置。
- 当前包的精确 revision 由 GBS 自动生成 spec 中的 `VCS` 字段记录。
- `PatchN` 数量、编号、文件名、patch SHA、patch-id 与对应 commit 均不在
  本文件记录。

这没有自指：新提交 SHA `48fc3a29...` 不出现在文件中。实测 GBS export
生成：

```text
VCS: platform/upstream/llvm#48fc3a299ba20dd321a7d4a37c4e13ce095176cd
```

同时仍生成 sandbox 实际的 Patch0—Patch9，证明 metadata-only 提交不会
增加源码 patch，而包 revision 确实由生成 spec 承担。

文件开头已加入英文说明，明确本文件记录什么、不记录什么，以及不记录
补丁拓扑的原因，避免未来重新引入不稳定字段。

## 3. 推送前现状

目标分支原 HEAD：

```text
08a64b6eb95ed74339f1d862a6c22a2f93fa78f9
```

推送前 `sandbox/lhmax2025/*` 全部 ref：

| 分支 | SHA |
|---|---|
| `libcxx-ehabi-backport` | `08a64b6eb95ed74339f1d862a6c22a2f93fa78f9` |
| `llvm_only_clang` | `f8277158cf44f872e2b2d62a2e839cb33bd0afea` |
| `mlgo` | `08ffd8cd7c72f5dd6e612d8395362def96d84029` |
| `mlgo_aot` | `88ff57cadcff52ab50ca6865db7faed7388358fe` |

正式分支对照：

| 分支 | SHA |
|---|---|
| `tizen_base` | `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c` |
| `tizen_base_next` | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` |

## 4. 修改前完整文件

```text
component=llvm-project libcxx+libcxxabi
version=22.1.8
source_tarball=llvm-22.1.8.tar.gz
source_tarball_sha256=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
source_tarball_anchor=R9C_GBS_STANDARD_EXPORT
local_tizen_base_vcs=platform/upstream/llvm#3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
local_tizen_base_branch=tizen_base
upstream_tag=llvmorg-22.1.8
gbs_export_tag=upstream/22.1.8
upstream_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
official_release_archive=llvm-project-22.1.8.src.tar.xz
official_release_archive_sha256=922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888
official_release_anchor_scope=CONTENT_COMPARISON_ONLY
export_patch_count=6
export_patch_0001_file=0001-Add-support-for-Tizen-toolchain-vendor.patch
export_patch_0001_sha256=a71a7503e851f4ebe1c192326d832c28f79a81ff1c9db43696d0487c118d587c
export_patch_0001_patch_id=cf2d5d98ab6c48ef843b1c5d9372cc5640ad2b6c
export_patch_0001_commit=53accdc47cd72c0daf61bf70e52946b2d8d80f2e
export_patch_0002_file=0002-Link-libclang_rt.asan.so-against-libatomic-when-nece.patch
export_patch_0002_sha256=cefb73ec25a64f8c70cc0e585dfc688a23eb44b740093755ffaf8ee86f7e716e
export_patch_0002_patch_id=d47008f74357f06fa89e89aa04ac02242138568e
export_patch_0002_commit=f31417256f953cc5d26cd188704c89ad41572b50
export_patch_0003_file=0003-Fix-armv7l-build-when-compiler-is-clang.patch
export_patch_0003_sha256=9ce436eb2f73e01b112cfeb10d6108e33c719b1db54a8fc404abdc10c367102b
export_patch_0003_patch_id=3f0c4ed2b51658bc1585361385439d453458851d
export_patch_0003_commit=9576175b59fa577c940eff1712a59b129ae03f03
export_patch_0004_file=0004-clang-add-support-for-LLVM-gold-plugin-in-case-of-ac.patch
export_patch_0004_sha256=45d68e84754f93ebe10ba80725e6cbb28826890e344f0ae910e061690105e944
export_patch_0004_patch_id=4584830adae087a4fb6348b80da4c755699bae35
export_patch_0004_commit=9ad40bd946a75b44d8e05de8ba3263a5ce2ed885
export_patch_0005_file=0005-Merge-MLGO-AOT-compilation-support-and-optimization-.patch
export_patch_0005_sha256=da2cacae22ef8ececf808596b14ff53b6741db23e3f34d7fc5e07a75fa3543c2
export_patch_0005_patch_id=bac21649416c0ea19ee8fa7ef00d4a7d8fa3dafa
export_patch_0005_commit=fd7a4929c0e424d564cdd1f0c55af9153dcea321
export_patch_0006_file=0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch
export_patch_0006_sha256=b9fdf66418276bbb97183be952aab96606c0662b17b95de077e99fecc7d2d79c
export_patch_0006_patch_id=ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479
export_patch_0006_commit=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
ehabi_backport_export_patch=0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch
ehabi_backport_spec_patch_entry=Patch5
platform_frozen_tarball_sha256=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
platform_frozen_tarball_status=SUPERSEDED_BY_R9C
superseded_platform_spec_vcs=platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146
superseded_platform_spec_vcs_status=SUPERSEDED_BY_R9C_TEST_COMMIT_ID_NON_AUTHORITATIVE
platform_vs_r9c_source0_difference=clang/bindings/python/.git_archival.txt only
platform_vs_r9c_source0_difference_cause=git export-subst expansion in local standard export
platform_vs_r9c_source0_build_impact=NO_BUILD_IMPACT
platform_vs_r9c_source0_build_impact_evidence=progress/R9C/build_impact_verdict.tsv
source_export=GBS standard non-native export from upstream/22.1.8 plus six auto-generated downstream patches; --no-patch-export forbidden
provenance_revision_basis=R9-C adjudication 2026-08-05; local standard export object is the primary Source0 anchor
stdlib_abi=libcxxabi
unwinder=libgcc_s
compiler_rt=disabled
llvm_unwinder=disabled
symbol_version_node=LLVM_22
```

## 5. 修改后完整文件

```text
# This file records stable, content-addressed source and release anchors.
# The exact package revision is recorded by the VCS field generated by GBS.
# Downstream patch topology is intentionally not recorded: patch counts,
# filenames, numbering, and commit IDs change under squash, rebase, or
# cherry-pick even when the resulting source content is identical.

provenance_format=2
provenance_scope=stable_source_content_and_release_anchors
package_revision_source=GBS_GENERATED_SPEC_VCS_FIELD
downstream_patch_topology=INTENTIONALLY_NOT_RECORDED
downstream_patch_topology_reason=UNSTABLE_UNDER_SQUASH_REBASE_AND_CHERRY_PICK

component=llvm-project libcxx+libcxxabi
version=22.1.8

source_tarball=llvm-22.1.8.tar.gz
source_tarball_sha256=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
source_tarball_anchor=GBS_STANDARD_EXPORT_FROM_upstream/22.1.8
source_export_mode=GBS_STANDARD_NON_NATIVE
source_export_no_patch_mode=FORBIDDEN
gbs_export_tag=upstream/22.1.8
gbs_export_tag_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1

upstream_release_tag=llvmorg-22.1.8
upstream_release_tag_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
official_release_archive=llvm-project-22.1.8.src.tar.xz
official_release_archive_sha256=922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888
official_release_anchor_scope=CONTENT_COMPARISON_ONLY
source_tarball_vs_official_release_difference=clang/bindings/python/.git_archival.txt_only
source_tarball_vs_official_release_difference_cause=git_export_subst_expansion
source_tarball_vs_official_release_build_impact=NO_BUILD_IMPACT

validated_component_tree_hash_algorithm=git-sha1
validated_libcxx_tree=66966034e27ae7b2856aaee7d0aaac6dd06fbd3b
validated_libcxxabi_tree=8ce21d5fcd3eacb51fb43978ed4c7d05b1fc4ecf

stdlib_abi=libcxxabi
unwinder=libgcc_s
compiler_rt=disabled
llvm_unwinder=disabled
symbol_version_node=LLVM_22
```

修改前 SHA256：
`1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37`。

修改后 SHA256：
`5d4069a1e904f12dfda8c3ed6320dac87b40e30733bc22e4754fa00be428c9dd`。

## 6. 逐字段说明与依据

### 新增的语义字段

| 字段 | 新值/作用 | 依据 |
|---|---|---|
| `provenance_format` | `2` | 表示拓扑无关的新格式 |
| `provenance_scope` | 稳定源码内容与 release 锚点 | 明确范围，不声称记录包 revision |
| `package_revision_source` | `GBS_GENERATED_SPEC_VCS_FIELD` | 实测生成 spec 精确写入新 HEAD |
| `downstream_patch_topology` | `INTENTIONALLY_NOT_RECORDED` | 人工确认的第 3 种方案 |
| `downstream_patch_topology_reason` | squash/rebase/cherry-pick 下不稳定 | 解释为何不可重新加入 PatchN 清单 |

### 重命名/去除内部任务语义

| 旧字段 | 新字段 | 理由 |
|---|---|---|
| `source_tarball_anchor=R9C_GBS_STANDARD_EXPORT` | `GBS_STANDARD_EXPORT_FROM_upstream/22.1.8` | 删除内部任务编号，保留可复现来源 |
| `upstream_tag` | `upstream_release_tag` | 与本地 GBS export tag 明确区分 |
| `upstream_peeled_commit` | 分成两个 tag 的 peeled commit | 明确二者都落到同一上游对象 |
| 带 patch 数量的 `source_export` | `source_export_mode` + `source_export_no_patch_mode` | 保留稳定导出政策，删除不稳定数量 |
| `platform_vs_r9c_*` | `source_tarball_vs_official_release_*` | 保留稳定内容差异、原因和构建影响，删除内部任务路径 |

### 新增的最终组件内容锚点

| 字段 | 值 | 核对 |
|---|---|---|
| `validated_libcxx_tree` | `66966034e27ae7b2856aaee7d0aaac6dd06fbd3b` | `08a64b6e...` 与 `e79f1c3d...` 相同 |
| `validated_libcxxabi_tree` | `8ce21d5fcd3eacb51fb43978ed4c7d05b1fc4ecf` | `08a64b6e...` 与 `e79f1c3d...` 相同 |

这两个是 Git `sha1` repository format 下的子树对象，锚定最终组件内容而
不是提交拓扑；修改 `packaging/SOURCE_PROVENANCE` 不会改变它们。

### 保留并复核的稳定字段

- GBS Source0 SHA256 `1f5fe8da...`：R83、R88 与 R89 三次导出一致。
- `upstream/22.1.8` peeled commit `ca7933e...`：本地 tag 实测。
- 官方 `llvmorg-22.1.8`、peeled commit 与 archive SHA256：本地官方 archive
  实物及 R9C 原始 release 记录复核。
- `stdlib_abi`、unwinder、compiler-rt、LLVM unwinder 与 symbol node：保持
  原值，本次不改构建配置。

## 7. 自洽性与自指检查

- 30 个非注释键，键名唯一。
- 当前提交 SHA `48fc3a29...` 未出现在文件中。
- 没有 `local_*_vcs`、branch、`export_patch_*`、patch count、
  `ehabi_backport_*` 或 `PatchN` 映射字段。
- 原先容易误解的“第六个文件对应 spec Patch5”映射已整体删除，不再存在
  1-based 文件编号与 0-based spec 编号的混用。
- GBS export 后的 `SOURCE_PROVENANCE` 与提交文件逐字节一致。
- 生成 spec 的 `VCS` 精确指向 `48fc3a29...`。
- 实际 export 仍为 Patch0—Patch9；此事实由生成 spec/SRPM表达，不重复写入
  安装文档。
- 除上述稳定 tag peeled commit 与组件 tree 外，没有任何 commit-like
  revision 字段；它们都不是当前 metadata commit，因此不会自指失效。

## 8. 信息粒度变化

完整分析见 [`INFORMATION_RECOVERY.md`](INFORMATION_RECOVERY.md)。摘要如下：

1. 包 revision 不再从该文件取得，改从生成 spec/RPM 的 `VCS` 取得。
2. Patch 数、编号、文件名、SHA256、patch-id 与对应提交不再从该文件取得；
   可从 SRPM 的 spec 与 patch 文件、GBS build log、git 历史恢复。
3. `ehabi_backport` 到 PatchN 的映射不再提供；可从当次 SRPM/spec 得到，但
   这种映射本身不应作为跨构建稳定身份。
4. 旧 platform frozen tarball 与 superseded spec 的历史字段不再装入包；
   项目 git 历史和 R9C 归档仍保存。它们不是当前构建输入。
5. 旧内部证据路径 `progress/R9C/...` 不再装入包；内容差异、原因、影响已
   以拓扑无关字段保留。

只拿一个二进制 RPM、且无法访问对应 SRPM/构建记录/git 时，具体 PatchN
清单和旧平台历史会从安装文档中真正消失。对复现当前 libc++/libc++abi
内容的影响有限，因为 Source0 SHA256、两个最终组件 tree 和生成 spec VCS
仍在；对提交级归因有影响。建议发布系统长期保存 SRPM、生成 spec、patch
文件与构建日志，而不是把一次构建的拓扑复制进稳定安装文件。

## 9. 提交、推送与分支对照

提交：

```text
48fc3a299ba20dd321a7d4a37c4e13ce095176cd
packaging: make libc++ provenance topology-independent
```

提交 diff：`28 insertions, 42 deletions`，仅
`packaging/SOURCE_PROVENANCE`。

普通推送命令：

```bash
git -C tmp/R89/sandbox push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

Gerrit 接受推送，但给出 subject 超过 50 字符、若干 message 行超过 72
字符的非阻断 warning。推送已成功；由于任务严禁 force，没有在推送后
改写提交历史来处理纯格式 warning。

推送后：

| 分支 | 推送前 | 推送后 | 结果 |
|---|---|---|---|
| `libcxx-ehabi-backport` | `08a64b6e...` | `48fc3a29...` | 预期快进 |
| `llvm_only_clang` | `f8277158...` | `f8277158...` | 未变 |
| `mlgo` | `08ffd8cd...` | `08ffd8cd...` | 未变 |
| `mlgo_aot` | `88ff57ca...` | `88ff57ca...` | 未变 |
| `tizen_base` | `8dfebafe...` | `8dfebafe...` | 未变 |
| `tizen_base_next` | `fd7a4929...` | `fd7a4929...` | 未变 |

namespace 分支数前后相同，没有新建分支。

## 10. 对既有验证结论的影响

**不影响此前运行时与代码验证结论。** 新提交只改安装到文档目录的
packaging metadata：

- `libcxx` tree 仍为 `66966034...`；
- `libcxxabi` tree 仍为 `8ce21d5f...`；
- GBS export 的 Source0 SHA256 未变；
- sandbox 的 10 个源码 patch 未变；
- spec、构建选项、头文件和库源码均未变。

因此以 `08a64b6e...` 为对象完成的两架构全量测试、代码评审和场景验证仍
适用于新 HEAD 的运行时内容。R89 没有重新编译或运行测试，新的平台构建
结果记 `NOT_OBSERVED`；R89 仅实测了 GBS export 和元数据落包输入。

## 11. 总部后续合入注意事项

1. 在 `e79f1c3d...` 之后携带本提交的单文件 metadata 变化；可作为独立
   packaging-only 提交，也可 squash/cherry-pick，文件内容都不会因拓扑
   改变而失实。
2. 不要把 sandbox 的 10-patch 或 tizen_base 的 7-patch 清单重新写回文件。
3. 合入后执行一次 GBS export，确认生成 spec 的 `VCS` 指向总部最终提交，
   且安装的 `SOURCE_PROVENANCE` 与仓内文件一致。
4. 必须重建 `libcxx-runtimes`，因为该文件属于 `libc++-devel` 包；只更新
   源仓不会修正已经生成的 RPM。
5. 若总部同时改动 `libcxx` 或 `libcxxabi` 内容，必须重新计算并更新对应
   subtree ID；若只改变提交组织，不需更新。

## 12. 自行判断、异常与尚存疑问

自行判断：

1. 使用 `libcxx`/`libcxxabi` subtree object，而不是完整仓 tree。完整仓
   tree 会因本文件自身变化而变，组件 subtree 则准确锚定本包关心的最终
   运行库源码且不自指。
2. 保留稳定的 non-native export / 禁止 no-patch-export 政策，以及 Source0
   与官方 release 的唯一内容差异；删除其中的内部任务编号和内部路径。
3. Gerrit push 后的 message 行宽 warning 不影响提交内容或接收结果；任务
   又禁止 force，因此没有改写已推送提交。

异常：

- 第一次稳定锚点命令末尾因本地不存在官方 tag 名返回 128；通过本地
  `upstream/22.1.8`、官方 archive 实物和 R9C 官方 tag 原始证据补查完成。
- 第一次 pre-push `ls-remote` 无输出挂起；在确认未执行 push 后按精确
  process group 终止，显式记录 `NOT_OBSERVED`，再以连接/总时限重试一次并
  通过。清理命令第一次因 awk quoting 自身退出，未执行 kill；修正版完成。
- 第一次 commit message 中换行成为字面 `\n`；在推送前仅 amend message，
  文件内容未变。

尚存疑问：无阻断项。GNU Build ID、Release 与 changelog 按任务书明确不在
R89 处理。

## 13. 原始证据索引

- 资源门禁：`raw/001_*`
- 推送前 ref 与修改前全文：`raw/002_*`
- 稳定锚点：`raw/003_*`—`raw/005_*`
- 工作树与 hook：`raw/006_*`
- 修改前后全文、diff、单文件范围：`raw/007_*`
- 自洽性与实物 hash：`raw/008_*`
- commit/amend：`raw/009_*`、`raw/010_*`
- GBS export：`raw/011_*`—`raw/013_*`
- pre-push 网络异常与门禁：`raw/014_*`—`raw/021_*`
- push：`raw/022_*`
- post-push 分支对照：`raw/023_*`
- 交付目录初始化：`raw/024_*`
- 最终文件身份与完整全文：`raw/025_*`
- 旧字段消费者检索及低负载等待记录：`raw/026_*`—`raw/029_*`
- 交付材料归档：`raw/030_*`
- 最终文件、报告嵌入内容与提交对象逐字节审计：`raw/031_*`
