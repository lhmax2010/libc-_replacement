# MLGO release-mode 观测方式修正

首次脚本把 `print<inline-advisor>` 作为内联后的附加诊断 pass。模型已经
输出多轮 `[MLInlineAdvisor]` 状态后，该打印 pass 在
`MLInlineAdvisor::print` 中访问已删除函数并段错误（内部退出码 139）。
栈顶是 `llvm::Value::getName()`，调用点为 `MLInlineAdvisor.cpp:566`；
不是 `ReleaseModeModelRunner` 或 AOT 生成类的 `Run()`。

不修改 LLVM 源码。正式观测改用 LLVM 已有的 `inline-ml` optimization
remark：release advisor 的 `MLInlineAdvice::record*` 路径会把 Callee、
全部输入特征和 `ShouldInline` 写入 YAML。该方式能同时证明：

1. advisor 是 `inline-ml`，不是默认启发式；
2. AOT runner 已返回具体决策；
3. pass 正常结束且输出 IR 可再次通过 verifier。

x86_64 独立探针退出码 0，YAML 中有 6 个 `ShouldInline` 字段。原始打印
pass 崩溃保留为测试工具局限，不作为模型契约失败；更新后的脚本再进行
一次正式执行。
