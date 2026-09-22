# W4 状态

| 部分 | 状态 | 产物 | 备注 |
|---|---|---|---|
| 资源预检 | PASS，light 退出 0 | raw/001 | 无构建/板测 |
| 四份副本恢复 | COMPLETE | RESTORE_RESULT.md | 事故文件保留 |
| 安全提取脚本修正 | COMPLETE | BUILD_AUDIT_FIX.diff、FIX_SMOKE.json | 新路径输出 |
| 提速线索只读核查 | COMPLETE_WITH_LIMITATIONS | BUILD_DISPATCH.md | 唯一原因 NOT_OBSERVED |
| bpftrace 本地配方提交 | BLOCKED_SCOPE_CONFIRMATION | DECISIONS.md、BASELINE_TO_VERIFIED_RECIPE.diff | 待范围裁决，未修改包仓 |
| prep 检查 | NOT_OBSERVED | W4_SIGNOFF.md | 不替人工选择候选 |
| 项目证据交付 | 按结尾授权提交推送 | 本目录及 docs/LINE_STATUS.md | 不推包仓 |

停止交人工审阅；没有以“只加两行”冒充完整已验证配方。
