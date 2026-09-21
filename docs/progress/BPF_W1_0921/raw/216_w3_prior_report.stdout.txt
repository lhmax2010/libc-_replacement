# W3：armv7l 物理板验证

**PARTIAL。取消探针完成；安装与功能等价未闭合。** 没有安装候选或原资产 RPM，没有修改板上系统库。

## 环境与输入

- 物理板：`192.168.108.26:26101`，内核 `6.12.80-arm-rpi4-v7l #1 SMP Tue Jul 28 02:41:25 UTC 2026 armv7l`。
- 板上 glibc `2.40-2.8`、libgcc/libstdc++ `14.2.0-1.14`。命令及实际输出见 `board-preflight/`、raw/518。
- 普通用户与板上 root 两次尝试 `/proc/config.gz`、`/boot/config-*`，均未得到配置内容，记 `NOT_OBSERVED`，不能推断 BPF 已关闭。
- 预检未见指定测试进程、任务目录、bpftrace 包或 `/usr/bin/bpftrace`。仅覆盖列明的进程、RPM 和路径，不声称扫描了整块磁盘。
- 使用本轮已核验运行库 RPM 中的 ARM `libc++.a`、`libc++abi.a`，不是新 W2 LLVM 部分产物；输入和归档 SHA 见 `verify_runtime_armv7l/result.json`、`arm_cancel_compile_commands.json`。
- 原样探针 `condition_cancel_probe.cpp` SHA256 `5942cf9cbb335e57b2660e71c6f7db521af8fce01d708413a4841fcd4e7fa805`；工具链实际 Clang 22.1.8、目标 `armv7l-tizen-linux-gnueabi`，编译仍经既有 QEMU 入口。没有将 aarch64 原生工具许可外推到 ARM。
- 限时运行器通过返回 0、返回 7、超时返回 124 三个自检；每个试验禁 core、最多 20 秒。脚本、链接命令、map、ELF 动态表和符号表保存于本阶段材料。

## 取消实测（物理板）

| 链接形态 | wait | system_clock | steady_clock | custom_clock | 合计 |
| --- | --- | --- | --- | --- | --- |
| 静态 libc++/libc++abi + 共享 libgcc_s | 5/5 | 5/5 | 5/5 | 5/5 | 20/20 PASS |
| 静态 libc++/libc++abi + 静态 libgcc | 5/5 | 5/5 | 5/5 | 5/5 | 20/20 PASS |

每次均实际出现等待确认、清理一次、`cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1`，远端退出码 0。不是仅依据 SDB 传输返回码。逐次原文见 `board_cancel/results.json`，聚合表 `BOARD_CANCEL_MATRIX.tsv`。

共享组 SHA256 `c5aaec1fdf777e30faa1eb68205a11f10bcc68270182177f3c98b588eeb88c9d`，静态组 `be3dfa4d7b6f1f0066615449a27576c339283b7efc91b307c3f268f695993100`。传输前后 SHA 一致，见 `board_cancel/transfer_sha256.json`。

共享组的 DT_NEEDED 有 `libgcc_s.so.1`；静态组没有。两组没有 libc++/libc++abi/libstdc++ 动态依赖；探针另依赖 libatomic、glibc/libm/libpthread 和加载器，不能把探针依赖表冒充 bpftrace 候选的表。静态 map 明确引入 GCC `14.2.0/libgcc_eh.a(unwind-arm.o)`、`libgcc_eh.a(libunwind.o)`、`pr-support.o` 和 `libc++abi.a(cxa_personality.cpp.o)`；这里的 `libunwind.o` 是 GCC 归档的成员名，不是 LLVM libunwind 库的证据。

**关键差异：静态 ARM 对照没有按任务书预期崩溃。** 因而不能声称 ARM 已复现 x86_64 的寄存器尺寸表故障。为何两个架构结果不同，本阶段未追踪到调用/源码级，记 `NOT_OBSERVED`；不根据 ARM 使用另一套展开 ABI 就推断确切原因。通过也不证明所有 ARM 程序或原预编译资产必然安全。

## RPM 安装与功能测试

候选 ARM libc++ 主包未完成写包，所以候选安装、`%post static` 覆盖、版本/info/probe/三脚本对照均 `NOT_OBSERVED`。不使用 GCC 包代替候选、不借用 aarch64 结果。

为补可做的部分，独立尝试已核验的原资产 GCC 路径三包（main/common/static）基线：

1. 三个 RPM 和限时运行器传输后逐一 SHA256 核对通过，见 `board-install-original-only/transfer_sha256.json`。
2. 正常 `rpm -ivh --test` 远端退出 **3**，缺 `libbcc_bpf.so.0`、`libclang-cpp.so.22.1`、`libclang-cpp.so.22.1(LLVM_22.1)`、`libclang.so.22.1`、`libclang.so.22.1(LLVM_13)`。
3. 原文在 `board-install-original-only/016.stdout`；helper raw/529 退出 1 表示门禁未闭合，不是连接中断。
4. 没有使用 `--nodeps`、没有安装/升级系统依赖、没有执行 RPM 安装。原资产板上功能仍为 `NOT_AVAILABLE`（已尝试正常依赖预检）；缺的运行库需后续准备并确认部署范围。

## 清理与权限

取消试验的三个上传文件、原基线的四个上传文件均已删除；三个专用目录已核验不存在；板上测试包和 `/usr/bin/bpftrace*` 没有遗留；取消进程检查为空。恢复最初普通用户/root-off 状态。只删除本任务上传副本，本地原件和记录仍保留，可重新上传。

证据：`board_cancel/cleanup-process-check.json`、`board_cancel/summary.json`、`board-install-original-only/cleanup.json` 及其逐条命令。全部板上 root 操作清单见 `BOARD_ROOT_OPERATIONS.tsv`；PC sudo 调用 **0**，没有读取密码。连接全程未中断。

## 本阶段自行判断与限制

- 原基线可独立尝试，但不构成候选比较；为此给尚未执行的安装 helper 增加明确的 `--original-only` 模式，保留原比较模式，执行版本已快照。
- ARM 静态对照与 x86_64 不同是实测事实；没有为匹配预期修改探针或参数。
- 合格 ARM 候选 RPM 及板上依赖仍缺，因此不能以取消通过宣布可替换发布资产。
