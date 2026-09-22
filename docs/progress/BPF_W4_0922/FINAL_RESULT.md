# W4 准备结果：PARTIAL

四份误改主包副本已全部恢复并保留事故文件，安全提取脚本已修正。提速线索完成只读核查，但不足以确认历史编译器分派差异及唯一原因。**包提交发现任务范围与真实基线冲突，已停下待人工确认；没有创建或推送包提交。** 不能把本轮标为“提交已备好待签字”。

## 一、恢复与防复发

| 项 | 状态 | 证据 |
|---|---|---|
| 四个原 RPM 身份 | 4/4 PASS | RESTORE_RECORDS.json，原 RPM_INVENTORY 对照 |
| 四份恢复后 SHA = 改前 SHA | 4/4 PASS | RESTORE_RESULT.md |
| 四份事故副本保留 | 4/4 PASS | 原路径后缀 .objcopy-modified-0922，事故 SHA 不变 |
| 修正 objcopy 明确输出新 ELF | PASS | BUILD_AUDIT_FIX.diff、build_audit.py |
| 一次性副本验证 | PASS | FIX_SMOKE.json，输入字节和元数据未变 |

## 二、构建提速线索

| 项 | 结论 | 证据 |
|---|---|---|
| 两轮 launcher 显式 QEMU 包裹 make/cmake | 所查脚本无该语句 | BUILD_DISPATCH.md，带原文行号 |
| 两轮当前根 QEMU 与 /emul | 同一 ARM QEMU SHA、均有原生工具 | read-checks/*-root.out |
| 当前 binfmt | arm=P/qemu-arm-binfmt；qemu-arm=空 flags/qemu-arm-static | read-checks/host-*.out |
| 历史调度器差异 | 已观测 QEMU make 与直接 make 的不同记录 | W3R history；不等同于编译器差异 |
| W1 历史编译器 /emul 分派、提速唯一原因 | NOT_OBSERVED | 日志没有逐编译进程映像 |
| 正式 OBS 与本轮完全同形 | NOT_OBSERVED | 本地 GBS 保留根具备 accel，但缺 OBS 实际执行记录 |

## 三、包配方提交

| 项 | 状态 | 证据 |
|---|---|---|
| fetch 远端基线 | PASS，f895f8c0373d224847fc7d3ecbeaac3bf926a1a1 | raw/010、012 |
| 两份 libbpf patch 内容一致 | PASS | PATCH_IDENTITY.json |
| 原基线只增两行能否等于完整验证 recipe | 不能，缺 static 构建/安装等前置改动 | BASELINE_TO_VERIFIED_RECIPE.diff |
| 本地包提交 / format-patch | 未做，BLOCKED_SCOPE_CONFIRMATION | DECISIONS.md |
| 包仓工作树 / ahead、behind | 干净；相对 FETCH_HEAD 0/0 | raw/018、019 |
| 本轮 prep-only 验证 | NOT_OBSERVED，范围未确认 | W4_SIGNOFF.md |
| sandbox/包仓推送、QuickBuild、构建、上板 | 均未执行 | 本轮日志范围 |

请确认是否允许将完整已验证 recipe 的前置变更一并纳入本地提交；否则只能准备两行 patch，但它不是已验证 static 配方。未做此裁决。项目材料按任务结尾授权单独提交推送；回执见交付时远端 SHA。原 spec、Source1002、codes 不变。
