# 记录器外的恢复性诊断

以下命令是在恢复会话、修正记录器调用或确认后台 export 状态时直接执行，
没有通过 `run_logged.sh`。它们不修改任务输入；同一判据均在随后编号命令中
重新观测并落盘。为避免遗漏命令原文，在此补记。

```bash
pwd && git status --short && find progress/R83 -maxdepth 2 -type f -printf '%P\n' | sort | tail -80 && ls -l progress/R83/code/run_logged.sh
```

退出 0；用于恢复后盘点。结果由编号 037 的完整文件盘点和最终状态核对覆盖。

```bash
od -An -tx1c progress/R83/raw/010b_prior_injection_and_inventory.command.txt && cat progress/R83/raw/010b_prior_injection_and_inventory.stderr progress/R83/raw/010b_prior_injection_and_inventory.exitcode
```

退出 1；当时路径名实际带 `_bash`，因此报
`No such file or directory`。随后用 `find` 确认真实文件名并重跑为 010c。

```bash
find progress/R83/raw -maxdepth 1 -type f -printf '%f\n' | sort | tail -20 && sed -n '1,3p' progress/R83/raw/010b* 2>/dev/null || true
```

退出 0；确认 010b 把 `bash` 误当作 label。

```bash
cat progress/R83/raw/015_sandbox_gbs_export.exitcode && wc -c progress/R83/raw/015_sandbox_gbs_export.stdout progress/R83/raw/015_sandbox_gbs_export.stderr && find tmp/R83/export -maxdepth 2 -type f -printf '%p %s\n' | sort | tail -30
```

退出 1；export 当时尚未结束，exitcode 文件尚不存在。

```bash
find progress/R83/raw -maxdepth 1 -type f -name '015*' -printf '%f %s\n' | sort && ps -eo pid,ppid,stat,etime,cmd | rg 'gbs export|run_logged.sh 015' || true
```

退出 0；确认低优先级 export 仍在运行。完成状态随后由 016/017 记录。

```bash
ps -eo pid,ppid,ni,stat,etime,%cpu,%mem,rss,cmd | rg 'run_logged.sh 024|find \. -type f|xargs -0 rg' || true; find progress/R83/raw -name '024*' -printf '%f %s\n'
```

退出 0；定位过宽只读扫描的明确 PID，随后由 025 记录并终止。

```bash
sed -n '1,120p' progress/R83/artifacts/PROJECT_CONFIG_CHANGES.diff | cat -A
```

退出 0；检查首版 diff 的换行/反斜线。最终 diff 已由 035 以 fuzz=0 验证。

```bash
cp -a progress/R83/. docs/progress/R83/
```

最终提交前同步，退出 0、无标准输出；用于把 043 自身完成后才生成的
stdout/exitcode 以及本补记复制到交付目录。

```bash
cp -a progress/R83/. docs/progress/R83/ && diff -qr progress/R83 docs/progress/R83 && git add -- docs/progress/R83 && git status --short && git diff --cached --stat && git diff --cached --check
```

同步和只暂存 `docs/progress/R83/` 成功；末尾的全量 `diff --check` 因原样
证据文件保留上游输出尾空格而非零。044/045 随后把“原始证据不改写”和
“作者文件无尾空格”分开核验。
