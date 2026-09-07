# R111 四项待决事项的补充事实核查

## 总览

本任务只核查事实，不修改平台源码或四个补丁，不实施方案，不给出处置建议或
优先级。

| 核查项 | 方法 | 结果状态 | 详细材料 |
|---|---|---|---|
| `wbuffer_convert::__close()` 调用面与影响 | 静态核查 | 已完成；外部调用方依赖仍为 `NOT_OBSERVED` | `ITEM1_WBUFFER_CLOSE_REPORT.md` |
| Boost 边界在 Base 适配后的状态 | 静态清单 + 真实 aarch64 产物符号核查 | 已完成计划范围与 provider 覆盖核对；consumer 实际迁移产物及发布时序为 `NOT_OBSERVED`/`NOT_AVAILABLE` | `ITEM2_BOOST_TRANSITION_REPORT.md` |
| 解除条件变量异常规格后的展开 | x86_64 原生实测 | 四种取消路径和普通无人捕获错误均完成，每格不少于 5 次；可选 armv7l/aarch64 为 `NOT_OBSERVED` | `ITEM3_CONDVAR_UNWIND_REPORT.md` |

## 逐项结果

### `wbuffer_convert::__close()`

**静态核查事实：**当前 libc++ 主实现和 C++03 镜像中，`__close()` 各只有一个
实际调用点，均为析构中的独立表达式，返回值无人使用。析构随后执行的 codecvt
及 owned buffer 删除不依赖 `__close()`；限定为取消析构调用或取消其内部
`sync()` 时，当前可见的资源释放语句、数据布局、函数签名和 ABI 标注不变。
`seekoff`、`seekpos` 与显式 `pubsync()` 路径仍各自同步。

**静态核查事实：**`wstring_convert` 析构只删除 codecvt，没有 `__close()`、
`sync()` 或 stream-buffer 析构同步，因此没有同一问题，也不是该五处站点之一。

**限制：**外部使用代码是否依赖作用域结束时自动写出尾部，为
`NOT_OBSERVED`；既有覆盖外的产品/应用材料为 `NOT_AVAILABLE`。

### Boost Base 适配后的 90 条边

**静态清单事实：**13/13 consumer source RPM 全部属于既定迁移计划（2 个
`PHASE1`、11 个 `PHASE2`），但本项没有取得它们的 libc++ 重建产物，所以
“已完成迁移”统一为 `NOT_OBSERVED`。90/90 条边均以 Boost 的五类 provider
子包为一端，没有不涉及 Boost 的边。

**已有构建事实：**提交 `4168e873584ae50a12733262026c007dce18ec89` 在 Base
Toolchain 的 Clang 条件下把 Boost 构建/安装切换到 libc++；三架构构建矩阵中相关
provider 不依赖 libstdc++。提交本身不改 consumer、Boost API 或 SONAME。

**真实产物静态核查事实：**将 30 条 aarch64 冻结 consumer 旧边与真实 libc++
Boost provider 动态符号连接，30/30 仍有至少一个同 mangled name 符号；15 条全部
旧符号保留，15 条部分保留。因而在“新 provider 已部署、旧 consumer 未迁移”
这个前提下，旧边不会统一因名字改变而消失；同名边仍跨两个 C++ ABI，另有部分
名字已变化。本项没有运行该新方向组合，具体运行现象为 `NOT_OBSERVED`。

**时序事实：**构建 provider 在前不等于必须先部署；同快照切换在技术概念上可
区分于分批部署。当前没有发布系统的原子快照、双版本共存和回滚能力材料，记
`NOT_AVAILABLE`，不能确认实际过渡窗口是否发生或是否可避免。

### 条件变量取消展开与无人捕获错误

**x86_64 原生实测：**`wait`、system clock、steady clock、自定义 Clock 四种
调用各测两种规格、每格 5 次。保留规格为 20/20 terminate、0/20 清理；移除规格
为 20/20 强制展开穿过边界、20/20 清理、20/20 join 得到
`PTHREAD_CANCELED`，且 20/20 进程正常返回 0。头文件内联的 steady/custom 两处
由 overlay 头文件重编用例生效。

**x86_64 原生实测：**锁未持有、无人捕获时，两种规格最终均终止；各自 5 次默认
handler 均为 SIGABRT，诊断同为 `std::__1::system_error` / `mutex not locked`；
各自 5 次 instrumented handler 均显示局部清理未执行。回溯差异是：保留规格在
non-throwing 边界经 `__clang_call_terminate` 终止；移除规格后异常经过无规格
`wait` 边界，最终作为无人捕获异常终止。

**限制：**armv7l、aarch64 均未执行，记 `NOT_OBSERVED`；结果不向其他架构、
runtime 版本、异步取消或另含 non-throwing 帧的调用链外推。

## 本任务中的限定解释与自行判断

以下只说明核查口径，没有替人工作方案判断：

1. 将“libc++ 源码树中全部调用点”限定为当前 revision 已跟踪的
   `libcxx/include` 与 `libcxx/src`，并同时覆盖主实现和 C++03 镜像；测试源码和
   仓外 consumer 不冒充库实现调用点。
2. 将“那 90 条边”按 R82 的冻结 90 条边清单、并用 R110 最新分类状态连接；不把
   平台其他 C++ 边加入该集合。
3. 将“是否都会改用 libc++”拆成“是否列入迁移计划”和“是否已有实际 libc++
   产物”两个事实，避免从计划成员身份推断完成状态。
4. 将“三个定时变体”对应为 system clock、steady clock、custom Clock 的
   `wait_until`，与普通 `wait` 合计四种测试形态；steady/custom 作为头文件内联
   路径重编。
5. 将“线程干净退出”的实测判据限定为：强制展开通过、worker 局部 RAII 清理
   执行、创建方 join 得到 `PTHREAD_CANCELED`、进程不终止。没有把它扩写成对所有
   外部资源或应用状态的证明。
6. 可选架构未纳入本次结果：任务明确要求 x86_64 原生，armv7l/aarch64 仅在时间
   允许时执行；本轮没有物理板，也没有把 QEMU 用户态作为同等架构结论。

## 尚存疑问与缺失材料

没有出现必须在本任务中二选一、且会改变核查方法的未决岔路。仍缺下列外部事实：

- 外部产品/应用代码是否依赖 `wbuffer_convert` 析构自动同步；所需源码、BOM 或
  真实调用材料当前 `NOT_AVAILABLE`。
- 13 个 consumer 各自的 libc++ 适配、重建和发布完成状态；本项
  `NOT_OBSERVED`。
- 发布系统能否原子切换 Boost 与 consumer、是否允许两套包共存、实际部署顺序；
  发布能力材料 `NOT_AVAILABLE`。
- 条件变量行为在 armv7l/aarch64 的结果；本项未测，`NOT_OBSERVED`。

这些缺口只限定证据强度；本文不替人工裁决如何处理。

## 证据组织

- `raw/`：资源闸门、命令原文、stdout、stderr、退出码；失败尝试也保留。
- `code/`：只用于核查和实测的脚本/探针，均有 SHA256 快照。
- `tables/`：三项机器可读结果。
- `build/`：可复现实测的中间产物；结论所需身份、hash、加载关系和代表性输出已
  固化到 tracked raw 记录，不依赖把 ELF 纳入提交。

三次资源检查均为 light 且通过。完整细节、输入身份、正向对照、失败恢复和限制见
三份分项报告。

命令记录完整性初检 `raw/044_*` 因把自身尚未生成的 exitcode 也列为检查对象而
退出 1；修正为排除当前记录后，`raw/045_*` 检查通过。两次外部终止导致无法取得
原始 shell 退出码的命令以 `NOT_AVAILABLE` 写入对应 `.exitcode`，没有伪造数值。
