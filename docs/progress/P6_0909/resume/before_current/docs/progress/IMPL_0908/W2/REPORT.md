# 条件变量双版本符号：真实库验证

结论：`DUAL_VERSION_READY`，限定于本轮 x86_64 原生真实构建与所列调用方。第四提交为
`72e7a0f21ceeb1a2a4de60fc5137be194c748b90`，接在三个实施提交之后。
代码推送到 `sandbox/lhmax2025/libcxx-noexcept-relief`；远端回执见 raw/015、017。
这不是全部架构、配置或旧对象布局的兼容性证明。

## 设计与 ABI

- `LLVM_22` 保留普通 `wait` 和 system-clock `__do_timed_wait` 的 noexcept 包装；
  新默认节点 `LLVM_22_TIZEN_1` 提供可展开实现，继承旧节点。其余既有导出保留旧节点。
- 原等待算法抽成文件内 static 共用实现；旧包装仅在该实现外保留 noexcept，
  新实现没有该边界。使用类的公开 `native_handle()`，不访问私有字段、不作布局强转。
- ELF 别名的自由函数显式首参数为 `condition_variable*`，对应成员函数的 this；
  其余参数与原成员完全一致。本轮 x86_64 已实测；ARM 的实测留给两架构验证。
- `.symver helper,原成员符号@LLVM_22, remove` 与
  `.symver helper,原成员符号@@LLVM_22_TIZEN_1, remove` 分别建立旧与新默认别名。
  完整两个成员修饰名见提交 diff。`remove` 去除实现名，版本脚本还用
  `local: __libcpp_tizen_cv_*;` 排除 `global: *;` 的误导出；共用实现是 static。
- 平台 CMake 开关 `LIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS` 默认关闭；打包 recipe 打开，
  仅作用于 shared libc++，且检查 Linux 与 `__1` 命名空间。其他命名空间并未实现此方案。
  不带开关保留普通成员定义；本轮没有声称静态链接获得 ELF 双版本保护。
- steady/custom 两个头文件内联入口无可供 DSO 版本化的挂载点，仍须随消费方重编；
  不把库端双版本说成能补回旧调用方的展开表或内联代码。

## 构建与版本检查

完整构建 libc++、libc++abi、cxx_experimental，沿用前阶段的匹配基线与工具链；
不是截取实现的模拟 DSO。构建退出 0，编译警告零，既有 warning 原文作为正向对照。
配置阶段沿用 CMP0219、Clang 开发包缺失两项提示；clang-tidy 测试不可用。

`readelf -V` 确认两节点及继承，`readelf --dyn-syms --wide` 中两个成员各有
`@LLVM_22` 与 `@@LLVM_22_TIZEN_1`，后者才是默认版本证据。
helper 动态导出为零；同一模式在源文件四个 helper 名称上正向命中，且 notify 导出存在。
见 [检查结果](raw/009_checks.stdout)、[动态符号](raw/006_dynsyms.stdout)、[版本表](raw/005_versions.stdout)。
四个既有补丁的源码字节与 SHA 检查仍通过，见 raw/010。

## 绑定矩阵：每格五次

所有程序运行时均加载新完整运行库；每次 `/proc/self/maps` 验证 libc++、libc++abi
来自指定目录，拒绝系统库混入。旧/new 头文件分别编译目标文件，独立链接单/双版本库；
`readelf` 核对每个程序的实际版本需求。身份探针独立翻译单元，不在旧 quiet 工作线程中
增加调用或改变其清理布局。线程为裸 pthread，延迟取消；没有添加 noexcept 调用帧。

| 情形 | 导入版本 | 具体观测 | 次数 |
|---|---|---|---|
| 已链接旧 ELF | LLVM_22 | terminate 标记，探针终止处理器退出 86 | 5/5 |
| 旧 quiet.o 重链接 | LLVM_22_TIZEN_1 | 线程 canceled；frame_cleanup=0、target_unlock=0，竞争者不能获得锁 | 5/5 |
| 新头对象链接新库 | LLVM_22_TIZEN_1 | canceled；frame_cleanup=1、target_unlock=1、contender_rc=0 | 5/5 |
| 新头对象先链接旧单版本库 | LLVM_22 | terminate 标记，退出 86 | 5/5 |
| 旧 system-clock 程序 | LLVM_22 | terminate 标记，退出 86 | 5/5 |
| 新 system-clock 程序 | LLVM_22_TIZEN_1 | canceled=1、cleanup_count=1 | 5/5 |

[逐轮表](bindings_native.tsv)，raw/binding_build 是构建及导入证据，raw/binding_runs 是运行原文。
终止处理器的 86 是测试标记，不是系统默认 SIGABRT 的声称。
旧 quiet.o 的锁滞留是本次选定代码布局的实际值；不能外推所有旧目标文件都滞留，
也不能从线程已回收推断资源已清理。

## 范围与记录

- 本轮尚未验证 armv7l、aarch64、任意编译选项或完整官方套件；后续阶段独立报告。
- `packaging/SOURCE_PROVENANCE` 保留基线的历史 validated tree / LLVM_22 锚点，未把
  新 sandbox 树冒充此前已完成的全平台验证；当前两个节点以本报告与实际 ELF 为准。
  这份历史验证元数据不构成新产品的发布认证。
- 本阶段未出现需人工选择的设计岔路。使用公开原生句柄、独立 C 身份参数、平台 opt-in
  是实现选择，已完整呈现，不修改四个既有补丁。
- 正式构建、运行、检查、提交、推送的命令原文、stdout/stderr、退出码与时间均归档；
  交互式只读翻阅有部分仅留会话记录，不补造命令记录。
- [脚本及 diff 快照](snapshots/) 与 [SHA256](SNAPSHOT_SHA256SUMS) 固定测法；构建物留 tmp。
