# armv7l EHABI 上游回移与验证报告（R6）

## 一、结论

状态：`PASS`。

- LLVM 22.1.8 的平台源码与 `codes/llvm` 回移前源码在 `libcxxabi/`、`libunwind/` 全树逐目录比对中无差异，结论为 `SOURCE_IDENTICAL`。
- 上游修复提交 `8119f1854948b50358bbfaea08f207f51970f06c` 已以 `git am` 原样回移到 `tizen_base`，本地提交为 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`。
- armv7l 在全新单用 buildroot 中构建成功，`cxa_personality.cpp.o` 编译成功，写出 7 个二进制 RPM 和 1 个 SRPM，并通过 RPM `check-files`。
- 补丁后未发现新的编译 warning、compiler error 或 fatal build error；三架构的非架构相关 RPM 元数据差异为 `NONE`。
- armv7l 的 `libc++.so.1`、`libc++abi.so.1` 执行 `ldd -r` 后强未解析符号均为 0；`__gnu_unwind_frame` 由目标根内 `libgcc_s.so.1` 提供；两库均存在 `.ARM.exidx`、`.ARM.extab`；导出版本节点为 `LLVM_22`。
- 未向 Gerrit 或任何 LLVM 外部源码仓执行 push。本项目证据仓的提交与 GitHub push 单独记录。

本任务在此停止，不进入 T4。

## 二、源码树身份与同源核对

| 项目 | 实测结果 |
|---|---|
| 源码树 | `/home/toolchain/development/libc++_replacement/codes/llvm` |
| 分支 | `tizen_base` |
| 回移前 HEAD | `fd7a4929c0e424d564cdd1f0c55af9153dcea321` |
| remote | `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm` |
| 回移前工作树 | `git status --porcelain=v1` 零输出 |
| 数值版本 | `LLVM_VERSION_MAJOR=22`、`MINOR=1`、`PATCH=8` |
| 平台 tarball | `tmp/corpus/extracted/llvm-22.1.8-18.1.src.rpm/payload/llvm-22.1.8.tar.gz` |
| tarball SHA256 | `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e`，与裁决值一致 |
| `libcxxabi/` 全树 diff | 退出码 0，无差异 |
| `libunwind/` 全树 diff | 退出码 0，无差异 |
| 同源结论 | `SOURCE_IDENTICAL` |
| 回移前声明复现 | `cxa_personality.cpp` 中声明 0 处、调用 1 处 |

比对在回移前完成；平台 tarball 解包到 `tmp/R6/source_compare/llvm-22.1.8` 后只读比较。原始身份、版本、tarball 哈希及目录 diff 见 `raw/source_git_identity.raw.txt`、`raw/source_version.raw.txt`、`raw/platform_tarball_sha256.raw.txt`、`source_compare_exit.tsv`。

## 三、上游 patch 与合入提交

| 项目 | 结果 |
|---|---|
| 上游提交 | `8119f1854948b50358bbfaea08f207f51970f06c` |
| 上游 patch | `upstream/8119f1854948b50358bbfaea08f207f51970f06c.github.patch` |
| 上游 patch SHA256 | `d4a8fdcec396dbd7a2d8c70904be674b8bf03e5b3177d0116619435e7599b3d0` |
| patch 范围 | 仅 `libcxxabi/src/cxa_personality.cpp`，`2 insertions(+), 0 deletions(-)` |
| 两个新增行的实测含义 | 1 行 `extern "C"` 声明加 1 个空分隔行；这是上游 commit 的逐字内容，无自行改写 |
| 合入方式 | `git am`，随后仅补入裁决要求的追踪字段并 amend |
| 本地提交 | `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` |
| patch-id | 上游与本地均为 `ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479` |
| 合入后文件 SHA256 | `55e136184adbaeee078a339afde2d6da103f4af7cb7c5c31926f2911127eff03` |

合入后声明位于 `#if !defined(_LIBCXXABI_ARM_EHABI)` 的 `#else`（EHABI）分支：声明在第 1128 行，调用在第 1137 行。文件内实测恰一处声明、恰一处调用，且声明先于调用。

本地提交保留上游作者与正文，并追加：

```text
Upstream-Status: Backport [8119f1854948b50358bbfaea08f207f51970f06c]
Link: https://github.com/llvm/llvm-project/pull/189787
```

## 四、armv7l 构建验证

### 4.1 输入与调用上下文

打包策略和冻结输入未修改：

| 输入 | SHA256 |
|---|---|
| `packaging/libcxx-runtimes.spec` | `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74` |
| `packaging/libcxx-llvm22.map` | `e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d` |
| `packaging/libcxx-runtimes.manifest` | `db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3` |
| `packaging/SOURCE_PROVENANCE` | `1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74` |
| GBS conf | `8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf` |
| buildconf | `9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5` |

最终构建使用源码提交 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b` 的 disposable linked Git worktree。工作目录为：

```text
/home/toolchain/development/libc++_replacement/tmp/R6/gbs-source-worktree-r3b
```

完整命令为：

```text
gbs -c /home/toolchain/development/libc++_replacement/progress/T3R/config/gbs_llvm.conf build -A armv7l -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b -C --debug --style git --include-all --no-patch-export --packaging-dir r6-packaging --spec libcxx-runtimes.spec /home/toolchain/development/libc++_replacement/tmp/R6/gbs-source-worktree-r3b
```

相对目录 `r6-packaging/` 是四项冻结 packaging 输入的逐字副本；tracked 源码零 diff。与 T3R2 的明确差异只有源码由平台预导出 tarball 改为所裁决的 Git 工作树；其余输入 SHA 保持不变。

第一次 r3 调用将外部绝对 `--packaging-dir` 传给 depanneur，工具把该路径错误拼接到源码路径下，退出前尚未扫描包、未出现 circle、未创建编译动作。该根已标记 `RETIRED_SINGLE_USE`。依据错误原文，只把相同内容放入 disposable worktree 的相对 `r6-packaging/`，使用另一全新 r3b 根；未变更 spec、源码或配置。两次调用和退出码分别完整保存在 `build_armv7l_git.full.log`、`build_armv7l_git_r3b.full.log`。

### 4.2 构建结果

| 断言 | 结果 |
|---|---|
| 新根 | `tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260805-r3b`，此前不存在 |
| 单包目标 | `there is no circle in 1 packages`；`[1/1] building libcxx-runtimes` |
| repo 初始化 | 完整 `init_buildsystem --repository ...` 在全量日志第 134 行 |
| GBS 退出码 | 0 |
| `cxa_personality.cpp.o` | 编译成功，全量日志第 2448 行 |
| `%files` | `check-files` 执行成功，无 missing/unpackaged 红项 |
| 产物 | 7 binary RPM + 1 SRPM |
| 根状态 | `RETIRED_SINGLE_USE` |

构建期主要工具链 NEVRA：clang/llvm/libllvm/llvm-devel/lldb `22.1.8-18.1.armv7l`；gcc/g++/libgcc/libstdc++ `14.2.0-1.14.armv7l`；binutils `2.43-2.9.armv7l`；cmake `3.31.2-1.14.armv7l`；rpm-build `4.14.1.1-2.8.armv7l`。完整列表见 `armv7l/installed_all_nevra.tsv`，筛选表见 `armv7l/toolchain_nevra.tsv`。

### 4.3 armv7l 制品 SHA256

| 文件 | SHA256 |
|---|---|
| `libc++-22.1.8-1.armv7l.rpm` | `6d1403f6ef573c0eed5a19b57371a18f7c0de824d702991515b88aaf075ba9b2` |
| `libc++-debuginfo-22.1.8-1.armv7l.rpm` | `a5989c7a6393e8e340187c5db151fc8cb5f5c03decc74583e8874c3778bd05d9` |
| `libc++-devel-22.1.8-1.armv7l.rpm` | `d3b2f25347f5932d11e913dbf66e9abbd80280cbaadb5675ed39bf734bb7cf31` |
| `libc++abi-22.1.8-1.armv7l.rpm` | `a1711752ef16212209cc65c644dbb05a2d9cb3f424b9976ad99fdfe57a959fe4` |
| `libc++abi-debuginfo-22.1.8-1.armv7l.rpm` | `6f4d11b23afd3e1be6d6be1a6a3b528cd967b5d7daa6f4d9bb4a437ffb4fa941` |
| `libc++abi-devel-22.1.8-1.armv7l.rpm` | `14af1b02e01cfed151141b2acef29b01d3507883939a8e5ceeede43d967153b8` |
| `libcxx-runtimes-debugsource-22.1.8-1.armv7l.rpm` | `8f6318ff962d6e1bcd43119dcc65016d8ec9beff731ef6e077b409fc1abbebd5` |
| `libcxx-runtimes-22.1.8-1.src.rpm` | `9d88b88d4a4bde6d58e7f9965951c00ee9fba594c51576e9dfed539b96aab22f` |

## 五、补丁后完整性核查

### 5.1 warning/error 与两个 64 位成功构建对照

| 类别 | armv7l | aarch64 | x86_64 | 判读 |
|---|---:|---:|---:|---|
| compiler warning | 0 | 0 | 0 | 一致 |
| compiler error | 0 | 0 | 0 | 一致 |
| fatal build error | 0 | 0 | 0 | 一致 |
| missing build-id | 6 | 6 | 6 | 三架构共有 |
| debugsource cpio missing | 1 | 1 | 1 | 三架构共有 |
| Lua postinstall 缺失 | 1 | 1 | 1 | 三架构共有 |
| LTO plugin `mv` 缺失 | 1 | 1 | 0 | armv7l/aarch64 共有，非 armv7l 独有 |
| `/etc/profile.rpmnew` | 1 | 0 | 0 | armv7l 独有，原文已单列；不属于编译/链接失败 |

armv7l 唯一独有的诊断原文为：

```text
[  412s] setup-0.9-1.3 ##############warning: /etc/profile created as /etc/profile.rpmnew
```

未发现补丁后 armv7l 独有的编译或链接问题。

### 5.2 三架构 RPM 一致性

- 三架构子包构成均为同 7 包：`libc++`、`libc++-devel`、`libc++-debuginfo`、`libc++abi`、`libc++abi-devel`、`libc++abi-debuginfo`、`libcxx-runtimes-debugsource`。
- 六个 runtime/devel/debuginfo 包的 `%files` 在归一化 `lib`/`lib64` 和构建架构目录后均匹配。
- Provides 在归一化 RPM 架构能力和 64 位后缀后均匹配。
- Requires 去除已保留在 raw diff 中的 ELF 自动生成架构依赖（loader、libc、libgcc、libatomic、libm、libpthread、rtld）后均匹配。
- debugsource 的文件路径因架构选择不同翻译单元/头文件而不同，分类为 `DIFF_ARCH_COMPILE_PATH`；完整差异未删除。
- 非架构相关差异清单为 `NONE`。
- 三架构包内 `SOURCE_PROVENANCE` 均从 `libc++-devel` 的 `/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE` 实际提取，均为 520 字节，SHA256 均为 `1ab73051dc45ee72e681ec0393889bdf527c4d7407df13eb3effa79c7dd4be74`。

初次 provenance 提取误选 `libc++` 包，得到三个空文件；收口审计发现后已更正为实际持有该文件的 `libc++-devel`，并增加“非空 + 三者逐字相同”双断言。第一次更正脚本的断言调用还出现 helper 参数错误（退出 127），提取与哈希本身已成功；第二次仅修正 helper 参数并以退出 0 完成。两次日志均保留，不以空哈希充当通过证据。

### 5.3 armv7l ELF/EHABI 检查

`ldd -r` 执行方式：在宿主 bash 中运行 glibc 原始 `ldd` 脚本逻辑，仅把 `RTLDLIST` 改为静态链接的宿主 helper；helper 以 `execve` 调用：

```text
/usr/bin/qemu-arm-static -L <retired-armv7l-root> <ARM-ld-linux.so.3> <target-library>
```

原 `ldd` 与适配脚本的 diff 仅有 `RTLDLIST` 一项；并断言输出未加载任何 x86_64 库。此前的硬链接分析根、ARM bash 二次启动 RTLD、动态 shell wrapper 三种失败尝试及退出码完整保留在 `commands/11*` 和对应 disclosure 文件中；最终结果只取静态 helper 的 attempt4。

| 检查项 | `libc++.so.1` | `libc++abi.so.1` |
|---|---|---|
| `ldd -r` 退出码 | 0 | 0 |
| 强未解析符号 | 0 | 0 |
| `.ARM.exidx` | PRESENT | PRESENT |
| `.ARM.extab` | PRESENT | PRESENT |
| `LLVM_22` | PASS | PASS |
| `LLVM_21` | 无 | 无 |
| `__gnu_unwind_frame` | N/A | `UND @GCC_3.5`，由目标根原生 ARM `libgcc_s.so.1` 的 `GLOBAL @@GCC_3.5` 定义满足 |

`libc++abi.so.1` 的 DT_NEEDED 与实际 `ldd` 解析均指向目标根的 `libgcc_s.so.1`，符合维持 libgcc unwinder 的裁决。

### 5.4 将来启用 LLVM libunwind 的声明冲突预检

| 来源 | 声明 |
|---|---|
| `libcxxabi/src/cxa_personality.cpp` | `extern "C" _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception*, _Unwind_Context*);` |
| `libunwind/include/unwind_arm_ehabi.h` | `extern _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception *, _Unwind_Context *)`，位于 `extern "C"` 块内 |

两者返回类型、参数类型和 C linkage 相同，仅空格/所在声明块形式不同。实测 probe 同时包含 LLVM libunwind 的 `<unwind.h>` 并再次声明本次声明，在 armv7l target、clang 22 下执行 `-fsyntax-only`，退出码 0，结论为 `SYNTAX_OK_NO_CONFLICT`。本项仅验证重复声明的语法兼容性；未启用 LLVM libunwind、未改变配置。

## 六、交付物与边界

| 交付物 | SHA256 |
|---|---|
| `delivery/0001-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personality.patch` | `cea4f50bc0d20a35724f80d9effdf51d8b5c6dd2258ce34ff0ae85ceff958823` |
| `delivery/suggested_gerrit_commit_message.md` | `7b73833bcf5ac918498cd4e8db526d8a9d5ecfa5ecc9fa6c9bfa7f61e4f4918d` |

建议提交说明包含上游完整 SHA、PR #189787、引入提交 `3c100d5d548d511d09746dd5a2fee29bdd00805c`、引入 PR #116261、issue #194232 和 `Upstream-Status: Backport`。格式化 patch 供人工上传 Gerrit；本次未对 `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm` 或任何其他外部源码仓执行 push。

二进制制品保存在 `artifacts/rebuild_2218/armv7l/`；证据仓只提交 R6 文本证据、原始日志和人工上传用 patch，不把 329 MiB RPM 制品重复上传 GitHub，逐 RPM SHA 已完整落盘。

## 七、证据导航

- 全量构建日志：`build_armv7l_git_r3b.full.log`
- 所有命令原文/退出码：`commands/`
- 上游 patch 原文：`upstream/8119f1854948b50358bbfaea08f207f51970f06c.github.patch`
- 源码身份与同源：`raw/source_git_identity.raw.txt`、`source_compare_exit.tsv`、`source_identity_conclusion.txt`
- armv7l 制品哈希与工具链：`armv7l/`
- 三架构元数据结论：`three_arch_metadata_comparison.tsv`、`three_arch_raw_metadata_differences.diff`
- ELF 原始输出：`raw/ldd_r_*attempt4*`、`raw/libcxxabi_gnu_unwind_*`、`raw/*sections*`
- 可上传 patch 与提交说明：`delivery/`
