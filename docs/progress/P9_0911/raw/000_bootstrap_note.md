日志器建立前的只读定位命令：`pwd`（退出码 0）、`rg --files -g AGENTS.md -g '*resource*' -g '*abi_size_align*' -g '*gate*' -g '*P9*' -g 'README*' -g '*ABI*' -g '*abi*report*'`（退出码 0；工具输出截断）、`git status --short --branch`（退出码 0；工具输出截断）、`ls -la`（退出码 0）、`find .. -name AGENTS.md -print`（当时仍运行，后续补录退出码）。原始完整标准输出 NOT_AVAILABLE；随后重新记录有关定位结果。该说明不冒充首次命令的原始输出。

补记：初始 find 搜索范围过大，已定点停止本次进程；工具返回退出码 143。初始五条命令原文及已观测退出码补存在 pre_001–pre_005，不伪造其完整 stdout/stderr。一次浏览器结果保存尝试在 exec_command 创建进程前因 Argument list too long 被拒，shell 未启动，无 shell 退出码；后改为保存 URL 索引。其余调查过程中的缺文件和非零返回均保留在相应 raw 文件中；多行 shell 命令的退出码是整体命令真实返回值，不回填子命令退出码。
