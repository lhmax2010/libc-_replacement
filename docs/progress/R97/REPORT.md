# R97：aarch64 编译参数在两套 project_config 中的定义核实

## 结论

**核心问题的明确回答：使用当前 LLVM Toolchain 配置后，aarch64 环境
不会再注入 Clang 22.1.8 不接受的复合写法
`-mtune=cortex-a57.cortex-a53`。**

当前 GCC 配置把该参数定义在 `Tizen-Base/project_config:264`；当前 LLVM
配置在 `Tizen-Base-Toolchain/project_config:332` 使用
`-mtune=cortex-a53`。Clang 22.1.8 的逐参数实测确认前者退出 1，后者退出
0；LLVM/aarch64 的完整有效编译参数组也退出 0。实际历史构建日志进一步
确认 LLVM 配置曾以该参数组进入 CMake，并由 Clang 22.1.8 完成编译器检测。

因此，就本次实际检查的配置版本与 Clang 22.1.8 而言，人工提出的事实判断
成立：该冲突出现在“使用 GCC Base 的参数集合、但单个包强制改用 Clang”
的混合情形；LLVM Toolchain 的 aarch64 参数集合没有复现该冲突。

本报告只陈述查证结果，不对包级或平台级配置该如何修改提出建议。

## 1. 查证口径

R96 的两个 GBS profile 各组合 Base 与 Unified 仓：

- GCC 组：`Tizen-Base` + `Tizen-Unified`；
- LLVM 组：`Tizen-Base-Toolchain` + `Tizen-Unified-Toolchain`。

因此本任务检查了四个物理文件，而不是把“两套”误读成任意两个文件：

1. `codes/qb/TIZEN/Tizen/Tizen-Base/project_config`；
2. `codes/qb/TIZEN/Tizen/Tizen-Unified/project_config`；
3. `codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config`；
4. `codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config`。

两份 Unified 文件自身没有 `Optflags:` 行，末尾分别写明 derived from
`Tizen:Base`。实际构建日志中的最终 CFLAGS 与相应 Base 文件的定义逐项
吻合，故参数来源定位到两个 Base 文件；没有仅凭目录名推断继承结果。

配置仓 HEAD 为 `63d0d4ace65a45ac6da21b1577346c1d6cace7a0`。四文件
SHA256 与任务开始时一致，`git diff` 和 `git status` 均为空，见
`raw/019_source_integrity_and_scope_audit.*`。

## 2. 参数来源

### 2.1 GCC 配置

公共参数定义于：

`codes/qb/TIZEN/Tizen/Tizen-Base/project_config:260`

```text
%define __global_cflags -O2 -fno-inline-functions -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong -Wformat-security -fmessage-length=0 -frecord-gcc-switches -Wl,-z,relro,--as-needed
```

aarch64 参数定义于同文件第 264 行：

```text
Optflags: aarch64 %{__global_cflags} -feliminate-unused-debug-types -Wformat -march=armv8-a+fp+simd+crc+crypto -mtune=cortex-a57.cortex-a53
```

这是架构专用 `Optflags: aarch64` 行，只对 aarch64 生效。该行在公共参数
之外增加：

- `-feliminate-unused-debug-types`；
- `-Wformat`；
- `-march=armv8-a+fp+simd+crc+crypto`；
- `-mtune=cortex-a57.cortex-a53`。

其中复合 CPU 名是本次唯一经 Clang 22.1.8 实测拒绝的参数。配置中另有
GCC 取向的参数：`-feliminate-unused-debug-types` 被 Clang 接受但报告未
使用；公共的 `-frecord-gcc-switches` 被 Clang 接受；aarch64 行没有
`--param=...`。

`Tizen-Unified/project_config` 不含 Optflags，在第 180 行标注
`# derived from Tizen:Base`，没有覆盖上述 aarch64 定义。

### 2.2 LLVM Toolchain 配置

编译器及按工具链选择的参数机制位于
`Tizen-Base-Toolchain/project_config:118-140`：

```text
%__cc_clang %{_host}-clang
%__cxx_clang %{_host}-clang++
%__optflags_toolchain_clang -Os -fstack-protector -Wno-unused-command-line-argument -Wno-error=unused-but-set-variable -Wno-error=unused-command-line-argument %{?___optflags_toolchain_clang:%{___optflags_toolchain_clang}}
...
%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}
...
%__optflags_toolchain %{expand:%%{__optflags_toolchain_%{_toolchain}}}
```

也就是说，该配置默认 `_toolchain=clang`，有效公共编译参数走
`__optflags_toolchain_clang`，不是同文件中保留的 GCC 分支。

公共参数组合位于第 328 行：

```text
%define __optflags %{__optflags_toolchain} -g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat -Wformat-security -fmessage-length=0 -frecord-gcc-switches
```

aarch64 定义位于第 332 行：

```text
Optflags: aarch64 %{__optflags} -march=armv8-a+fp+simd+crc+crypto -mtune=cortex-a53
```

此处没有复合 `cortex-a57.cortex-a53`，没有
`-feliminate-unused-debug-types` 进入默认 Clang 路径。文件第 108、131 行
虽定义了 GCC 分支所用的相关宏，但默认 Clang 选择不会消费它们。

`Tizen-Unified-Toolchain/project_config` 不含 Optflags，在第 160 行标注
`# derived from Tizen:Base`，没有覆盖 Toolchain Base 的 aarch64 定义。

对两个 Toolchain 文件进行精确检索，复合 `-mtune` 为零命中，见
`raw/019_source_integrity_and_scope_audit.stdout`。

## 3. aarch64 两套参数逐项对照

| 类别 | GCC Base | LLVM Base-Toolchain |
| --- | --- | --- |
| 优化 | `-O2 -fno-inline-functions` | `-Os` |
| 栈保护 | `-fstack-protector-strong` | `-fstack-protector` |
| Clang 诊断兼容 | 无 | 三个 unused 相关抑制参数 |
| 共同项 | `-g2 -gdwarf-4 -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -Wformat-security -fmessage-length=0 -frecord-gcc-switches` | 同左，且公共层含 `-Wformat` |
| 链接参数位置 | `-Wl,-z,relro,--as-needed` 混在 Optflags | 不在 Optflags；第 348 行单独定义于 `build_ldflags` |
| 未使用调试类型 | `-feliminate-unused-debug-types` | 默认 Clang 路径无 |
| 架构 ISA | `-march=armv8-a+fp+simd+crc+crypto` | 相同 |
| CPU 调优 | `-mtune=cortex-a57.cortex-a53` | `-mtune=cortex-a53` |

机器可读版本见 `AARCH64_CONFIG_COMPARISON.tsv`。

实际日志交叉核对：

- R96 GCC/aarch64 的最终 CFLAGS 含复合 `-mtune`，并在 CMake 编译器测试
  中被 Clang 22.1.8 拒绝；
- 历史 LLVM/aarch64 完整构建的最终 CFLAGS 含单核
  `-mtune=cortex-a53`，C/C++ 编译器识别为 Clang 22.1.8 并通过检测。

原文摘录见 `raw/018_correlate_config_with_actual_build_logs.*`。

## 4. 其他架构

### 4.1 armv7l

GCC Base 第 262 行和 LLVM Base-Toolchain 第 330 行都使用单核
`-mtune=cortex-a8`，并共同使用 ARMv7/NEON/softfp/thumb 相关参数。GCC
配置额外含 `--param=ssp-buffer-size=4`、三项 `-Wl,...` 和
`-Wa,-mimplicit-it=thumb`；LLVM 配置不含这些 GCC Base 附加项。

Clang 22.1.8 逐项探针中没有拒绝项。GCC 整组仅因 `-c` 模式下的三项
链接参数产生“linker input unused”诊断，退出码仍为 0；LLVM 整组无诊断、
退出码 0。更强的现成证据是 R96 两格 armv7l 均使用对应的完整实际参数
完成了整个 libc++ RPM 构建。

### 4.2 x86_64

两套都使用 `-m64 -march=nehalem -msse4.2 -mfpmath=sse` 等参数，没有复合
`-mtune`。GCC Base 还含 `--param=ssp-buffer-size=4` 和
`-feliminate-unused-debug-types`；LLVM x86_64 Clang 路径含
`-momit-leaf-frame-pointer` 与两项 MLGO `-mllvm=...` 参数。

Clang 22.1.8 逐项探针没有拒绝项。GCC 整组退出 0，但编译模式下
`-Wl,...` 和 `-feliminate-unused-debug-types` 报未使用诊断；LLVM 整组
退出 0 且无诊断。

### 4.3 aarch64 为何不同

三架构的实测差异集中为一项：只有 GCC Base 的 aarch64 行把两个 CPU 名
用点号合成 `cortex-a57.cortex-a53`。armv7l 使用单一 `cortex-a8`，
x86_64 没有该复合形式。Clang 22.1.8 接受后二者的完整参数组，但明确拒绝
前者，故 aarch64 单独失败。

## 5. Clang 低成本实测

### 方法

使用现有构建根中的真实目标 Clang 22.1.8，对每个配置参数单独执行一次
最小 C 编译，再以完整有效参数组执行一次。armv7l/aarch64 通过构建根内
QEMU 运行目标编译器；x86_64 直接运行目标前缀编译器。未修改任何配置，
输出目标为 `/dev/null`。

aarch64 的 R96 失败构建根经其动态加载器运行编译器时发生退出 139，故
隔离逐项探针使用此前可正常运行、同版本同目标三元组的 LLVM aarch64
构建根。该探针对 GCC 参数集合的复合 `-mtune` 得到的错误文本，与 R96
实际 GCC 环境构建中的错误逐字一致。有关尝试和限定见 `raw/008_*` 至
`raw/010_*`。

### 结果

| 架构 | 配置集合 | 逐项拒绝 | 整组 |
| --- | --- | --- | --- |
| aarch64 | GCC | `-mtune=cortex-a57.cortex-a53` | REJECTED |
| aarch64 | LLVM | 无 | ACCEPTED |
| armv7l | GCC | 无 | ACCEPTED_WITH_DIAGNOSTIC |
| armv7l | LLVM | 无 | ACCEPTED |
| x86_64 | GCC | 无 | ACCEPTED_WITH_DIAGNOSTIC |
| x86_64 | LLVM | 无 | ACCEPTED |

完整逐项命令和诊断在 `raw/011_*` 至 `raw/016_*`；汇总见 `raw/017_*` 和
`CLANG_PROBE_SUMMARY.tsv`。

`ACCEPTED_WITH_DIAGNOSTIC` 不表示参数全部发挥作用：例如链接参数在
compile-only 探针中不会被消费，`-feliminate-unused-debug-types` 被当前
Clang 报告为未使用。本分类只回答 driver 是否拒绝及最小编译是否返回
非零；其中 armv7l 另有 R96 完整构建证据。

## 6. 核心判断的范围

已观测事实：

1. 当前 GCC Base 配置含且实际注入复合 `-mtune`；
2. 当前 LLVM Toolchain 配置不含该形式，使用 Clang 可接受的单核调优值；
3. 当前 LLVM/aarch64 完整有效参数组通过 Clang 22.1.8 独立编译探针；
4. 历史实际 LLVM/aarch64 构建日志也确认相同参数进入 Clang 22.1.8 并
   通过 CMake 编译器检测。

据此，核心判断在当前配置快照上成立。没有把该结论外推到未来配置版本、
未检查的派生项目（ASAN、SC、Dev 等）或其他 Clang 版本。

## 7. 自行判断、疑问与纪律确认

自行判断：将“两套配置”按 R96 实际仓组合解释为两个配置集合，因而同时
检查四个物理文件；参数接受性探针覆盖公共参数和架构附加参数，而不只测
两种 `-mtune`。

尚存疑问：本地仓没有 QuickBuild 服务端的项目继承元数据；Unified 的
继承关系在文件中仅以注释出现。不过 R96/历史构建的最终 CFLAGS 与对应
Base 定义一致，参数来源和核心结论不依赖未观测的服务端细节。

纪律确认：未修改任何 `project_config`，`codes/qb` 工作树干净；未修改
平台源码，R96 候选 spec SHA256 仍为
`f17402b373a9d74841faf28e364b23c48bc54771a5ff8e96173c5a530b0d350e`；
未推送 Gerrit；本报告不作处置建议。
