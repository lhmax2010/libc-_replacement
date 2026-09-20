# 实现定义类型差异扫描：核查结果

状态：**PARTIAL，交人工审阅**。旧结果重判及本轮 585 个固定表达式的两库测量完成；公开暴露扫描已有阳性证据，但没有闭合全部实例的公开签名/消费者。不是“所有标准库类型已经穷尽”的报告。

## 一、缺失断言与十处改判

新脚本 [classify.py](code/classify.py) 在允许 `RESULT_NO_OBJECT` 前要求两侧实际类型一致；不一致另记 `RESULT_TYPE_DIVERGENT`。只写本目录，不修改原脚本或原结果。实际两侧声明已证明为 const bool 的旧静态规则沿用，不新增“谓词值已测”的结论。

| 逐处结果 | 改前 | 改后 |
| --- | ---: | ---: |
| RESULT_NO_OBJECT | 15,780 | 15,770 |
| RESULT_TYPE_DIVERGENT | 0 | 10 |
| RESULT_STDLIB_OBJECT | 13 | 13 |
| RESULT_UNRESOLVED | 15,793 | 15,793 |
| 总位置数 | 31,586 | 31,586 |

十处正好是 `numeric_limits` 1 处（`streamoff` 的 long / long long）及 `decay` 9 处（两库的 bind 占位符类不同）。逐处位置、实际表达式和两侧值见 [RECLASSIFIED_CHANGES.tsv](RECLASSIFIED_CHANGES.tsv)，65 项计数见 [RECLASSIFIED_CANDIDATES.tsv](RECLASSIFIED_CANDIDATES.tsv)。没有继续推进这 65 项的关闭。

31,586 是包含静态规则与未定记录的总位置数，不是“两库成功测量了 31,586 个不同实例”。重判核验 6,952 个来源路径 SHA；最终另外核验旧目录 2,465 个文件均未变化，见 [AUDIT.json](AUDIT.json)。`BUILTIN_ALIAS` / `EMPTY_TYPE` 的既有结论及 plan.json 未修改。

## 二、测量范围、方法与结果

### 范围

本次仅 x86_64、C++17。GNU 侧头文件 `_GLIBCXX_RELEASE=14`、`__GLIBCXX__=20240801`、`_GLIBCXX_USE_CXX11_ABI=1`；libc++ `_LIBCPP_VERSION=220108`、命名空间 `__1`。指针及 long 均为 8 字节。实际编译命令、宏、包含头及 SHA 见 [ACTIVE_CONFIG.json](ACTIVE_CONFIG.json) 和各测量记录；使用既有 Tizen Clang 驱动，不把 GNU 兼容宏当作 GCC 编译器版本。

从实际两侧头文件扩充至 **585 个表达式、20 族**，含 I/O、cstddef/cstdint、容器嵌套类型、迭代器、常量、chrono、随机引擎、线程、文件系统、原子及正则等。依据见 [CATALOG_SOURCE_EVIDENCE.json](CATALOG_SOURCE_EVIDENCE.json)、[CATALOG.tsv](CATALOG.tsv)。容器采用清单明确列出的固定实参，不覆盖任意 allocator、char_traits 或其他模板实例。标准版本、配置与安装头范围的已知洞见 [SCOPE.md](SCOPE.md)。

### 实测结果

| 表达式结果 | 数量 | 含义 |
| --- | ---: | --- |
| IDENTICAL | 283 | 两侧完整类型编码一致；不证明类布局或行为一致 |
| DIVERGENT | 242 | 两侧完整类型编码不同 |
| NOT_AVAILABLE | 60 | 57 项有成员探测不可形成；另 3 项在本次 C++17 配置编译失败 |

三个编译失败表达式为 `atomic_char8_t`、`atomic_signed_lock_free`、`atomic_unsigned_lock_free`。未替换实参或切换标准使它们“过关”。成员探测不可形成不能区分“没有声明”与“本访问上下文不可访问”，不推断接口不存在。

1,170 个表达式×库格中，1,053 格取得类型，111 格取得成员不可形成结果，6 格编译失败。前两类各五轮：共 5,820 个逐格输出观察，合并在 200 个实际成功进程运行记录中。**不是 5,820 次独立进程启动**。40 份最终探针源码已核 SHA；每份源码包含一组独立类型查询。

`__PRETTY_FUNCTION__`、sizeof、alignof、完整 TOKEN 与独立 DIRECT 均保存。TOKEN 保留 cv/ref，用于判同；DIRECT 是真正独立的类型编码，但会丢掉顶层 cv/ref。最终清单见 [TYPE_RESULTS.tsv](TYPE_RESULTS.tsv)、[DIVERGENT_TYPES.md](DIVERGENT_TYPES.md)。第一版解码上下文错误的修正、全量重测及保留记录见下文。

### 不同类型并非同一种问题

| 差异机制 | 表达式数 |
| --- | ---: |
| 内建底层类型选择不同 | 13 |
| 仅内联命名空间/ABI 标签不同 | 114 |
| 枚举与内建类型或枚举身份不同 | 4 |
| 实现类、模板实参或组合类型不同 | 111 |

不能把 242 全部宣传成新发现的“long 对 long long”问题。分类依据是完整类型编码的规范解码，不是 sizeof 是否相同。

13 个内建底层类型差异均为 **GNU `long`（`l`）/ libc++ `long long`（`x`），本架构两者 sizeof/alignof 均为 8**：

| 类型表达式 | 条数 | 测量编号 |
| --- | ---: | --- |
| `std::streamoff` | 1 | T0001 |
| `std::char_traits<char/wchar_t/char16_t/char32_t>::off_type` | 4 | T0014、T0019、T0024、T0029 |
| `std::istreambuf_iterator<char>::difference_type` | 1 | T0356 |
| `std::chrono::system_clock/steady_clock/high_resolution_clock::rep` | 3 | T0365、T0369、T0373 |
| `std::chrono::nanoseconds/microseconds/milliseconds/seconds::rep` | 4 | T0378、T0380、T0382、T0384 |

逐项完整行见 [BUILTIN_DIVERGENT.tsv](BUILTIN_DIVERGENT.tsv)。斜线合并仅用于上表排版，不是从一种字符或时钟向其余实例推断。

I/O 的四个标志类型另列：fmtflags、iostate、openmode 在 GNU 是具名枚举、libc++ 是 unsigned int（`j`）；seekdir 两侧枚举身份不同。`streampos` 的显示名看起来相同，但完整编码有 `std::fpos` / `std::__1::fpos` 区别。32 个 cstddef/cstdint 表达式在本配置中均 IDENTICAL；这不是对 armv7l 的判断。

### 符号不匹配的可复核对照

控制用例 `accept_offset(std::streamoff)` 的实际定义符号：GNU `_Z13accept_offsetl`，libc++ `_Z13accept_offsetx`。同侧两格各 5 次，输入 13、输出 20，与断言一致；两个跨侧方向各 5 次链接均被拒，错误为找不到对应 `accept_offset`。见 [LINK_CONTROL.json](LINK_CONTROL.json)、[控制源码](controls/offset_provider.cpp) 及所列编译/链接 stderr。

这是方法对照，不是真实平台包运行实验。无参数、仅返回 streamoff 的 `offset_return()` 两侧名字均为 `_Z13offset_returnv`，说明返回类型并不总进入普通函数的符号名；不能据“能链接”证明该返回边界安全。

## 三、公开暴露与跨包边

### 覆盖及尚未闭合的部分

沿用 S_R119_20260918 / S_A。安装头扫描得到 85,383 个限定名根词法位置；这些位置包含模板体、局部变量、注释及标准库头，**不是 85,383 个公开接口**。本次原始可读行比旧清单多 1 个非 C++ 工具文件，已排除出声明证据，有效范围未因此扩大；详见 SCOPE.md。

全部 242 项均在 [DIVERGENT_EXPOSURE.tsv](DIVERGENT_EXPOSURE.tsv) 有一行：

| 当前证据状态（表达式口径） | 数量 |
| --- | ---: |
| 已闭合真实跨包结构链 | 4 |
| 已确认公开声明，但消费边界尚未闭合 | 15 |
| 未闭合该具体实例的公开签名，NOT_OBSERVED | 223 |

共有 17 条经过实际行号与 SHA 断言的公开声明记录，覆盖 16 个表达式；其中 streamoff 同时已有另一条闭合跨包链。加上 Dali 结构链的 3 个表达式，已确认公开暴露的并集为 19。见 [PUBLIC_DECLARATIONS.tsv](PUBLIC_DECLARATIONS.tsv)、[EXPOSURE_SELECTED_HEADERS.json](EXPOSURE_SELECTED_HEADERS.json)。这些“公开”指冻结 devel 安装头中的公开声明，不追加 SDK 对业务应用开放的承诺。

32 个函数身份查询前，`bundle::Add` 正向对照确认同时找到定义及消费者 UND。Cynara flags/open、PoDoFo Tell/Seek/Clear、Glib 迭代器函数、Abseil FromChrono 等查询未找到异包 UND，仅对指定身份与冻结 ELF 范围成立；不排除模板实例、内联、虚调用、静态链接、dlopen、生成头或未覆盖架构。其他词法零命中没有被提升为“无公开使用”。

### 已闭合的结构链

1. **Dali 的 `WaitUntil`**：公开头 `conditional-wait.h:71` 定义 TimePoint，`:133` 按值传入；TimePoint → steady_clock::time_point → duration → 非静态 rep 存储，两套库源行快照见 structure/。真实 consumer / provider 符号匹配，provider SONAME 在消费者 NEEDED 中。dali2-toolkit → dali2 原已登记；本轮补入 **dali2-ui-foundation → dali2**。后者源码直接 include 路线尚未找到，不把 ELF 引用写成“已找到直接包含语句”。
2. **`zypp::xml::Reader`**：公开 Reader 构造接收 `InputStream const&` → InputStream.h:134 的 `DefaultIntegral<std::streamoff,-1> _size` → DefaultIntegral.h 的 `value_type=Tp` 和 `Tp _val`。真实 **zypper → libzypp** 构造函数定义/UND 匹配，版本节点均 `ZYPP_plain`，SONAME/NEEDED 对应。该符号只写 InputStream 类名，不写其成员 streamoff，因此**不据此宣称这条函数一定因 l/x 链接失败，也未实测这条边运行错值**。

完整源码行、RPM、ELF 路径及 SHA、原始 readelf 行见 [CONFIRMED_EDGE_PROOFS.json](CONFIRMED_EDGE_PROOFS.json)、[EDGE_STRUCTURAL_HEADERS.json](EDGE_STRUCTURAL_HEADERS.json)、[VERSION_CHECK.json](VERSION_CHECK.json)。这些是静态结构与 ELF 证据，不是跨库业务调用实测。

### 新旧计数

| 单位 | 原已确认 | 本轮新增 | 更新后已确认 |
| --- | ---: | ---: | ---: |
| 包对（消费方源码包、提供方源码包） | 16 | 2 | **18** |
| 符号边（包对、两端 ELF 路径、原始符号） | 21 | 2 | **23** |

见 [CROSS_PACKAGE_PAIRS_UPDATED.tsv](CROSS_PACKAGE_PAIRS_UPDATED.tsv)、[CROSS_SYMBOL_EDGES_UPDATED.tsv](CROSS_SYMBOL_EDGES_UPDATED.tsv)。本轮 4 个表达式关联 3 条边，其中 1 条已存在；表达式、包对、符号边不得相加。原始表未覆盖。

扩展检索还发现 Dali `ReadFile` 的 streampos& 重载：7 个消费者 ELF、4 个源码包族有匹配；provider 有普通、TV、mobile 三个同 SONAME 配置。尚未唯一绑定消费者实际采用的 provider，作为候选保留，不混入上述已确认下界。`GetFileSize` 仅返回 streampos，未找到异包 UND。OpenCV `setException` 的异包匹配属于 `cv::Exception const&` 重载，不是 `std::exception_ptr`；未继承阳性。LLVM getLastModified 的旧版本/SDK/bootstrap 同名库也没有仅凭名字合并。原始候选均见 [BOUNDARY_FUNCTION_QUERIES.json](BOUNDARY_FUNCTION_QUERIES.json)。

## 四、armv7l 复测清单

本轮不用板子，armv7l 一律 **NOT_OBSERVED**。逐项清单 [ARMV7L_RETEST.tsv](ARMV7L_RETEST.tsv) 有全部 585 表达式，包括 x86_64 上相同和不可得的项。需要重测底层展开、完整编码、sizeof/alignof、五轮与对照，并重新绑定 armv7l 公开签名及 ELF。不能把 x86_64 的 `int64_t`、size_type、ptrdiff_t、streamoff、时钟 rep 或迭代器结论外推到 ILP32；这里没有预填目标架构的值。

## 五、可用于对外报告的一段说明

同一个公开类型名字，在两套标准库里可能代表不同的实际类型。例如本次 x86_64 测得 `streamoff` 一侧是 long，另一侧是 long long；两者虽然都占 8 字节，编译器给参数生成的函数名称却不同，混用时可能在链接阶段就找不到函数。此前的布局或状态问题通常是函数能够被调用，但双方对对象内容的理解不同；本次检查增加的是“函数名称能否对得上”这一层。不过，返回类型或藏在类成员里的类型不一定体现在函数名称中，所以链接成功仍不能替代对象内容的核查。

## 六、自行判断、自检修正与剩余问题

- 有限清单选择、20 族扩展、固定模板实参和 C++17 是本轮的范围判断，依据绑定到实际头文件；不称为全部标准版本/扩展/模板实例的全集。
- 第一版把模板包装去除后单独解码，破坏了复杂类型替换索引上下文。已全量重测并改为解码完整包装；独立编码另取 DIRECT。总数未变，分类从 13/102/4/123 修正为 13/114/4/111。第一版源码、输出、measurements_pass1 保留供复核。
- 首次边证明脚本误把 SQLite 的已解码符号列当原始列，断言失败；改用真实 ELF 的 readelf 输出后重验。首次 chrono 私有字段名和 Abseil 重载行号选择错误均被断言拒绝，按实际源码修正。汇总曾把 defaultdict 的空键数量当阳性项数，已改为非空证据计数。审计读取 LINK_CONTROL 的 JSON 层级错误也已修正。相关失败日志均保留，没有把失败轮算作通过。
- 111 个成员不可得格的说明改为“访问上下文不可形成”，不继续声称一定不存在该成员；原始探针输出未改。
- 剩余 223 项需要具体实例、别名/成员链及函数身份绑定；仅靠限定名根扫描无法闭合。15 项虽有公开声明，但现有符号查询不足以关闭间接、内联等消费路径。没有把这些项改判为 INTERNAL_ONLY 或 PUBLIC_NO_CONSUMER。
- Dali 多 provider 的实际部署绑定、其他未覆盖结构链、目标架构材料均作为未定保留。18/23 是本次确证下界，**不是完整跨包面**。
- 串行执行、nice19/ionice3、RLIMIT_AS 9,921,875,558 字节；Git 普通优先级。资源闸门均通过，未使用开发板，未修改平台源码/配置，未推 Gerrit。开工定位命令的补录及一次退出 130 见 BOOTSTRAP.md。

最终机器核验见 [AUDIT.json](AUDIT.json)，命令原文、stdout、stderr、退出码、时间分别保存在 raw/；脚本快照 SHA 见 SCRIPT_SHA256.json。完成本轮材料交接后停止，未把未定部分包装为“全部完成”。
