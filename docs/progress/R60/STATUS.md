# R60 暂停状态

上游基线为 LLVM 仓提交 `5ed6c77278dfa7a470667cf1a137723d3c96fe60`。实验副本依次应用既有层 A、层 B、当前层 C 头文件批和库内批；`codes/` 未修改。

## 已完成并在 x86_64 验证

- F1：同时修正 badbit 与 failbit 汇聚函数。普通或外来异常的条件重抛保持在活动 handler 内；定向用例退出 0，与 libstdc++ 一致。
- F2：`basic_istream::get(streambuf&, delim)` 的 typed handler 在重抛前调用不抛异常的状态提交路径；实测 `badbit=1`、取消完成、退出 0，与 libstdc++ 一致。
- M1-A：forced handler 在共享状态锁下恢复 deferred 标记并通知等待者；等待循环在唤醒及重试后重新检查 deferred。单等待者用例 callable 调用 2 次并取得 42。4 个并发等待者用例当前版和 libstdc++ 各重复 20 次，均为 20/20 通过。

## M1/M2 共用基础设施

两者共用 `__assoc_sub_state::__handle_forced_unwind`：同一把共享状态锁、同一 ready/已有值幂等检查、同一不向外抛异常边界和同一 `notify_all`。M1 的动作是恢复 deferred；M2 的动作是未 ready 时提交错误。两者不能共用同一个状态结果，但共用一套同步与提交接口。

## M2 暂停状态

当前实现严格采用“ready 时 no-op”：未 ready 时原子提交捕获到的 `broken_promise`，ready 时保持现有结果，随后重抛原强制展开。R59 的确定性窗口用例在 x86_64 上超时 124；libstdc++ 同格返回 `broken_promise`、退出 0。该冲突已交人工重新裁决，当前实现仅供审阅，不是最终结论。

## 已构建但尚未运行的范围

x86_64 与 armv7l 的 libc++abi/libc++ 均构建成功，未出现指向本次修改源码的新增警告。armv7l 测试件也已交叉编译成功，但 R60 暂停后未连接开发板、未部署、未运行任何 armv7l 格。

## 证据说明

- x86_64 原始矩阵：`evidence/raw/matrix/x86_64/`。
- 构建与编译命令：`evidence/raw/build/`；调用入口和退出码：`evidence/commands/`。
- 运行时身份：`evidence/raw/matrix/x86_64/ldd_current.txt`、`ld_debug_current.log` 和 `sha256.txt`。
- 两次早期语法探针因工具名及缺少完整 sysroot 配置失败，原始记录保留在 `008`–`010`；正式 CMake 构建及测试件编译随后成功，不以失败探针替代正式结果。
