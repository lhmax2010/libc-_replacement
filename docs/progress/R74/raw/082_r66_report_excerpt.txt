# R66 libstdc++ 与 libc++ 行为差异系统排查总报告

## 结论摘要

本轮建立了源码结构差分、真实构建诊断、ELF/RTTI、ABI manifest、发布
依赖图、上游文献和运行测试可达性七类互补观察面。校准阶段在未把四个
已知答案写入规则的前提下，重新发现了标准库防护缺失、实现私有 API、
第三方库 ABI 和 typeinfo 不统一四类已知项，因此方法集通过校准。

排查没有发现另一个已经具备线程取消问题同等完整证据链的新增确定缺陷；
但发现了一组只能在运行期定性的高风险边界：同名 ABI、header-only 对象
布局、RTTI/exception_ptr、跨 DSO allocator ownership、静态 runtime、
插件加载顺序、ARM EHABI，以及 glibc 特有故障路径。它们不能由“链接成功”
排除，也未被现有 45 包源码分类完全覆盖。逐项素材见
`tables/risk_inventory.tsv`，本报告不做优先级排序或处置建议。

厂商包复核推翻了“四个源码身份均技术上不可迁移”的旧口径：三个包含
可重编源码，只有 `pen-wave` 在本次 source RPM 中仅含预制 DSO。永久旧
runtime 的技术下界为 1 个源码身份、2 个二进制包名；许可、签名和组织
流程不在本语料中。

## 一、方法校准

完整结果见 [CALIBRATION.md](CALIBRATION.md)。四项均为
`REDISCOVERED`：

1. typed catch-and-rethrow 差分找出 65 个 `__forced_unwind` 防护；
2. 真实 libc++ 诊断结构解析找出 `_S_badbit`、`__filebuf_type`；
3. 未定义符号的 `std` ABI 命名空间识别出 Boost.Test 边界；
4. 多 ELF RTTI 的 symtab/dynsym 二维检查识别出 typeinfo 不唯一形态。

因此后续没有以单一 grep 代替语义判断，而是把源码、诊断、ELF 和运行
身份作为不同证据层。校准过程中两次无效命令尝试已保留并明确排除。

## 二、源码级差异

### 2.1 标准库防御性构造

完整结果见 [DEFENSIVE_AND_ABI.md](DEFENSIVE_AND_ABI.md)。唯一重新识别
出的“libstdc++ 生产实现具有专用外来异常类型、libc++ 整层缺失”的机械
家族仍是 forced unwind。`__gnu_cxx::forced_error` 的 34 个 catch 全属
测试注入，不是新的产品运行防护。

两边均有无异常配置、异常分配兜底和状态不一致处理，但实现形态不同：

- libstdc++ 以 `__try`/`__catch` 等宏覆盖 `-fno-exceptions`，libc++ 以
  feature branches/throw-or-abort 处理；无异常构建未执行；
- libstdc++ emergency pool 与 libc++abi emergency stash 都存在，OOM
  行为没有实测；
- libstdc++ 的 `secure_getenv`、`GLIBCXX_TUNABLES`、
  `__libc_single_threaded` 等 glibc 耦合在 libc++ 没有同 token 对应；
  这只证明形态不同，不能据此宣称功能缺失；
- future/iostream 状态机、异常替换和 terminate 路径在两边均存在，项目
  既往已证明若干运行差异，本轮没有改代码或重复设计修法。

### 2.2 371 包实现私有 API/扩展

完整结果见 [SOURCE_COMPAT.md](SOURCE_COMPAT.md)。全量扫描产生 53,452
条命中；生产形态家族并集为 89/371 个包身份，其中 39 个属于既有 45、
50 个在其外。所有权和条件分支复核后，既有 45 外保留 **13 个源码 RPM
身份的硬编码 `-lstdc++` 构建引用**；没有把其余 feature probe、兼容分派、
`cxxabi.h` 可用 API 或路径前缀碰撞写成缺陷。13 个包是否在当前生产配置
走到该引用均为 `NOT_OBSERVED`，所以它是新增构建配置引用面，不是 13 个
已证构建失败。

扫描分开记录 `PRODUCTION_LIKE`、`TEST`、`BUNDLED_OR_VENDOR` 与
`BUNDLED_TOOLCHAIN`。既有 45 中 39 个进入生产 scope，两个 GCC 身份在
toolchain scope；`connectedhomeip` 的内层 397 MB tar 未在冻结树展开，
三个 jsoncpp 身份的旧命中来自本轮 suffix 外的补丁/生成输入。token 在
libc++ 头中出现只代表精确字符串存在，不代表标准或语义等价。

### 2.3 ABI 与符号面

GCC x86_64 baseline manifest 有 6,057 个 export，平台 T4 libc++ 实测
有 1,969 个 defined export，精确 mangled-name 交集只有 17。版本节点分别
为 GLIBCXX/CXXABI 与 LLVM_22。绝大多数差异会在链接或 loader 版本校验
暴露；更值得保留的运行期边界是对象布局、RTTI、异常、重复 runtime、
加载顺序和同名 ABI 语义。

17 个同名符号的 manifest 不含对象大小和 DWARF，不能由此判定兼容，统一
标为 `NOT_DETERMINABLE_FROM_SYMBOL_MANIFEST`。ARM 精确导出数字没有从
x86_64 外推。

### 2.4 第三方、测试框架与预编译组件

完整结果见 [THIRD_PARTY.md](THIRD_PARTY.md) 与
[VENDOR_AUDIT.md](VENDOR_AUDIT.md)。R22 发布 ELF 图中有 82 个真实
C++ ABI provider 源码身份、383 个 consumer 身份；这是 483 包发布范围，
不是本轮 371 包源码分母。共享边界含 gtest、Boost、ICU、Abseil、
Protobuf、LLVM、gRPC、TBB，也含 bundle/aul/app-core/rpc-port 等平台
provider 泄漏出的 C++ 符号。

gtest 的 153/1450 是 R23 BuildRequires 源码实例口径，125 是 R22 真正
符号相交 consumer 口径，两者不互相替代。`.comment` 中的 GCC producer
标记也不等于闭源或不可重编。

厂商四包的明确回答：

- `drm-service-core-tizen`：`SOURCE_BUILDABLE`；
- `hal-api-drm`：生产实现为 C，C++ 只用于 haltests，`SOURCE_BUILDABLE`；
- `hal-backend-drm-generic`：`SOURCE_BUILDABLE`；
- `pen-wave`：当前 source RPM 只有五架构预制 DSO，
  `PRECOMPILED_ONLY`。

`hal-api-drm` 的“永不迁移”是旧发布边界口径，不是源码技术限制；因此仅由
该口径施加给 app-core/bundle/gtest/rpc-port 的永久约束可以解除。但三个
包的真实 libc++ 重建均为 `NOT_OBSERVED`。

## 三、上游行为差异编目

完整 14 项记录见 [LITERATURE.md](LITERATURE.md) 和
`tables/upstream_behavior_inventory.tsv`。官方/公开材料支持以下风险
类别：一进程混用 STL、dual ABI、ABI 配置与可见性、atomic 布局、静态
runtime 状态分裂、驱动搜索路径、deque/filebuf/stringbuf 等实现行为。

Android/Bionic 的打包结论不能直接外推到保留 glibc 的本平台；异常、RTTI、
分配跨 DSO 的 ABI 拓扑约束仍有可比性。Chromium 的 Linux 迁移更接近本
平台，但其自带 libc++、异常/RTTI 选项和构建封装不同，也不能替代实测。

## 四、主动发现运行时差异

完整评估见 [RUNTIME_DISCOVERY.md](RUNTIME_DISCOVERY.md) 与方法表。

用本平台 patched libc++ 构建 LLVM 能覆盖大量编译、链接和部分启动路径，
但故障回退、取消、future 状态机必须主动触发；构建成功不是运行时兼容
证明。ARM 交叉构建同样不能替代真机 EHABI 运行。

冻结源码静态计数为 libc++ 10,028 个测试文件（8,620 个 pass.cpp）、
libc++abi 98/71、libunwind 30/17。它们不是当前配置的可运行分母。机器上
没有现成 lit；x86_64 可配置后原生执行，armv7l 需要 SDB executor 适配。
可运行总数为 `NOT_DETERMINED`，须由 lit feature probe 后统计。

其他观察面包括 ELF/版本/RTTI 门禁、差分设施探针、fault injection、
differential fuzzing、loader binding、sanitizer/压力测试和 371 包真实
重建。各方法发现能力和盲区不同，本报告不作排序。

## 五、汇总清单

`tables/risk_inventory.tsv` 逐项给出暴露时机、影响面、现有覆盖、与
线程取消问题的可比性和证据限制。`HIGH`/`MEDIUM`/`LOW` 仅表示“若浮现
时与线程取消在发现阶段和定位性质上的可比程度”，不是优先级。

## 六、本次排查的盲区

- 没有全量构建 371 包，编译器模板实例化后才产生的问题仍可能漏掉；
- 没有全量运行 libc++/libc++abi/libunwind 测试，可运行分母未知；
- 没有运行 OOM、locale/tzdb、entropy、`-fno-exceptions`、LTO 或 sanitizer
  配置；
- 没有在 armv7l 对 ABI 导出面做与 x86_64 同规模的精确比较；
- 符号相交证明 ABI 使用，不证明对象、allocator 或异常一定穿越公开 API；
- 文献检索无命中不能证明不存在差异，标准允许的 unspecified/QoI 行为也
  没有有限全集；
- `dlopen` 插件、两份静态 runtime、混合运行时帧、卸载生命周期和加载顺序
  没有平台运行实测；
- source RPM 能构建不代表具有供应商授权、签名或发布权限，这些输入
  `NOT_AVAILABLE`；
- `pen-wave` 若有未交付的私有源码，本语料不可见。

## 七、自行判断与尚存疑问

### 自行判断

1. 把“可运行测试数”定义为 lit feature probe 后实际进入执行的分母，不用
   pass.cpp 文件数冒充；
2. 把导出名差异按类别汇总，不把 7,992 个独有名称写成 7,992 个缺陷；
3. 把符号相交、BuildRequires、编译器 `.comment`、源码可重建性保持为四种
   不同证据口径；
4. 把 `SOURCE_BUILDABLE` 限定为源码和构建规则存在，不推断迁移构建成功；
5. 对 glibc token 与标准允许行为只登记候选风险，不因实现不同即定缺陷；
6. “与线程取消可比性”只描述暴露阶段、故障性质与定位条件，不作为排序。

### 尚存疑问

1. 后续若运行全量测试，需要人工确定 C++ 标准档、ABI/hardening、locale、
   时区数据和板端资源矩阵；
2. 板端是否允许上千次 SDB 部署/执行及哪些临时路径可用，本轮未连板；
3. 第三方 provider 的公开 API 是否实际传递 STL 对象/异常，需逐边场景或
   DWARF/API 审计；
4. 三个可重建厂商包是否具有组织与发布许可，源码语料不能回答；
5. ABI 17 个同名符号的对象形态和语义是否一致，需要 DWARF/探针而非
   manifest。

## 八、纪律与交付声明

本任务没有修改 `codes/` 或任何产品补丁，没有构建、运行或连接开发板，
因此没有触发资源门禁。所有脚本、表、命令原文和退出码均在本目录；无
Gerrit 或外部源码仓推送。项目仓分部分 commit/push，最终索引另见
`INDEX.tsv`。
