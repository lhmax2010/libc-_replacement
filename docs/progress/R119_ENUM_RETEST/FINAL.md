# 四项流状态类型的目标架构复测与公开暴露核查

状态：实验与限定暴露核查完成，待人工审阅；**不下产品兼容性结论**。

## 一、结论

- armv7l：4/4 分歧；aarch64：4/4 分歧。两库各五轮一致；两架构均使用 QEMU 用户态，没有占用开发板。
- fmtflags / iostate / openmode：GNU 为各自具名枚举，libc++ 为 unsigned int；seekdir 是两个不同枚举。全部 size=4、align=4，类型身份和实际编码仍不同。
- 真 ARM podofo devel 头编译所得 Seek 名称不同；普通虚调用编译端选中的虚表索引却均为 6。该组合不能由符号差异推导槽位不同，更不能推导业务安全。
- 复用既有公开暴露记录：四种类型、四个函数身份、五条签名；其旧索引位置分别为 61 / 85 / 87 / 12，共 245。位置数量不等于公开签名数。
- 对上述已确认身份的限定查询未发现异包 UND，新增确认包对/符号边均为 0；x86_64 仍为 18/23。先前已证明的两个 ARM 包对/符号边不增不减，ARM 全集仍未查。不是平台级“无消费者”。

## 二、探针、配置与具体数据

| 架构 | 表达式 | GNU 实际类型 / 编码 | libc++ 实际类型 / 编码 | GNU size/align | libc++ size/align |
| --- | --- | --- | --- | --- | --- |
| armv7l | std::ios_base::fmtflags | std::_Ios_Fmtflags / `St13_Ios_Fmtflags` | unsigned int / `j` | 4/4 | 4/4 |
| armv7l | std::ios_base::iostate | std::_Ios_Iostate / `St12_Ios_Iostate` | unsigned int / `j` | 4/4 | 4/4 |
| armv7l | std::ios_base::openmode | std::_Ios_Openmode / `St13_Ios_Openmode` | unsigned int / `j` | 4/4 | 4/4 |
| armv7l | std::ios_base::seekdir | std::_Ios_Seekdir / `St12_Ios_Seekdir` | std::ios_base::seekdir / `NSt3__18ios_base7seekdirE` | 4/4 | 4/4 |
| aarch64 | std::ios_base::fmtflags | std::_Ios_Fmtflags / `St13_Ios_Fmtflags` | unsigned int / `j` | 4/4 | 4/4 |
| aarch64 | std::ios_base::iostate | std::_Ios_Iostate / `St12_Ios_Iostate` | unsigned int / `j` | 4/4 | 4/4 |
| aarch64 | std::ios_base::openmode | std::_Ios_Openmode / `St13_Ios_Openmode` | unsigned int / `j` | 4/4 | 4/4 |
| aarch64 | std::ios_base::seekdir | std::_Ios_Seekdir / `St12_Ios_Seekdir` | std::ios_base::seekdir / `NSt3__18ios_base7seekdirE` | 4/4 | 4/4 |


数据：[TYPES.tsv](TYPES.tsv)（SHA256 `f1c2e9551a6a379141ab91b462612ae5799dee1dc077e2044e5fd1816705a377`）；完整 TypeToken 编码、每格源/可执行文件 SHA、五次原输出与编译命令索引：[MEASUREMENTS.json](MEASUREMENTS.json)（SHA256 `2520239d2116191b9d632f3869ec4af1ece3f73aa0dd4e52e4a1ae3e8d9d3d94`）。

直接复用原测量器的 PRE/source 生成逻辑，emit、TypeToken、四个正向编码控制不改；仅选取这四个表达式。原 ARM types.cpp 的版本与目标尺寸打印也原样保留。两套库版本实测为 `_GLIBCXX_RELEASE=14` 和 `_LIBCPP_VERSION=220108`。[PROBE_REUSE.json](PROBE_REUSE.json)（SHA256 `096af3378d055aba6fb7e14af5590a0ced065abf2b0770d037fce15b592d093b`）

配置的 --target、sysroot、标准模式、优化、链接选项逐项复用既有 JSON；原头文件摘要先硬校验，再编译。armv7l 为 `armv7l-tizen-linux-gnueabi`，编译产生 Thumb ARM 目标；aarch64 为 `aarch64-tizen-linux-gnu`。前者 `/usr/bin/qemu-arm-static`，后者 `/usr/bin/qemu-arm64-static`。QEMU 只执行结果打印，类型选择由编译时目标头/宏决定。[CONFIGURATIONS.json](CONFIGURATIONS.json)（SHA256 `bc367b185c0ec450b655ecf3e57b14bb31ad3250af36bcef7cb65e3cd1525eb9`）

单架构为 4 表达式 × 2 库 × 5 轮 = 40 条目标结果观察，装在 10 次进程运行中；两架构合计 16 个类型/库/架构格、80 条目标结果观察、20 次进程运行。正向控制每次执行并断言 int/long/long long/const long& 的编码；无不可得格被替换实参。实际定义摘录和头文件 SHA：[ENUM_DEFINITIONS.json](ENUM_DEFINITIONS.json)（SHA256 `14e960f0a0ce127a1efe1cd84cd8d2a836c222a03b8e09041b4e22120ddefdc6`）。

## 三、真实 ARM Seek 声明及虚调用

真实头：`podofo-devel-0.9.7-0.armv7l.rpm`，RPM SHA256 `6d7c622f861f505dc2b16558bf7b633effbe87d2d835b23a9ebf0509b37cc81e`；来源 revision `platform/upstream/podofo#96f17d0ba63484f444d003128280c370f9476ce3`。`/usr/include/podofo/base/PdfInputDevice.h:121`：

```cpp
virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);
```

头文件 SHA256 `d92f794449a30db68e2f100d30c4275aa78884daf1bc3751d196b62dfdd9d467`。已将实际使用的 podofo 头与 RPM 内文件摘要逐项绑定，不凭提取目录名推定来源。[RPM_BINDING.json](RPM_BINDING.json)（SHA256 `13a40f98a4b542ba658dc6395f332fc1a58e64ece497dbdf0486b8ef81276e02`） [SEEK_HEADER.json](SEEK_HEADER.json)（SHA256 `dbc6c54de7a4e837ad056592ff262168c0189f15e8208edee7c46bbed20cb10b`）

探针有两种调用：显式类限定调用只为抑制虚派发、显露真实声明的 UND 名称；普通 `d.Seek(off, dir)` 保留虚派发。两库各编译一次对象文件，另生成 LLVM IR。没有手写替代声明，也没有构建或运行 podofo 提供方。

GNU：

```text
_ZN6PoDoFo14PdfInputDevice4SeekExSt12_Ios_Seekdir
```

libc++：

```text
_ZN6PoDoFo14PdfInputDevice4SeekExNSt3__18ios_base7seekdirE
```

来源及对象 SHA：[SEEK_SYMBOLS.json](SEEK_SYMBOLS.json)（SHA256 `32ccb97b8ce8012af090b855466c334cc2eae73aae8c0cacf289cc9e1ef7f0d5`）；程序：[seek.cpp](probes/seek.cpp)。两边 streamoff 已同为 long long（`x`），剩下的差异来自 seekdir 类型身份；这正是 ARM 上仍存在、不同于先前整数别名差异的因素。

**编译观察**：两份普通虚调用 IR 都先读取对象虚表指针，再用 `getelementptr ... i64 6` 取函数指针，最后 `call void %...`。索引 6 是相对所用虚表 address point 的从零开始索引，不是“第六个声明”，也不是完整虚表符号起点偏移。这个虚调用块没有按 Seek 的修饰名直接调用。两份 IR、提取段落及 SHA：[VIRTUAL_CALL.json](VIRTUAL_CALL.json)（SHA256 `f928bf2ead58d70a78ad55060a48aed7a3d374172e6c4414b740901001d97e41`）。

**机制说明**：函数名参与按名链接；虚调用则从对象所指虚表的选定槽位取得入口。声明/继承/覆盖关系决定槽位组织，不是按修饰名排序。因此名称不同不必导致槽位移动；同一槽位仍不足以证明对象、参数值约定、真实提供方表项、派生覆盖及销毁兼容。通用机制参考 [C++ ABI 虚表组织](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-components) 与 [调用方规则](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vcall)；目标行为的直接证据是本次 ARM 编译输出，不把通用文档当成完整 Tizen 产品证明。

**反驳边界**：若要反驳“本配置编译调用方选择同一槽位”，需给出不同的实际编译输出/配置；若要证明混合产品兼容或不兼容，仍需真实跨包动态对象、提供方、调用路径和运行值。该运行结果为 NOT_OBSERVED。

## 四、公开签名与限定消费方核查

| 类型 | 旧索引位置 | 已确认身份 / 签名条数 | 头文件位置 | 本轮定义 / UND / 异包配对 |
| --- | --- | --- | --- | --- |
| std::ios_base::fmtflags | 61 | Cynara::ChecksumStream::flags / 2 | /usr/include/cynara/storage/ChecksumStream.h:53,54 | 2 / 0 / 0 |
| std::ios_base::openmode | 87 | Cynara::ChecksumStream::open / 1 | /usr/include/cynara/storage/ChecksumStream.h:51 | 1 / 0 / 0 |
| std::ios_base::seekdir | 12 | PoDoFo::PdfInputDevice::Seek / 1 | /usr/include/podofo/base/PdfInputDevice.h:121 | 1 / 0 / 0 |
| std::ios_base::iostate | 85 | PoDoFo::PdfInputDevice::Clear / 1 | /usr/include/podofo/base/PdfInputDevice.h:148 | 1 / 0 / 0 |


原已确认的完整五条声明逐行保留于 [PUBLIC_DECLARATIONS.json](PUBLIC_DECLARATIONS.json)（SHA256 `f14298da39956fddb3cfe92e92f6173835e17e2fc4b044766d3c7fd061d6ae9e`），原文含参数、返回、inline/virtual 限定和头 SHA。fmtflags 的 `flags()` 返回值与 `flags(fmtflags)` 参数重载分别计一条；另外各一条。[prior_DIVERGENT_EXPOSURE.json](prior_DIVERGENT_EXPOSURE.json)（SHA256 `8526e0dd77e85a7cd9f9e4670930ec3dc46fb6086d7d3f7b2adf4a007140d99e`）

245 是旧索引的命中位置，包含尚未逐一完成签名判定的上下文，**本轮没有将其全部升级为公开签名**。本轮按任务要求直接复用已确认清单，只查询这些函数身份，不重开全部命中语义分析。

正向对照先定位真实 `tizen_base::Bundle::Add`，两端索引 SHA 与原提供/消费方记录匹配且来源源码包不同，确认查询能找到异包配对。随后按完整函数身份筛出定义，并用完整规范化函数名找相等的 UND，再比较源码 RPM，而不是查四个类型名字的字面交集。[BOUNDARY_CONTROL.json](BOUNDARY_CONTROL.json)（SHA256 `0a8d08bb6b8dddb528f55d176d7126f5c0af127a937ff4efc92c2deb67355092`） [BOUNDARY_QUERIES.json](BOUNDARY_QUERIES.json)（SHA256 `ffc287e4232035231ee82c88a28f0aaeb57621d5871b5b9a4021f4096d9fcf06`）

范围仍为 `S_R119_20260918` 的 3,085 个 x86_64 ELF，不是所有平台产物。四个身份分别找到 2/1/1/1 个定义、均未找到对应 UND。没有构成需要进一步升级的新符号边。内联、虚调用、函数指针、静态链接、生成头、同名不同配置、未扫描架构/产物均不由该阴性排除；尤其 Seek/Clear 不能用无 UND 证明无实际调用。

**既有成员边保留**：fmtflags 已随 `app-core → gtest` 的 ostream 内部状态出现；openmode 已随 `enlightenment → gtest` 的 stringstream 内部状态出现。它们与本轮 ChecksumStream 等直接签名查询不是同一条证据，不能因本轮阴性而抹掉，也不新增重复包边。[EXISTING_MEMBER_EDGES.json](EXISTING_MEMBER_EDGES.json)（SHA256 `f5567a3d70094070614535e8024f8101d98993d9dbc317221b619b5d0d6b04ad`） [EDGE_COUNTS.json](EDGE_COUNTS.json)（SHA256 `4b0c46314d362435471999674fe81cc4c982a29099418e50bba6fdbc5e592c44`）

## 五、可直接并入汇编的差异说明

先前的 13 项是同名整数别名在两库中选择 long 或 long long；它们在已测 armv7l 配置下恰好选择相同类型。本轮四项则是流状态类型的身份分歧：前三项在 GNU 中是具名枚举，在 libc++ 中是 unsigned int，seekdir 在两库中是不同枚举。实测 armv7l 与 aarch64 均保留分歧，虽然各项两侧都占 4 字节、按 4 字节对齐。相同宽度不能让它们形成相同的 C++ 链接名称；Seek 已展示这个差异，但虚调用可以通过表中入口而不按函数名查找，因此不能把符号分歧直接写成虚调用必失败或产品不兼容。

## 六、范围与交付

四项从原“不测”集合独立列出：类型差异已确认；已登记直接公开签名的异包消费仍未闭合。其余 568 项不重开。未测真实混合库 podofo/Cynara 调用、真实提供方虚表内容、派生类覆盖与对象销毁、枚举值组合的转换语义；没有以测量宽度替这些问题作答。

汇编仅更新枚举条目、相关挂账与不测集合构成，并标版本。改前原件保留在本目录 `summary_v1/`，对应旧提交 `bf35a916d9a9d058e91e851a74129a4efb76f9c8`；变更清单与 SHA 见 SUMMARY_UPDATE.json，逐行差异见 SUMMARY.diff。没有覆盖旧类型测量、定界或投影产物。

自行判断：用显式限定调用观察函数名、普通虚调用观察编译端槽位；不将无 UND 升级为无调用；将既有两条成员关联与本轮直接签名阴性分开；不重开 245 个词法位置的全量语义分析。没有板上操作，没有修改平台源码或配置，没有新包部署，未推 Gerrit。
