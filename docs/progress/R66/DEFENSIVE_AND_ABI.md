# R66 标准库防御性构造与 ABI 导出面对照

## 计数口径

脚本 `code/scan_stdlib_features.py` 对冻结源码树逐文件扫描，按 CODE、TEST、
DOCUMENTATION、ABI_METADATA 分开。报告使用 CODE；“hit”是正则 token 出现
次数，不是函数数、语义分支数或等价实现数。完整逐位置表和摘要分别为
`tables/stdlib_feature_hits.tsv`、`tables/libcxxabi_feature_hits.tsv`、
`tables/*_feature_summary.tsv`。命令与退出码见 `commands/032`、`033`。

## 防御性构造

汇总见 `tables/defensive_construct_summary.tsv`。机械方法重新识别出的唯一
“libstdc++ 生产代码具有专用外来异常类型、libc++ 整层缺失”的家族仍是
`__forced_unwind`：65 个实际 catch/rethrow 防护。另有 34 个
`__gnu_cxx::forced_error` catch，全部属于异常安全测试/注入设施，不是运行
时兼容标记。生产代码对 `bad_alloc`、`ios::failure`、`system_error` 等标准
异常的恢复在两边都有，形态与状态机不同；不能因 token 同名就宣称等价。

### -fno-exceptions

libstdc++ 用 `exception_defines.h` 把 `__try`/`__catch`/重抛宏变成恒真、
恒假或空操作；libc++ 以 `_LIBCPP_HAS_EXCEPTIONS` 条件分支和各设施的
throw-or-abort 形态实现。两者都覆盖无异常构建，但不是可逐 token 对齐的
统一宏层。这里发现的是形态差异，未做无异常配置构建，运行结果
`NOT_OBSERVED`。

### 线程取消与线程退出

upstream libstdc++ 的类型定义、Itanium/ARM personality 映射和 65 个库内
防护形成闭环。upstream libc++abi 能看到 `_UA_FORCE_UNWIND`，但 upstream
libc++ 没有 GNU marker 类型或库内 typed guards。这个缺失与项目已知故障
相同，已被三层补丁覆盖；本报告没有把冻结实验补丁写回 `codes/`。

### 分配失败与状态不一致

libstdc++ `libsupc++/eh_alloc.cc` 有 emergency pool；libc++abi
`fallback_malloc.cpp` 有 emergency stash，故“libc++ 一侧完全无异常分配
兜底”不成立。二者的池大小、环境调优、锁与失败行为不同，本任务未注入
OOM，语义差异 `NOT_OBSERVED`。future/iostream 的 shared-state、状态位、
`exception_ptr` 与 `terminate` 路径两边均大量存在，项目既往运行测试已经
证明若干差异；本轮只编目，不重复实现或验证修法。

### glibc 耦合

libstdc++ CODE 中机械命中 144 次，libc++ 65 次，libc++abi 0 次。二者都
有 glibc 条件代码，覆盖不相同。libstdc++ 独有的可核实例子包括：

- filesystem 临时目录读取使用 `secure_getenv`；
- exception emergency pool 从 `GLIBCXX_TUNABLES` 读取调优；
- guard 快路径读取 `__libc_single_threaded`；
- random/entropy 和 long-double compatibility 的 glibc 版本条件。

libc++ 未出现同 token 不等于未处理同需求，可能直接使用 POSIX/API 或采用
不同算法。因此这里只登记“平台耦合形态不同”和潜在后果类别：环境变量
安全语义、线程安全快路径、随机源/兼容符号选择、异常低内存行为。除线程
取消外，没有把它们升级成已证缺陷。

## ABI 与符号面

`compare_abi_manifests.py` 比较 GCC x86_64 baseline manifest 与 T4 平台
libc++ x86_64 实测 defined exports。输入 SHA 和命令见 `commands/031`。
结果：libstdc++ 6,057 个 manifest export、libc++ 1,969 个 export；精确
mangled name 交集只有 17。独有符号按机械类别：

| 类别 | libstdc++ 独有 | libc++ 独有 |
|---|---:|---:|
| C++ library API | 5,079 | 1,526 |
| RTTI/vtable | 780 | 425 |
| C++ ABI runtime | 86 | 0 |
| 其他 | 95 | 1 |

这是不同实现命名空间、显式实例化和 runtime 拆分的结果，不等于 7,992 个
独立缺陷。完整逐符号表在 `tables/x86_64_abi_only.tsv`。

版本节点完全不同：GCC manifest 有 GLIBCXX_3.4.*、CXXABI_1.3.*、
CXXABI_TM/FLOAT128；平台 libc++ 导出统一使用 LLVM_22。直接携带
`GLIBCXX_*` 需求的预制 DSO 在换库时通常在链接或 loader 版本校验阶段
立即失败。正常 `std::__cxx11` 对 `std::__1` 符号差异也通常链接期暴露。

只在运行期暴露的主要是：

- header-only/inline 对象布局不一致但没有对应 UND；
- RTTI/typeinfo、exception_ptr 和异常跨 DSO/跨 runtime；
- 两份静态 runtime 或加载顺序导致同一逻辑类型/全局状态不唯一；
- unversioned C++ ABI 函数被错误 provider 满足；
- 同名函数接口相同但状态机/错误语义不同。

17 个精确同名主要是 `logic_error`/`runtime_error` 构造赋值、
`nested_exception` 析构、`current_exception`/`uncaught_exception(s)` 和
`__throw_bad_alloc`。manifest 没有对象大小和 DWARF，**不能从本次符号表
判断其同名语义或形态是否一致**，记
`NOT_DETERMINABLE_FROM_SYMBOL_MANIFEST`；没有把“同名”伪写成“兼容”。

## 范围限制

- 数量对照使用 x86_64；ARM 的架构辅助符号不同，未把 x86 数字外推为
  ARM 精确数字。
- GCC 输入是上游源码所带 x86_64 baseline manifest，libc++ 输入是平台
  T4 实测库；二者版本身份不同，适合证明导出面/版本体系分离，不适合做
  同版本替换兼容承诺。
- 未运行 OOM、locale、entropy 或无异常构建；潜在后果均明确保持“候选”。
