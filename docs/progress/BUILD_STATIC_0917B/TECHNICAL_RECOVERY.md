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
