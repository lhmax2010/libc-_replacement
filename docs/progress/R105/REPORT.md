# R105：三个未通过验证包的处理报告

## 1. 执行边界与资源

- 仅处理 `libsigc++`、`boost`、`tensorflow2`；`llvm`、`bcc-tools`、`bpftrace` 继续暂停。
- 开工资源门禁 `tools/resource_gate.sh --level medium` 返回 0；构建统一使用 `nice -n 15`、`ionice -c 3`、GBS 单包串行与包内 `-j2`。
- 未推送正式分支或上游；包仓只允许普通快进推送到既有的
  `sandbox/lhmax2025/libcxx-migration`。

## 2. libsigc++：改动有效，原单一判据不适用

R104 的六格完整构建均成功。本轮直接复核了六格的完整构建日志与已解包产物，
没有重跑已完成实验。

三个 libc++ 格的实际 C++ 编译命令都使用 Clang 22.1.8 并带
`-stdlib=libc++`，链接命令带 `-lc++abi`；三个 GCC 格实际使用 GCC
14.2.0，命令中没有 libc++ 参数。libc++ 产物各含 8 个 `std::__1`
动态符号、无 `std::__cxx11`，GCC 产物则相反。典型差异是
`sigc::signal_base::insert/erase` 的参数类型分别落在 libc++ 的
`std::__1::__list_iterator` 与 libstdc++ 的 `std::_List_iterator`。

libc++ 产物没有 `DT_NEEDED: libc++.so.1`，但有 `libc++abi.so.1`；这不是
参数未生效，而是该模板型库没有留下对 libc++ 非内联定义的引用，链接器在
`--as-needed` 下不记录未被引用的共享库。没有为满足旧判据而强制制造依赖。

这类包的有效判据应组合使用：实际编译器及完整命令、产物中的实现特有 ABI
命名空间、对照格不存在另一套运行时痕迹；只有确实引用共享库定义时，才要求
相应的 `DT_NEEDED`。

提交并推送：`4f2f592d2f88eb6a3a949cec8970a4cd5d40165b`。推送后远端目标
SHA 与本地一致，既有提交保留，其他远端分支逐项未变。

## 3. boost：让 `%install` 的第二次 B2 调用继承同一组参数

根因是 RPM 的 `%build` 与 `%install` 是独立脚本环境。首次实现只在
`%build` shell 中设置了 Boost.Build 属性；`%install` 再次执行
`b2 install` 时变量已经丢失，因此发生二次构建并回到 libstdc++。

最终修正遵循 Boost.Build 自身的属性传递机制：第一次读取平台 `.build`
配置后，把最终的 `CFLAGS`、`CXXFLAGS`、`LDFLAGS` 分别保存为构建树内的
三个文件；`%build` 与 `%install` 的 B2 调用均显式传入同一组
`cflags=...`、`cxxflags=...`、`linkflags=...`。这样不仅保留
`-stdlib=libc++`，也逐字保留架构、优化及平台链接参数。

六格均完整构建并通过逐 RPM/逐 ELF 核验。所有含 C++ 运行时依赖的子包在
同一格中一致选择同一套标准库；少量无标准库引用的中性 ELF 单独计数，没有
发现混用。提交并推送：`4168e873584ae50a12733262026c007dce18ec89`。

## 4. tensorflow2：`std::abs` 重载修正与验证

### 4.1 根因和标准正确的修正

失败代码把 `std::abs<float>`、`std::abs<int32_t>` 直接作为接受
`std::function<T(T)>` 的参数。`std::abs` 是重载集合：浮点重载不是要求可用
显式模板实参的函数模板，而整数模板的实现细节也不保证这种写法能唯一选中
目标。因此原写法依赖 libstdc++ 的实现形态，不是可移植接口用法。

修正为对重载集合做显式目标函数指针转换：

```cpp
static_cast<float (*)(float)>(std::abs)
static_cast<int32_t (*)(int32_t)>(std::abs)
```

它只消除重载歧义，不改变调用的函数、输入输出类型或运行行为。源码扫描确认
同文件只有这两个“标量 `std::abs` 重载集合传给 `std::function`”站点；
`complex_support.cc` 使用的是标准规定的 `std::abs(std::complex<T>)` 模板，
另一个整数站点是直接调用，机制不同。

### 4.2 静态归档与真实消费路径的判据

不以 `.a` 的 `DT_NEEDED` 作判断。每个格检查归档成员与强定义符号；libc++ 格
还检查 `std::__1`、排除 `std::__cxx11`，并检查本包实际链接归档形成的
`benchmark_model` 最终 ELF。另用 R103 的精确强符号/归档成员归属脚本，
把新归档与已发布的 `inference-engine-tflite`、`nnstreamer`、`nntrainer`
消费 ELF 对照。

### 4.3 六格结果

候选构建的三个 libc++ 格全部完整成功：实际使用 Clang 22.1.8，命令带
`-stdlib=libc++`；两组静态归档均只见 `std::__1` 而无
`std::__cxx11`，`benchmark_model` 只依赖 `libc++.so.1`，五处防护头文件
全命中。三个架构的新归档均再次与 R103 三个实际消费方的已发布 ELF 得到
数千个精确强符号交集，并定位到数百个归档成员。详见
`TENSORFLOW_ARCHIVE_DETAILS.tsv`。

为回答“真实 GCC 是否受影响”，独立验证 worktree 在不改变候选 spec 的前提下
增加一个仅验证宏，使三目标架构进入 spec 已有的 `USE_CLANG=OFF` 分支；构建
同时设置 `_toolchain_override gcc`。aarch64 与 x86_64 全包构建成功，实际为
GCC 14.2.0，归档只见 libstdc++ ABI，最终 ELF 只依赖
`libstdc++.so.6`；两格的 `elementwise.cc.o` 均实际生成。armv7l 也确认使用
GCC 14.2.0，但在 737/4213 处被 XNNPACK 的 ARM dot-product 汇编提前阻断：

```text
Error: selected processor does not support `vsdot.s8 q8,q2,d0[0]' in ARM mode
```

失败来自原有 GCC 分支未关闭 `XNNPACK_ENABLE_ARM_DOTPROD`，早于本次源码站点，
不是 `std::abs` 修正导致；本任务没有修改 XNNPACK 配置来绕过它。

因此 TensorFlow 的最终状态是：libc++ 三格 PASS；真实 GCC 两格 PASS、一格因
既有 GCC/XNNPACK 约束 FAIL。它不满足“六格全部通过才推”的门禁，**未提交、
未推送**；候选改动保留在本地供人工裁决。完整矩阵见
`TENSORFLOW_MATRIX.tsv`。

## 5. 技术性非零与处置

- `021`、`096`：从子目录调用相对日志包装器，命令在 GBS 启动前以 127
  失败；均记录后改用绝对路径重跑，未触及判据。
- `028`：停止已确认参数不完整的 Boost 尝试时，过滤表达式匹配了包装进程，
  因信号退出；实际无效构建已终止，后续从干净构建根重跑。该次不作为验证结果。
- `092`：验证器误把 GBS 完成后移走的 scratch `.build.log` 当作日志路径，
  立即返回 1；改用已经逐字落盘的完整构建 stdout 重跑，验证器逻辑与产物未改。
- `105`：曾尝试在保留的构建根中定点编译 GCC 对象；该特权夹具随即废弃，
  未改变文件或配置。后续不再使用特权隔离命令。
- `107`–`110`：改用非特权方式直接运行目标 GCC 的探索未形成有效证据：目标
  编译器自身为目标架构 ELF，其子程序执行上下文不完整；其中一次还是脚本路径
  拼写错误。最终结论完全以 GBS 的真实完整构建为准。
- `111`：从验证 worktree 使用相对日志包装器，GBS 启动前返回 127；`113`
  改用绝对路径后正常重跑。
- `119`：TensorFlow 未推送核对中的第一次完整 `ls-remote --heads` 成功并包含
  目标分支 `1cdba735…`；随后为了单独打印同一 ref 的冗余第二次 SSH 查询超时。
  包装脚本因管道末端 `cut` 返回 0 而记录总退出码 0。结论只采用第一次成功的
  完整列表，不把第二次空输出解释为分支不存在，也没有继续重试。

## 6. 尚存问题与自行判断

- `tensorflow2.spec` 在本任务三架构上固定选择 `clang/clang++`；因此
  `_toolchain_override gcc` 只能关闭 libc++ 注入，不能使该包实际改由 GCC
  编译。为同时验证源码修正的真实 GCC 兼容性，本轮使用独立 worktree 中的
  **仅验证开关**进入 spec 原有 `USE_CLANG=OFF` 分支；该开关不会进入候选提交。
  这是对验证夹具的选择，不改变产品构建语义。
- armv7l 真实 GCC 全包格暴露的是本包原有、此前未纳入三目标架构路径的
  XNNPACK 配置限制。若人工仍要求该架构的“实际 GCC 全包成功”作为推送门禁，
  需另行授权处置或定义验证配置；本轮没有自行扩大到修复它。
- `std::abs` 修正对 aarch64/x86_64 的 GCC/libstdc++ 完整构建无负面影响；
  armv7l 因在到达该源文件前被无关汇编失败阻断，不能宣称该架构的全包行为已验证。

## 7. 最终状态

| 包 | 六格状态 | 包仓提交/推送 | 结论 |
|---|---|---|---|
| `libsigc++` | 6/6 PASS（libc++ 格使用组合判据） | `4f2f592d2f88eb6a3a949cec8970a4cd5d40165b`，已推 | 完成 |
| `boost` | 6/6 PASS | `4168e873584ae50a12733262026c007dce18ec89`，已推 | 完成 |
| `tensorflow2` | libc++ 3/3 PASS；真实 GCC 2 PASS、armv7l 1 FAIL | 未提交、未推 | 部分完成，等待人工裁决 GCC/XNNPACK 门禁 |

整个任务未触碰正式分支或上游。`llvm`、`bcc-tools`、`bpftrace` 未操作。
