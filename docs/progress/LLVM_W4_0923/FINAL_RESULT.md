# LLVM 配方补齐与11包对账：PARTIAL，待人工裁决

LLVM主spec已补唯一授权行并创建本地提交 `2cba97e503d506cc184c34b6a4301087b6fa4bb7`，未推包仓。两批完整RPM的spec身份一致，libclang.a子包对应正确，ARM %prep退出0；**指定的 `_toolchain_override gcc` 门禁不通过，不能标“可签字推送”**。11包主配方比对完成：9个仅VCS差异、bpftrace一致、LLVM为已知的一行未推差异。另发现静态运行库输入的本地候选配方未进入本任务所查LLVM sandbox，须人工确认后续范围。

## 一、已验证LLVM配方身份

| 项 | 结果 | 实际证据 |
| --- | --- | --- |
| ARM 22份RPM所用spec | `tmp/WEEKEND_0918/specs/llvm.spec`，SHA `cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68` | ARM_W5_0921/cell_commands/w5-original.json 的 /tmp/task绑定与spec参数；INPUT_SHA256.json；本轮raw/002、004 |
| AArch64 12份RPM所用spec | `tmp/STATIC_0917B/rpm-aarch64/SPECS/llvm.spec`，同SHA、逐字节相同 | BUILD_STATIC_0917B/native_rpm_serial_command.json；resume_0917/aarch64_complete_1417/AARCH64_RPM_RESULT.md；rpm_spec_hashes.json；raw/003、004 |
| 与f203923a完整差异 | 只有 `-DLIBCLANG_BUILD_STATIC=ON`，位于defined _toolchain及build_with_libcxx条件内 | BASELINE_TO_VERIFIED_LLVM.diff、SPEC_IDENTITY.json |
| 非功能差异 | **0行**，这两份最终写包spec没有VCS/Release差异；不凭早期描述补写 | raw/009完整diff；与当前提交spec cmp退出0 |
| ARM libclang.a | llvm-static-devel-22.1.8-1.armv7l，`/usr/lib/libclang.a`；RPM SHA `62cd7fabb07aebac977a036e6340c70bbd4862f3fe6ae398eba5bf54ac7bd501` | 22份逐个rpm -qlp，LIBCLANG_PACKAGE.json、input-commands.json |
| AArch64 libclang.a | llvm-static-devel-22.1.8-1.aarch64，`/usr/lib64/libclang.a`；RPM SHA `38a572af0e5083f853bbff59f8d4256d1842b3ab17cfdfbda40b0eab69537f49` | 12份逐个rpm -qlp，同上 |
| 子包依赖对应 | LLVM `%files static-devel` 的 `%{_libdir}/lib*.a`覆盖；bpftrace已直接BuildRequires: llvm-static-devel（也有clang-devel） | llvm.spec:98–103,545–548；bpftrace已验证recipe:28–30，raw/011；不是仅靠clang-devel间接推断 |

RPM查询是本轮只读实测；完整构建及归档ABI结论沿用原验收，不称本轮新构建。SHA不同的归档身份继续只登记不归因。

## 二、本地提交与门禁

| 项 | 状态 | 依据 |
| --- | --- | --- |
| 可写克隆 | `tmp/BUILD_W1BC_0911/target-fetch`；沿用f203923a推送克隆 | BUILD_W1B_0909/raw/018；本輪raw/006–007 |
| 远端基线 | fetch后与末次ls-remote均为f203923a1508c9344f5fc6b17bd8822f011655c4，起始工作树干净 | raw/010、016；commit-commands.json |
| 本地分支与提交 | sandbox/lhmax2025/libcxx-ehabi-backport；`2cba97e503d506cc184c34b6a4301087b6fa4bb7` | LOCAL_COMMIT.json，SANDBOX_COMMIT.diff，format-patch |
| 作者/签字 | Author为Hao Lin <hao.lin@samsung.com>；保留原克隆committer hao.lin <hao.lin@samsung.com>，没有代签；人工amend时重写committer | git show全文；本地Git提交无法不含committer字段，未擅设人工签字身份 |
| 工作树/差异 | 干净，ahead 1 / behind 0（相对本轮FETCH_HEAD）；仅llvm.spec一行 | raw/031、commit-commands.json |
| ARM %prep | PASS，退出0，新独立tmp树；%setup及ARM libc++ MLGO资产解包成功，未进%build | prep-check/prep-command.json、prep.stderr.txt |
| Clang宏展开 | PASS，静态开关1次 | prep-check/clang.stdout.txt:205；命令退出0 |
| GCC override宏展开 | **FAIL判据**：命令退出0，但静态开关仍1次、LLVM_ENABLE_LIBCXX仍1次、CXX为Clang | prep-check/gcc.stdout.txt:197,204–205；macro-result.json |
| 未定义_toolchain宏展开 | 静态开关0次、LLVM_ENABLE_LIBCXX 0次；退出0 | prep-check/undefined.stdout.txt；只证明所要求开关不引入，不宣称整个未定义构建可用 |

### GCC门禁为何不通过

本轮没有换成 `_toolchain gcc` 来代替指定 `_toolchain_override gcc`。原平台用户宏 `.../home/abuild/.rpmmacros:54–55` 是：

```text
%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}
%toolchain_is() %((echo "%_toolchain" | %{__grep} -qE "%{1}") && echo 1 || echo 0)
```

原有llvm.spec:11–12又在_toolchain已定义时执行 `%define _toolchain_override clang`。因此指定gcc被覆盖，build_with_libcxx变成1。**远端基线同条件展开也选择Clang、含LLVM_ENABLE_LIBCXX，但无新增静态开关；候选增加了静态开关。** 这是已实测的宏路径差异，不是GCC driver实际编译回归实测。不能据此改写历史真正GCC构建结论，也不能声称本轮指定override格通过。

历史BUILD_NIGHT_0917/check_llvm_guards.py使用的是字面 `_toolchain gcc`；与本任务override选择器并不等价。未重新使用旧夹具取代新门禁，未修改已有强制Clang逻辑。本地提交正文的“GCC路径不变”不能覆盖这次override组合，签字前需要裁决口径或授权修法；提交留作候选，不擅自amend/推送。

## 三、11包全量比对与额外挂账

| 包 | sandbox SHA前缀（完整见RECIPE_AUDIT.tsv） | 主spec/patch结论 |
| --- | --- | --- |
| abseil-cpp | 7a4f04da | 仅VCS非功能差异 |
| boost | 4168e873 | 仅VCS非功能差异；1份patch相同 |
| icu | dcbb3fb4 | 仅VCS非功能差异；6份patch相同 |
| jsoncpp | 9cbab147 | 仅VCS非功能差异 |
| libsigc++ | 4f2f592d | 仅VCS非功能差异；R105复核沿用R104实际构建 |
| pcre | 09a5bfc9 | 仅VCS非功能差异；1份patch相同 |
| taglib | 8b552bdf | 仅VCS非功能差异 |
| tensorflow2 | 6d761390 | 仅VCS非功能差异；采用R105 libc++候选，R108 GCC基线不冒充候选 |
| llvm | f203923a | 有功能差异未推送：LIBCLANG_BUILD_STATIC=ON；本地已补，门禁未全过 |
| bcc-tools | 5bd0654e | 仅VCS非功能差异 |
| bpftrace | 7831fb34 | 一致；static-link-libbpf.patch一致 |

逐包证据路径、完整SHA、spec快照/diff、packaging每文件哈希在 RECIPE_AUDIT.md/.tsv、recipe-audit/*/result.json；所有11个远端本轮重查一致。9个VCS字段是导出元数据，所有差异原文都保留，没有纳入提交。比对覆盖保存的配方与packaging文件，不是所有源码tar成员的重新审计或新一次构建；不把当前路径保存文件的哈希冒充历史运行时逐文件取证。详见RECIPE_AUDIT.md范围说明。

### 额外发现：libcxx-runtimes静态配方仍是本地候选

这不是llvm.spec的第二项差异，而是同一LLVM仓中的另一个spec、bpftrace输入链的缺口：

- f203923a 的 packaging/libcxx-runtimes.spec 仍为 `LIBCXX_ENABLE_STATIC=OFF`、`LIBCXXABI_ENABLE_STATIC=OFF`，安装静态库开关也OFF。
- BUILD_NIGHT_0917/W1_REPORT.md:14–20记载的候选开启四个静态开关并为两个devel子包加入 `.a` 文件；`tmp/NIGHT_0917/specs/libcxx-runtimes.spec` 与周末候选SHA均为 `c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe`。完整差异在 UNPUSHED_LIBCXX_RUNTIME.diff。
- BPF_W1_0921/installed-armv7l-inputs.json:59–79记录从本地runtime RPM提供libc++/libc++abi及devel；其INPUT_STATUS说明实际安装19份输入而非只8份LLVM。bpftrace新spec明确链接libc++.a/libc++abi.a。
- 只补libclang.a不能证明这两个静态运行库的服务端供给已闭合；QuickBuild是否另有已提供它们的runtime分支/输入，**NOT_OBSERVED**。本轮不修改这个spec、不追加提交、不擅自选供给方案。

**停下交人工审阅。** 包仓不推；本地提交保留。需要裁决GCC override门禁与静态运行库配方范围，之后再决定签字推送及QuickBuild。本轮项目仓只提交证据与LINE_STATUS。
