# R88：总部 squash 提交核对报告

核对日期：2026-09-01（Asia/Shanghai）

## 1. 结论先行

### 1.1 内容身份

提交对象 `e79f1c3dde12c356142b77231e304c0da67adaf1` 与验证基准
`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9` 的 **Git tree ID 完全相同**：

```text
e35b4a413b8437d05a89047e09d6bd5ff524237d
```

`git diff --quiet 08a64b6e... e79f1c3d...` 返回 0。squash 涉及的 13 个
文件集合一致，每个最终文件的 SHA256、字节数和内容均一致。因此，**就
提交对象内容而言，`e79f1c3d...` 是此前验证过的同一棵树；若该对象原样
进入目标分支，既有两架构全量测试和代码评审结论可直接沿用。**

### 1.2 “已经合入”这一前提与远端现状不符

2026-09-01 本次只读观测显示：

- `refs/heads/tizen_base` 仍为
  `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`；
- Gerrit change 352523 的 current patch set 1 是 `e79f1c3d...`，但查询结果为
  `status=NEW`、`open=true`；
- `e79f1c3d...` 的父提交正是 `8dfebafe...`。

所以，严格答案是：**当前能够确认 proposed patch set 与验证树相同，但
尚不能确认“合入后的 `tizen_base`”相同，因为远端分支尚未包含它。** 在
实际提交后必须再核对一次远端 ref；当前不得以“已合入”为发布前提。

### 1.3 打包结论

使用本机 `gbs 2.0.8` 对 `e79f1c3d...` 执行 standard non-native export 的
实测结果与任务书中的预期不同：**squash 仍被自动导出为一个补丁**，不是
直接进入 Source0 tarball：

- `Patch0`—`Patch5`：既有 6 个下游源码补丁；
- `Patch6`：`0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch`，
  对应整个 `e79f1c3d...` squash；
- `%prep` 中存在并按序执行 `%patch0`—`%patch6`；
- 7 个声明均有实际文件，不存在悬空或错位；
- `Patch6` 在 `8dfebafe...` 内容上对 9 个源码文件执行
  `patch --dry-run --fuzz=0 -p1`，全部通过。

因此没有由 squash 造成的 spec 构建阻断。R88 没有实际编译包，实际构建
结果记 `NOT_OBSERVED`；但导出结构、前像和补丁试应用均已核实。

### 1.4 必须处理的元数据问题

`SOURCE_PROVENANCE` 仍是旧版本，而且会安装到
`/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE`（属于
`libc++-devel`）：旧 HEAD、补丁数、Patch6 身份及总体描述均与本次导出
不符。它不参与编译和运行，不会导致 `%prep` 失败，但会把错误的来源、
补丁清单和复现依据公开装入包中，因此发布前必须修正并重建相应源包。

完整可用的建议文件见
[`SOURCE_PROVENANCE.suggested`](SOURCE_PROVENANCE.suggested)。它按
`e79f1c3d...` 的实际 GBS export 生成，并对 7 个 patch 的文件名、SHA256、
stable patch-id 及 Source0 SHA256 做了机械复核。

## 2. 取得方式与纪律

1. 资源门禁：`tools/resource_gate.sh --level light`，退出码 0。
2. `git ls-remote` 只读取得两个分支 ref。
3. `git fetch --no-tags origin e79f1c3d...` 仅把对象写入 `FETCH_HEAD`；没有
   更新、切换或推送任何分支。
4. `ssh ... gerrit query --format=JSON --current-patch-set change:352523`
   只读取得 change 状态；没有留言、投票或改状态。
5. 全部比较使用 Git 对象接口；没有 checkout、没有修改 `codes/llvm`。
6. GBS export 输出到 `tmp/R88/export`，使用 `nice -n 15 ionice -c 3`；没有
   编译。补丁 dry-run 使用 `tmp/R88/patchcheck`。

网络与命令原文、stdout、stderr、退出码见 `raw/`。两次包装器参数误用和
导出等待观察命令如实补录在 `raw/UNLOGGED_RECOVERY_COMMANDS.md`。

## 3. 逐文件比对

以下 13 个文件既是 squash 的完整文件集，也是 sandbox 五提交（打包提交
加四层改动）的完整文件集。两提交最终内容逐字节相同。

| 文件 | SHA256（两侧相同） | 字节数 | 结果 |
|---|---|---:|---|
| `libcxx/include/__ostream/basic_ostream.h` | `e9dbe41f142f1bbf2925546cd2d7dbe73c1b1863ae41f7f94387d51c7a18ded6` | 23081 | PASS |
| `libcxx/include/future` | `a52817e321b7659a3e0f174a66c8d09523945c071ceaff7e4d94935f938fade5` | 71598 | PASS |
| `libcxx/include/istream` | `0a2db1722f84b9ce453152131dcfc7db608d7d64b22dea84a64da19fb8caa9a1` | 48850 | PASS |
| `libcxx/include/string` | `a9d0adfef9ba6f86cb168b64b1500ad8fc4a6b97e4c54aef5e51175733bb6935` | 187843 | PASS |
| `libcxx/src/future.cpp` | `004ecd2f870bea2d54441ae94443a774f77467676909345473dbc40aa455c7f3` | 5988 | PASS |
| `libcxx/src/ios.cpp` | `ff2addf1a743c273fbf034192721126f440f611a0421a748c44eaf5339ba74fb` | 12411 | PASS |
| `libcxxabi/include/cxxabi.h` | `45561ece403db01e742e3d8300b6527ae0e029458a96eab618e9399ac6b1f04c` | 8571 | PASS |
| `libcxxabi/src/cxa_exception.cpp` | `8212a5b7a077bf7a2002ef0668deb2d3890c59158d35910d47526abad224bd03` | 33835 | PASS |
| `libcxxabi/src/cxa_personality.cpp` | `08fb954c0f681973a92b873a8dbc43611daf6c295fb2fc64fcdf128b02257b86` | 63198 | PASS |
| `packaging/SOURCE_PROVENANCE` | `1cd5a45ea027d8f22b8dc5812eebbcf56f6c52e42446ad531729ce80289e7a37` | 3662 | PASS（内容相同但共同过期） |
| `packaging/libcxx-llvm22.map` | `e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d` | 30 | PASS |
| `packaging/libcxx-runtimes.manifest` | `db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3` | 71 | PASS |
| `packaging/libcxx-runtimes.spec` | `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74` | 4933 | PASS |

注意：“完全一致”指两个提交的完整 Git tree，不只是上述文件。没有发现
任何额外基础树差异；两个不同父提交 `3d3e3da4...` 与 `8dfebafe...` 的 tree
本身也相同，只是提交元数据不同。

## 4. 11 项关键改动

逐项结果均为 PASS：

| 项 | 结果 | 核对事实 |
|---|---|---|
| A-1 | PASS | 外来异常调用 `_Unwind_Resume_or_Rethrow` |
| A-2 | PASS | ARM EHABI forced foreign 路径以 `results.ttypeIndex != 0` 识别 catching handler |
| B-1 | PASS | `cxxabi.h` 定义可见的抽象类 `__forced_unwind`，并有 out-of-line 析构 |
| B-2 | PASS | forced foreign typed catch 用 `typeid(__cxxabiv1::__forced_unwind)` 匹配 |
| F1 | PASS | badbit/failbit mask 判断在内层 `catch (...)`；forced catch 立即重抛；函数尾无裸 `throw;` |
| F2 | PASS | `get(basic_streambuf&, delim)` 的 forced typed 分支调用 `__setstate_nothrow` 后重抛 |
| N1 | PASS | 算术输出只保留普通 `catch (...)` 调 helper，未增加 forced 裸重抛 handler |
| N2 | PASS | `basic_ostream.h`、`future`、`istream`、`string` 四处均含 `<cxxabi.h>` |
| M1 | PASS | deferred forced 分支调用 `__restore_deferred`；`__sub_wait` 为 `while (!__is_ready())` |
| M2 | PASS | 用户 `_Rp` 析构位于第一段锁作用域结束后，随后重新加锁提交 ready |
| 引用特化 | PASS | `__assoc_state<_Rp&>` 同时具备 `__construct_value_unready` 和 `__complete_forced_unwind` |

实际代码片段集中见 [`KEY_CHANGE_EVIDENCE.md`](KEY_CHANGE_EVIDENCE.md)，原始
带行号输出见 `raw/010_*`、`raw/011_*`、`raw/012_*`。

## 5. `SOURCE_PROVENANCE` 逐项核对

### 5.1 当前文件与实际导出的差异

| 字段/范围 | 当前文件 | `e79f1c3d...` 实际 export |
|---|---|---|
| revision | `3d3e3da4...` | 源码内容锚点 `e79f1c3d...`；生成 spec 的 `VCS` 也为 `e79f1c3d...` |
| patch 数 | 6 | 7 |
| 第 6 个 patch commit | `3d3e3da4...` | `8dfebafe...` |
| 第 6 个 patch SHA256 | `b9fdf664...` | `f52129a2...` |
| forced-unwind squash | 未记录 | 第 7 个 patch，commit `e79f1c3d...` |
| export 描述 | “plus six” | “plus seven” |

第 6 个 patch 的 stable patch-id 仍为 `ab69099b...`，说明源码差异相同；
SHA256 改变来自总部提交的邮件头/提交元数据变化，provenance 仍应记录实际
导出文件的 SHA256。

### 5.2 所谓“内部编号矛盾”

`export_patch_0006_file=0006-...` 对应 `Patch5` **在 GBS 数字规则上并不
矛盾**：文件名按 1 起始，spec 的 `PatchN` 按 0 起始。本次实测生成 spec
也正是该映射。

但存在实质性的**语义含混和过期**：键名 `ehabi_backport_export_patch`
指向的是既有 `__gnu_unwind_frame` 声明补丁，而当前要追踪的 forced-unwind
squash 实际是新 Patch6/文件 0007，且完全没有记录。建议文件将二者明确
拆成 `gnu_unwind_frame_*` 和 `forced_unwind_*`，消除误读。

### 5.3 实际后果

- 对编译/运行：无直接影响；该文件只在 `%prep` 被复制并安装为文档。
- 对发布/审计：有直接影响；包会公开声称错误 revision 与补丁集合，无法
  用它重建、核验或回答供应链来源问题。
- 对已产 RPM：仅改仓库文件不够，需重建 `libcxx-runtimes`，因为错误文件
  已装入各架构 `libc++-devel`。

### 5.4 为什么建议使用后续 packaging-only 提交

若直接 amend `e79f1c3d...` 并在文件里硬编码 `e79f1c3d...` 及其 Patch7
SHA，amend 会改变提交和导出补丁本身，形成自引用失效。建议先让代码对象
`e79f1c3d...` 成为稳定内容锚点，再用一个只改 packaging 元数据的后续提交
加入建议文件。GBS 不把 packaging-only 提交导出成 Source0 补丁，因此 7
个源码 patch 身份保持稳定；最终包 revision 则由生成 spec 的 `VCS` 字段
记录。建议文件因此使用：

```text
validated_source_tree_vcs=platform/upstream/llvm#e79f1c3dde12c356142b77231e304c0da67adaf1
package_revision_identity=GBS_GENERATED_SPEC_VCS_FIELD
```

避免声称文件能够包含自身提交 SHA。

### 5.5 建议的完整文件内容

以下内容适用于以 `e79f1c3d...` 为稳定源码内容锚点的后续
packaging-only 提交；与单独交付的 `SOURCE_PROVENANCE.suggested` 完全
相同：

```text
component=llvm-project libcxx+libcxxabi
version=22.1.8
source_tarball=llvm-22.1.8.tar.gz
source_tarball_sha256=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
source_tarball_anchor=R9C_GBS_STANDARD_EXPORT
validated_source_tree_vcs=platform/upstream/llvm#e79f1c3dde12c356142b77231e304c0da67adaf1
package_revision_identity=GBS_GENERATED_SPEC_VCS_FIELD
local_tizen_base_branch=tizen_base
upstream_tag=llvmorg-22.1.8
gbs_export_tag=upstream/22.1.8
upstream_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
official_release_archive=llvm-project-22.1.8.src.tar.xz
official_release_archive_sha256=922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888
official_release_anchor_scope=CONTENT_COMPARISON_ONLY
export_patch_count=7
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
export_patch_0006_sha256=f52129a2f008603a3cf74fc1bf448d984b0f1827a48a9597f346223d87b98062
export_patch_0006_patch_id=ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479
export_patch_0006_commit=8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c
export_patch_0007_file=0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch
export_patch_0007_sha256=b2d657339c18c5c1416bc9b3890afd39394b9ce46af061c1db809315ad4746d3
export_patch_0007_patch_id=b00e455e21b89a23a7b13ce9b68103fbc9b28fb4
export_patch_0007_commit=e79f1c3dde12c356142b77231e304c0da67adaf1
gnu_unwind_frame_export_patch=0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch
gnu_unwind_frame_spec_patch_entry=Patch5
forced_unwind_export_patch=0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch
forced_unwind_spec_patch_entry=Patch6
platform_frozen_tarball_sha256=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e
platform_frozen_tarball_status=SUPERSEDED_BY_R9C
superseded_platform_spec_vcs=platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146
superseded_platform_spec_vcs_status=SUPERSEDED_BY_R9C_TEST_COMMIT_ID_NON_AUTHORITATIVE
platform_vs_r9c_source0_difference=clang/bindings/python/.git_archival.txt only
platform_vs_r9c_source0_difference_cause=git export-subst expansion in local standard export
platform_vs_r9c_source0_build_impact=NO_BUILD_IMPACT
platform_vs_r9c_source0_build_impact_evidence=progress/R9C/build_impact_verdict.tsv
source_export=GBS standard non-native export from upstream/22.1.8 plus seven auto-generated downstream patches; --no-patch-export forbidden
provenance_revision_basis=GBS standard export at platform/upstream/llvm#e79f1c3dde12c356142b77231e304c0da67adaf1 verified 2026-09-01
stdlib_abi=libcxxabi
unwinder=libgcc_s
compiler_rt=disabled
llvm_unwinder=disabled
symbol_version_node=LLVM_22
```

## 6. squash 后的补丁导出与构建影响

源仓 `libcxx-runtimes.spec` 本身没有手写 `PatchN` 或 `%patchN`；GBS export
依据 `upstream/22.1.8..HEAD` 自动注入。`e79f1c3d...` 同时改 packaging 和 9
个源码文件，因而其源码部分被导出成一个新的下游 patch。实际生成结果：

```text
Patch5: 0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch
Patch6: 0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch
...
%patch5 -p1
%patch6 -p1
```

这和 sandbox export 的差别只是四个独立 patch（旧 Patch6—Patch9）合并为
一个 squash patch（新 Patch6）。最终源码 tree 相同，补丁 dry-run 通过。
故：

- 没有悬空 `PatchN`；
- 没有 `%prep` 漏应用；
- 不会因编号收缩导致构建失败；
- stale provenance 不会阻止构建；
- R88 未实际跑构建，平台构建结果为 `NOT_OBSERVED`。

## 7. spec、版本与 commit message

- `libcxx-runtimes.spec`、`libcxx-llvm22.map`、manifest 与 sandbox 验证树
  均逐字节一致。
- spec 为 `Version: 22.1.8`、`Release: 1`。若 `22.1.8-1` 从未进入正式仓，
  作为新包的首次正式发布不必仅因这次 squash 强制 bump；若同 NEVRA 已在
  任何目标仓发布，则必须提高 Release，避免同 NEVRA 不同内容。正式仓
  历史在 R88 未查，记 `NOT_OBSERVED`。
- `%changelog` 最后一项为 2026-08-04，只描述打包迁移，没有描述 2026-09-01
  forced-unwind 改动。它不影响构建，但建议发布前补一项准确说明；Release
  是否随之 bump 取决于上一条的正式发布事实。
- squash commit message 列出了五个被合并提交（打包 + 四层），与实际 13
  文件及变更内容一致；描述准确但简略，没有发现虚假陈述。

## 8. 此前发布前事项及其他发现

1. `SOURCE_PROVENANCE`：仍未处理，且本次从 sandbox 的“实际 10 patch”
   变为 squash export 的“实际 7 patch”；必须按当前形态重新修正。
2. GNU Build ID：spec 与验证树完全相同，仍没有针对先前产物缺 Build ID 的
   修正。R85 中三架构 `libc++.so.1.0`/`libc++abi.so.1.0` 均缺 Build ID，
   QuickBuild 日志有 `Missing build-id`。`e79...` 自身的新构建产物未在 R88
   生成，故现象对新产物记 `NOT_OBSERVED`；发布负责人仍须确认政策，若
   门禁要求 Build ID，应补链接/打包配置后重建。
3. 新发现：change 352523 尚为 `NEW/open`，远端 `tizen_base` 未移动。这是
   当前最先要解决的状态问题。

## 9. 可直接交总部的意见

完整可复制措辞见 [`GERRIT_COMMENT_DRAFTS.md`](GERRIT_COMMENT_DRAFTS.md)。
本任务没有在 Gerrit 留言。

### 必须改/必须确认

1. **先确认并完成实际 submit。** 当前 change 352523 仍为 `NEW/open`，
   `tizen_base` 仍指向父提交；发布前应确认 ref 已快进到预期提交。
2. **修正随包安装的 `SOURCE_PROVENANCE` 并重建。** 现文件记录 6 个旧
   patch，实测当前 export 为 7 个；建议用 packaging-only 后续提交采用
   本报告完整文件，避免自引用 SHA 问题。
3. **发布前给出 Build ID 政策结论。** 若平台要求 GNU Build ID，当前打包
   配置/既有产物不满足，须修正并重建；若允许，需明确记录豁免依据。

### 建议改

1. `%changelog` 增加 forced-unwind squash 的实际日期与说明。
2. 若 `22.1.8-1` 已进入任何正式仓，提高 `Release`；若从未发布则可保持。
3. 元数据修正后重新做一次 GBS export 身份检查，并至少核对生成 spec 的
   VCS、7 个 patch、安装后的 provenance 与 GNU Build ID。

## 10. 自行判断、疑问与未覆盖

自行判断：

1. 把 Git tree ID 相同作为“同一验证对象”的最强内容判据；提交作者、父
   SHA 与 message 不同不影响该结论。
2. 没有接受“合入后不会导出 patch”的预设，而以 `gbs 2.0.8` 实测为准。
3. 将错误的公开安装 provenance 归为发布阻断，而非运行时/构建阻断。
4. 为避免提交自引用，建议用稳定代码提交 + packaging-only 元数据提交，
   并让生成 spec 的 VCS 表达最终包 revision。

尚存疑问/未覆盖：

1. 为什么任务背景称已合入，而 Gerrit/远端 ref 仍显示未合入；R88 只能
   报告观测事实，未推断流程原因。
2. `22.1.8-1` 是否曾进入正式仓：`NOT_OBSERVED`。
3. Tizen 发布门禁是否强制 GNU Build ID：`NOT_AVAILABLE`，此前已查不到
   权威策略材料。
4. R88 未实际编译，也未验证 `e79...` 新产物；只验证导出和补丁可应用。

本任务没有修改平台源码、补丁、project_config 或 Gerrit 状态；没有推送
Gerrit/LLVM 分支，也没有在 Gerrit 留言。

## 11. 原始证据索引

- 远端 ref：`raw/003_remote_refs.*`
- commit、父提交、文件集合：`raw/005_commit_identity.*`
- 13 文件 SHA256 与全树比较：`raw/006_tree_comparison.*`、
  `raw/007_tree_and_base_identity.*`
- 完整 squash diff：`raw/008_squash_full_diff.*`
- 11 项代码：`raw/009_*`—`raw/012_*`
- 原 spec/provenance 全文：`raw/013_packaging_files.*`
- GBS export：`raw/017_squash_gbs_export.*`、`raw/018_*`—`raw/020_*`
- Gerrit 状态：`raw/021_gerrit_change_status.*`
- 建议 provenance 机械复核：`raw/023_*`、`raw/026_*`
- Patch6 dry-run：`raw/025_exported_squash_patch_dry_run.*`
