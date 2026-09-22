# 当前继续点

**本轮已停止，勿自动续跑**（2026-09-22）：两架构新配方写包及RPM/ELF验收PASS，共6RPM；W3在正常static安装预检退出1后停止。未正式安装，成对功能和取消均NOT_OBSERVED。未重试、未绕过、未修复。根文件系统92MB提示与MSM策略报错需人工裁决；原始预检输出保留。

板端无bpftrace相关包，五个受影响系统路径仍与装前一致，SDB root模式已恢复。五个上传文件按“失败现场保留”留在 `/var/tmp/bpf_static_0922/`，SHA清单见board/CLEANUP.json。没有运行中的本轮构建或板测进程。下一步仅完成材料发布/项目commit/push及远端核对，不再上板；后续W3、W4、QuickBuild等待人工。下文是过程记录，不是继续构建指令。

任务 BPF_STATIC_0922；项目分支 codex/r5-ehabi-diagnosis。

顺序：ARM整轮 → ARM三RPM与ELF验收 → aarch64整轮与验收 → ARM物理板。不得并行两个构建，不得拿BPF_LIBBPF旧ELF替代RPM内产物。板测必须ARM构建0且ELF PASS。

当前：两架构整轮退出0，各3RPM；ARM raw/046及aarch64 raw/068核验PASS。两套link-audit均完成，stdc++fs实际解析GCC14.2静态归档；不改。正准备执行raw/074板测，cell=board-armv7l。不得重建两架构。scope=bpf-static-0922.scope，内存16536457216。

新临时输入与spec位于tmp/BPF_STATIC_0922；原来源不变。所有命令与退出码在progress/BPF_STATIC_0922/raw。每步失败保留现场，不重跑覆盖。当前编号071，下一072；artifacts.py已汇总6RPM。上板脚本经审阅、SHA在raw/071；board.py与board-reviewed.py相同。

board.py已写、语法检查过但未运行，不可假称已有板测结果。执行前对脚本再核查：ARM PASS门禁；sdb正常连接192.168.108.26:26101，检查旧包/进程、root状态、空间，备份受影响文件，正常--test只安装static，RPM与原件分别旁放，stdout/stderr逐次远端分离并拉回，四种取消各5次/共享静态各20；正常卸载/恢复文件/root/清理自己文件。连接失败不重试；不猜环境失败原因；原始结果保留。用户已授权上板，不需要再次请求。

report.py在两架构和板测结束后生成最终汇总；需人工模型逐项读成对结果写BOARD_ANALYSIS.md，不能仅靠BOTH_NONZERO分类宣布功能成功。更新LINE_STATUS五节，publish.py/check_staged.py后只推项目分支，不推包仓。脚本snapshot落盘，各实际执行脚本不能被新版本冒充。

本轮无新增总时限，不套用上轮截止。未做板端任何动作，不能把前轮结果写成此次实测。用户明确要两格各3RPM、RPM提取ELF精确依赖/命名空间/归档交集、-lstdc++fs解析待办、板上原资产成对对照与两种取消各20次、装前后恢复。完整任务见会话最新用户请求。
