# 原生交叉工具集续建核查

## 授权与范围

人工批准使用已有原生工具集，但须先核对身份、抽查五个已由 QEMU 编译的目标，且构建进程实际处于 50% 内存限制内。截止时间不变：2026-09-16 22:09:25 +08。本记录不声明完整 RPM 或功能验收通过。

## 工具链身份

| 项 | 原 QEMU 入口 | 原生交叉入口 |
| --- | --- | --- |
| 二进制 | `tmp/STATIC_0917B/llvm-root-minimal/usr/bin/clang-22` | `tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/emul/usr/bin/clang-22` |
| SHA256 | `605deb4eb006d0a9e4788de5ea45fae7c6bbbf8c1562ee7aeeecfa089b6a8afe` | `590fa22e2859a15c3d05c46aca08958949bb0cc7ab5abf293ae8ea2f338daa1b` |
| 宿主 ELF 架构 | AArch64，经 QEMU 执行 | x86-64，宿主原生执行 |
| 输出目标 | `aarch64-tizen-linux-gnu` | 相同 |
| 显式 sysroot 参数 | 无；私有根 `/` | 相同，复用同一私有根 |
| sysroot 宿主映射 | `tmp/STATIC_0917B/llvm-root-minimal` | 相同 |
| 显式 `-march` / `-mcpu` | 未传入 | 相同 |
| cc1 实际默认值 | `-target-cpu generic`，`+v8a,+fp-armv8,+neon,+outline-atomics,-fmv` | 相同 |

两者 `--version` 完整输出均为：

```text
clang version 22.1.8
Target: aarch64-tizen-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /usr/bin/clang++.cfg
```

实际 driver 输出见 `native-eq-qemu-driver.json` / `native-eq-native-driver.json`。两者都选择该根的 `/usr/bin/../include/c++/v1`、`/usr/lib64/clang/22/include` 与 `/usr/include`。宿主 ELF、SHA256、入口路径有差异，不能由版本相同直接推断代码等价。原生工具自身链接 libstdc++ 是宿主程序的实现依赖，不等于目标代码选用 libstdc++。

## 五个对象的抽查

从原 QEMU 完成的目标选择 C++ demangle、整数运算、MD5、线程相关支持及 portable C hash 实现。选择理由是覆盖不同代码形态；不是随机抽样，也不是逐目标证明。原对象保持不动，保存原件副本与原生新输出。原 Ninja 给出的编译命令除编译器入口和输出/依赖文件路径外保持一致；具体命令、输入及 SHA256 见 `native-equivalence-inputs.json`。

| 对象 | nm 行数 | 反汇编行数 | 符号/反汇编差异 | 整个对象 SHA256（两份相同） |
| --- | ---: | ---: | --- | --- |
| Demangle.cpp.o | 36 | 169 | 0 / 0 | `8c61ea590642bea02a958884f233a402a70b9a0b1fbd1261da38c3ae12222893` |
| APInt.cpp.o | 535 | 22062 | 0 / 0 | `c229e44df26b19d6bb1aa6e1f882329a6edffad03c1d447f1a6c5c132aaf94e9` |
| MD5.cpp.o | 61 | 1446 | 0 / 0 | `41a6b98dcb88d096cee8f4e3388ab9d5f47dc0077fe7527be4810c354043244b` |
| Threading.cpp.o | 132 | 617 | 0 / 0 | `3986c344362a4bdec9f3b3bb2427a57c6ba7ee7722813a00c19fe2f4c46a9992` |
| blake3_portable.c.o | 29 | 1883 | 0 / 0 | `92dc718a0eea0387fa2ba57c19319750a2e6a7efc1c75cc9f2c6224619993172` |

对两份对象使用同一套 `llvm-nm -an` 和 `llvm-objdump -d`。比较全部反汇编代码，而非仅选几条指令；仅规范化 objdump 首行的对象文件名，没有删除地址、指令或符号。五份完整对象也逐字节相同。结果见 `native-equivalence-summary.json`、各 `native-eq-*-result.json` 与空差异文件。

因此允许按人工指定门禁继续：**aarch64 LLVM 后续对象由原生交叉工具集编译；等价性由上述五对象抽查支撑，不是逐目标证明。**

## 资源限制实测

物理总内存 32,297,756 KiB；一半为 16,536,451,072 字节。内核按 4096 字节页向下取整，实际设置 `MemoryMax=16536449024`（小于等于一半）。通过 `systemd-run --user --scope` 执行，不使用 sudo；`nice -n 19` 与 `ionice -c 3` 放在 scope 内。额外 ulimit 不是 cgroup 的替代证据。

五对象门禁的实际 Ninja PID 643893：

```text
cat /proc/643893/cgroup
0::/user.slice/user-1000.slice/user@1000.service/app.slice/build-static-0917b-native-equivalence.scope
cat /sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/build-static-0917b-native-equivalence.scope/memory.max
16536449024
```

每个抽查目标均在编译前验证实际 Ninja 的 cgroup、memory.max、nice=19、ionice=idle 与 `-j1`。完整 RPM 续建另有 scope `build-static-0917b-native-rpm.scope`，实际 Ninja 证据记录在 `llvm-aarch64-native-rpm.events.jsonl`，不能用抽查进程冒充完整构建进程。

每 500 目标 I/O 探测、超 30 秒暂停 10 分钟、连续三次仍慢停止，固定截止也在暂停期间生效；每小时写入 HOURLY_STATUS.md。

## 技术性失败与修正申报

1. 初次 MemoryMax 请求采用未页对齐的精确一半，读取值被内核向下取整，严格检查退出 1；尚未编译。显式改用向下取整值后检查通过，未放宽限制。
2. raw/154 的抽查脚本假定宿主 `/usr/bin/ninja`，实际不存在；在开始抽查编译前失败。只改为实际 `/home/toolchain/.local/bin/ninja` 后 raw/159 通过。旧执行脚本单独保存。
3. 首个样本冷缓存等待期间只读 I/O 探测 1.226 秒、未达 30 秒；未把 D 状态本身当作失败，未绕过暂停规则。
4. raw/174 的完整续建在配置阶段发现 Ninja 未显式带 `-j1`，20:24:14 主动停止，退出 124。该次停止不是截止时间到达，也不是样本不一致。原监控未记录该 Ninja 的完整 argv，不能补称已确认它是版本查询还是编译。
5. 为保证配置探测与主构建都单任务，在私有 namespace 将 Ninja 入口换成仅追加 `-j1` 的包装器，实际 ELF 以 `ninja.real` 调用；不改 spec、平台源码或系统 Ninja。raw/176–178 保存修正与重启命令。
6. 准备重启时两条外层命令直接执行无执行位的 `run_logged.sh`，退出 126；未执行复制/准备子命令。随后显式 `bash run_logged.sh` 重跑。旧监控脚本由逆向撤销本次两处编辑恢复，SHA256 `f57a5e16a36a9bd72fabaa9d1e02f53466707556c2a852965636c63829e17c75` 与 raw/174 实际启动记录一致；不是冒称事前快照。对应命令原文与恢复记录另列 `TECHNICAL_RECOVERY.md`。
7. 20:29–20:36 的 `%build` 准备阶段，`find -maxdepth 8 -name config.sub` 反复处于 `wait_on_buffer`，累计读取量增加；追加新文件读写探测约 0.58/0.65 秒，实际已定位目录读取也在 30 秒内完成。可证明准备阶段遍历缓慢，不能仅由 D 状态宣称同一目录卡死或探测超时。没有因此提高优先级，保留固定截止。

密码仍不存在，没有新增 sudo。五对象通过不等于完整写包、安装、功能或板上门禁通过；最终状态以本轮构建结束记录为准。
