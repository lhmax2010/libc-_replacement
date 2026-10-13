# QuickBuild #1194606 修复任务：基线门禁阻断

**BLOCKED_BASELINE_MISMATCH。** 在克隆前的只读远端核对中，cereal 的当前 tizen HEAD 与任务指定的实际 checkout SHA 不一致。按第一部分第1条“不一致则停下回报”停止，未继续其它包、未应用补丁、未构建、未建立或推送包仓分支。

## 1. 实际证据

命令（原文与退出码也在 raw/002_cereal_refs.*）：

```sh
git ls-remote ssh://lhmax2025@review.tizen.org:29418/platform/upstream/cereal refs/heads/tizen refs/heads/sandbox/lhmax2025/libcxx-migration
```

退出码：0。stdout 原文：

```text
896a21f0cbcb34835bba8d39f2a4254d691ea98f	refs/heads/tizen
```

| 核对项 | 任务要求 | 实测 | 判定 |
|---|---|---|---|
| cereal 当前 tizen HEAD | 28aedc95fdc600568c57babf28ff717e0b2bca7d | 896a21f0cbcb34835bba8d39f2a4254d691ea98f | 不一致，触发停止 |
| cereal libcxx-migration | 从 tizen 新建 | 精确目标 ref 查询无返回行 | 当前查询未列出该分支；未创建 |
| icu 指定 sandbox HEAD | dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 | NOT_OBSERVED | cereal 门禁失败后未继续查询 |
| qemu-accel 当前 tizen HEAD | 2737f3bf78ed33be4333114c0e5c741a1deca467 | NOT_OBSERVED | 同上 |

没有把“不一致”解释为远端一定前进、回退或分支写错；原因未核实。也没有自行改用 tizen_base、旧 SHA 或其它分支来绕过“当前 tizen HEAD”要求。

## 2. 本地验证与推送状态

| 包/验证项 | 状态 | 退出码/验收 | 原因 |
|---|---|---|---|
| cereal x86_64 构建及 performance/头文件验收 | NOT_STARTED | NOT_OBSERVED | 基线不一致 |
| icu x86_64 构建、-iquote、libicu NEEDED | NOT_STARTED | NOT_OBSERVED | 已按第1条停止 |
| qemu-accel-armv7l x86_64 构建及清单 | NOT_STARTED | NOT_OBSERVED | 同上 |
| qemu-accel-aarch64/riscv64 宏展开 | NOT_STARTED | NOT_OBSERVED | 同上 |
| 三个包的提交与 git show | NOT_CREATED | 不存在本轮包提交 | 未修改包仓 |
| sandbox 推送 | NOT_STARTED | 未执行任何包仓 push | 未过源码/验证门禁 |
| QUICKBUILD_BRANCHES_R5.tsv | NOT_GENERATED | 无新 SHA，不伪造清单 | R4 保留原状 |
| cereal 台账与12/55/6/0计数 | 未修改 | 不能登记“已推送” | 未发生推送 |

medium资源门禁退出0，load1=0.87，上限20；磁盘可用226GiB（raw/001、003）。未启动构建进程，因此本轮 cgroup/并行度/内存实际落实、I/O构建探测、RBS repomd SHA均 NOT_OBSERVED；未宣称执行过。

## 3. 待人工确认

请明确 cereal 的起点：是否允许基于已核验的 `28aedc95fdc600568c57babf28ff717e0b2bca7d` 创建指定 sandbox 分支，而不以当前 `tizen` HEAD 为起点；或提供应核对的源分支。本轮不自行选择。恢复时其它两个仓仍须实时核对，不沿用历史 HEAD 冒充本轮结果。

已定的三个修法不重新询问，也没有改为跳过 performance。仅提交本轮阻断材料与 LINE_STATUS，保留所有既有不相关修改；不改 codes/config/spec、不上板、不起 QuickBuild、不推 Gerrit/包仓。项目仓回执见 PUSH_RECEIPT.md；停止等待人工。
