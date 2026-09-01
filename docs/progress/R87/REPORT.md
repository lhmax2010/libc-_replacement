# R87：平台对 D01–D05 的函数地址与异常规格类型使用查证

日期：2026-09-01
状态：`COMPLETED_WITH_DECLARED_BLIND_SPOTS`
边界：只读既有源码/RPM 语料；只新增 R87 查证脚本、最小 ABI 探针与报告材料；未修改平台源码、补丁或配置，未向 Gerrit/LLVM/其他外部源码仓推送。

## 1. 结论

在本次实际覆盖的语料中，**没有观察到平台代码以会让 D01–D05 的异常规格进入符号名的函数指针、成员函数指针、模板类型参数、`decltype`/type-trait 等形态使用这五处函数**。

- 源码：R32/T1 已解包的 371 个 source RPM record（347 个去重包名）精确扫描全部完成；31 个词法候选逐条复核后，有效目标使用为 0。
- 已发布 ELF：R11 冻结 Base+Unified 的 11,332 个二进制包记录中，39,166/39,166 个 ELF 均完成 `.symtab`/`.dynsym` 扫描；约 1,089 万条 mangled 符号中，目标函数名型 `Do/PDo`、只含 `condition_variable` 类名的 type-only `DoF`、`wbuffer_convert` 的 `DoF`/布尔模板候选均为 0。
- 静态归档：枚举 8,911 个 `.a` 路径（4,531,706,815 bytes），`nm` 读取约 494 万个 mangled 符号；目标类型编码为 0。15 个路径有严重诊断，逐条查明为 8 个 Go 编译器测试对象、4 个缺成员的内核 thin archive、3 个实际为 GNU ld 文本脚本的 `libm.a`，不是观察到的 C++ D01–D05 使用。

这里的“没有”严格表示 `NOT_OBSERVED_IN_COVERED_CORPORA`，不是对平台外源码、未纳入快照的包、闭源/预置二进制或已剥离局部符号的绝对不存在证明。

## 2. 查找的形态

### 2.1 直接形态

1. `&std::condition_variable::wait`，包括通过 cast/上下文消除重载后的地址；
2. `&...::__do_timed_wait` 及该私有 helper 的引用；
3. 存入普通/成员函数指针变量，或作为函数参数/返回类型的一部分；
4. 作为模板类型参数，或函数地址作为非类型模板实参；
5. `decltype(&...)`、`noexcept(...)`、`is_nothrow_invocable`、`is_invocable`、`invoke_result`、`result_of`、析构 traits 等；
6. `using`/`typedef` 到 `condition_variable` 后的 `&Alias::wait`；
7. 宽检索中的任意 `&SomeType::wait`，再人工判断 `SomeType` 是否为目标。

D02–D04 是 libc++ 私有成员，正常外部源码还受访问控制限制；仍检索了全部名字引用，以免遗漏库内、测试或绕过访问限制的代码。

D05 的析构函数不能被直接取址。最小探针 `&W::~W` 被 GCC 拒绝并报告 `taking address of destructor`。因此 D05 的有效风险形态主要是析构 `noexcept(...)`、`is_nothrow_destructible`、SFINAE/overload/template 选择等类型依赖，而不是“析构函数指针”。

### 2.2 还会间接改变符号/实例的形态

R87 最小探针对 R86 的区分作了补充：

| 形态 | 实测 |
| --- | --- |
| 成员函数自身 | 有/无顶层 `noexcept` 都是 `_ZN4Site4waitERi`，不变 |
| 成员函数指针作为函数参数 | `M4SiteDoFvRiE` 对 `M4SiteFvRiE`，变化 |
| 成员函数指针类型作为类型模板实参 | `IM4SiteDoFvRiEE` 对 `IM4SiteFvRiEE`，变化 |
| `template<auto P>` 中传入 `&Site::wait` | 本探针两侧均编码引用函数自身的同一名字，不变 |
| `noexcept(expr)`/析构 trait 结果进入布尔模板实参 | `Lb1E` 对 `Lb0E`，变化 |

因此，除了显式函数指针，以下因素也可能导致符号/实例变化：

- `decltype(&function)` 或别名最终流入函数参数类型、类型模板实参等 Itanium ABI 会编码的上下文；
- `noexcept(expr)`、`is_nothrow_invocable`、`is_nothrow_destructible` 等结果进入布尔非类型模板参数；
- 异常规格变化改变 SFINAE、约束或 overload resolution，间接选择另一个模板实例/函数。

限制：`decltype`/trait 自身没有外部符号；只有当结果流入上述编码或选择上下文时才可见。普通全局变量名通常不编码其变量类型。函数返回类型是否进入名字取决于具体 ABI/模板上下文，不能把“含函数指针返回类型”一概等同为必然改名。

## 3. 源码查证

### 3.1 语料与方法

- 输入：`docs/progress/R32/tables/scan_status.tsv` 中 `SCAN_OK` 的 371/371 个现有解包根；347 个去重包名。
- 上游分母边界：T1 原有 372 个候选 source RPM，`chromium-efl` 因既有解包失败不在这 371 个扫描分母内。
- 精确扫描：常见 C/C++ 源码与头文件后缀，`rg --hidden --no-ignore --threads 1`；查直接地址、成员指针类型、D02–D04 名字、D05 析构/trait 形态。371/371 完成，失败 0。
- 补充宽扫描：加入别名、任意 `&...::wait`、宽 trait/noexcept 形态，单 worker、每包 60 秒。366/371 完成；5 个超大包超时。它是补充口径，不替代已完成的精确扫描。
- 额外后缀：`.mm/.m/.cu/.cuh/.hip/.sycl/.i` 单路补查，结果 `NO_MATCH`。
- 方法性质：词法扫描，不是完整 build-aware AST；不能解析任意宏拼接、生成源码、条件编译后才出现的 token 或跨文件任意别名传播。

补充宽扫的 5 个超时包为：

1. `emulator-kernel-6.18.2.tizen-0.src.rpm`；
2. `gcc-14.2.0-1.14.src.rpm`；
3. `llvm-22.1.8-18.1.src.rpm`；
4. `rpi4-linux-kernel-6.12.80-0.src.rpm`；
5. `tensorflow2-2.18.0-1.3.src.rpm`。

这些包的精确目标扫描均完成；未完整的是别名/宽 trait 的补充口径。

### 3.2 候选逐项结论

完整文件、行号和源码原文在：

- `tables/source_candidates_no_ignore.tsv`；
- `tables/source_candidates_expanded.tsv`；
- `tables/source_candidate_classification.tsv`。

归类如下：

| 候选 | 行 | 结论 |
| --- | --- | --- |
| GCC 两个 source RPM 的 `compatibility-condvar.cc` | 各 73/75/77/80/81 | `std::__condvar std::condition_variable::*` 是 `__condvar` **数据成员**指针，不是 `wait` 成员函数指针 |
| LLVM libc++ 主/C++03 condition-variable 头及 `condition_variable.cpp` | 17 处 | 全部是 D02–D04 的声明、定义或直接调用；无取址、指针变量、模板参数、`decltype`/trait |
| GCC/LLVM 的 `wbuffer_convert` | 4 处 | 析构声明/定义本身；无 trait/type-query 使用 |
| Boost.MPI Python binding | `py_request.cpp:94` | `&cl::wait`，`cl` 是 Boost.MPI request wrapper，不是 D01–D05 |
| Hailo Python binding | `infer_model_api.cpp:475` | `&AsyncInferJobWrapper::wait`，不是 D01–D05 |

有效命中清单：**空**。

## 4. 二进制查证

### 4.1 已发布 ELF

输入是 R11 已下载、校验并解包的冻结 Base+Unified RPM：

| 架构 | 二进制包记录 |
| --- | ---: |
| aarch64 | 3,756 |
| armv7l | 3,692 |
| x86_64 | 3,609 |
| noarch | 261 |
| i586 | 5 |
| riscv64 | 9 |
| 合计 | 11,332 |

方法：逐 ELF 读取 `.symtab` 和 `.dynsym`，保留 `_Z` 名字并以 `c++filt` 反解；同一文件/符号表内去重。除含具体函数名的形式外，还查：

- 仅含 `condition_variable` 类名与 `DoF` 的 type-only 成员函数指针类型；
- `wbuffer_convert` 与 `DoF`；
- `wbuffer_convert` 与 `Lb0E/Lb1E` 的 trait 布尔候选。

最终结果：

| 指标 | 数量 |
| --- | ---: |
| ELF records / 成功解析 | 39,166 / 39,166 |
| 扫描的 mangled symbol records | 10,889,742 |
| 含普通目标站点名字的记录 | 661 |
| 具体函数名 + noexcept type encoding | 0 |
| 具体函数名 + pointer noexcept encoding | 0 |
| `condition_variable` type-only `DoF` | 0 |
| `wbuffer_convert` `DoF` | 0 |
| `wbuffer_convert` 布尔模板候选 | 0 |

661 条普通记录是正常的 `condition_variable::wait` 定义/引用/模板 wait 符号，没有目标函数指针类型编码，不能算 R87 命中。

首次 ELF 扫描有一个 U-Boot 镜像使用扩展 section count；扫描脚本只修正了该 ELF 路径解析，并单独重解析成功。扩大 type-only 过滤后从头重扫，最终 `parse_failures=0`。首次失败记录未删除。

### 4.2 静态 `.a`

- `.a` 路径：8,911；总大小 4,531,706,815 bytes。
- `nm -A --no-demangle`，每批 20、单进程低优先级；失败批次再逐归档复核。
- 扫描 mangled symbol records：4,937,655。
- 普通站点候选：154；均为直接 wait 定义/引用。
- 所有目标 `Do/PDo`、type-only `condition_variable DoF`、`wbuffer_convert DoF/布尔候选`：0。

15 个严重诊断路径：

- 8 个 `/usr/lib/golang/.../gcimporter/testdata/versions/*.a`：Go 编译器版本测试对象；文件头实见 `go object darwin amd64` 或 Go 专用成员；
- 4 个 RubikPi3 kernel-devel `built-in.a`：thin archive 引用的成员不在解包树，`nm` 报 `No such file or directory`；
- 3 个 `libm.a`：`file` 识别为 ASCII，内容是 `GROUP ( ...libm-2.40.a ...libmvec.a )` 的 GNU ld script，不是归档；目标真实归档另有独立路径并已扫描。

这些 15 条未被伪记为扫描成功；它们的格式/用途判断和原始诊断均保留。

## 5. 明确回答任务问题

### 5.1 平台是否存在会因此受影响的函数指针等用法？

**在本次覆盖语料中：`NOT_OBSERVED`。**

- D01：未发现 `&std::condition_variable::wait`、对应成员函数指针变量/参数/类型模板实参或 type-trait 用法。
- D02–D04：只发现 libc++ 自身声明、定义、直接调用；未发现取址/类型依赖。
- D05：析构取址本身是非法 C++；未发现 `noexcept`/destructibility trait 等依赖其异常规格的源码候选或可归属二进制候选。

### 5.2 函数指针以外是否有其他导致符号变化的因素？

**有。** 最小探针实际观察到：

1. 成员函数指针类型进入普通函数参数，名字变化；
2. 该类型成为类型模板实参，模板实例名变化；
3. `noexcept`/析构 trait 的布尔结果成为模板实参，`Lb1E/Lb0E` 变化；
4. SFINAE/overload/constraint 若依赖这些类型或布尔值，可能间接改选实例。

同时，`template<auto P>` 直接传 `&function` 在本次 GCC 13/Itanium 探针中没有额外编码异常规格，符号不变；不把 R86 的“地址作为模板参数一律变化”继续泛化。变化的是**类型模板参数/包含该类型的签名**；函数地址值参数需按实际 ABI 形态区分。

## 6. 无法覆盖与证据边界

1. 源码层只覆盖当前已有的 371 个 T1/R32 解包 source RPM，不是 R11 的 1,450 个平台 source RPM 全集；`chromium-efl` 是已知缺一项。平台全源码层结论因此不是穷尽证明。
2. 补充别名/宽 trait 口径有 5 个超大包超时；精确目标口径对它们已完成。
3. 词法规则无法完全解析宏生成、build-time codegen、任意跨文件别名、条件编译和编译数据库上下文。
4. 已剥离 ELF 的局部/内联符号可能不可见；没有调试信息时不能从最终机器码可靠恢复全部原始 C++ 类型表达式。
5. D03/D04/D05 可能完全内联且不留下可命名符号；二进制“0”必须与源码证据合看。
6. R11 是冻结快照，不覆盖快照之后的包、未发布包、平台外插件、闭源预置对象、运行时 JIT/动态生成代码。
7. `noexcept`/trait 改变 overload 或 SFINAE 后，最终符号可能不再包含目标类/函数名；仅凭二进制不能穷尽反推“哪个表达式导致选择变化”。这部分主要依赖源码扫描，仍受其分母限制。

## 7. 自行判断与尚存疑问

### 自行判断

1. 采用 R32/T1 的 371 个已解包 source RPM 作为“平台已解包源码包”的可用源码语料，并明确不把它称为平台 1,450 个 source RPM 全集。
2. 因首次 `rg` 默认 ignore 规则漏掉 libc++ `include/__...`，改为 `--hidden --no-ignore` 后从头重跑；只采用修正后结果。
3. 双路宽扫遇到超大包 I/O 等待时，为控制系统资源终止该轮（退出 143），改为单 worker/每包 60 秒；该补充口径的 5 个超时不隐藏。
4. 在 ELF 首轮过滤后，依据 ABI 探针补入“不含函数名、只含类名 + `DoF`”口径，并从头重扫；只采用扩大口径后的最终表。
5. 将 15 个静态路径按实际文件头和 `nm` 诊断分为 Go 测试对象、kernel thin archives、ld scripts；没有把它们计入 C++ 目标命中，也没有伪称其由 `nm` 成功解析。
6. 为减小二进制盲区，主动补扫 `.a`；没有复制或修改任何包内容。

### 尚存疑问

1. 用户所指“平台已解包源码包”是否预期覆盖 R11 的 1,450 个 source RPM 全量，而非目前可用的 371 个 T1/R32 迁移相关语料？当前没有 1,450 包全部已解包的本地语料可供同口径扫描。
2. 是否还有 R11 冻结仓之外的闭源/预置/供应商二进制需要纳入？本地未提供其清单，记 `NOT_AVAILABLE`。
3. 未剥离或带完整 DWARF 的发布对应物是否存在？若不存在，内联/type-trait 选择的二进制反推仍受上述限制。

## 8. 执行异常与更正记录

所有命令、stdout、stderr、退出码在 `raw/`，失败尝试没有删除：

- `003`：shell 引号错误，`004` 更正；
- `010`：双路宽扫造成不必要 I/O 等待，主动终止为 143；`014` 改成单 worker 并得到 366/371 补充结果；
- `012`：ELF 扩展 section count 未实现，`013` 定点验证修正，`017` 全量重扫 0 失败；
- `015`：首版 ABI probe 的“空异常规格”使析构仍隐式 non-throwing；`016` 用 `noexcept(false)` 更正，只采用 `016`；
- `019`：静态扫描以批为失败单位；`023/027` 逐归档复核并修正“return 0 仍可能有 stderr”的判定，最终列出 15 条；
- `020`：诊断 heredoc 引号错误，`021` 改用脚本；
- `025`：`rg` 无命中按工具语义退出 1；`026` 同口径正规化为 `NO_MATCH`/退出 0；
- `029`：审计摘要误用包表第 3 列（包名）作为架构，未用于结论；首次重记又在进入日志包装器前拆坏参数，留下 `030_metrics_corrected_bash`/127；随后 `030_metrics_corrected` 改用 Python、读取正确列并得到权威架构计数。

## 9. 证据索引

- 结论汇总：`tables/result_summary.tsv`
- 覆盖/盲区：`tables/coverage_summary.tsv`
- 源码逐候选：`tables/source_candidates_no_ignore.tsv`、`tables/source_candidates_expanded.tsv`
- 源码人工分类：`tables/source_candidate_classification.tsv`
- ELF 最终汇总/候选：`tables/binary_symbol_summary_type_only.tsv`、`tables/binary_symbol_candidates_type_only.tsv`
- 静态归档汇总/候选：`tables/static_archive_summary.tsv`、`tables/static_archive_candidates.tsv`
- 15 条逐项复核：`tables/static_archive_recheck2_summary.tsv`
- ABI 形态：`tables/abi_shape_results.tsv`、`evidence/abi_probe_corrected/`
- D05 取址编译拒绝：`raw/028_destructor_address_probe.*`
- 扫描器与探针源码：`code/`

本报告不提出处置建议、不做路线推荐，也未实施任何改动方案。
