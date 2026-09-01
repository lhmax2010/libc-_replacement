# R91 第一阶段：事实查明与待确认方案

## 阶段状态

- 第一阶段四组事实已查清。
- 尚未修改 `packaging/libcxx-runtimes.spec`，未构建、未提交、未推送。
- 当前停在任务书规定的确认门，等待人工确认第二阶段做法。
- 资源门禁：`tools/resource_gate.sh --level light` 退出码 0，结果 `PASS`。

## 1. `%{__cc}` / `%{__cxx}` 的实际解析

### Base Toolchain

Base 的 `project_config` 直接定义：

```spec
%__cc_clang %{_host}-clang
%__cxx_clang %{_host}-clang++
%__cc_gcc %{_host}-gcc
%__cxx_gcc %{_host}-g++
%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}
%__cc %{expand:%%{__cc_%{_toolchain}}}
%__cxx %{expand:%%{__cxx_%{_toolchain}}}
```

因此：

- 未定义 `_toolchain_override` 时，默认 `_toolchain=clang`；
- 若某个 spec 定义 `_toolchain_override gcc`，同一组 `%{__cc}` / `%{__cxx}` 会改为 GCC；
- 总部指出“`%{__cc}` 可能解析为 GCC”是成立的：问题不在当前默认值，而在这两个宏不是固定的 Clang 入口。

此前 Base QuickBuild 实际值：

| 架构 | `%{__cc}` 实际值 | `%{__cxx}` 实际值 |
| --- | --- | --- |
| armv7l | `armv7l-tizen-linux-gnueabi-clang` | `armv7l-tizen-linux-gnueabi-clang++` |
| aarch64 | `aarch64-tizen-linux-gnu-clang` | `aarch64-tizen-linux-gnu-clang++` |
| x86_64 | `x86_64-tizen-linux-gnu-clang` | `x86_64-tizen-linux-gnu-clang++` |

### Unified Toolchain

Unified 自己的 `project_config` 不重复定义 `_toolchain`、`__cc`、`__cxx`；它只重写 `%build`，仍导出继承到的 `%{__cc}` / `%{__cxx}`。该配置尾部注明 `derived from Tizen:Base`，`snapshot_info` 也明确把 Base Toolchain 快照列为 base。

为避免只凭继承关系推断，又读取了 Unified 快照 `tizen-unified-toolchain_20260829.015247` 中 `gccmakedep` 的三架构完整构建日志。实际导出值为：

| 架构 | `CC` 实际值 | `CXX` 实际值 |
| --- | --- | --- |
| armv7l | `armv7l-tizen-linux-gnueabi-clang` | `armv7l-tizen-linux-gnueabi-clang++` |
| aarch64 | `aarch64-tizen-linux-gnu-clang` | `aarch64-tizen-linux-gnu-clang++` |
| x86_64 | `x86_64-tizen-linux-gnu-clang` | `x86_64-tizen-linux-gnu-clang++` |

结论：两种配置当前均实际选择 Clang；架构变化只改变目标三元组/ABI 前缀，不改变编译器家族。QuickBuild 服务端合并 project config 的内部实现未直接观测，记为 `NOT_OBSERVED`；但 Unified 的 base 声明与三架构实际日志形成了直接结果证据。

## 2. 此前 QuickBuild 实际使用的编译器

R85 保存的 Base QuickBuild 三份 `libcxx-runtimes.buildlog.txt` 均显示：

- 构建根安装 `clang-22.1.8-2.3`；
- CMake 命令使用上表的目标前缀 `clang` / `clang++`；
- CMake 识别结果为 `The C compiler identification is Clang 22.1.8` 和 `The CXX compiler identification is Clang 22.1.8`；
- `--rtlib=libgcc` 确实由 Clang driver 接收。

Clang 的选择链为：Base `project_config` 默认 `_toolchain=clang` → `%{__cc}` / `%{__cxx}` 展开为 `%{_host}-clang{,++}` → 当前 spec 把这两个宏传给 CMake。它不是当前 spec 自己强制出来的，因此仍会受 `_toolchain_override` 影响。

## 3. 平台显式选择编译器的惯例与交叉编译影响

检索口径为 R11 已归档的 1,449 条 source-spec 记录（1,313 个按仓库与包名去重的包）。找到两类明确写法：

1. 平台 LLVM spec 使用：

   ```spec
   %if %{defined _toolchain}
   %define _toolchain_override clang
   ...
   %endif
   ```

   随后的 CMake 仍使用 `%__cc` / `%__cxx`。这是与当前包最接近、也最能保留平台交叉编译接口的惯例。

2. TensorFlow2、dotnet-launcher、dynpart-tools、heaptrack、netcoredbg 等包直接写 `clang` / `clang++` 或直接传给 CMake。该方式在平台存在，但没有利用 `%{_host}` 前缀，和本包的交叉目标设置不如第一种贴合。

交叉编译方面，Base 配置为 armv7l/aarch64 等目标使用 x86_64 host，并预装相应 `clang-accel` 包；`%__cc_clang` 和 `%__cxx_clang` 保留 `%{_host}` 前缀。若只强制 `_toolchain_override=clang`：

- CMake 编译器仍是目标前缀 driver；
- 现有 `-DCMAKE_{C,CXX,ASM}_COMPILER_TARGET=%{_host}` 不变；
- 现有目标 triple、sysroot/构建根选择和架构 flags 不变；
- 不会把交叉构建误改为宿主机裸 `clang`。

## 4. `BuildRequires: clang = %{version}` 与 driver 名称

结论分两层：

- 该 BuildRequires 足以要求构建根安装相同版本的 `clang` 包；此前 QuickBuild 日志也实际显示 `clang-22.1.8-2.3` 被安装。
- 它不负责把 `%{__cc}` 指向 Clang；编译器选择仍由 `_toolchain` 宏完成。因此仅保留 BuildRequires 不能消除 review 指出的可变路径。

driver 名称有平台打包证据：LLVM spec 在安装阶段创建
`%{_prefix}/bin/%{_host}-clang` 与 `%{_prefix}/bin/%{_host}-clang++`，并把两者列入 `clang` 子包文件清单。三架构 QuickBuild/Unified 日志又分别证明这些名称在构建根中可执行。因此不需要写死 `/bin/...` 的绝对路径。

## 待确认的第二阶段做法

建议遵循平台 LLVM spec 的惯例，只在 `packaging/libcxx-runtimes.spec` 顶部加入一行：

```spec
%define _toolchain_override clang
```

其余保持不动，尤其继续让 CMake 使用 `%{__cc}` / `%{__cxx}`，不改为裸 `clang` / `clang++`。宏展开探针已验证：即使外部原先给出 GCC 路径，spec 中的 override 为 clang 时，`%{__cc}` / `%{__cxx}` 会固定展开为目标前缀 Clang。

这个方案的预期行为是：GCC 构建路径不再进入；`--rtlib=libgcc` 始终只交给 Clang driver。若构建根缺失 Clang，则会在查找/执行编译器时明确失败，但当前精确版本 BuildRequires 与 clang 包的文件清单共同保证其可用。

请人工确认：第二阶段是否按上述“一行 `_toolchain_override clang`，其余 CMake 编译器行不动”的方案执行。

## 自行判断、未决事实与过程异常

### 自行判断

- 将平台 LLVM spec 视为本包最接近的编译器选择惯例；因此提出宏 override，而非写死裸命令或绝对路径。
- 用 Unified 已发布快照中普通包的实际 `%build` 展开，补强仅阅读 `derived from Tizen:Base` 注释不足的问题。

### 未决/尚未执行

- QuickBuild 服务端合并 Base/Unified project config 的内部代码路径：`NOT_OBSERVED`；实际展开结果已由三架构日志确认。
- 第二阶段 spec diff、实际构建、产物核验、提交与推送：均尚未执行，等待确认。

### 技术性过程异常

- 一次记录器调用漏写 `bash`，退出 126，未执行目标命令；已原样记录。
- 两次路径枚举范围过宽并持续运行，主动以 SIGTERM 停止（退出 143），改用 R11 索引表和 R83–R85 限定目录；未修改任何源码。
- 一次已有表检索因后续 `find`/匹配组合返回 2；所需表内容已由后续限定命令重新取得。以上均不影响事实判据。

## 主要证据索引

- `raw/001_resource_gate.*`：资源门禁。
- `raw/020_quickbuild_compiler_evidence.*`：Base QuickBuild 三架构 Clang 证据。
- `raw/030_unified_actual_macro_expansion.*`：Unified 三架构实际 `CC/CXX`。
- `raw/031_collect_stage1_exact_excerpts.*`：两个 project_config、当前 spec 与工作树状态原文。
- `raw/019_extract_platform_spec_examples.*`：LLVM/TensorFlow2 平台 spec 示例。
- `raw/032_quantify_platform_convention_evidence.*`：检索口径与命中量化。
- `raw/033_macro_expansion_probe.*`：默认/GCC override/Clang override 的宏展开探针。
- 每条 shell 命令均有 `.command.txt`、`.stdout`、`.stderr`、`.exitcode` 配套文件。
