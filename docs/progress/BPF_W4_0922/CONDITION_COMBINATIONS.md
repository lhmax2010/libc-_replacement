# 工具链与 static 构建条件核查（续轮，只读）

## 宏在哪里定义

以下针对本地实际文件，不冒充 QuickBuild 服务端最终宏快照。完整快照、SHA、逐行摘录在 `condition-check/`。

| 材料 | 原文与行号 | 值及作用范围 |
|---|---|---|
| `codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config:118,133–134` | `Macros:`；`%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}`；`%toolchain_is() %((echo "%_toolchain" \| %{__grep} -qE "%{1}") && echo 1 \|\| echo 0)` | 此宏段不在包名/架构条件内；项目默认 clang，允许各包 `_toolchain_override` 改写。不是全部包无条件使用 clang。 |
| 同一 project_config 全文 | 未找到 `build_with_libcxx` 定义 | **不全项目定义该宏**。SHA `aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209`。 |
| `config/gbs_llvm.conf:1–12` | 第 6 行 `repos = repo.base-standard, repo.unified-standard`；第 9/12 行为 Base-Toolchain / Unified-Toolchain reference 仓 URL | 文件只有 profile/buildroot/repos 设置；**没有定义上述任一宏**。不能把仓地址当作宏定义。SHA `8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf`。 |
| 提交后的 `packaging/bpftrace.spec:1–5` | 下列原文 | 解析本包 spec 时自动派生 `build_with_libcxx=1`，不是另一个须人工开启的全局开关。 |

```rpm
%if %{defined _toolchain}
%if %{toolchain_is clang}
%define build_with_libcxx 1
%endif
%endif
```

ARM 两个原 GBS 构建根 `/home/abuild/.rpmmacros:54–55` 实际也含同样 `_toolchain`/`toolchain_is` 定义；aarch64 对应内容见 `condition-check/EXCERPTS.txt`。原文和行号保留在快照，不改 config/codes。当前 `%prep` 根的预解析 `rpm --eval` 输出：

```text
payload=w5.xzdio;toolchain=clang;clang=1;build_with_libcxx_before_spec=
```

这是**解析 spec 前**未定义的状态；随后 spec:3 定义为 1，并在本次 `%prep` 实际执行 `test 1 = 1`/sed。不能据此称“clang 但实际构建未开启 libc++”已被一格覆盖。

## W1 六格：历史 argv 与构建阶段有效条件

不是本轮重跑六格。取原 `cell_commands`、`build.log` 和 spec 条件链核对；两种常规工具链格没有历史直接打印两个宏数值的转储，表中值为**静态条件核查并由历史实际分支/编译器记录佐证**，不虚构 `--define build_with_libcxx` 参数。完整逐行摘录与输入身份见 `condition-check/SIX_CELLS.json`、`EXCERPTS.txt`、`INPUT_IDENTITIES.json`。

| W1 格 | 原 argv 中相关覆盖 | `_toolchain`（有效） | `build_with_libcxx`（spec 解析后） | 历史佐证 |
|---|---|---|---|---|
| armv7l / libc++ | 没有给这两个宏加 CLI 定义 | clang | 1，spec:3 定义 | `cells/bpf-armv7l-libcxx/build.log:26` test；`:50–56` clang/stdlib；`:62` Clang 22.1.8 |
| armv7l / GCC | `--define '_toolchain_override gcc'`，cell_commands:69 | gcc | 未定义，`0%{?…}` 为 0 | `cells/bpf-armv7l-gcc/build.log:54` GNU 14.2.0；实际安装原资产 |
| armv7l / 未定义 | `--undefine _toolchain`，cell_commands:68–69；undefined harness 再次 undef | 未定义 | 未定义，条件为 0 | `cells/bpf-armv7l-undefined/build.log:3–5`：`HARNESS_AFTER_INCLUDE=0\|\|armv7l-tizen-linux-gnueabi-gcc\|armv7l-tizen-linux-gnueabi-g++` |
| aarch64 / libc++ | 没有给这两个宏加 CLI 定义 | clang | 1，spec:3 定义 | `cells/bpf-aarch64-libcxx/build.log:26` test；`:63` Clang 22.1.8 |
| aarch64 / GCC | `--define '_toolchain_override gcc'`，cell_commands:69 | gcc | 未定义，条件为 0 | `cells/bpf-aarch64-gcc/build.log:55` GNU 14.2.0；实际安装原资产 |
| aarch64 / 未定义 | `--undefine _toolchain`，cell_commands:68–69；undefined harness 再次 undef | 未定义 | 未定义，条件为 0 | `cells/bpf-aarch64-undefined/build.log:3–5`：`HARNESS_AFTER_INCLUDE=0\|\|aarch64-tizen-linux-gnu-gcc\|aarch64-tizen-linux-gnu-g++` |

上表路径均相对 `docs/progress/BPF_W1_0921/`。六格都没有 CLI 的 `--define build_with_libcxx ...`。

| 特殊组合（指解析后/构建阶段） | 六格覆盖情况 | 说明 |
|---|---|---|
| `_toolchain=clang` 但 `build_with_libcxx` 未定义 | **NOT_OBSERVED** | 正常解析会由 spec 自动定义 1；解析前未定义不算独立组合测试。 |
| `build_with_libcxx=1` 但 `_toolchain=gcc` | **NOT_OBSERVED** | GCC 格没有外部强制该宏。不能外推任意外部宏组合都安全。 |

## QuickBuild 条件是否必然同时成立

**对已核对的 Base-Toolchain 配置与该 spec，在没有额外宏注入/后续重定义的默认 Clang 路径，两条件由 spec:1–5 保证同时成立；但本轮没有 QuickBuild 服务端最终宏快照，不能无条件断言实际 QuickBuild 的所有配置必然同样成立。** 人工启动前应核对最终配置与提交身份。

额外的静态观察（不是实验结论）：spec 在 GCC/未定义分支并未显式清除外部预先设置的 `build_with_libcxx`。如果外部强制为 1 再选 GCC，构建段与安装段可能不一致；这一组合未测，不擅自修改配方，也不把正常 GCC 六格结论扩成该组合通过。签字时需审阅这一边界。
