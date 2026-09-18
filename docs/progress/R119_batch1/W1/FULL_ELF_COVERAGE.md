# 扩大后的消费方索引与判定边界

本轮将 x86_64 消费方定位范围扩大至冻结清单中全部 3,085 个 `runtime_elf=YES && cpp_indicator=YES` 的 ELF，逐个读取动态符号，3,085 次读取均成功。完整输出压缩保存于 `full_elf/*.stdout.gz`，各 ELF 的来源、SHA256、命令与退出码见同编号 JSON。SQLite 工作缓存留在 `tmp/`，不作为唯一证据。

这不是全平台所有 ELF 的完整语义覆盖：该架构冻结清单共有 10,946 个运行 ELF；未扩扫其余 C 形态产物。armv7l、aarch64 等未在本轮扩大索引，不将 x86_64 阴性外推到其他架构。现有安装头索引覆盖情况另见 `HEADER_COVERAGE.json`。

索引队列只是定位工具，不能直接作为跨包结论。新闭合的证据另外比对原始修饰名、消费者 UND、提供者定义、DT_NEEDED/SONAME、安装头及源码包含链。

本轮实际遇到并排除的误计：

- Rust 符号中的 `std::thread::Packet` 不是 C++ 的 `std::thread` 对象。
- `app-rootstrap` 等包中携带的同名库不自动成为该消费方的实际 provider。
- `gcc`、`llvm` 源包也提供编译器接口，不能将这些源包全部当成标准库运行库。现行运行库关系列已改用实际运行库文件、带版本的符号交集及消费者 NEEDED，依据 `RUNTIME_CALL_PROOFS.json`。早期中间记录中的 49 项旧启发式计数已被现行结果取代。
- Json::Reader 的同名实现还出现在 nnfw 中；xwalk 消费方的 `NEEDED=libjsoncpp.so.26` 与 jsoncpp SONAME 相符，不能把 nnfw 副本也计为其 provider。
- 返回类型、隐式 this 指向对象的成员，以及异常对象传播通常不能靠函数名字搜索穷尽。`deque`、`stack` 的本次证明就是通过 Reader 的实际成员与 this 调用链补齐。

阳性证明可以闭合“存在一条真实包边”；没有名字交集不够证明“没有任何包边”。因此目前 34 项 CROSS_PACKAGE、66 项未定，不把全部 100 项记录齐备称为全部定界完成。

补充命令偏差记录：`060_json_context` 的显示辅助器不兼容新增证据 JSON 的字段结构，退出 1，不是证据验证失败；`061_json_usage` 未加 `-uuu` 受忽略规则影响，退出 1，随后 `062` 已纠正。两次失败均未用于阴性判断。
