# 9 月 17 日续跑交付入口

本次结果：**PARTIAL，停止等待人工确认**。不覆盖以前的原始报告。

- 最新结论：[FINAL_RESULT.md](resume_0917/resume_stop_1909/FINAL_RESULT.md)
- 状态与完整技术性修正记录：[STATUS.md](resume_0917/STATUS.md)
- aarch64 LLVM 完整 RPM 结果：[AARCH64_RPM_RESULT.md](resume_0917/resume_stop_1909/AARCH64_RPM_RESULT.md)
- armv7l LLVM 整轮失败 / 单独产物核验的区分：[ARM_RPM_PARTIAL.md](resume_0917/resume_stop_1909/ARM_RPM_PARTIAL.md)
- 单文件复制诊断：[copy_read_diagnostic.json](resume_0917/resume_stop_1909/copy_read_diagnostic.json)
- 逐条命令与实际退出码：[raw/](resume_0917/raw/)
- 无退出码的中断复制记录：[interrupted_raw/](resume_0917/resume_stop_1909/interrupted_raw/)
- 快照与哈希：[MANIFEST.json](resume_0917/resume_stop_1909/MANIFEST.json)
- 此前受限 sudo 调用与密码文件删除记录：[COPY_RESUME_REPORT.md](COPY_RESUME_REPORT.md)。本次续跑新增 sudo 调用为 0。

aarch64 LLVM 的 12 个 RPM 已完整通过核查；armv7l 完整写包退出 1，18 个已写出 RPM 的单独校验不能替代整轮通过。恢复后的 aarch64 私有输入根复制遇到 rsync 读取 EPERM；普通读取与单文件 cp 成功且 SHA256 一致，但原因尚未确认，未自行改用其他方法整批复制。

bpftrace 两架构三路径完整 RPM、安装与 armv7l 物理板功能/取消验证未完成。没有包仓推送；候选不能据本次结果替换发布资产。需要确认的问题在最新结论中逐项列出。
