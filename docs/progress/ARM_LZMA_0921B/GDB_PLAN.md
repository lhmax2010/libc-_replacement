# 本轮 GDB 现场观测方案

开始时间：2026-09-21 14:42:54+08:00；硬截止：20:42:54+08:00。

本轮按人工最新任务执行 GDB 观测，不执行此前建议的扩展 LD_PRELOAD。
此前第一节结论保留在 STAGE1_RESULT.md；人工已接受“第 19 包零 lzma_code 调用”。

## 输入、变量与现场

**15:11 修正记录**：第一次尝试的 `-0 rpmbuild` 与原 binfmt 实际 argv[0] 不同，已主动停止，未到任何 lzclose，更未到目标失败点。停止退出 -15；GDB 随目标终止退出 1；不算失败复现、也不算失败消失。完整记录在 cells/gdb-original。原 binfmt 与显式 QEMU 的只读 shell 对照均证实 argv[0] 为解析后的 /bin/sh；原 rpmbuild 路径为 /bin/rpmbuild。修正入口为 `/usr/bin/qemu-arm-static -g 49192 /bin/rpmbuild`，不再指定 -0；原始 RPM 参数尾部不变。有效轮使用 cells/gdb-preserved，差异见 ARGV_DIFF_CORRECTED.txt。前述错误尝试约耗时 15 分钟，硬截止不延长。以下含旧方案的历史描述保留，以上述修正为准。

- 原 argv：`progress/ARM_REPRO_0920/original-argv.json`。
- `ARGV_DIFF.txt` 逐项列出差异：host trace 的输出改为本轮目录；原 `rpmbuild` 入口改为构建根原有 `/usr/bin/qemu-arm-static -g 49192 -0 rpmbuild /usr/bin/rpmbuild`。`-0` 保留 guest argv[0]。
- 原 binfmt 规则没有 F 标志，因此在该构建根解析 `/usr/bin/qemu-arm-static`，它指向 QEMU 9.2.3；本轮显式入口仍是同一文件，而不是宿主另有的 QEMU 5.2.0。
- 原 strace 过滤器、RPM argv 尾部、spec、输出绑定、`QEMU_RESERVED_VA=0x100000000` 全部保持；无 `-vv`，无 LD_PRELOAD，无客体 `-strace`。
- 仍使用共享增量 BUILD 树；没有恢复到历史时刻的完整 BUILD 快照。因此不能声称历史环境所有时变状态逐字节相同。
- 上轮输出与 BUILDROOT 共 16,992 个条目复制至 `tmp/ARM_LZMA_0921B/previous-preload-failure`，全部普通文件 SHA256、符号链接目标、mode/uid/gid 校验一致；不清理原现场。

## 工具与断点

- 宿主原 GDB 不支持 ARM；下载并独立解包 gdb-multiarch，无系统安装。
- 独立解包同版本 rpm-debuginfo。三个 ELF 的 debuglink CRC 全部匹配，见 SYMBOL_IDENTITY.json。
- 独立符号有函数地址但没有 LZFILE 的完整类型。字段按实际 ARMv7 反汇编和结构声明读取：strm+0x8000，file+0x8058，encoding+0x805c，strm.internal+0x8024；FD fps+12，FDSTACK io+0/fp+4。保留反汇编原文。
- 返回点使用真实 epilogue 指令地址，不调用 finish、不单步、不在 inferior 调用函数，不修改变量。
- 启动时仅用动态加载器 rendezvous 解析 DSO 基址；随后仅 lzclose 计数断点开启。第 18 次计数时启用三个观察点，第 19 次关闭时关闭；每次自动继续。此前不设置高频 lzwrite 观察陷阱，避免无意义的调试往返。
- 三个观察点：lzopen_internal 返回寄存器 r0 已装好处、lzwrite 入口、rpmcpioHeaderWrite 入口。后两者入口参数遵循 ARM AAPCS。
- IO 字段实际为 `ioname="xzdio"`、`name="xz"`，两者一起记录；不能把正常的 name=xz 当作错误派发。
- 只有观察到用户条件 b（同指针且 encoding=1）才启用三个调用层的静态返回点，读 r0 并继续。失败消失则停止，不换其他仪表、不重编 RPM。
- GDB 全输出和每次即时 flush 的 JSONL 同时保存。调试器观察错误即中止，不能把失去仪表后的构建结果当作有效现场。

## 资源

`systemd-run --user --scope` 设置 `MemoryMax=16536457216`（50%）；nice 19、ionice idle；原 serial ninja 入口与 -j1 保留。guard 核验 /proc cgroup 和 memory.max。每 500 个目标做 I/O 探测，退化按既定等待规则；每小时落盘进度；截止前终止保存现场。

## 准备阶段技术性修正

调试 RPM 最初尝试的两个子目录 URL 返回 404，最终从仓库 flat debug 目录取得；两次过宽的历史资料检索被主动停止并改为精确文件读取。plain ARM 默认反汇编未正确识别 MOVW，已改为 armv7 并重取；独立符号自动搜索路径未命中，改为校验后显式加载。备用返回点的初始偏移落在文字池，正式复现前按反汇编修正为真实 pop 指令。上述均为准备阶段，未启动正式写包。
