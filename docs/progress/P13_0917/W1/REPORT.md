# 异常边界修复：当前反向组合通过

结论：W1_FIXED_IN_MEASURED_CONFIGURATION。原后端分配失败用例由 5/5 SIGABRT 变为 5/5 返回 -ENOMEM、进程存活；GNU-only 同一用例仍 5/5 返回 -ENOMEM。

## 事实与修复

当前真实组合为 **libc++ 前端 → GNU 后端 → 真实 GNU EWK**，不是 libc++ 后端调用真实 libc++ EWK。后者产物仍不可得。旧记录不能被转述成已经确定是 libc++ 后端抛错。

1. 旧后端已有 bad_alloc/length_error typed catch。绑定记录显示 GNU TU 的 personality/typeinfo 曾落在 LLVM_22，而 GNU 库的抛出入口使用 CXXABI_1.3；见 `raw/003_binding_probe.*`。这里是实际 ELF 绑定观察，不靠标准库名字推断。
2. 仅补 invalid_argument/std::exception/catch-all 后，5/5 存活但返回 -EIO，**不是**要求的 -ENOMEM。见 CATCH_ONLY.json。不能把“不崩了”当修复完成。
3. 固定每个 TU 对当前实际 runtime 的版本化未定义引用；并给 TU 内 weak/COMDAT 辅助定义加 ABI 侧后缀，防止 DW.ref personality、终止辅助函数等同名弱定义在同 DSO 合并。版本从输入 ELF 读取，映射和前后对象 SHA 都逐个落盘（`*_binding.json`、`raw/pin_*`）。无 LD_PRELOAD、无依赖加载顺序来选择 ABI，无运行库重建。
4. 原工具链 linker 无法完成相关版本化 vtable 链接，禁用 relaxation 仍失败；使用本机 GNU gold 2.42 构建成功。失败 stderr 保留。**这是新增构建条件，产品 linker/GN/RPM 未验证。**
5. 在 EWK 后端、前端回调与公开出口加同步异常映射；catch 内仅赋/返整数，不调用 what、不构造对象、不分配。vector 两侧也补充同类出口，运行回归留给 W2。

细化的映射候选：bad_alloc→ENOMEM；length_error→EOVERFLOW；invalid_argument→EINVAL；其他 std::exception/非标准异常→EIO。公开 EWK 仍是 Eina_Bool，内部 POD 才用负 errno。取消、强制展开和析构双异常不包含在 catch-all 的安全声明中。

## 实测

| 项 | 单位与结果 | 记录 |
| --- | --- | --- |
| 原后端分配失败 | 双运行时、GNU-only 各 5 个进程，均返回 -12 且存活 | PINNED_ALLOCATION.json |
| 逐阶段 bad_alloc 注入 | 两种放法 × 11 个位置 × 5 次 = 110 次，全部 false/ENOMEM | W1_ERRORS.json |
| 合法 64 MiB 输入触发后端复制分配失败 | 两种放法各 5 次，共 10 次，全部 false/ENOMEM | 同上 kind=10 |

每个完整链路进程先成功调用真实 EWK 预热，再确认 maps 包含双库、转发层和真实 provider，dladdr 确认入口是转发层。注入位置覆盖输入之前/之后、四个初始字段复制之后、provider 返回后，以及四次转换输出之后。逐次断言四字段内容、存储地址、expiry 与调用前一致；失败之后恢复调用也通过。

资源守卫作用域计数 created=1/destroyed=1，守卫析构位于局部字符串析构之后。这证明所测控制流完成清理作用域，**尚不等同于实际 malloc/free 的无泄漏证明**；W2 将增加分配计数。原超大借用长度的单元用例完整保留，并另加真正有效的 64 MiB 输入对照，避免只依赖构造的超大指针长度组合。

## 复现与限制

源码/脚本快照在 code/，SHA256 在 SNAPSHOT_SHA256.json；变化对照 ADAPTOR_CHANGES.patch。执行脚本仍放 tmp/P12；入口 `p13_driver.py pinned` 与 `p13_driver.py w1_faults`，会使用新的 P13 证据目录和 build_P13，原 P12 证据不覆写。重复执行需新原始记录前缀；不应覆盖既有记录。

当前通过只是固定 x86_64 运行库/工具链组合的边界修复；不是对任意 runtime 版本或全部异常对象的证明。POD 借用区必须真实可读，不能修复悬空指针或损坏对象。生产方向 NOT_AVAILABLE，armv7l NOT_OBSERVED_BOARD_OFFLINE；不测试取消、不占用开发板。
