# ARM lzma 返回值取证

**最终：失败复现，未观测到 lzma_code 错误返回；已停止实验。** 12:56:20–13:48:33，退出1，18个Wrote，7559/7560行复现。209303次调用中209285次OK、18次正常STREAM_END，序号连续、单调时间戳有序；18个流的total_in与18个RPM的ARCHIVESIZE逐项一致。未触发错误VA快照；fwrite短写仍NOT_OBSERVED，不据此认定VA耗尽。详见FINAL_RESULT.md、RESULT.json、POSTCHECK.json。以下保留历史状态。

**12:56:20+08 正式轮preload-original已启动。** 原host strace过滤保持，实际包装器SHA为a663845ffa1e25895507de2e56b355946a443fa0bb2e281c848c4d0d21095f2f；记录CLOCK_MONOTONIC。实际guest rpmbuild PID824601，header确认QEMU_RESERVED_VA=0x100000000。本轮只跑一次，失败消失就停止，2c记NOT_OBSERVED。尚无最终结果。以下为历史准备记录。

12:54收到选2确认：原host status=failed保持，第2c为NOT_OBSERVED。包装器时间戳改为CLOCK_MONOTONIC，三个案例重新校准通过；旧CLOCK_REALTIME版本已另存，旧SHA不是本轮实际执行版本。沿用17:20总截止，不自行延时。以下是此前准备阶段记录。

2026-09-21 11:21+08 开工，保守总截止17:20+08。尚未启动正式复现。

11:31状态：准备完成，等待host strace过滤口径确认，正式rpmbuild未启动。包装器已在guest编译并校准；旧成功输出树16993项已完整备份/校验。详见PREPARATION.md。实际失败轮的lzma_ret仍NOT_OBSERVED，不以人工构造校准样本代替。

磁盘210675826688字节可用，medium门禁0，I/O探针0.065513秒。
只在tmp/ARM_LZMA_0921中准备独立诊断代码，不改平台源码/spec/config/rpm/liblzma。
原argv中已有host strace status=failed过滤；第2c要求成功文件访问记录，与严格保持该过滤有冲突，已请求人工确认是否允许去掉失败过滤。未取得许可前保持原过滤，不自行扩大。

原始首次查看（在日志器建好前）命令：pwd；rg --files -g AGENTS.md -g '!codes/**' -g '!tmp/**' -g '!progress/**' -g '!docs/**'；git branch --show-current；df -h .；sed -n '1,160p' progress/ARM_REPRO_0920/run.py；sed -n '1,160p' progress/ARM_REPRO_0920/cell_commands/original-argv-repro.json。最后退出0；是只读查看，原输出在会话中。
