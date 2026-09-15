# 夜间 adaptor 异常边界任务状态

实际执行日期按 UTC 记录；目录名沿用任务指定日期。分支 `codex/runtime-validation`，实验改动仅 `tmp/P12/`，平台源码和配置未改。armv7l 全部 `NOT_OBSERVED_BOARD_OFFLINE`。

| 阶段 | 状态 | 用时 | 产物 | 说明 |
| --- | --- | --- | --- | --- |
| W1 后端与前端异常边界 | FIXED_IN_MEASURED_CONFIGURATION | 约 20 分钟，精确见 raw 时间戳 | W1/REPORT.md | 原失败与 GNU-only 各 5/5 返回 ENOMEM；两放法逐位置及真实分配失败共 120 次保持输出 |
| W2 回归与扩展 | REGRESSION_PASS / CONTRACT_LIMIT | 约 10 分钟，精确见 raw 时间戳 | W2/REPORT.md | 正常矩阵未退化；70 个扩展格存活、10 个析构双异常终止；180 次分配窗口计数通过 |
| W3 契约 | NOT_OBSERVED | 未开始 | W3/ | 待以上实测完成 |

资源：medium gate 通过。串行编译/运行，nice19、ionice3；构建上限 4 GiB 虚拟内存（小于本机 50% 物理内存），普通测试同上限，分配压力用例进一步降低。只重建 adaptor 与转发源小目标，不全量构建 Chromium。

新增构建事实：原工具链的 GNU ld 在两个版本化 vtable 引用上链接失败；`--no-relax` 未解决；本机 GNU gold 2.42 可完成候选链接。此为固定工具链配置下的实测，不等于产品打包工具链已经验证。
