# R86 小规模验证与证据索引

## 本轮实际执行的验证

所有命令、stdout、stderr 和退出码分别保存在 `raw/`。本轮没有编译 libc++，没有改源码，也没有实施任何路线。

### V1：顶层 `noexcept`、函数类型与 caller 展开元数据

源码：

- `code/noexcept_symbol_probe.cpp`
- `code/noexcept_callsite_probe.cpp`

命令记录：`raw/012_gcc_abi_probe.*`，退出码 0。

结果：

1. D01/D02/普通模板式函数自身的符号名在有/无顶层 `noexcept` 时相同。
2. 函数指针类型的模板实例不同：`PDoFvvE` 对 `noexcept`，`PFvvE` 对 potentially-throwing。
3. potentially-throwing caller 生成 personality、LSDA、局部清理和 `_Unwind_Resume`；non-throwing 声明的 caller 对该调用没有对应清理 landing pad。

用途：确认“同名可链接”不足以证明旧 caller 能安全接受新实现抛出/forced unwind。

### V2：D05 派生虚析构异常规格约束

源码：`code/destructor_override_probe.cpp`。

命令记录：`raw/013_destructor_override_constraint.*`，外层验证脚本退出码 0；内层预期编译退出码 1。

结果：派生析构声明 `noexcept(false)` 覆盖 non-throwing 虚基类析构被 GCC 拒绝，错误原文在 stderr。因此 D05 不能单独“删标注/放宽”。

### V3：GNU old/new 符号版本表达能力

源码/脚本：

- `code/symver_probe.cpp`
- `code/symver_probe.map`
- `code/symver_probe_tizen_shape.map`

命令记录：`raw/018_symver_mechanism_probe.*`、`raw/019_symver_tizen_map_shape_probe.*`，均退出 0。

结果：独立共享库同时导出 `api@LLVM_22` 和 `api@@LLVM_23`；当前 Tizen `LLVM_22 { global: *; };` 形态追加继承节点也能工作。通配节点会同时导出 helper，是实际设计必须控制的副作用。

用途：只证明本机 GNU 工具链机制可用；没有证明真实 libc++ wrapper 已能在 QuickBuild/所有架构交付。

## 复用的既有动态证据

没有重复 R72 已完成的运行实验。`raw/024_cxx03_wbuffer_and_prior_dynamic_evidence.*` 索引了：

- R72 D01：libc++ 134，libstdc++ clean cancellation；
- R72 D03：libc++ 134，libstdc++ clean cancellation；
- R72 D04：libc++ 134，libstdc++ clean cancellation；
- D02、D05：`NOT_OBSERVED`（有静态可达路径，没有动态样本）。

## 源码与历史证据索引

| 记录 | 内容 |
|---|---|
| `001_resource_gate` | light 资源门禁，退出 0。 |
| `002_source_and_prior_evidence_inventory` | llvm/gcc 源码身份及既有材料盘点。 |
| `003_five_site_libcxx_source` | D01-D05 当前 libc++ 源码与 future 路径。 |
| `005_wbuffer_throwing_operations_corrected` | D05 的虚 `unshift`/streambuf 操作；对 004 错误路径猜测的更正。 |
| `006_libstdcxx_corresponding_source` | libstdc++ 当前对应实现、compat wrapper、wbuffer。 |
| `007`/`008` | GCC PR103382 历史、完整 commit message/diff。 |
| `009` | Tizen libc++ map/spec 与 libc++ ABI 宏。 |
| `014`/`015` | libc++ LWG2135 历史和 Noexcept Policy。 |
| `016` | GCC 取消回归测试与本机 GLIBCXX 双版本符号。 |
| `017` | R85 三架构真实 libc++ D01/D02 `@@LLVM_22` 导出。 |
| `020` | D03/D04/D05 无动态符号及 hidden/header 属性。 |
| `021` | libstdc++ 修复前后五站点矩阵。 |
| `022` | 两库 future 等待实现结构。 |
| `023`/`024` | libc++ C++03 镜像及 R72 动态证据索引。 |

## 保留的异常/失败记录

- `raw/004_*`：最初猜测了不存在的 granularized streambuf 路径，命令退出 2；随后用 `005_*` 定位正确文件并保留原失败记录，没有篡改。
- `raw/010_*`：尝试调用不存在的 `clang++`，退出 127；随后使用已安装 GCC 13 完成 V1。
- `raw/011_*`：一次受限编译器盘点进入不可中断 I/O，人工终止已核验的精确 PID；记录退出 143 及 `interruption.txt`。之后没有再作宽目录遍历。

这些属于取证命令自身问题，不改变事实判据。

## 未执行的推荐性验证

由于任务要求只评估、不实施，本轮没有临时修改 libc++ 来运行“删掉某处后能否取消”的真实库测试。若人工选择路线，需按 `ROUTES.md` 的相应验证矩阵另立实施任务。
