# W1 重启

开始：2026-09-21 21:01 +08:00。八小时时限：2026-09-22 05:01 +08:00，含本轮准备与收尾。

先原样推送 BASE_LEDGER_0921；不修改该轮任何内容。其后以 ARM_W5_0921 的 22 个 RPM 及既有 aarch64 12 个 RPM 作为固定输入。本轮不修改任何 spec 或上游 C++；codes/ 只读；不推包仓、不覆盖 Source1002，不上板。QuickBuild 按最新人工裁决等待 static 问题解决，本轮不启动。

开场日志器建立前的命令补记：`pwd` 退出 0（本项目根）；`git branch --show-current` 退出 0（codex/r5-ehabi-diagnosis）；`ls progress/BASE_LEDGER_0921 docs/progress/BASE_LEDGER_0921 tools/resource_gate.sh` 退出 2（docs 对账目录尚不存在，其他项存在）。
