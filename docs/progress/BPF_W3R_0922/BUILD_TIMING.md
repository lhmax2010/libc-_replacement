# 二、构建时长与对象来源核对（未起构建）

**只读纪律违反：** 提取.text使用的llvm-objcopy漏写独立输出ELF，改写了四份已有tmp主包解包副本；发现后未恢复、未再用它们作输入。详见INCIDENT.md与INPUT_ELF_FINAL.json。随后仅在内存从未改变的原RPM读取ELF与.text，复核下表原始SHA和.text比较全部成立（RPM_READONLY_CHECK.json）。这不消除操作违规；四份副本待人工批准恢复。

| 架构/轮次 | 开始 → 结束（+08:00） | 耗时 | 进度公告 |
|---|---|---|---|
| armv7l / BPF_W1_0921 | 2026-09-21T22:56:12.426683+08:00 → 2026-09-22T00:55:22.771823+08:00 | 7150.345s（119.17min） | 840 |
| armv7l / BPF_STATIC_0922 | 2026-09-22T17:03:26.886851+08:00 → 2026-09-22T17:13:28.115637+08:00 | 601.229s（10.02min） | 840 |
| aarch64 / BPF_W1_0921 | 2026-09-21T21:16:13.453245+08:00 → 2026-09-21T21:29:35.371062+08:00 | 801.918s（13.37min） | 840 |
| aarch64 / BPF_STATIC_0922 | 2026-09-22T17:18:47.958554+08:00 → 2026-09-22T17:28:47.966570+08:00 | 600.008s（10.00min） | 840 |

**应纠正一处人工侧表述**：ARM确为约119.17分钟降至10.02分钟；aarch64上一轮实际为13.37分钟，本轮10.00分钟，并非同样从约两小时缩短。

两架构各轮840是guard统计的进度公告数，不等于840份独立目标文件。本次另抽取以百分比开头的Building/Linking记录，各274行，两轮逐行相同；history/*-targets.diff为空。既有840计数未改，也不把274与840混用。

## 编译器与缓存

| 项 | 实测/静态核查 | 限制 |
|---|---|---|
| ARM clang/clang++软链接 | /usr/bin与/bin的target-prefixed入口均解析到/usr/bin/clang-22；file为ARM ELF32 | build-audit/root-tools-armv7l.out |
| aarch64入口 | 解析到/usr/bin/clang-22；file为AArch64 ELF64 | build-audit/root-tools-aarch64.out |
| ccache命令 | 两个只读隔离根的PATH中未找到 | 不宣称全宿主所有位置不存在 |
| 四个指定缓存目录 | /home/abuild/.ccache、/root/.ccache、/var/tmp/ccache、$HOME/.cache/ccache均ABSENT；本次HOME=/home/toolchain | 当前根观测，不倒填历史状态；命中统计NOT_OBSERVED，不写0命中 |
| CMake launcher | 两轮两架构main/static CMakeCache中均无CMAKE_CXX_COMPILER_LAUNCHER/ccache条目 | 所查Cache原始输出见build-audit/*-cache-* |
| 旧ARM执行记录 | scheduler为/usr/bin/qemu-arm-static /bin/make等 | history/BPF_W1_0921-armv7l-scheduler.json |
| 新ARM执行记录 | 出现直接/bin/make，以及/usr/bin/qemu-arm binfmt…形式 | history/BPF_STATIC_0922-armv7l-scheduler.json |
| 当前原生加速 | /emul/usr/bin/clang-22为x86-64 ELF、Clang22.1.8；仅--version的只读execve trace实测转至/emul//usr/bin/clang-22 | compiler-read/armv7l.trace:3；不是重跑构建，也不当作历史逐目标证据 |
| 当前QEMU | 输入根为Tizen/Tools QEMU9.2.3，/usr/bin/qemu-arm-static为qemu-arm软链接；宿主同名文件为另一个x86静态ELF | history/*-emul.out；raw/013、012 |
| 历史其它负载 | NOT_OBSERVED：找到的全机进程审计为21:03、21:26，早于ARM22:56开始；小时记录只有本任务进度 | 不把本任务cgroup用量当全机负载；缺同期连续CPU/进程记录 |
| I/O暂停 | 两轮各有成功的探测，均无pause记录 | history/TIMINGS.json，不能据单次探测断言全程无等待 |

**对象来源一句话**：记录支持本轮对象来自新解包源码树内执行的编译/链接流程（%prep删除旧树、解包，随后Building/Linking及命令记录）；在所查路径未观测ccache，当前入口实测能转原生Clang加速。**但缺历史逐编译进程映像及缓存统计，无法把ARM约12倍提速唯一归因于某一种因素，确切原因仍为NOT_OBSERVED。** 没有为了补证而重构、改配置或清缓存。

新版任务log中的%prep及编译记录来源：../BPF_STATIC_0922/cells/bpf-armv7l-libcxx/build.log:4–17及后续Building行。原生加速存在与观察到的入口变化提供解释线索，不等同于本轮已经完成同环境性能归因实验。

## 两轮主包身份

| 架构/轮次 | 主包ELF SHA256 | .text SHA256 |
|---|---|---|
| armv7l / BPF_W1_0921 | `046115d23a6cda0bda5a38ca31bba8ed9f495fcdc7c2ae42f615d0d24dc7f3f4` | `0e24a9f54f7395e963fabc881725546c5455830981f65490f703379ce936e0b2` |
| armv7l / BPF_STATIC_0922 | `44b1b04faa5e75e3a7274b651fc65d0333c0bebeeff4a0ac08d8058a74598b8b` | `9756a28d094a7e0d7b4919e4a0ca907c103852163341d4027098fe3978abeec1` |
| aarch64 / BPF_W1_0921 | `7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec` | `f3032b4850f5fa85a0d0ad3252801b27fdba3ccf1fc655c76b8c9edd52a63528` |
| aarch64 / BPF_STATIC_0922 | `7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec` | `f3032b4850f5fa85a0d0ad3252801b27fdba3ccf1fc655c76b8c9edd52a63528` |

**ARM两项均不同；aarch64两项均相同。** 这不是static ELF的SHA比较。路径、提取.text的实际llvm-objcopy命令及退出码见build-audit/commands.json、ELF_COMPARISON.json。ARM差异未归因，不能以编译目标列表相同推断字节等价，也不能仅凭不同判定功能回归。留W4人工审阅；未修改主包或配方。
