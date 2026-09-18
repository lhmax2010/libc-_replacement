# 接续初始只读命令的恢复记录

本节事后抄录本次接续最初的工具返回，不冒称日志包装器在执行时自动落盘；下列命令均返回退出码 0。随后的诊断从 raw/284 起使用原日志包装器。没有读取凭据内容。

```bash
pwd
ls progress/BUILD_WEEKEND_0918/raw/283* progress/BUILD_WEEKEND_0918/raw/276*
sed -n '1,180p' progress/BUILD_WEEKEND_0918/RUNBOOK_CHECKPOINT.md
sed -n '1,160p' progress/BUILD_WEEKEND_0918/view_logged.py
sed -n '1,120p' progress/BUILD_WEEKEND_0918/raw/283_resume_watch.stdout.txt
sed -n '1,220p' progress/BUILD_WEEKEND_0918/STATUS.md
sed -n '1,130p' progress/BUILD_WEEKEND_0918/guard.py
```

另通过原执行句柄 82717 查看仍在运行的命令；该轮询不构成新的构建命令或其完成退出码。实际 ARM 未定义工具链构建的完成状态由原 raw/276 和对应 cell 的 finished 事件记录，退出 0。
