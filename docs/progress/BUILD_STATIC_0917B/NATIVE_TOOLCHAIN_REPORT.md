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
5. 为保证配置探测与主构建都单任务，在私有 namespace 将 Ninja 入口换成包装器，实际 ELF 以 `ninja.real` 调用；不改 spec、平台源码或系统 Ninja。raw/176–178 的第一版把 `-j1` 放在参数末尾，这是错误实现，后续已按第 8 项修正。
6. 准备重启时两条外层命令直接执行无执行位的 `run_logged.sh`，退出 126；未执行复制/准备子命令。随后显式 `bash run_logged.sh` 重跑。旧监控脚本由逆向撤销本次两处编辑恢复，SHA256 `f57a5e16a36a9bd72fabaa9d1e02f53466707556c2a852965636c63829e17c75` 与 raw/174 实际启动记录一致；不是冒称事前快照。对应命令原文与恢复记录另列 `TECHNICAL_RECOVERY.md`。
7. 20:29–20:36 的 `%build` 准备阶段，`find -maxdepth 8 -name config.sub` 反复处于 `wait_on_buffer`，累计读取量增加；追加新文件读写探测约 0.58/0.65 秒，实际已定位目录读取也在 30 秒内完成。可证明准备阶段遍历缓慢，不能仅由 D 状态宣称同一目录卡死或探测超时。没有因此提高优先级，保留固定截止。
8. 第一版包装器错误地让 `ninja -t restat build.ninja -j1` 将 `-j1` 解释为 restat 子工具参数，实际报 `restat: invalid option -- 'j'`。这是本次新增包装器引入的技术性错误，不是 LLVM 源码或五对象等价失败。raw/178 20:27:32 开始、21:03:54 退出 1，用时约 36 分 22 秒（包含前面的慢 I/O），主构建新完成目标为 0。修正为全局参数置前，并拒绝其他并行数；版本、subtool 列表、restat、单任务空图执行均实测通过，`-j2` 返回 64（raw/215、ninja_wrapper_test.json）。旧版和新版快照均保存，raw/218 在原截止内重启。
9. 失败轮 CMake 实际提示编译器变量改变、需删除缓存，随后 `CMakeCache.txt` 不存在、生成文件不完整。raw/216 的 rg 因该文件不存在退出 2；没有将其当作缺失源码或另一个成功检查。重新执行正常配置来恢复生成状态，未伪造缓存、未删原对象或 Ninja 断点，未跳过 RPM 构建流程。
10. 完整 `git diff --check` 对原始 command.txt 的尾空格返回 2；这是日志包装器原样格式，不改写原始证据。对本轮说明和 Python 脚本单独检查返回 0；凭据模式检查通过。检查点 `8e5b4fc15cb8a992145166adccef68da6545f714` 仅推项目编译线，另外三个远端分支逐项未变（native_checkpoint_push.json）。该检查点早于包装器 restat 失败，不能当作完整构建通过。
11. raw/225 小规模读写探针实际超时，退出 124，触发十分钟暂停规则。暂停控制器本身也遭遇磁盘等待，先经核对直接对本任务 PGID=659012 发送 SIGSTOP，退出 0、进程状态 T；恢复记录见 TECHNICAL_RECOVERY.md。新增 external-io-pause.events.jsonl 负责暂停/复测记录。主监控中的 `running` 只指监控尚未退出，暂停期间不代表编译在运行；此处以显式暂停记录和进程 T 状态为准。
12. 控制器第一轮暂停从 21:22:31 开始，21:33:16 记录复测超时 124（系统调度/终止等待使观察耗时约 45 秒，不冒称精确 30 秒返回），第二轮从 21:33:41 开始。新增仅针对核对过的同一 PID/PGID/启动 tick 的截止信号保护器，在截止前预留十秒发 TERM、到截止发 KILL；发送信号之前不写磁盘日志，避免日志 I/O 延误停止。不会触碰其他进程或改变原截止，见 raw/227 与 deadline_signal_guard.py。

密码仍不存在，没有新增 sudo。五对象通过不等于完整写包、安装、功能或板上门禁通过；最终状态以本轮构建结束记录为准。

## 最终结果：PARTIAL，断点保留

| 项目 | 结果 |
| --- | --- |
| 两套工具链身份与默认目标参数 | 已记录，差异为宿主 ELF、SHA256、入口路径；均为 Clang 22.1.8，目标 aarch64 |
| 五对象原生重编 | PASS；nm、全部反汇编及整个 `.o` SHA256 均相同 |
| 抽查 Ninja 的实际 cgroup / MemoryMax / nice / ionice / -j1 | PASS，逐对象核对 |
| 完整 RPM 启动 scope | 实际 MemoryMax=16536449024、nice=19、ionice=idle 已记录 |
| 完整 RPM 主 Ninja 的实际进程资源核验 | NOT_OBSERVED：未进入主编译，不能用抽查 Ninja 冒充 |
| 完整 RPM 新完成目标 | 0；两次配置入口修正和慢 I/O 后仍未完成配置/主构建 |
| aarch64 / armv7l bpftrace 三路径完整 RPM及安装 | 未完成，本轮未新增这些 RPM |
| armv7l 物理板功能/取消验证 | NOT_STARTED，缺本轮完整 RPM及相应 arm 静态输入；未上板变更 |
| 包仓推送、Source1002 替换 | 均未进行 |

实际暂停与结束时间（+08）：

| 事件 | 时间 | 结果 |
| --- | --- | --- |
| 第一轮十分钟等待 | 21:22:31 | 21:33:16 复测退出 124，观察耗时约 45.03 秒 |
| 第二轮十分钟等待 | 21:33:41 | 21:44:27 复测退出 124，观察耗时约 46.28 秒 |
| 第三轮十分钟等待 | 21:44:57 | 21:55:54 复测退出 124，观察耗时约 56.48 秒 |
| 达到三轮停止条件 | 21:56:29 | 停止原因明文记录：三轮等待后 I/O 探测仍失败 |
| 向已核对进程组发送 TERM / CONT | 21:56:50 / 21:57:02 | 构建监控 21:57:02 记录退出 -15，外层记录器退出 241 |
| 暂停控制器 | — | 退出 75；不是构建成功码 |
| 截止保护器 | — | 目标已提前结束，未发送信号 |

原截止 22:09:25 未延长。进程退出后因系统响应/日志读取延迟，残留核查工具输出时间为 22:18:59；该次 ps 未列出构建、主监控或暂停控制器。**不能把材料核查的较晚时间写成构建在截止后继续运行，也不能声称材料提交在截止前完成。** 后续仅材料整理与项目仓推送。

保留 `tmp/STATIC_0917B/llvm-root-minimal`、`rpm-aarch64/BUILD/llvm-22.1.8/`、`native-equivalence/`、两版 Ninja 包装器快照及原始输出。旧 QEMU 主构建到 181/7546 的记录沿用此前证据；本轮原生编译完成的是独立输出目录的五个抽查对象，并非主构建完成五个新目标。CMake 在失败轮已自行删除缓存、留下不完整生成文件；下一轮须正常重新配置，不能把该目录当作已配置完成。

本轮额外成本：第一次资源检查停止和一次包装器参数错误均如实保留；参数错误那轮耗时约 36 分 22 秒且新主目标为 0。未将整个期间都归因于工具链性能，也没有以版本相同代替对象抽查。

尚缺：完整 LLVM 静态开发 RPM、两架构 bpftrace 三路径写包/安装、armv7l 物理板取消与功能对照。没有足够实际进度据以给出可靠的完成耗时，记 NOT_OBSERVED；未以抽查、宏展开或旧独立候选替代这些门禁。

### 结束后归档的 I/O 限制

全量增量归档 raw/228 在原始日志目录枚举处进入长时间 I/O 等待；raw/229 的结束核查包装器也在文件打开阶段等待。只读核对了这两个进程的身份后，发送 `kill -TERM 665961 666493`，保存原日志目录和断点，不删除任何源证据。改用显式已知文件路径归档核心报告、三轮暂停/结束事件与关键命令退出码，不再遍历目录。**该恢复交付不等于全部新增 raw 文件已上传**；原生五对象的完整证据已随前一检查点上传，后续完整原始日志仍保留在本地 progress 目录，尚缺部分待 I/O 恢复后补齐。
