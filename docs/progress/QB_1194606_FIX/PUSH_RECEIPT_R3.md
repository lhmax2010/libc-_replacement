# R3 项目材料推送回执

主体提交：`dbd57ba3583ffe545fd73b8b59ed14b31bdc8479`。

普通推送 `origin HEAD:refs/heads/codex/r5-ehabi-diagnosis` 退出 0，随后 ls-remote 返回同一 SHA；原文见 raw/087_R3_project_push.* 与 raw/088_R3_remote.*。本回执及提交/推送日志另作材料提交，最终项目远端 SHA 在交接回复给出。

三个包仓均未推送，只有指定 sandbox 名下的本地候选提交。cereal FAIL；ICU/qemu 验收 NOT_OBSERVED。R5 共 12 条已存在远端分支，未加入未验收候选；不是三项修复完成的 QuickBuild 开工清单。

提交自检只纳入本轮目录、LINE_STATUS、R101 cereal 一行与 BASE_LEDGER 补记；常见凭据格式扫描无命中，未读取凭据文件。原始日志的尾空格保留，raw/085 的全量 whitespace check 因原始输出返回2；随后只对手写 Markdown/Python/台账做格式检查通过，不改写原始证据。失败构建现场保留，无清理。
