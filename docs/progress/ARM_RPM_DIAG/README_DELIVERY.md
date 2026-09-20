# 诊断材料交付说明

本目录是 `progress/ARM_RPM_DIAG/` 的交付副本，不改写原结论。先读 `FINAL_RESULT.md`；`INTERIM_RESULT.md` 是等待复现确认时的早期状态，不代表最终状态。

原目录 1522 个文件全部保留，另补入原先位于 `tmp/ARM_RPM_DIAG/guest-rpmbuild.strace` 的客体 trace，共 1523 个原始文件、1821469744 字节。没有静默省略文件。原 RPM 产物位于 tmp，不在本次诊断目录内；其清单、哈希与校验记录已包含。

以下大件不以未压缩形式入 Git，改为无损 xz 分卷（每卷不超过 32 MiB）：

| 原文件 | 原字节数 | 交付形式 |
| --- | ---: | --- |
| host.strace | 1567709470 | host.strace.xz.part-000 至 part-002 |
| guest-error-calls.tsv | 26048731 | guest-error-calls.tsv.xz.part-000 |
| tmp 下 guest-rpmbuild.strace | 177511348 | external/guest-rpmbuild.strace.xz.part-000 |

原文件名、原大小、SHA256、分卷顺序和压缩命令/退出码在 `DELIVERY_MANIFEST.json`。按清单顺序连接分卷再 `xz -dc` 可还原；请还原到新的临时目录，不覆盖原证据。例如在本目录执行：

```sh
diag_unpack=$(mktemp -d)
cat host.strace.xz.part-000 host.strace.xz.part-001 host.strace.xz.part-002 | xz -dc > "$diag_unpack/host.strace"
xz -dc external/guest-rpmbuild.strace.xz.part-000 > "$diag_unpack/guest-rpmbuild.strace"
xz -dc guest-error-calls.tsv.xz.part-000 > "$diag_unpack/guest-error-calls.tsv"
sha256sum "$diag_unpack/host.strace" "$diag_unpack/guest-rpmbuild.strace" "$diag_unpack/guest-error-calls.tsv"
```

交付校验会对全部文件（压缩项先解压）重新计算大小与 SHA256，结果见 `DELIVERY_VERIFICATION.json`。`SECURITY_CHECK.json` 记录凭据检查范围和结果；检查不输出凭据。绝对本机路径、进程号及内部任务名称是本项目证据，未伪装成匿名对外报告。

头部观察器留下的 `*.header-prefix` 不是可安装 RPM；不要把其存在当作写包完成。
