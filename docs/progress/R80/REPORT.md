# R80：D-09 同名符号与 D-11 头文件对象布局确认

## 结论

状态：`COMPLETED`。

- D-09：x86_64 口径的 17 个精确同名符号全部具有同一标准层含义，没有发现
  “同名异义”。在各自正确配对的 runtime 中，分组行为探针输出逐字一致。
  但它们不是任意混用兼容承诺：异常消息的引用计数存储、`exception_ptr`、
  未捕获异常计数和 RTTI 均由各自 runtime 所有；双 runtime、无版本引用、
  `dlsym`、静态 runtime 或错误加载顺序仍需运行时绑定证据。
- D-11：对象布局差异确定存在。33 个代表对象中 10 个的 x86_64
  `sizeof/alignof` 不同；最小跨库用例在链接和装载均成功后，把生产方长度 4
  读成 `106884723786536`，证明该类差异确实可以绕过链接器、到运行时才显现。
- 平台迁移存在真实的触发边界，但当前发布物尚未形成“新旧两侧各编一半”的
  实测故障：R22 ELF 图中 Boost 共享库有 13 个 consumer source RPM、75 个
  架构化 consumer ELF、90 条真实 C++ 符号相交边；其中
  `boost::filesystem::path` 的公开头内含 `std::string`。若 provider 与 consumer
  分批换库，这就是对象模型穿越 DSO 的可达路径。现有发布物两侧仍同为
  libstdc++，所以平台现状故障记 `NOT_OBSERVED`。

## 口径与输入

- D-09 清单严格重放 R66 的两个输入与解析器：
  `codes/gcc/libstdc++-v3/config/abi/post/x86_64-linux-gnu/baseline_symbols.txt`
  （6,057 exports）和 T4 平台 x86_64 `libc++.so.1.0` 实测导出
  （1,969 exports）；交集仍为 17。
- 两边符号版本并不相同：libstdc++ 为 `GLIBCXX_*`/`CXXABI_*`，libc++ 为
  `LLVM_22`。因此常规已版本化引用通常先在链接或 loader 暴露；本报告所述
  静默运行时风险针对无版本绑定、返回类型/成员布局未进入符号名、双 runtime
  或静态复制等边界。
- D-11 布局探针用系统 GCC 13.3 编译 libstdc++ 侧；libc++ 侧用工作区既有
  GBS 缓存中的 Clang/LLVM 21.1.1 RPM（仅解包到 `progress/R80/build`，未安装）
  编译，并链接 T4 x86_64 平台 libc++/libc++abi。源码与完整命令均存档。

## D-09：17 项逐项判断

完整逐行表见 [tables/d09_common17.tsv](tables/d09_common17.tsv)。17 项为：

1. `logic_error` 的 char-pointer 完整/基对象构造（2 项）、复制完整/基对象构造
   （2 项）及复制赋值（1 项）；
2. `runtime_error` 的同一五项；
3. `nested_exception` 的 deleting/complete/base 三个析构入口；
4. `std::__throw_bad_alloc()`；
5. `std::current_exception()`；
6. `std::uncaught_exception()` 与 `std::uncaught_exceptions()`。

### 静态可判定部分

- 两套库的 `logic_error`/`runtime_error` 都是 `std::exception` 基类加一个指针
  大小的引用计数字符串；本机探针两者均为 16 字节、8 字节对齐。libc++ 的
  `__libcpp_refstring` 明确采用与旧 GCC COW string 相同的
  `length/capacity/refcount/data` 形态以维持异常 ABI。两边构造、复制、赋值的
  标准目的一致。
- `nested_exception` 两边均为虚类加一个单指针 `exception_ptr`，均为
  16 字节；三个符号只是 Itanium ABI 的析构变体，含义一致。
- `current_exception` 两边都读取当前 caught chain、拒绝无法引用计数的 foreign
  exception 并保留 native primary exception；含义一致。
- 两种 uncaught API 都读取本 runtime 的线程局部计数；bool API 判断计数是否
  非零，int API返回计数；含义一致。
- `__throw_bad_alloc` 两边在启用异常时均抛 `std::bad_alloc`；禁用异常时均不
  返回而终止，诊断形式不同但不是相反语义。

### 运行确认与剩余条件

分组探针对构造/复制/赋值、当前异常、展开期计数、空 nested exception 与
bad_alloc 逐项运行。libstdc++ 和 libc++ 两份输出逐字相同，均退出 0，见
`raw/d09_*_run.txt` 与命令 042。

下列不是已观察故障，而是源码证明无法由 matched-pair 探针排除的条件：

- Linux 上 libc++ 的 refstring 不启用 Apple 专用的 GCC empty-string singleton
  检查；若 GCC 创建的空消息异常被 libc++ 的复制/赋值/析构路径处理，需单独
  运行确认引用计数与释放行为。
- `exception_ptr` 和 `nested_exception` 的外层大小虽相同，其指针指向的异常头
  与引用计数属于具体 runtime；错误 provider 会操作另一 runtime 的对象。
- 双 runtime 时两个 uncaught counter/caught chain 是不同状态；函数绑定到未
  执行当前展开的 runtime 时，可能得到 0/null。

因此：单 runtime、正确 libc++/libc++abi 或 libstdc++/libsupc++ 配对下不需要
额外语义确认；若平台承诺双 runtime 间异常对象穿越或无版本符号可互换，仍缺
加载顺序/绑定轨迹、foreign/native exception 与空消息跨 provider 运行数据。

## D-11：分布、数量与布局

这类对象不能给出有限“实例总数”：容器、词汇类型、智能指针、atomic、
iterator、range view 和闭包大多是头文件模板，元素、allocator、traits、extent
和闭包组合令实例数无上限。按家族的分布见
[tables/d11_header_object_families.tsv](tables/d11_header_object_families.tsv)。

为给出可复核的粗略规模，单行词法代理统计得到：libstdc++ 854 个 include
文件中 81 个文件命中 393 个 class/struct definition 行；libc++ 排除复制的
`__cxx03` 树后，1,104 个文件中 657 个命中 2,478 行。它们包含内部类型、
遗漏跨行声明，也不是唯一 public type 数，只能说明实现规模为数百至数千个
定义点，不能冒充 ABI 对象总数。

33 个代表对象的完整结果见
[tables/d11_layout_comparison.tsv](tables/d11_layout_comparison.tsv)：

- 明确不同 10 个：`any`、`function<void()>`、`deque<int>`、`map<int,int>`、
  `set<int>`、`unordered_map<int,int>`、`unordered_set<int>`、`string`、
  `future<int>`、`promise<int>`；
- 其余 23 个仅 `sizeof/alignof` 相同，不能据此推断成员偏移、节点、控制块、
  iterator 或操作语义兼容。

### 最小可运行用例

`layout_provider.cpp` 由 libstdc++ 构建共享库，返回
`std::deque<int>*`；`layout_consumer.cpp` 由 libc++ 构建。Itanium ABI 不把
普通函数返回类型编码进 `make_payload()` 的符号名，故 consumer 的 UND 与
provider 的 DEF 精确匹配。ELF 核对显示进程同时需要 provider、libc++、
libc++abi，而 provider 需要 libstdc++；链接、装载均成功。

运行输出：

```text
provider_sizeof=80
consumer_sizeof=48
provider_view=4
consumer_view=106884723786536
RUN_EXIT=42
```

退出 42 是用例主动标记“两侧视图不一致”，不是崩溃。对象由原 provider
销毁，避免把本用例扩大成跨 allocator 释放测试。

### 平台迁移触发路径

- R22 发布 ELF 图总体已有 82 个真实 C++ ABI provider source identity 和
  383 个 consumer identity；这证明跨 DSO C++ 调用边存在，不证明每条都传
  对象。
- 对 Boost 的收窄复核确认 13 个 consumer source RPM、75 个架构化 ELF、
  90 条 true symbol-intersection 边，覆盖 aarch64/armv7l/x86_64。生产路径
  包括 `security-manager` 对 boost-filesystem/program-options/iostreams、
  `dotnet-launcher` 对 boost-filesystem、`libzypp` 对 boost-thread；另有测试
  路径。
- 冻结 Boost 头的 `boost/filesystem/path.hpp` 定义 `path::string_type` 为
  `std::basic_string`，对象成员 `m_pathname` 即该类型；R22 consumer 符号中有
  参数为 `boost::filesystem::path const&` 的接口。这种函数名只编码 Boost
  类型，不编码其内部 std::string 布局，符合最小用例所证实的静默边界。
- 当前边两侧都是既有 libstdc++ 构建，坏行为没有发生；触发条件是 provider
  与 consumer 非原子迁移、预制旧 DSO 与新 consumer 共存，或插件/静态
  runtime 形成两套对象模型。全量同一工具链重建并保持边两侧一致时，没有
  该“跨实现解释同一对象”的条件。

事实判断是：平台迁移存在需要逐边确认的布局边界，不能用“链接成功”关闭；
但本轮没有把 82 个 provider 的全部公开 API、插件加载顺序和运行调用全部
执行，因此平台故障数量仍为 `NOT_OBSERVED`，不能从 90 条 Boost 边推成 90
个故障。

## 未覆盖范围

- ARM/aarch64 的逐类型布局和最小用例未运行；33 项结果只代表 x86_64。
- ABI 配置宏、debug iterator、sanitizer、不同 allocator/traits、`-fno-exceptions`
  和静态链接组合未展开。
- 没有获得所有平台 DSO 的 DWARF，也未验证成员偏移/控制块/节点布局全量。
- 未做 82 provider × 383 consumer 的 API 语义审计和产品场景运行。
- D-09 混合 runtime 的 empty-message、foreign exception、symbol preemption 与
  `dlsym` 场景未运行，记 `NOT_OBSERVED`。

## 过程异常

- 命令 007 首次调用错 R66 parser API，退出 1；命令 008 按脚本实际函数名
  重跑，恢复 17 项。
- 命令 020/021 证实 GCC 13 无法编译 LLVM 22 头；没有修改头文件，而是解包
  既有缓存 Clang 工具。解包后依次缺 `libLLVM.so.21.1`、`libxml2.so.16`，
  命令 029~032 从同一缓存补齐，未安装系统包。
- 命令 028 的内层显式 `exit 2` 绕过日志 footer，其退出码未由 wrapper 捕获，
  记 `NOT_OBSERVED`；现象与后续修复均完整保留。
- 首个跨布局用例用 `std::string` 返回值；libstdc++ 的 `abi:cxx11` tag 进入符号
  名，consumer 链接退出 1，说明该变体会提前暴露。命令 036 改用无该 tag 的
  `deque` 后验证静默运行时差异。两版源码演进与错误原文均存档。

## 材料

- 命令原文与退出码：`commands/`
- 完整 stdout/stderr：`raw/`
- 用例源码：`code/`
- 逐项与布局表：`tables/`
- `build/` 仅含可重建 ELF 与 311 MiB 临时工具解包，不纳入项目仓提交。

本任务未使用开发板，未修改 `codes/`，未向 Gerrit、正式分支或 LLVM 上游
提交。
