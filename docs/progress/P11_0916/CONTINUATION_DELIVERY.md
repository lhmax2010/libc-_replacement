# 续任务交付记录

- 材料主提交：`e20f9e5b8bb7263313cd21d91bc678bb441132d1`。
- 实际普通推送：`git push origin codex/runtime-validation`，退出码 0。
- 推送后 `git ls-remote origin refs/heads/codex/runtime-validation` 返回同一 SHA；只推本线 GitHub 分支，没有 force、Gerrit 或平台分支写入。
- 主提交包括中英现行稿与归档、实验 1/3 原始记录和脚本、更新处置表、疑问/状态、自检及摘要。实验 2 已审阅的材料未修改；tmp 实验 ELF 未入库；其他会话的两个已有修改保留原摘要。
- 封存记录为 4,042 个文件、792 组完整命令；后续提交/推送回执另存于 [continuation_delivery](continuation_delivery/)，不伪造自引用摘要。
- 主提交版本的交付预检脚本 SHA256：`8a72583447977923970add64b01741c70c6d3599380ffbb491041576729670fd`，见 [原命令](continuation_delivery/009_preflight_digest.command.txt)。回执阶段仅为新增的本记录加入路径白名单，版本与新摘要另存后续记录；不放宽其他目录。

原始推送 [命令](continuation_delivery/007_push.command.txt)、[stderr](continuation_delivery/007_push.stderr)、[退出码](continuation_delivery/007_push.exitcode)；远端核验 [命令](continuation_delivery/010_remote_verify.command.txt)、[输出](continuation_delivery/010_remote_verify.stdout)。

本记录随回执补充提交再次普通推送；补充提交自身的最终 SHA 由最终交接消息及本地末次核验记录给出，以免自引用。核查及文档修正完成，停止实施，交人工审阅；未取得的真实反向 EWK 和未解决的 ARM 异常组合没有记成通过。

回执预检首次拒绝了新增 `CONTINUATION_DELIVERY.md`（旧白名单只包含主材料文件）；核对目标后仅补这一名称并重跑。该检查失败不是实验失败，也没有触及其他目录，原 stderr 保留。
