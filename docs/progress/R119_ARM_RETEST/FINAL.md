# 13 项内建底层类型差异：armv7l 复测

## 结论

**选择结论 a：armv7l 上两侧一致。** 指定 13 项全部为 `long long`，DIRECT 编码 `x`，完整 TOKEN 为 `9TypeTokenIxE`，sizeof/alignof 均为 8。GNU 与 libc++ 各五轮一致。本次 13 项“内建底层类型选择不同”的类别在当前先导架构配置下不成立，未出现 4 对 8 的宽度差异。

但是，**该差异消失不等于真实跨包调用消失**：

| 受检边 | armv7l 内建底层差异 | armv7l 两库函数符号 | 真实 GNU 包间引用 |
| --- | --- | --- | --- |
| dali2-ui-foundation → dali2，WaitUntil | 消失，rep 均 long long | **仍不同**，包含两库不同的 chrono/时钟类型名称 | 已确认存在 |
| zypper → libzypp，Reader | 消失，streamoff 均 long long | **相同**；参数只编码 InputStream 类名 | 已确认存在 |

两边的实际编译符号及真实 ARM ELF 条目在下文。没有将“符号相同”提升为对象布局或跨库运行安全。

**aarch64 顺带实测完成**：同一 13 项仍全部为 GNU `long(l)` / libc++ `long long(x)`，两侧 sizeof/alignof 均为 8，各五轮。此为本次目标架构实际编译执行所得，不是从 LP64 或 x86_64 结果外推。

## 一、13 项逐项数据

完整逐项表：[ARMV7L_TYPES.md](ARMV7L_TYPES.md) / [ARMV7L_TYPES.tsv](ARMV7L_TYPES.tsv)。包含用户指定的 T0001、T0014、T0019、T0024、T0029、T0356、T0365、T0369、T0373、T0378、T0380、T0382、T0384，没有增加其他类型。

每项两侧记录在 measurements/，含实际类型、TOKEN、DIRECT、size、align、源码 SHA、可执行文件 SHA、编译命令、符号表和五轮输出路径。13 项 × 两库 × 五轮是 **130 个逐项输出观察，装在 10 次进程运行里**，不是 130 次进程运行。

aarch64 对应 [AARCH64_TYPES.tsv](AARCH64_TYPES.tsv) 与 aarch64_measurements/，同样为 13 项、两库各五轮。其余 **572 项的 armv7l 状态仍为 NOT_OBSERVED**；本轮也未补它们的 aarch64 数据。

口径修正：旧清单中仅命名空间/ABI 标签造成的 114 项、实现类差异的 111 项属于已知迁移前提，不作为本次新发现。对外只称“13 项内建底层类型选择差异”，不脱离构成单独引用旧总数。Dali 尚存的名称差异归到已知前提，不重新计为第 14 个新类型。

## 二、探针与执行环境

### 探针复用

直接从冻结的原 measure.py 提取 `PRE` 与 `source()`，复用原 `emit()`、`TypeToken<T>`、`typeid(TypeToken<T>).name()`、sizeof/alignof 和四个正向对照，没有重写类型测量算法。仅在 main 开头追加库版本、指针/long/long long 尺寸输出。提取方法与原脚本 SHA 见 [PROBE_REUSE.json](PROBE_REUSE.json)，生成源码见 [types.cpp](probes/types.cpp)。

判同异仍比较完整 TOKEN，不比较漂亮打印。int、long、long long、const long& 的实际编码均有断言；两库必测、每项五轮一致断言、结果钉死断言都保留。最终审计还把 ARM 13 项全同、aarch64 13 项全异分别逐项断言，不用预期值替代测量。

### 环境申报

**没有使用物理板，没有执行 SDB/SSH 板上命令，也没有清理或占用编译线的板子。**

| 项 | armv7l | aarch64 |
| --- | --- | --- |
| 目标 triple | `armv7l-tizen-linux-gnueabi` | `aarch64-tizen-linux-gnu` |
| 执行 | `/usr/bin/qemu-arm-static` 用户态 | `/usr/bin/qemu-arm64-static` 用户态 |
| QEMU 版本 | 5.2.0 | 5.2.0 |
| 指针 / long / long long 字节数 | 4 / 4 / 8 | 8 / 8 / 8 |
| GNU 头文件版本宏 | `_GLIBCXX_RELEASE=14` | `_GLIBCXX_RELEASE=14` |
| libc++ 头文件版本宏 | `_LIBCPP_VERSION=220108` | `_LIBCPP_VERSION=220108` |
| C++ 标准/优化 | C++17 / O0 | C++17 / O0 |

ARM 额外选项：`-march=armv7-a -mfpu=neon -mfloat-abi=softfp -mthumb`。两套库都使用 `-nostdinc++ -nostdlib++`，显式指定各自头与库。完整配置、包括每个实际包含头的 SHA，见 [CONFIGURATION.json](CONFIGURATION.json) 和 [AARCH64_CONFIGURATION.json](AARCH64_CONFIGURATION.json)。

armv7l sysroot：

```text
/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/BUILD-ROOTS/scratch.armv7l.0
```

aarch64 sysroot：

```text
/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0
```

编译器沿用既有 `progress/R33/tools/tizen-clang++`；命令显式覆盖目标与 sysroot。aarch64 仅复用已有库/工具，在本目录增加目标链接器调用包装，没有构建或安装平台包。

**QEMU 对本项结论的含义**：所用类型、编码、sizeof 和 alignof 在交叉编译时由目标头文件、宏、编译器配置确定；QEMU 执行的是把这些结果打印出来的程序，不参与重新选择 C++ 类型。因此它足以读取本次类型定义结果。这里只验证类型和输出一致性，不据此确认真实板子的调度、展开、并发或跨库业务运行行为。两条接口符号另用交叉编译生成 ARM 对象文件取得，不依赖 QEMU 运行这些接口。

## 三、两条边的实际符号与影响

使用真实 **armv7l devel RPM 头文件**，不是手工仿写接口声明。Dali 用其 ConditionalWait 头；Reader 用其完整 Reader 头并补齐现有 ARM Boost/libxml2 头依赖。每库每接口编译五次、读取实际对象的 UND 符号并断言五次一致；没有构建 Dali/libzypp provider，也没有运行这两条业务调用。

四格完整记录：[SIGNATURES.json](SIGNATURES.json)。ARM GNU 原产物另行检查定义、UND、SONAME/NEEDED：[ACTUAL_ARM_EDGES.json](ACTUAL_ARM_EDGES.json)。9 个原 RPM 的 SHA 与其中 226 个实际使用文件的 RPM 内文件摘要已硬校验：[RPM_BINDINGS.json](RPM_BINDINGS.json)。

### Dali WaitUntil

GNU 头交叉编译得到：

```text
_ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt6chrono10time_pointINS4_3_V212steady_clockENS4_8durationIxSt5ratioILx1ELx1000000000EEEEEE
```

libc++ 头交叉编译得到：

```text
_ZN4Dali15ConditionalWait9WaitUntilERKNS0_10ScopedLockENSt3__16chrono10time_pointINS5_12steady_clockENS5_8durationIxNS4_5ratioILx1ELx1000000000EEEEEEE
```

两侧 duration 的数值表示均为 `long long`（`x`）；但 GNU 的 `std::chrono::_V2::steady_clock` 与 libc++ 的 `std::__1::chrono::steady_clock` 等名称仍不同。因此 **13 项里的 rep 差异消失，WaitUntil 跨库符号名称不一致仍在**，属于已知类型命名差异。

真实 ARM `libdali2-ui-foundation.so.2.0.0` 的 GNU UND 与 ARM `libdali2-core.so.2.0.0` 定义匹配，后者 SONAME 位于前者 NEEDED。前者并没有因此不再调用后者。本轮 libc++ 符号来自真实头的调用方对象；没有把它描述成“已取得 libc++ 构建的 Dali provider”。

### zypp Reader

GNU 与 libc++ 头交叉编译得到同一符号：

```text
_ZN4zypp3xml6ReaderC1ERKNS_11InputStreamERKNS0_8ValidateE
```

实际参数是 `zypp::InputStream const&` 和 `zypp::xml::Validate const&`，**不会把 InputStream 内部成员的类型编码进这个构造函数名**。所以该符号本来就不能用来直接判断成员 streamoff 的底层类型；旧 x86_64 记录也没有证明 Reader 因 `l/x` 发生符号名冲突。

本次 ARM streamoff 两库均 long long，故该成员的新底层类型差异消失。真实 ARM zypper 的 UND 与 libzypp 定义仍匹配（保留 ZYPP_plain 版本信息）。这条真实引用边仍在。InputStream 还含 `_name` 等标准库对象成员；**本轮没有重测这些成员的布局、状态或业务调用，不能从本次符号相同推导 Reader 跨库安全**。

## 四、计数如何处理

必须区分“发现的包间传递边”和“某架构上某一种差异影响的边”。

| 口径 | 本轮可报告的数据 |
| --- | --- |
| 本次 13 项内建差异，armv7l | 0 / 13；受检两边均不再有这项内建底层差异 |
| 本次受检 armv7l 真实包间引用 | 两包对、两符号边均存在 |
| 本次受检 armv7l 跨库符号名称不同 | WaitUntil 有；Reader 无 |
| 原已确证 x86_64 跨包面 | 仍为 18 包对 / 23 符号边，**仅及原 x86_64 范围** |
| armv7l 完整跨包面总数 | NOT_OBSERVED；没有重核其余原始边 |

**不执行机械回退至 16/21**：用户给出的前提“若两条边在 armv7l 上不成立”未被本轮证明，反而两条 ARM 真实引用均已找到，且 WaitUntil 的跨库符号仍不同。另外，原 16/21 是原 x86_64 证据基数，不能把它当作已经核查过的 ARM 分母。

在“本次新增内建差异所影响的边”这一专门口径下，可以明确报告 ARM 受检两边均不受该差异影响；这不删除真实包间引用登记。机器可读口径与两条 ARM 边分别见 [COUNTING_SCOPE.json](COUNTING_SCOPE.json)、[ARM_CONFIRMED_EDGES.tsv](ARM_CONFIRMED_EDGES.tsv)。旧目录任何表均未覆盖。

## 五、限制、问题与自检

- 范围严格为指定 13 项，加用户要求的两条接口符号；没有把其余 572 项的 ARM 状态改为已测。
- aarch64 只做相同 13 项的类型测量，未补两条边的 aarch64 符号/消费者核查。
- GNU 14 / libc++ 22.1.8 的本次目标配置有效；不外推到其他版本、宏、数据模型、构建模式。
- 首次 Reader 编译缺 Boost 头，已从冻结 ARM RPM 补入对应 devel 包和 libxml2 包的只读 include 路径。保留失败与成功记录，没有删改接口声明使其通过。
- 首次 aarch64 链接器调用因 ELF 解释器预期 `/lib/ld-linux-aarch64.so.1`、现有 sysroot 实际提供 lib64 路径而失败。改为显式调用现有目标加载器后通过；没有创建系统软链接或改 sysroot。
- 没有平台源码或配置修改，没有板上操作，没有 Gerrit 推送。旧产物 7,470 个文件的 SHA 前后相同；并行度 1，nice19/ionice3，RLIMIT_AS 9,921,875,558 字节。资源闸门均通过。
- 自行判断限于：采用 QEMU 读取编译结果；复用现有 aarch64 工具链做低成本补测；区分差异类别与包间引用计数。没有据此作生产兼容性或迁移处置裁决。

核验结果见 [AUDIT.json](AUDIT.json)，源码/脚本 SHA 见 [SCRIPT_SHA256.json](SCRIPT_SHA256.json)。命令原文、stdout、stderr、退出码、开始/结束时间均在 raw/。本轮完成后停止，交人工审阅。
