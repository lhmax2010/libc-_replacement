# 探针日志包装器恢复记录

第一次直接执行复制来的 `run_logged.sh`：

```bash
docs/progress/BUILD_W4_0909/code/run_logged.sh 002 option_probe_all_drivers bash -lc '...'
```

因复制后的脚本没有可执行位，shell 返回 `126`，且没有产生 `002_*` 日志。
随后显式使用 `bash docs/progress/BUILD_W4_0909/code/run_logged.sh ...` 重跑；
完整修正后命令、输出与退出码见同目录的
`002_option_probe_all_drivers.{command.txt,stdout,stderr,exitcode}`。该非零是技术性
调用错误，不是选项判据失败。

第一次重新调用 QuickBuild 下载时网络请求超时，退出码 `2`，原文见
`004_download_failed_build_1165447.*`；不改变请求参数的重试退出码为 `0`，
见 `005_download_failed_build_1165447_retry.*`。完整日志仍只保存在忽略目录。

首次 trace-only GCC 探针在复用的 armv7l buildroot 中因缺少驱动器预期路径
下的 `liblto_plugin.so` 提前退出，不能判定三个链接选项。修正探针仅为 GCC
trace 增加 `-fno-use-linker-plugin`，使 `-###` 能输出 linker argv；待测选项
没有改变。初次与重试记录分别见 `008_*`、`009_*`。
