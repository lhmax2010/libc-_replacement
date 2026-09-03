# 11 个包的 libc++ 改法设计（待确认，尚未实施）

状态：`AWAITING_IMPLEMENTATION_CONFIRMATION`

本文件只描述拟议改法与验证判据。11 个仓库的 spec 均未修改，尚未创建
提交、尚未构建、尚未把任何代码提交推到这些 sandbox 分支。

## 1. 一致的启用判据

建议 11 个 spec 统一使用以下有效条件：

```spec
%if %{defined _toolchain}
%if %{toolchain_is clang}
%define build_with_libcxx 1
%endif
%endif
```

需要 libc++ 的声明和 flags 全部放在：

```spec
%if 0%{?build_with_libcxx}
...
%endif
```

`tensorflow2` 还要同时满足其既有 `%{USE_CLANG} == "ON"`，防止该包的
armv7hl GCC 特例误入 libc++ 路径。

这个判据的依据不是编译器名称猜测：

1. `Tizen-Base-Toolchain/project_config:133-139` 定义 `_toolchain`，默认值
   是 `clang`，并由它展开 `__cc`/`__cxx`；包内
   `_toolchain_override gcc` 会使 `toolchain_is clang` 返回假。
2. 工作区全部 project_config 的实查结果中，只有
   `Tizen-Base-Toolchain` 定义该机制；标准 `Tizen-Base` 不定义它。
3. 当前 `llvm.spec:11-16` 已用 `_toolchain` 是否存在来区分 Toolchain 项目
   与普通发布构建，这是可复用的平台做法。
4. 没有找到现成的 libc++ 专用开关。project_config 确实记录了
   `%bcond_with`/`_with_*` 的通用惯例，但引入新 `_with_libcxx` 还需要修改
   project_config；本方案不需要额外配置改动。

由此得到三条机械行为：

| 环境 | 判据 | 行为 |
| --- | --- | --- |
| Base Toolchain 默认 Clang | 真 | 加 libc++ 构建依赖与 flags |
| Base Toolchain 中 `_toolchain_override gcc` | 假 | 不加依赖、不加任何 `-stdlib`，保持现状 |
| 不定义 `_toolchain` 的非迁移配置 | 假 | 保持现状 |

本判据是针对当前 Base Toolchain 项目范围的选择器，不宣称是所有 Tizen
项目的通用 libc++ 开关。若人工希望同一 sandbox 分支在“定义 `_toolchain`
且使用 Clang、但仍明确要求 libstdc++”的未来配置中保持旧行为，则需要改用
显式 `_with_libcxx` 开关；现有配置中未观察到这种第四种状态。

## 2. 共同改动模板

除 LLVM 和 TensorFlow 的特殊路径外，拟在有效条件内：

```spec
BuildRequires:  libc++-devel
BuildRequires:  libc++abi-devel
```

并在进入各自构建系统前加入：

```sh
export CXXFLAGS="${CXXFLAGS} -stdlib=libc++"
export LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed"
```

不改 `CFLAGS`。Boost 与 PCRE 现有 devel 子包显式依赖
`libstdc++-devel`，只在有效条件为真时把该依赖换成 `libc++-devel`；条件为
假时保留原行。其他包不新增没有先例的 devel 运行依赖。

现有 `gcc-c++` BuildRequires 暂不删除：它是依赖集合的一部分，不负责实际
选择 CXX；删除会扩大改动面。实际 compiler 仍以构建日志和 CMake/B2
verbose 命令核验。

## 3. 逐包设计

### abseil-cpp

- 位置：`packaging/abseil-cpp.spec`，BuildRequires 区；`%build:45` 后、
  `%{cmake}:48` 前。
- 改法：条件加入两个 devel BuildRequires；在 CMake configure 前导出共同
  CXXFLAGS/LDFLAGS。
- GCC/非迁移：条件为假，不增加依赖与环境变量，原 `cmake/build` 流程不变。
- 验证：检查全部 `libabsl_*.so.*` 的 `DT_NEEDED`；运行包自测或代表性的
  absl string/status/synchronization 链接 smoke test。

### bcc-tools

- 位置：`packaging/bcc-tools.spec`，BuildRequires 区；`%build:58` 后、
  `LLVM_VERSION_MAJOR` 与 `cmake:72` 前。
- 改法：条件加入依赖并在 CMake 前导出共同 flags；不改
  `ENABLE_LLVM_SHARED` 选择。
- GCC/非迁移：条件为假，原 `gcc >= 3.4` BuildRequires 和构建命令均保留，
  GCC 命令不出现 `-stdlib=libc++`。
- 验证：检查 `libbcc.so.*`、`libbcc_bpf.so*`；对 `libbcc.a` 等静态归档检查
  `std::__1`/GLIBCXX 符号，并链接代表性 consumer。

### boost

- 位置：`packaging/boost.spec`，BuildRequires/`boost-devel` Requires；
  `%build:429` 中现有 arm flags 之后，B2 `:524-527` 之前。
- 改法：条件加入两个构建依赖；条件切换 devel 依赖；把 `-stdlib=libc++`
  合入 `CXXFLAGS`，把 libc++/ABI 参数合入现有 `LDFLAGS`。B2 已在
  `:525-526` 显式把这两个变量传作 `cxxflags=`/`linkflags=`，无需修改
  Boost 源码或 Jam 规则；`--with-toolset=${CC##*-}` 仍由现有工具链选择。
- GCC/非迁移：条件为假，`libstdc++-devel` Requires 和 B2 参数保持当前值。
- 验证：逐个检查所有 `libboost_*.so.*`，并编译/运行 filesystem、thread、
  exception、iostreams 的代表性 consumer；验证头文件实例化同样使用
  `std::__1`。

### bpftrace

- 位置：`packaging/bpftrace.spec`，BuildRequires 区；`%build:67` 中 sanitizer
  `gcc-unforce-options` 之后、`mkdir build:71` 之前。
- 改法：在 unforce 操作之后条件导出共同 flags，避免该脚本把新 flags 清掉；
  其余 CMake 参数不变。
- GCC/非迁移：条件为假，原 GCC/sanitizer 路径不接收 `-stdlib`。
- 验证：检查 `bpftrace` 及其交付 C++ ELF；执行 `--info`、简单 trace parse/
  compile smoke test，并核验对新 BCC/LLVM 的实际依赖。

### icu

- 位置：`packaging/icu.spec`，BuildRequires 区；在现有 `%build:74` 的
  CXXFLAGS 行上按条件追加 `-stdlib=libc++`，并在 `%configure:81` 前条件导出
  LDFLAGS。
- 改法：保留 `-D_TIZEN_FEATURE_ -fPIC` 与 `LINKFLAGS=" -pie"`，只在真分支
  追加共同参数。
- GCC/非迁移：条件为假，现有 flags 逐字保持。
- 验证：检查 `libicu*.so*`，运行 C/C++ API 的字符集、locale、格式化 smoke
  test；确认 C API 仍可被非 C++ caller 使用。

### jsoncpp

- 位置：`packaging/jsoncpp.spec`，BuildRequires 区；`%build:35` 后、
  `%cmake:38` 前。
- 改法：条件加入依赖并导出共同 flags，其余 JSONCPP 选项不变。
- GCC/非迁移：条件为假，配置与产物选择保持现状。
- 验证：检查 `libjsoncpp.so.*`，编译/运行 parse/write C++ consumer。

### libsigc++

- 位置：`packaging/libsigc++.spec`，BuildRequires 区；`%build:39` 后、
  `%meson:40` 前。
- 改法：条件加入依赖并在 Meson setup 前导出共同 flags；Meson 会从首次
  configure 的环境初始化编译和链接参数。
- GCC/非迁移：条件为假，Meson 参数和现有构建保持不变。
- 验证：检查 `libsigc-2.0.so.*`，编译/运行 signal/slot C++ consumer。

### llvm

- 位置：只改 `packaging/llvm.spec`，不改 `libcxx-runtimes.spec`。有效条件定义
  放在当前 `_toolchain_override clang` 块之后；依赖放 BuildRequires 区；
  CMake 参数在 `:198-256`。
- 改法：真分支传 `-DLLVM_ENABLE_LIBCXX=ON`，复用 LLVM 自身
  `HandleLLVMStdlib.cmake`：它会先实测 compiler/linker 是否支持
  `-stdlib=libc++`，再把选项追加到 CXX、EXE、SHARED、MODULE flags。
  同时在真分支把 `-Wl,--no-as-needed -lc++abi -Wl,--as-needed` 合入 spec
  已显式指定的 EXE/SHARED linker flags；假分支保留现有两行原值。
- 对产出的编译器：该改动改变的是 LLVM/Clang/LLDB 等**宿主程序自身**的
  构建与运行时依赖，不设置 `CLANG_DEFAULT_CXX_STDLIB`，不改 driver 源码，
  因而不会把产出 clang 的默认标准库选择改写为 libc++。实施验证仍须用新
  clang 的 `-###` 对“无 `-stdlib`”和显式两种调用分别确认，不能只凭设计。
- GCC/非迁移：有效条件为假时不设置 `LLVM_ENABLE_LIBCXX`，保留当前 CMake
  linker flags；普通发布构建继续走 `llvm_release_build` 旧路径。
- 验证：检查 `libLLVM*.so*`、`libclang*.so*`、clang/lldb 工具；运行
  `clang -###`、编译 C/C++ hello、llvm-config 与 lldb smoke test。

### pcre

- 位置：`packaging/pcre.spec`，BuildRequires/`pcre-devel` Requires；
  `%build:99` 的现有 CFLAGS 后、`autoreconf:103` 前。
- 改法：条件加入依赖；真分支把 devel 依赖换为 `libc++-devel`；在 configure
  前导出共同 CXXFLAGS/LDFLAGS。纯 C `libpcre` 不被 CXXFLAGS 改写。
- GCC/非迁移：假分支保留 `libstdc++-devel` 及原 configure 行。
- 验证：重点检查 C++ wrapper `libpcrecpp.so.*`，同时确认纯 C
  `libpcre.so.*` 没有被无故增加 libc++ 依赖；运行 C 与 C++ 两套 API smoke。

### taglib

- 位置：`packaging/taglib.spec`，BuildRequires 区；`%build:28` 后、
  `%cmake:30` 前。
- 改法：条件加入依赖并导出共同 flags。
- GCC/非迁移：条件为假，现有 CMake 构建保持不变。
- 验证：检查 C++ `libtag.so.*` 与 C wrapper `libtag_c.so.*`；分别跑 C++/C
  读取标签 smoke test。

### tensorflow2

- 位置：`packaging/tensorflow2.spec`。在 `USE_CLANG` 架构选择完成后定义
  有效条件；条件加入 devel 依赖；在 `%build:162-170` 的 Clang 分支把
  `-stdlib=libc++` 加入 `CLANG_CXXFLAGS`，并设置共同 LDFLAGS。两次 CMake
  configure（TFLite 与 AOT）都继承该环境；GCC `USE_CLANG OFF` 分支不改。
- GCC/非迁移：需要同时满足 `USE_CLANG ON`、`_toolchain` 存在、
  `toolchain_is clang`；任一不满足均不加入 libc++ 参数。尤其 armv7hl 的
  `USE_CLANG OFF` 路径保持原样。
- 静态归档验证：`.a` 没有 `DT_NEEDED`，不能套用动态库判据。先对
  `libtensorflow2-lite.a` 与 `libtf_xla_runtime.a` 的每个成员做符号扫描，
  要求迁移 C++ 符号呈 `std::__1`，不得残留 `std::__cxx11`/`GLIBCXX_*`；再用
  代表性 consumer 实际链接，确认最终 ELF 依赖 `libc++.so.1`、不依赖
  `libstdc++.so.6`。最后重建 `inference-engine-tflite`、`nnstreamer`、
  `nntrainer` 并复跑 R103 的精确归档成员归属检查。

## 4. LLVM、BCC、bpftrace 的协调要求

R101 的实际符号边表中有：`bcc-tools → llvm` 584 个 C++ 符号、
`bpftrace → llvm` 201 个 C++ 符号，均被判为布局敏感；LLVM 自身不同子包间
另有 17,589 个布局敏感符号。三者不能独立混用新旧运行时。

顺序应为：先提供已验证的 `libcxx-runtimes`，再构建并发布迁移后的 LLVM，
再在同一仓状态下构建 BCC，最后构建 bpftrace（它还直接依赖
`bcc-tools-devel`）。验证期间不得让新 BCC/bpftrace 链接旧 LLVM，也不得让
新 bpftrace 取得旧 BCC。三者的 spec 提交仍保持独立，但 QuickBuild 构建集
必须协调生效。

## 5. 实施阶段的统一验证矩阵

每包、每个 `armv7l`/`aarch64`/`x86_64` 都执行：

1. **libc++ 格**：Base Toolchain 默认 Clang，确认实际 compiler/version、
   完整命令含目标 flags、构建成功；动态 C++ ELF 必须直接或按已声明方式
   依赖 `libc++.so.1`，不得依赖 `libstdc++.so.6`；检查 `GLIBCXX_*` 与
   `std::__cxx11` 残留。静态归档按 TensorFlow/BCC 所述另验。
2. **GCC 格**：同一 spec 令 `_toolchain_override gcc` 生效，实际完整构建；
   日志中 `-stdlib=libc++`/`-lc++abi` 必须为零，产物依赖与未改基线一致。
3. **非迁移格**：在不定义 `_toolchain` 的配置中至少完成 spec 展开与实际
   构建抽检，证明条件块消失；所有包在最终提交前保存展开后的 spec 对照。
4. **运行时补丁前置门禁**：构建根中的 `libc++-devel` 先执行 R85 五处头文件
   检查，libc++/libc++abi 再检查层 A/B/C 库内证据。11 包不会修改这些文件；
   包级验证不会被误写成重新验证四补丁本身。
5. **功能**：执行各包自测（若打包禁用则记录）和上节所列 smoke test；再次
   扫描 R101/R103 的布局敏感边，确认边的两端均为同一 ABI。

任一包任一必需格失败或不可得：该包不提交推送，完整记录并停在该包；其他包
即使已通过也不能用来替代该包的结果。三架构均未完成前不宣称该包验证通过。

## 6. 待人工确认后才能实施的点

1. 是否接受把“`_toolchain` 已定义且实际为 clang”作为本轮 Base Toolchain
   的 libc++ 启用判据；还是要求新增显式 `_with_libcxx` 开关并另改
   project_config。本文建议前者，理由见第 1 节。
2. 是否接受 LLVM 使用其原生 `LLVM_ENABLE_LIBCXX=ON`，同时仅补强制
   `libc++abi` 链接，而不是像普通 CMake 包一样只依赖环境 CXXFLAGS。
3. 是否接受按 `libcxx-runtimes → llvm → bcc-tools → bpftrace` 的有序构建集
   验证；当前材料不能证明 QuickBuild 会仅凭 BuildRequires 自动形成全部
   源码构建顺序，执行前需人工确认任务图或分轮触发。
4. Unified 的 `tensorflow2@ea3f134d…` 明确留到 Unified 包集环节另做，不在
   本轮 Base 分支上追加第二个基线提交。

## 7. 自行判断与未观察项

- 自行判断：沿用 `_toolchain`/`toolchain_is`，是因为它是当前唯一同时表示
  “属于 Base Toolchain 项目”和“最终实际工具链”的已存在机制；没有把
  `BuildRequires: clang` 或包名当作编译器判据。
- 自行判断：不删除既有 `gcc-c++` BuildRequires，避免把标准库迁移扩大成
  构建依赖清理。
- 自行判断：LLVM 采用上游已有 `LLVM_ENABLE_LIBCXX`，并把“宿主程序使用的
  标准库”和“产出 clang 的默认标准库”分开验证。
- `NOT_OBSERVED`：本阶段尚未修改 spec、尚未做任何一格构建、尚未产生包侧
  提交；所有验证结果必须等人工确认设计后取得。

