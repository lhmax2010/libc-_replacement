# 技术性退出的恢复记录

下述两条由工具直接返回退出码 126，未进入日志包装器。这里是事后恢复记录，非包装器自动日志；未含凭据。

```shell
progress/BUILD_STATIC_0917B/run_logged.sh 175_preserve_monitor cp -p progress/BUILD_STATIC_0917B/native_build_monitor.py progress/BUILD_STATIC_0917B/native_build_monitor.executed_174.py
```

实际错误：`/bin/bash: line 1: progress/BUILD_STATIC_0917B/run_logged.sh: Permission denied`。退出码：126。

随后一组以 `set -e` 开头的命令在第一条失败，后两条未执行：

```shell
set -e
progress/BUILD_STATIC_0917B/run_logged.sh 176_prepare_serial_ninja python3 progress/BUILD_STATIC_0917B/prepare_serial_ninja.py
progress/BUILD_STATIC_0917B/run_logged.sh 177_serial_scripts_sha sha256sum progress/BUILD_STATIC_0917B/native_build_monitor.py progress/BUILD_STATIC_0917B/run_native_rpm_resume.py progress/BUILD_STATIC_0917B/prepare_serial_ninja.py tmp/STATIC_0917B/ninja-serial
progress/BUILD_STATIC_0917B/run_logged.sh 178_native_serial_resume systemd-run --user --scope --expand-environment=no --unit=build-static-0917b-native-rpm -p MemoryMax=16536449024 nice -n 19 ionice -c 3 python3 progress/BUILD_STATIC_0917B/run_native_rpm_resume.py
```

实际错误：`/bin/bash: line 2: progress/BUILD_STATIC_0917B/run_logged.sh: Permission denied`。退出码：126。

修正：显式通过 bash 调用现有包装器，不修改执行权限。旧监控副本的生成晚于编辑，已在报告中标注为恢复版本；raw/180 校验其 SHA256 与原运行事件一致。

## I/O 超时后的暂停信号（恢复记录）

raw/225 的读写探针超时，实际退出 124。启动 raw/226 暂停控制器后，控制器自身处于 `D / wait_on_buffer`，尚未写出身份或信号记录。为避免等待日志 I/O 时构建仍继续，在核对 PID=PGID=659012、PPID=658933、完整 bwrap 命令及本任务私有根后，直接执行以下命令，工具返回退出 0，ps 显示目标为 `TNs`。以下为工具结果的恢复记录，不冒充日志包装器自动输出；未使用 sudo。

```shell
kill -STOP -- -659012
stop_rc=$?
printf 'command=kill -STOP -- -659012\nexitcode=%s\n' "$stop_rc"
ps -p 659012 -o pid,ppid,pgid,stat,args
exit "$stop_rc"
```

关键输出：

```text
command=kill -STOP -- -659012
exitcode=0
PID     PPID    PGID    STAT
659012  658933  659012  TNs
```

控制器之后若对同一组再次发送 SIGSTOP，是幂等暂停，不是另一次构建；十分钟从控制器实际计时开始，不缩短等待。最终仍遵守原截止。

## 停止受 I/O 阻塞的归档与核查

结束后，核对 PID 665961 为本任务 archive_native_final.py、PID 666493 为本任务 raw/229 记录器（两者均在 D 状态等待文件 I/O）后，工具执行 `kill -TERM 665961 666493`。这是对本任务材料收集进程的停止，不是对其他任务或系统服务操作；具体退出结果由该工具返回，原始文件没有删除。改为按明确文件名逐个收集核心交付物，完整 raw 归档仍未完成。此段为恢复记录，不冒充尚未写完的自动日志。
