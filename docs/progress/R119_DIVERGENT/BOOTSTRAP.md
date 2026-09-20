# 开工记录

本任务仅一个执行通道，无子代理；逐条等待退出后才发下一条命令。
扫描、分析、编译由现有 resume_limited.py 设置 nice19、ionice3、RLIMIT_AS 30%；Git 普通优先级。

开工定位命令（尚未建立 recorder，补录）：

```bash
pwd; rg --files -g AGENTS.md -g '!codes/**' -g '!tmp/**' -g '!config/**' -g '!docs/**' . /home/toolchain/development 2>/dev/null
```

该搜索范围过宽，主动 Ctrl-C，退出 130；没有启动第二个任务与它并行。随后定点查父目录指令：

```bash
for p in /AGENTS.md /home/AGENTS.md /home/toolchain/AGENTS.md /home/toolchain/development/AGENTS.md /home/toolchain/development/libcxx_runtime_validation/AGENTS.md; do if test -f "$p"; then printf '%s\n' "$p"; sed -n '1,240p' "$p"; fi; done
```

退出 0，无输出。以上命令没有改文件。之后任务命令与退出码通过 raw/ 同步落盘。
