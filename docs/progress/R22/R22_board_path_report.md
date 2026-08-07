# R22 第二部分：板上可执行路径探测

## 载体与探针

SDB `192.168.108.25:26101` 实测可达，设备为 rpi4，`uname -m` 为 armv7l。探针是 556 字节、ELF32 ARM EABI5、静态链接、入口 `_start` 直接以 syscall 退出 0 的最小文件；源码、完整 LLVM 22.1.8 汇编/链接命令、ELF 头和 SHA256 分别见 `board/exec_probe_armv7l.S` 与 `commands/011_compile_exec_probe_direct_llvm.log`。这不是平台包构建。

首次循环发现 `sdb shell` 的宿主退出码不会传递远端命令退出码，故 `board/path_probe_results.tsv` 全部标为 INVALID_CARRIER_RC，不用于结论。该首轮还因载体误判让 `sdb push` 自动创建了原本不存在的 `/opt/usr/home/owner/share/tmp`；已以精确路径 `rmdir` 删除并确认不存在，原文见 `commands/014_cleanup_sdb_created_candidate.log`。第二轮在每条远端命令输出显式 `__R22_REMOTE_RC` 后重测。

## 实测结果

6/7 个候选路径原本存在，且 6/6 都完成临时目录创建、探针写入、实际执行退出 0、删除退出 0、删除后不存在核验退出 0：`/opt/usr`、`/home/owner`、`/opt/home`、`/var/tmp`、`/opt/var/tmp`、`/root`。按可用空间降序的机械排列及每步 carrier/remote 退出码见 `board/board_path_ranked.tsv`。`/opt/usr/home/owner/share/tmp` 原本不存在，第二轮没有创建，状态 NOT_FOUND。

`/tmp` 挂载为 `rw,nosuid,nodev,noexec`；上述 6 个成功路径实际归属 `/`、`/opt` 或 `/opt/usr`，对应挂载项均未见 noexec。`/home/owner` 实际解析为 `/opt/usr/home/owner`，`/var/tmp` 解析为 `/opt/var/tmp`。最大剩余空间是 `/opt/usr` 挂载的 113,541,492 KiB；`/opt` 为约 1,263,900 KiB，根分区为 1,781,416 KiB。

Smack 已启用，执行会话标签为 `User::Shell`；SDB 推入的探针观测标签为 `User::App::Shared`，6/6 均实际执行成功。本轮没有执行拒绝，因此不存在可据以区分 noexec 与 Smack 拒绝的失败样本。板上存在 `chsmack` 和 `smackload`，但合法授权流程/审批依据不可得，记 NOT_AVAILABLE；只读取帮助，未实施任何放宽。

所有临时测试目录均已删除并核验。未安装包、未写持久文件、未改配置、未重启或启停服务、未部署项目制品。

## 当前状态与剩余限制（R23 核对）

“`/tmp` 为 `noexec`，因此没有板上可执行路径”这一阻塞已解决：本报告
实测的六个成功路径是 `/root`、`/opt/usr`、`/opt/home`、
`/home/owner`、`/var/tmp`、`/opt/var/tmp`。项目当前仅有这一块已识别的
armv7l 板卡。SDB 在 R15 会话不可达、在 R15b 与 R22 会话可达，未做
跨会话连续监测；板端无 `clang`/`clang++`/`gcc`/`g++`，须由宿主交叉
编译。这里仅同步能力边界，不改变 R22 的探测结论。
