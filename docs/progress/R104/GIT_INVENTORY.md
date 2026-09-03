# R104 第一阶段：11 个仓库 Git 信息

记录时间：2026-09-03。所有仓库位于 `codes/R104/<包名>`，均以账号
`lhmax2025` 从 Gerrit 的 SSH 端口 29418 拉取。拉取采用 `--depth 3
--single-branch --no-tags`；因此这里完整覆盖任务要求的当前 HEAD 与最近
3 条提交，但不是完整历史克隆。

## 汇总

| 包 | 完整仓库 URL | 当前分支 | HEAD（完整 SHA） | 工作树 |
| --- | --- | --- | --- | --- |
| `abseil-cpp` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/abseil-cpp` | `tizen_base` | `9c39c516dca54c786ddc5da45a5a15acf41ef75e` | 干净 |
| `bcc-tools` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/bcc` | `tizen_base` | `dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2` | 干净 |
| `boost` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/boost` | `tizen_base` | `69203710b8582c489f420ef53a769cc6077c250b` | 干净 |
| `bpftrace` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/bpftrace` | `tizen_base` | `30e51cd665360f85b665308dc13ba27df0c5f739` | 干净 |
| `icu` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/icu` | `tizen_base` | `e4a4d7411b0f288581cf020a176b5dddbb504ae7` | 干净 |
| `jsoncpp` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/jsoncpp` | `tizen_base` | `96bf0c36eed62a6d7bfc9bdb30f0e41d877aa6f0` | 干净 |
| `libsigc++` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/libsigc++` | `tizen_base` | `5f177f7c98ea29833989b5d0d6a79fece9701545` | 干净 |
| `llvm` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm` | `tizen_base` | `2d23367d74afbf2bb1e9e4013fce072b3a154109` | 干净 |
| `pcre` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/pcre` | `tizen_base` | `feba6a19627f5d6e74d551ef2fc8b33af0547780` | 干净 |
| `taglib` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/taglib` | `tizen_base` | `4e7de646d897ed5a92111799058b21705a3bd34c` | 干净 |
| `tensorflow2` | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/tensorflow2` | `accepted/tizen_base_toolchain` | `1cdba73549f741720f11dd53da1bb516b03763a2` | 干净 |

## 最近 3 条提交

### abseil-cpp

1. `9c39c516dca54c786ddc5da45a5a15acf41ef75e Remove compile filter`
2. `35e92e9ad44dc68ee292b2dd78d4789b7d714f9b Bump to 20250814.1`
3. `f7f0f39d688cc19f8dd14b033baf7c2b34f26d11 Imported Upstream version 20250814.1`

### bcc-tools

1. `dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2 packaging: disable bcc-tools-examples by default`
2. `97128ee3dbccd2851cfa94dc723edb2abcbefc1d cmake: fix LLVM 22 linkage dependencies`
3. `0a54eeacc246085ff828d732de623a52e1f1d23c Fix build with LLVM-22`

### boost

1. `69203710b8582c489f420ef53a769cc6077c250b build(boost): refine --as-needed linker flag placement and apply JAM rules`
2. `f7e388c3596c92d54ac2df54b4c4b611a3fbda0d build(boost): apply -Wl,--as-needed linker option`
3. `58b70f335aec5641540dbe04dfd56fad5af43658 Convert shell scripts from bash to dash compatibility`

### bpftrace

1. `30e51cd665360f85b665308dc13ba27df0c5f739 Add support for LLVM 22 (#5064)`
2. `b7e534a79890770bffb65c6184a7f0b9a2459b69 update for upstream LLVM argument removal to createSourceManager (#4710)`
3. `d63c8f0dae915021e9497981f257a89244582314 Convert shell scripts from bash to dash compatibility`

### icu

1. `e4a4d7411b0f288581cf020a176b5dddbb504ae7 Bump to 78.1`
2. `ba8b26fb2add907e855e23364619b83d1f8cb214 Imported Upstream version 78.1`
3. `5aac6e37ae317ac833899557d8cc4e60dbd6acd7 Imported Upstream version 76.1`

### jsoncpp

1. `96bf0c36eed62a6d7bfc9bdb30f0e41d877aa6f0 Convert shell scripts from bash to dash compatibility`
2. `21c9dcd2bc8f51f8bfe5173b49780dc707eef9f9 Bump to 1.9.6`
3. `130608da46e9272c7baab477520751fcd46365ad Imported Upstream version 1.9.6`

### libsigc++

1. `5f177f7c98ea29833989b5d0d6a79fece9701545 Bump to 2.12.1`
2. `65ecff9803dbbc90c5422550201798868421e99d Imported Upstream version 2.12.1`
3. `945cbc1f0b3892086bda0a5e7cb9c14e7c7a133c Imported Upstream version 2.10.7`

### llvm

1. `2d23367d74afbf2bb1e9e4013fce072b3a154109 packaging: select Clang explicitly for libc++ runtimes`
2. `6771dbc48b7e3db2ec53919a8eebd3514374137b libcxx: backport EH ABI forced-unwind handling`
3. `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c libcxxabi: declare __gnu_unwind_frame in cxa_personality (#189787)`

### pcre

1. `feba6a19627f5d6e74d551ef2fc8b33af0547780 Remove doc package`
2. `004bfd528225e1965f8831cb76a3eeb6f21327b0 Change BSD-2.0 to BSD-3-Clause`
3. `90330eae99984ae66da9e51c09131eeb849aaa84 Bump to pcre 8.45`

### taglib

1. `4e7de646d897ed5a92111799058b21705a3bd34c Fix m4a header checker`
2. `6383caaf260629b0ffa1673994263019923a3a09 Fix version`
3. `40efb8a696617363cdcd7d8e83c3237a7a58df1b Merge branch 'upstream' into tizen_base`

### tensorflow2

1. `1cdba73549f741720f11dd53da1bb516b03763a2 fix: Add include directories and EIGEN_NEON_GEBP_NR=4 for XLA AOT runtime`
2. `a5e093af9470d15b3673dc7650cb09ca636a7f35 fix: install AOT include/ to %{_datadir} for LLVM TENSORFLOW_AOT_PATH`
3. `36fb24a9d288f515c53eacb10f2e23f8d3489d78 Add XLA AOT compilation support for LLVM MLGO`

## revision 对照与 bcc-tools 说明

本次 11 个 HEAD 均与 R101/R103 的**最新记录**一致；这里不是说它们都与
R100 的旧快照一致。

`bcc-tools` 的时间线是：

1. R100 使用的快照为 `97128ee3dbccd2851cfa94dc723edb2abcbefc1d`；
2. R101 拉取时发现远端已前移至
   `dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2`，并把这个新 HEAD 记录为
   当前 revision；
3. R104 当前拉取到的仍是 `dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2`。

因此，R104 相对 R101 最新记录没有再次漂移；相对 R100 快照则前移 1 个
提交，即 `packaging: disable bcc-tools-examples by default`。

R101 已逐字比较 `97128ee3…` 与 `dd26ba03…` 的 spec；R104 又将当前 spec
与 R101 归档的当前 spec 做 SHA256 比较，结果一致。因此 R101 基于
`dd26ba03…` 修订后的建议改法仍适用于当前 spec。

## spec SHA256

| 包 | spec 路径 | 当前 SHA256 | 与 R101/R103 当前归档一致 |
| --- | --- | --- | --- |
| `abseil-cpp` | `packaging/abseil-cpp.spec` | `cf32dcff8fcdea079748c4c90b4c3853cd1c6009911cf439f77c7f59d8416916` | 是 |
| `bcc-tools` | `packaging/bcc-tools.spec` | `d4a572335a4d5ea8363c2a84099c17eae7b9718c28e06d55e1e0cf4eed458fc8` | 是 |
| `boost` | `packaging/boost.spec` | `e60a71ebfc711b22943b79666d1da33ef42276d2eca6ab221e0a5db6b01b6819` | 是 |
| `bpftrace` | `packaging/bpftrace.spec` | `d0a90d3d5fd4a7d6b0a95a4ddbd44568071ac18586a0bbbd1547299b3005aa2d` | 是 |
| `icu` | `packaging/icu.spec` | `c7a4f7a9ffe7464dc75faefaf66552dc498b074d384c211d2debf83dbe824b85` | 是 |
| `jsoncpp` | `packaging/jsoncpp.spec` | `353b19dae0ae4de1149c5a8a8bc5441268e58339c8a434d667da09950f868892` | 是 |
| `libsigc++` | `packaging/libsigc++.spec` | `709e8748b1df2b64e4efd0f1dc4196e51ae726a13de360aec30db76907f00e39` | 是 |
| `llvm` | `packaging/llvm.spec` | `7962d176484920de6138d70c7e8a8e1764dfada6655abf0332e9748167e889fd` | 是 |
| `pcre` | `packaging/pcre.spec` | `2f71ad87bb5ba2d167c7aa781de19d618827828a18e94dbeded29d0ca5e766ad` | 是 |
| `taglib` | `packaging/taglib.spec` | `c9e81f92f7af15ab8c854c09bc0ee52312ec2a6f2f5082482f7ffbaddca06aea` | 是 |
| `tensorflow2` | `packaging/tensorflow2.spec` | `b33f4c04d4b4e90e79821705a850c2202163536aa41510fb94c6c6fa80cd1a46` | 是 |

机器可读原表为 `tables/git_state_before_branch.tsv`。它同时保存完整 URL、
分支、HEAD、新旧 revision、spec SHA256、状态和最近 3 条提交。

## 拉取异常

前 10 个仓库一次成功。`tensorflow2` 首次拉取在 SSH 握手/传输阶段超时，
批处理命令因此非零退出；失败目录未残留。之后按相同 URL、分支和低资源
参数单独重试一次成功。原始材料分别见：

- `raw/007_clone_target_sources.*`：包含首次批处理及超时；
- `raw/010_inspect_failed_tensorflow_clone.*`：失败后目录核对；
- `raw/011_retry_tensorflow_clone.*`：重试命令、输出和退出码。

这属于命令/连接层的技术性错误；最终仓库身份、HEAD、spec 校验和与工作树
清洁度均已独立核对通过。

## 当前操作边界

已获准采用 `sandbox/lhmax2025/libcxx-migration`，但按人工新增要求，须先让
人工核对本材料。因此截至本记录提交时，11 个包仓库均未创建或推送该分支，
也未修改任何 spec。
