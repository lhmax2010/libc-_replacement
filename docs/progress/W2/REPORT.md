# W2 / R62：armv7l 全平台验证

> **勘误提示：** M2 的三项 armv7l 失败后来确认由探针夹具造成，详见 [ERRATUM-M2-ARMV7L-FIXTURE.md](ERRATUM-M2-ARMV7L-FIXTURE.md)。原报告结论文字保留，用于记录当时的观测状态。

## 结论

结论为 `PARTIAL`。

F1、F2、M1、七设施矩阵、R51 九格回归矩阵、十项 libc++abi 上游测试和九项非取消路径在 armv7l 真机上全部通过，并与同板 libstdc++ 参考逐格一致。F2 原文确认 `RDSTATE=1 BAD=1 FAIL=1`，M1 原文确认 callable 实际执行两次且第二等待者返回 `VALUE=42`。

M2 未达到 W2 判据：主窗口和 `wait_for` 在实验版与 libstdc++ 参考侧均未使等待方返回，15 秒外层保护终止进程，退出 143；async 多等待者在两侧均为 0/20，内部报告 `MULTI_WAITERS=TIMEOUT DONE=0 CANCEL_RC=0` 后退出 4。`void` 结果和真实异常格在两侧均通过。因为同板参考侧呈现相同失败，本轮只陈述 ARM 与 x86_64 的观测差异，不把它归因为实验补丁特有回归，也不推断其机制。

## 口径与环境

- 架构与执行方式：`armv7l`，192.168.108.26 开发板原生执行，不使用模拟器。
- 板卡：Tizen 11.0，build id `tizen-unified-toolchain_20260728.012216_tizen-headed-armv7l`。
- 基线源码提交：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
- 应用顺序：层 A、层 B、层 C 头文件累计补丁（含 W1 M2）、层 C 库内补丁；实验源 HEAD 为 `6cfe8835795d0ea791d68960a721a5b35737b267`。
- 构建：runtimes-only (`libcxx;libcxxabi`)，`RelWithDebInfo`，`LIBCXXABI_USE_LLVM_UNWINDER=OFF`，共享 libc++abi，`-j2`，`nice -n 15`、`ionice -c 3`。
- 资源门禁：`tools/resource_gate.sh --level medium` 退出 0。
- 分母：定向格按实际列出的独立运行次数；并发格每种运行时各 20 次；设施 7 个；R51 回归 9 个；上游测试 10 个；非取消路径 9 个。

## 构建与产物身份

构建成功，错误 0。日志含 144 条交叉编译器“参数在编译阶段未使用”警告（`--ld-path` 与 `--rtlib=libgcc`），没有一条指向本次修改源码。

| 产物 | SHA256 |
|---|---|
| armv7l `libc++.so.1` | `22d9ee51df953c470587d10c785567635b3a719b1456529db9cba99da61e6fc9` |
| armv7l `libc++abi.so.1` | `f63b9604b9a9490d62e2b87b0a8c8fd84e1a5d8947f2594bff2a656b2b175c46` |
| `r61_fixed` | `6cd47f81c296d792eb55690af77af6f5aba8a3473957e74bb83c4201145b90ba` |
| `m2_fixed` | `f82f58dce381689de4d490d167e0e63a52ff693ee135988931006aea1e0fee9a` |

`strings` 对两库检索 `R40_TRACE|R41_TRACE|R42_TRACE|R40_|R41_|R42_` 均无命中（`rg` 退出 1）。板上 `ldd` 与 `LD_DEBUG=libs` 均证明 fixed 测试件从 `/var/tmp/w2_r62_20260821/lib/` 加载上述实验版 `libc++.so.1` 与 `libc++abi.so.1`；参考测试件加载 `/lib/libstdc++.so.6`。

## 定向与并发结果

完整汇总见 [summary.tsv](tables/summary.tsv)，逐格退出码见 [009_board_matrices.tsv](evidence/raw/009_board_matrices.tsv)，每格未整理 stdout/stderr 位于 `evidence/raw/board/`。

- F1：两侧均 `F1_TOP_CATCH=ENTERED STATE_BAD=1`，foreign cleanup reason 为 1，退出 0。
- F2：两侧均取消成功、join 为 canceled，并提交 `RDSTATE=1 BAD=1 FAIL=1`，退出 0。
- M1：两侧均第一次执行被取消、第二等待者重试；`INVOCATIONS=2 VALUE=42`，退出 0。
- M2：value 主窗口与 `wait_for` 在两侧均超时；`void` 与普通异常格均通过。
- deferred 多等待者：实验版 20/20，参考 20/20。
- async 多等待者：实验版 0/20，参考 0/20；每次均 `DONE=0`、退出 4。

与 W1 x86_64 的分组逐格对照见 [arm_x86_comparison.tsv](tables/arm_x86_comparison.tsv)。决定性差异仅集中于 M2 的非 void 结果发布窗口和 async 多等待者路径；本轮没有取得其 ARM 机制层解释。

## 回归与非取消路径

- 设施矩阵：`cout`、自由 `getline`、成员 `getline`、`read`、算术输入、`packaged_task`、`async`，实验版与参考均 7/7。
- R51 回归：cancel、exit、真实异常、外来异常、无 catch、嵌套、cancel/exit catch-all 重抛等 9 格，两侧均 9/9。
- 上游：`forced_unwind{1,2,3,4}` 与 `unwind_0{1..6}`，两侧均 10/10。
- 非取消：async 正常值与真实异常、getline 成功/失败、packaged_task 异常、string 正常路径、packaged_task 正常/异常、算术格式失败，两侧均 9/9。

## 开发板收尾

首次长命令清理尝试被 sdb 报 `service name too long`，第二次推送辅助脚本到 `/var/tmp` 又受 Smack 执行限制；两次失败均完整保留，未冒充成功。随后用两条短的、精确目标命令删除 `/var/tmp/w2_r62_20260821` 与 `/opt/usr/home/owner/share/tmp/w2-r62-payload.tar`。最终只读核验确认：两个路径及辅助脚本均不存在，W2 相关进程匹配为空，`FINAL_VERIFY_EXIT=0`。

## 限制与未观测项

- M2 在 armv7l 的非 void 结果窗口、`wait_for` 和多等待者目标语义未得到验证；同板 libstdc++ 参考同样未通过这些用例。
- 本任务未诊断 ARM 与 x86_64 差异的根因；未观测到的机制结论记为 `NOT_OBSERVED`。
- 本轮只覆盖 armv7l 真机、动态 libc++、当前 toolchain 镜像；不覆盖 aarch64、LTO、混合运行时帧、动态加载边界。

## 材料

- 完整命令与退出码：`evidence/commands/`。
- 完整原始输出：`evidence/raw/`。
- 测试源码、四补丁、当前完整被改文件、构建配置与工具：`code/`。
- 文件来源和 SHA256：`code/INDEX.tsv`。
