# R90：R87 扫描器的阳性校准

日期：2026-09-01（Asia/Shanghai）

## 1. 结论

**R87 的三个原扫描器均能检出本次构造的全部阳性样本。**

- 二进制阳性：6 种独立 ABI 形态；分别放入 ELF 可重定位目标文件、共享库、
  可执行 ELF 和静态归档。
- 源码阳性：10 个预期判定分支。
- 自动核对：42 个“载体 × 阳性形态/判定分支”检查，42 个通过，0 个漏检。
- `scan_elf_symbols.py`：3/3 ELF 解析成功，五种二进制判定分支全部触发。
- `scan_static_archives.py`：1/1 归档扫描成功，五种二进制判定分支全部触发。
- `scan_noexcept_type_uses.py`：1/1 校准源码根扫描成功，D01、D02–D04、
  D05 与 GENERAL 的预期分支全部触发。

因此，R87 在其**已申报覆盖范围内**的零命中不能解释为“扫描器根本找不到
这些形态”；阳性校准建立了其判定路径的检出能力。R87 的原结论——在其已
覆盖语料中未发现目标用法——成立，证明力得到补足。

这不把结论扩大到未覆盖源码、宏/生成源码、被剥离且不留相关符号的代码，
也不修改 R87 的任何既有材料。

## 2. 原脚本身份与不修改证明

直接调用 `docs/progress/R87/code/` 中的三个既有文件；调用前后 SHA256
一致：

| 文件 | SHA256 |
|---|---|
| `scan_elf_symbols.py` | `dbcc1c1bfa21b6d277a8c5186091ad93dc67cef40d571736a75111e963e513f2` |
| `scan_static_archives.py` | `efc4af747c1ad0de2f6f2955a040cb5516f62f15a94aa2d142d3a6bafd6f917b` |
| `scan_noexcept_type_uses.py` | `e81bd6f7afec0fa932430819f3f2f292c4f0bdc1230a9e8efeba9ebdb92629a9` |

没有复制后修改判定逻辑。适配仅包括：

1. 为 ELF 扫描器制作一条与 R87 原接口相同的 shard JSON，并按
   `extracted/<prefix>/<id>/...` 布置三个 ELF；
2. 为静态归档扫描器提供一个单行绝对路径清单；
3. 为源码扫描器提供一个只有一个 `SCAN_OK` 根的 `scan_status.tsv`。

## 3. 阳性样本设计

### 3.1 二进制 ABI 样本

源码为 `samples/positive_symbols.cpp`。它是隔离的 ABI 探针：在最小
`std` 命名空间声明中固定目标类名和 `noexcept` 类型，不依赖宿主
libstdc++ 当前是否已移除某个标注，也不与平台源码或 libc++ 链接。

| 编号 | 构造 | 预期 mangled 形态 | 要触发的 R87 判定 |
|---|---|---|---|
| B01 | `&condition_variable::wait` 作为 `auto` 参数，同时把其成员函数指针类型作为深层模板类型参数 | 目标名 `condition_variable4wait` 与成员指针 `M...DoF` 同符号 | `SITE_WITH_NOEXCEPT_TYPE_ENCODING`、`SITE_WITH_POINTER_NOEXCEPT_ENCODING` |
| B02 | `&condition_variable::__do_timed_wait` 与其类型进入四层模板 | `condition_variable15__do_timed_wait` + `M...DoF` | 同上；校准 D02–D04 共用的扫描分支 |
| B03 | 只把 `wait` 的成员函数指针类型作为模板类型参数 | 只有 `condition_variable` 类名和 `DoF`，没有 `wait` | `CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE` |
| B04 | `noexcept` 成员函数指针作为普通函数参数 | `MSt18condition_variableDoF...`，没有目标函数名 | `CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE` |
| B05 | 深层模板中放入 `wbuffer_convert` 及 `void (*)(wbuffer_convert&) noexcept` | `wbuffer_convert...PDoF`，没有析构名 | `WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE` |
| B06 | `wbuffer_convert` 析构的 `noexcept(...)` 结果作为 bool 模板参数 | 同一符号含 `wbuffer_convert` 和 `Lb1E` | `WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE` |

B01/B02 中 `template<auto P>` 只负责把真实目标函数名带入符号；`DoF` 来自
另一个显式类型模板参数。这与 R87 已查明的“单独 `template<auto P>` 不编码
异常规格”不矛盾。

B02 使用一个结构等价的 `__do_timed_wait` 声明校准原扫描器的共用名字/类型
分支。原扫描器本身不区分 system_clock、steady_clock 和任意 Clock 的签名，
所以这里不是对三种 overload 语义或可达性的重新验证。

### 3.2 源码样本

`samples/source_positive.cpp` 使用宿主标准头并实编译通过；另一个 ABI 探针
源码也进入同一源码根。覆盖：

- D01：显式地址、成员指针类型、`decltype`/trait、别名地址；
- D02–D04：`__do_timed_wait` 私有 helper 引用；
- D05：析构拼写、`noexcept` 和 `is_nothrow_destructible`；
- GENERAL：宽地址、宽类型查询、`noexcept(cv.wait(lock))`。

### 3.3 载体

同一 ABI 探针以 `g++ -std=c++20 -O0 -g` 构建为：

| 载体 | 类型 | SHA256 |
|---|---|---|
| `positive_symbols.o` | ELF64 x86-64 relocatable | `c1e08bb1b8e8b9123605d4f9e1bbcd58ee865ba0e3f7b7d2aaeb55976359489a` |
| `libpositive_symbols.so` | ELF64 x86-64 shared object | `e96d770965d6eeaa084253411fcf12df03ed3c903c34ec5ada13111f4c9e1106` |
| `positive_symbols.elf` | ELF64 x86-64 PIE executable | `d6f6f8fb9186aaef652af0a1fc0348d09272d3cb7079e017c3f32533f1dda51f` |
| `libpositive_symbols.a` | ar static archive，含上述 `.o` | `efff566ff99ea5fbdfb1a766a111b070b8fb8fe66fdfcdffcc5daa43d9252fdb` |

本次是判定逻辑阳性校准，只生成宿主 x86_64 样本。Itanium ABI 的 `Do`/`PDo`
编码不随上述三个平台架构改变；R87 原扫描已经实际解析 armv7l ELF32、
aarch64/x86_64 ELF64。本次没有另行构造 ARM 阳性二进制。

## 4. 独立阳性确认

在运行 R87 扫描器之前，使用 GNU `nm --no-demangle`、`nm -C`、
`readelf --wide --symbols`、`ar t` 和 `c++filt` 直接检查目标文件、共享库和
归档。这条确认路径不调用 R87 的 ELF parser 或 `classify()`。

六个实际阳性符号如下（省略仅在说明文字中的换行，不截短原符号）：

```text
_Z14deep_real_siteIXadL_ZNSt18condition_variable4waitERiEEMS0_DoFvS1_E10deep_layerILi11ES4_ILi12ES4_ILi13ES4_ILi14EiEEEEEiT1_T0_
_Z14deep_real_siteIXadL_ZNSt18condition_variable15__do_timed_waitERlEEMS0_DoFvS1_E10deep_layerILi21ES4_ILi22ES4_ILi23ES4_ILi24ElEEEEEiT1_T0_
_Z19condition_type_onlyIMSt18condition_variableDoFvRiE10deep_layerILi7ES4_ILi8ES4_ILi9EiEEEEiT0_T_
_Z27condition_pointer_parameterMSt18condition_variableDoFvRiE
_Z17wbuffer_type_onlyISt15wbuffer_convertIcwEPDoFvRS1_E10deep_layerILi31ES5_ILi32ES5_ILi33ElEEEEiT1_T0_
_Z23wbuffer_destructor_boolISt15wbuffer_convertIcwELb1E10deep_layerILi41ES2_ILi42ES2_ILi43ES2_ILi44EcEEEEEiT1_
```

`c++filt` 独立显示：B01/B02 是目标地址加 `noexcept` 成员函数指针；B03/B04
是没有方法名的 `noexcept` 成员函数指针；B05 是 `noexcept` 普通函数指针；
B06 的布尔值为 `true`。`readelf` 在共享库及归档成员中均看到相同符号，
`ar t` 确认归档成员为 `positive_symbols.o`。完整原文见 `raw/006_*`。

## 5. 原脚本扫描结果

### 5.1 ELF

```text
json_package_records=1
elf_records=3
unique_elf_paths=3
mangled_symbols_scanned=32
site_symbol_rows=72
site_with_noexcept_type_encoding_rows=12
site_with_pointer_noexcept_encoding_rows=12
condition_variable_noexcept_function_type_rows=16
wbuffer_convert_noexcept_function_type_rows=4
wbuffer_convert_bool_template_candidate_rows=4
parse_failures=0
```

共享库同时从 `.dynsym` 与 `.symtab` 各命中一次，因此部分检查的匹配行数为
2；这不是两个独立样本。目标文件和可执行文件各为 1。

### 5.2 静态归档

```text
archive_paths=1
mangled_symbols_scanned=8
candidate_rows=18
SITE_WITH_NOEXCEPT_TYPE_ENCODING=3
SITE_WITH_POINTER_NOEXCEPT_ENCODING=3
CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE=4
WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE=1
WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE=1
scan_failures=0
```

### 5.3 源码

最终重扫：1 个根、32 行候选、失败 0；预期的 10 类源码检查均至少命中
一行。具体文件与行号见 `tables/source_candidates.tsv`。

### 5.4 逐项矩阵

完整 42 行见 `tables/calibration_matrix.tsv`。压缩展示如下：

| 样本 | `.o` | `.so` | 可执行 ELF | `.a` | 源码 |
|---|---:|---:|---:|---:|---:|
| B01 真实 wait 名 + `DoF` | PASS | PASS | PASS | PASS | 对应 S01/S03 PASS |
| B02 helper 名 + `DoF` | PASS | PASS | PASS | PASS | S05 PASS |
| B03 condition type-only 深层模板 | PASS | PASS | PASS | PASS | S02 PASS |
| B04 成员指针函数参数 | PASS | PASS | PASS | PASS | S02 PASS |
| B05 wbuffer type-only 深层模板 | PASS | PASS | PASS | PASS | S07 PASS |
| B06 析构 trait bool 模板参数 | PASS | PASS | PASS | PASS | S06/S07 PASS |
| 别名/GENERAL 源码分支 | 不适用 | 不适用 | 不适用 | 不适用 | S04/S08–S10 PASS |

没有漏检，因此不存在需要修正脚本并重扫 R87 语料的校准失败。

## 6. 1,079 项源码盲区评估

### 6.1 口径更正但不修改 R87

`1,079` 是报告中的算术差 `1,450 − 371`。逐 SRPM 文件名核对发现，R87
扫描的 R32/T1 语料与 R11 冻结快照不是严格子集：

| 指标 | 数量 |
|---|---:|
| R11 冻结源码记录 | 1,450 |
| R87 `SCAN_OK` 记录 | 371 |
| 报告的算术差 | 1,079 |
| R87 与 R11 文件名精确相同 | 345 |
| R87 已扫但版本/Release 不属于 R11 快照 | 26 |
| R11 快照真正未扫记录 | 1,105 |

26 条主要是 binutils、cmake、gcc、gettext、jsoncpp、llvm 等版本/Release
漂移，逐项映射见 `tables/source_gap_snapshot_drift.tsv`。这不推翻 371 个根的
扫描结果，但意味着若目标是补齐 R11 的 1,450 条冻结记录，应以 1,105 为
待处理分母。

### 6.2 是否能在合理成本内覆盖

**可以覆盖，但应视为一次有边界、分批、I/O 主导的独立工作，不是一次
轻量追加。**依据：

- R11 的 1,450 个 SRPM 已全部下载并通过 SHA256；本机逐 `target` 检查为
  1,450/1,450 存在，不需要再次下载。
- 1,105 个精确缺口的压缩 SRPM 总量为 `15,623,924,553` bytes；资源门禁时
  可用磁盘约 357 GiB。
- 已有 `analyze_dlopen_sources.py` 提供安全展开普通 tar/zip 的实现；系统有
  `rpm2cpio`、`rpm2archive`、`cpio`、`tar`、`7z`。
- 可以逐包执行“校验 → 临时展开 SRPM → 安全展开内嵌源码归档 → 原脚本
  单 worker 扫描 → 保存候选/状态 → 释放该批临时目录”，从而避免同时保留
  1,105 个完整展开树。

限制：未实际展开这 1,105 包，展开后峰值和总 I/O 为 `NOT_OBSERVED`。
`chromium-efl` 单个 SRPM 为 7,223,059,319 bytes，且 R11 已证明传统
`rpm2cpio` 会遇到 cpio 的 >4 GiB 限制；它必须单独走 `rpm2archive` 等可处理
大文件的路径并复核完整性。其余缺口压缩量约 8.40 GB，操作成本明显较低。

### 6.3 盲区影响与命中可能性

用 R11 已发布 ELF 的操作性 C++ 指标映射回 source RPM：

| 集合 | 记录数 | 至少一个 C++ 指标 ELF | 比例 |
|---|---:|---:|---:|
| 与 R11 精确相同的已扫记录 | 345 | 343 | 99.42% |
| R11 精确未扫记录 | 1,105 | 140 | 12.67% |

因此未覆盖集合整体**更不可能**包含目标用法：R32/T1 的 371 条本来就是为
C++/取消风险调查筛出的高富集语料，而余集大量是纯 C、脚本、数据、内核或
没有 C++ 指标运行时 ELF 的包。

但盲区不能忽略：

1. 仍有 140 个未扫 source RPM 已知产出 C++ 指标 ELF；其中包括
   `chromium-efl`、Rust、dotnet build tools、LLVM 和多个 GCC 交叉工具链。
2. “没有 C++ 指标 ELF”不等于源码没有 C++：可能只产生静态库、构建工具、
   测试或生成源码，也可能相关符号被剥离/内联。
3. R87 的二进制层已经全量覆盖这些包的已发布 ELF/静态归档，显著降低了
   ABI 可见用法遗漏的概率；但 bool trait、SFINAE/overload 选择等结果可能
   不携带目标名字，仍主要依赖源码层。

所以该盲区对“已覆盖语料零命中”没有影响，对“平台 1,450 个源码包的穷尽
零命中”仍有实质影响。完整平台源码结论需要按 R11 快照补扫 1,105 条。

## 7. 自行判断、限制、异常与尚存疑问

自行判断：

1. 阳性校准按原扫描器的判定分支设计，而不是重复验证 D02/D03/D04 三种
   overload 的运行时可达性；三者在扫描器中本来共用同一 helper 名分支。
2. 用隔离最小声明固定 ABI 形态，避免宿主 libstdc++ 的标注状态污染样本；
   另提供一份使用真实宿主头且编译通过的源码层样本。
3. 把 1,079 作为报告算术口径保留，同时给出严格快照交集得到的 1,105；
   没有回写 R87。
4. “更不可能”只依据已发布 ELF 的 C++ 指标富集度，不把 965 条无指标记录
   判成不可能。

限制：

- 阳性 ABI 载体只在 x86_64 生成；没有另造 ARM ELF 阳性。
- 本次不扩扫 1,105 个缺口，解压峰值、总耗时和 chromium 特殊路径成功率均
  为 `NOT_OBSERVED`。
- 阳性校准证明这些已知形态能被发现，不证明词法扫描可以发现任意宏展开、
  生成代码或 ABI 名中已完全擦除目标来源的语义变化。

执行异常（均已保留原始记录）：

- 初始误用宽 `find` 引发一次只读目录遍历等待，按精确 PID 终止；资源门禁
  仍通过，未改文件。
- `source_positive.cpp` 首次编译缺少 `<codecvt>`，只修正校准样本后重编通过；
  未改扫描器。
- 首次盲区量化因发现 371 并非 R11 的严格子集而主动退出，随后同时输出
  算术口径与严格快照口径。
- 一次只读 `du -sh tmp/R11` 产生不必要 I/O，主动终止；改用下载状态表逐
  target `stat` 和声明大小完成评估。
- 两次辅助清单查看分别因 `printf` 以 `---` 开头的格式串和 `rg` 无匹配配合
  `pipefail` 自身退出；均未改变数据，后续 `016`/`023` 用收敛命令取得所需
  证据。原非零退出码保留。
- `py_compile` 曾在 R87 的代码目录生成三个未跟踪 `__pycache__` 文件；最终
  审计发现后精确删除。三个既有脚本内容和全部 R87 已跟踪材料始终未改，
  最终 `git status -- docs/progress/R87` 为空。

尚存疑问：平台未来希望补扫的是 R11 冻结快照 1,450 条，还是当前更新后的
源码仓快照？两者会改变精确分母和 26 条版本漂移的处理，但不影响本次扫描器
阳性校准结论。

## 8. 证据索引

- 资源门禁：`raw/001_*`
- R87 脚本、接口与原调用复核：`raw/003_*`、`raw/004_*`
- 样本构建与独立 `nm/readelf/c++filt` 确认：`raw/005_*`—`raw/007_*`
- 原 ELF/静态/源码扫描器校准：`raw/008_*`—`raw/014_*`
- 盲区语料、精确分母、成本与工具评估：`raw/015_*`—`raw/023_*`
- 最终校验与清除校准产生的未跟踪 pycache：`raw/024_*`、`raw/025_*`
- 材料归档、交付审计与 R90 自身缓存清理：`raw/026_*`—`raw/028_*`
- 全部逐项结果：`tables/calibration_matrix.tsv`
- 原扫描器完整输出：`tables/elf_candidates.tsv`、`static_candidates.tsv`、
  `source_candidates.tsv` 及各自 summary
- 盲区量化：`tables/source_gap_metrics.tsv`、`source_gap_largest.tsv`、
  `source_gap_snapshot_drift.tsv`

## 9. 边界声明

本任务未修改 R87 材料或脚本，未修改平台源码，未推送 Gerrit，未实施
1,105 包扩扫。
