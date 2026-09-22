# 日志器创建前的只读命令补记

以下为实际命令原文；复合shell整体退出码为0（其中缺失AGENTS.md的ls退出2，路径未找到，不是忽略现有指示）。原始工具输出已在会话记录，不补造日志器创建前的stdout文件。

```sh
pwd; rg --files -g AGENTS.md -g '!codes/**' -g '!tmp/**'; git branch --show-current; sed -n '1,220p' progress/BPF_STATIC_0922/board.py
# exit 0
ls /home/toolchain/AGENTS.md /home/toolchain/development/AGENTS.md /home/toolchain/development/libc++_replacement/AGENTS.md; rg --files progress/ARM_RPM_DIAG docs/progress/ARM_RPM_DIAG | rg '(transaction|rpmprob|SOURCE|upstream|manifest)'; sed -n '1,100p' progress/BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json; cat docs/progress/BPF_W1_0921/W3_REFERENCE_INPUTS.json; sed -n '1,110p' progress/BPF_STATIC_0922/guard.py
# exit 0
sed -n '1,150p' progress/BPF_STATIC_0922/run.py
# exit 0
```

后续命令原文/退出码见raw；各Python脚本调用的SDB、bwrap、objcopy等子命令另在相应commands.json。第一节只读查询中的权限、readlink语法差异、缺失命令均保留，不把非零修饰为成功。raw/009的整体退出1来自最后rg零命中，并非前面的源码读取失败。
