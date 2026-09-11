# 脚本版本与计数边界

最终脚本/探针快照见 `snapshots/scripts/`，摘要见 `SCRIPT_SHA256SUMS`。规范结果可由最终源码与相应命令复核，旧日志不删除。

- x86_64 EWK 主矩阵：`ewk_matrix.json`，原始命令 `raw/ewk_*_to_gnu_[1-5].command.txt`。`ewk_probe.cpp` 在这些测试后未修改；GDB 为额外诊断，不计五次矩阵。
- Dali 早期 `dali_matrix.json` 使用初版 C 探针。随后增加可选 Control 前置检查，再增加加载 binder 前的 core 映射检查；最终源码经 `056_dali_probe_final_compile` 重编，**全部六格重新各跑五次**。主结果只取 `dali_matrix_final.json`，不把早期 30 次再次加进主矩阵。
- `051`/`055` 的 Control 前置诊断使用中间版探针（已有 Control 选项、尚未添加加载前 core 映射）。编译时未单独保留这个中间源快照，因此不将其称为完整可复现的规范矩阵；命令、输出、回溯仍保留。最终源码具备该选项，但不声称它的文件摘要就是中间版本摘要。
- ARM 规范矩阵使用 `076` 的最终探针（包括 SIGSEGV/SIGABRT/SIGBUS/SIGILL/SIGFPE 和时限信号记录）及 `078`/`083` 的真实 core/包装构建。早期 QEMU 编译器、SDK 头文件定位、CMake C 编译器配置失败均不是运行格。
- GNU 13 表示探针保留为补充；平台 GNU 14.2 头文件规范结果为 `vector_gnu14_results.json`。它和 libc++ 表示探针均不是 SetSyncBinaryReply 运行格。
- shim 的完整 cookie、空输出对象用例与输出复用用例是**不同测试条件**：前者 5 次正确，后者 GNU 对照 5 次保留字段、原型 5 次丢失字段。不能挑选前者宣称原型已验收。
- 各编排脚本会随诊断补充头文件路径、归档解析等；实际历史编译/运行命令以 `raw/*.command.txt` 为准。中间所有编排版本没有逐版快照，最终版本快照不冒充历史版本。

本轮核心资源、解包、构建、运行、哈希、交付命令留有原文与退出码。只为显示既有日志的若干读取命令没有另行重复保存；因此审计结论是“规范证据可核，交互记录不全量”，不是无缺口的全量命令审计。
