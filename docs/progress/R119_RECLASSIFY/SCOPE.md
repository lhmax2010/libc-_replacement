# 本轮结论的具名范围

## S_R119_20260918：限定范围的阴性

输入集合不是“整个平台”，而是以下冻结快照：

- 安装头清单：`docs/progress/P7_0909/tables/headers.tsv.gz`；devel 筛选清单为同目录 `packages.tsv`，选择 `devel_name=YES` 的 RPM、`scope=INCLUDE_TREE`。348,449 条总记录中选中 223,602 条，223,533 条完成读取与摘要核对，69 条符号链接/非正文问题原样列于 `docs/progress/R119_batch2/W1/HEADER_COVERAGE.json`。头文件来源含 x86_64、armv7l、aarch64；同 SHA 的正文上下文去重不表示每个架构都编译过。
- 对这份安装头集合的限定名索引：`docs/progress/R119_batch2/W1/header_occurrences.tsv.gz` 与 `entity_full/`；补充无前缀记录见 `W1/unqualified/`。本轮 B 组逐项重新核对所引用文件的 SHA256 和完整已保存上下文。标准库本身的声明、CUDA 内嵌实现及 Boost/LLVM 的不同名字空间类型，不当成两个平台包传递本候选的签名。
- ELF 范围：`docs/progress/R119_batch1/W1/full_elf/` 的 3,085 个 x86_64 文件，选自冻结 R11 清单的 `runtime_elf=YES && cpp_indicator=YES`；每个有读取命令、退出码、来源与摘要。该架构 10,946 个运行 ELF 中其余未扩扫，armv7l/aarch64 ELF 也未在这次扩大扫描覆盖中。此集合只定义证据背景；**本轮五项阴性基于安装头用法，不使用候选名字的 ELF 零交集**。

`INTERNAL_ONLY(scope=S_R119_20260918)` 的准确含义是：S 内已扫描、已辨认的安装头用法未发现该候选的包间公开对象签名；观察到的为局部使用、标准库自身声明或同名异类。它不声称平台上不存在其他跨包使用。部分候选在 S 内无业务用法，按已授权四类规则归这一类，不虚构其“有内部调用”。

逐项保留的洞：宏生成/条件展开、模板实例化、静态链接、未扩扫架构、`dlopen`/间接函数指针、生成头、其他 `using`/别名后的无限定名及自动推导返回类型。smatch 等原名可能不进入修饰名的类型，不据符号零命中作任何阴性。已有无前缀补扫也不等于完整 C++ 名字解析。

## S_A：结构性不适用

本轮采用 x86_64 原生、C++17，复用已存在的 GCC 14.2 SDK 头与 libc++ 22.1.8 构建头；使用相同编译器驱动，实际编译命令、版本宏、预处理头位置、定义摘录、源文件摘要和五次运行全部保存。两套库分别构建，不做跨库运行测试。`A_CASES.json` 固定每一项的表达式或具体实例；`items/NNN.json` 同时关联真实平台安装头用法。

三种结论都只及所列标准实现、实例和用法：

- `BUILTIN_ALIAS`：两侧实际展开到同一内建类型。`nullptr_t` 的定义为 `decltype(nullptr)`；它是特殊内建空指针类型，不是整数也不是类对象。
- `COMPILE_TIME_ONLY`：不计元计算设施本身的独立对象；**结果类型仍可能承载对象**。例如 `aligned_storage<8,8>::type` 实测 8 字节，`common_type<int,double>::type` 是 double，`add_pointer<int>::type` 是 int 指针；这些结果的真实边界不能被这次归类抹掉。
- `EMPTY_TYPE`：本次实例两侧 `is_empty=1`、`sizeof=1`，不是大小为零。空基类、标签对象仍可存在，地址、调用约定、行为及自定义特化不由这一事实保证。本分类只按人工给定的“无非静态数据成员”结构口径排除本候选的标准库状态表示，不作任意混合调用安全承诺。

特别保留：`high_resolution_clock` 两侧展开分别为 system_clock / steady_clock，均为空；这**不证明其时间语义相同**。不把 clock 对象分类传给嵌套 time_point；duration 的具体周期仍须独立取证。

未验证 armv7l/aarch64 的结构结果，不作跨架构外推；未覆盖用户自定义特化或未列出的具体模板参数。既有 allocator/hash/less 的已证实存储链与 44 个跨包候选不变。
