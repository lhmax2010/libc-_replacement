# 开始阶段只读命令的恢复记录

以下两条命令在启用本任务日志包装器前执行；这是事后恢复记录，不是包装器
自动落盘。终端当时均返回退出码 0。它们仅用于定位工作区和读取仓库说明，
不作为正文事实或数量结论的证据。

```bash
pwd && rg --files -g 'AGENTS.md' -g '!codes/**' | sed -n '1,80p' && rg --files | sed -n '1,100p'
```

退出码：`0`

```bash
find .. -name AGENTS.md -print && sed -n '1,240p' AGENTS.md 2>/dev/null || true
```

退出码：`0`

