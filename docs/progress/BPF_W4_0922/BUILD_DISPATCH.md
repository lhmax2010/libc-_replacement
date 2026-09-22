# 构建提速线索：只读核对

## 结论与适用范围

W1 的已存调度器记录出现 `/usr/bin/qemu-arm-static /bin/make`，后轮出现直接 `/bin/make`；**没有找到启动脚本显式添加该 QEMU 包装的代码**。当前两轮输入根都有同一 Tizen QEMU 与 `/emul` 原生工具；W3R 的当前入口 `--version` trace 已证实一次原生 Clang 分派，但不是历史编译过程的 trace。**W1 当时编译器是否走 /emul、ARM 提速的唯一原因、与 OBS 正式执行形态是否一致，仍为 NOT_OBSERVED。** 此证据缺口不单独阻塞签字；本轮未重跑构建或性能实验。

## 启动脚本的实际原文

已读两轮 `run.py`、`guard.py`、实际 `cells/bpf-armv7l-libcxx/executed-guard.py`、`cell_commands/bpf-armv7l-libcxx.json`；快照 SHA 见 `read-checks/SCRIPT_IDENTITIES.json`，原文带行号摘录见 `read-checks/LAUNCH_EXCERPTS.txt`。

| 位置（相对 docs/progress） | 原文 | 能证明什么 |
|---|---|---|
| BPF_W1_0921/run.py:8 | `r=subprocess.run(cmd,stdout=out,stderr=err)` | 直接执行传入 argv |
| BPF_W1_0921/cells/bpf-armv7l-libcxx/executed-guard.py:66、80 | `cmd = json.loads((BASE/'cell_commands'/f'{LABEL}.json').read_text())`；`proc = subprocess.Popen(cmd,stdout=stream,stderr=subprocess.STDOUT,start_new_session=True)` | 实际 guard 读取 JSON 并原样执行 |
| BPF_W1_0921/cell_commands/bpf-armv7l-libcxx.json:2、65–66 | `"bwrap",`；`"--",`；`"rpmbuild",` | 隔离入口不是显式 QEMU 命令 |
| BPF_STATIC_0922/run.py:9 | `r=subprocess.run(cmd,stdout=out,stderr=err)` | 同上 |
| BPF_STATIC_0922/cells/bpf-armv7l-libcxx/executed-guard.py:51、65 | `cmd = json.loads((BASE/'cell_commands'/f'{LABEL}.json').read_text())`；`proc = subprocess.Popen(cmd,stdout=stream,stderr=subprocess.STDOUT,start_new_session=True)` | 同上 |
| BPF_STATIC_0922/cell_commands/bpf-armv7l-libcxx.json:2、65–66 | `"bwrap",`；`"--",`；`"rpmbuild",` | 同上 |

W1 guard:116–117、后轮 guard:100–101 的 `qemu-` 字符串用于**识别正在运行的调度器并检查资源**，不是启动包装：例如 `make=Path(args[0]).name in ('make','gmake') or ('qemu-' in args[0] and any(Path(x).name in ('make','gmake') for x in args[1:]))`。不能将监控匹配逻辑当作显式启动命令。

## 当前宿主与两轮隔离根

宿主原文 `read-checks/host-arm.out`：

```text
enabled
interpreter /usr/bin/qemu-arm-binfmt
flags: P
```

另有 `read-checks/host-qemu-arm.out`：

```text
enabled
interpreter /usr/bin/qemu-arm-static
flags:
```

两者 ARM magic/mask 一样；另存 armeb 条目。**只读了当前注册状态，没有历史注册先后证据，不推断历史优先匹配规则或擅自修改注册。** 隔离根挂入的 /proc 反映同一宿主内核的 binfmt，不是两套独立的历史注册快照。

| 项 | 两轮当前 ARM 根观测 | 身份/限制 |
|---|---|---|
| /usr/bin/qemu-arm-static | 软链接到 qemu-arm | read-checks/BPF_W1_0921-armv7l-root.out:2–14；STATIC 同名记录 |
| /usr/bin/qemu-arm | x86-64 静态 ELF；Tizen qemu-linux-user-x86_64-cross-9.2.3.1-3.2.armv7l 所属文件 | SHA256 `8ee43bc800f9d708a8606310fa9cce476ce4fb02d31cfd910fd9b50cdbba470a`；“x86 静态 ELF”与“Tizen QEMU”不是互斥性质 |
| /emul | 存在 | 同文件:29–30 |
| /emul/usr/bin/clang | 链到 clang-22，x86-64，解释器 /emul/usr/lib64/ld-linux-x86-64.so.2 | SHA256 `590fa22e2859a15c3d05c46aca08958949bb0cc7ab5abf293ae8ea2f338daa1b`；同文件:31–33 |
| /emul/usr/bin/make 与 cmake | 均 x86-64 ELF | 同文件:34–39，完整 SHA 原样落盘 |
| 加速包 | clang-accel-x86_64-armv7l、qemu-accel-x86_64-armv7l 均 0.4-1.1 | 同文件:64–66 |

两轮 aarch64 根也检查并落盘，`/emul` 存在，QEMU 身份见 `read-checks/*-aarch64-root.out`。这些是对保留目录**当前**状态的核对，不能保证历史目录完全未变。

## W1 构建日志与 GBS/OBS

W1 build.log 中可见 Clang 22.1.8 识别结果及 target-prefixed 编译器路径；未取得 `/emul` 原生编译器进程映像的历史证据。检索输出 `read-checks/BPF_W1_0921-armv7l-compiler-evidence.out` 中 `src/arch/x86_64.cpp` 只是项目源文件名，不能作为宿主原生编译器证据。相同版本也不能证明相同分派。

本轮额外只读打开**未覆盖本轮 BPF 输入 /usr 的原始 GBS 根**。命令及绑定见 `gbs-read/commands.json`，输出见 `gbs-read/armv7l.out` 与 `aarch64.out`：两根均有 `/emul` 与平台 accel 包，`qemu-arm-binfmt -> qemu-binfmt`，后者 x86-64 静态 ELF，SHA256 `d8d5a158f3ee93347a512119fc91c6c8ff99bf6d86b9d6d438ca233d8a4100b8`；qemu-arm 与上表相同。所查 `/etc/binfmt.d/*.conf`、`/usr/lib/binfmt.d/*.conf`、`/etc/build.conf`、`/etc/build.data` 没有可输出的配置文件，不能补写不存在的配置内容。

**GBS 本地保留根具备原生加速组件，是实测配置事实；OBS 正式服务器实际执行路径 NOT_OBSERVED。** 没有正式构建逐进程日志，不能据上述静态一致性宣称本轮与正式构建完全同形。也不能仅据旧调度器的 QEMU 入口断言其子编译器一定不走原生加速。

沿用 W3R 时长实测：ARM 7150.345s → 601.229s，aarch64 801.918s → 600.008s（不是两架构均约两小时）。既有对象来源记录支持执行了新解包源码的编译流程；ARM 主包 ELF/.text 不同，aarch64 相同。未增添性能归因结论。
