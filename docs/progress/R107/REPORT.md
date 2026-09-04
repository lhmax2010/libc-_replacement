# R107：运行时验证独立分支与交接

## 结论

状态：`COMPLETE`。

- 基线分支：`codex/r5-ehabi-diagnosis`
- 基线提交：`e7790fc87f1cfcbd3620e51867263a16ce46fad5`
- 新分支：`codex/runtime-validation`
- 创建方式：从上述精确提交直接创建，没有 merge、rebase 或 cherry-pick。
- 创建前远端同名分支：不存在。
- 创建前远端分支数：3；完整快照见 `remote_branches_before.tsv`。
- 交接文件：`docs/RUNTIME_VALIDATION_HANDOVER.md`。

首次交接提交为 `f6020d79fa5da0c09673be8fb2a355f68be2685e`，已用普通
`git push` 创建远端分支。推送后远端对照见 `POST_PUSH_VERIFICATION.md`：
既有 3 个分支逐项 SHA 不变，新增恰好 1 条且远端 SHA 与本地一致。

## 交接说明覆盖

交接文件已覆盖：

1. 运行时线与编译适配线的职责边界；
2. 四补丁、两架构官方套件、五个 `noexcept` 站点、扫描与阳性校准、异步
   取消、跨布局/Boost 边界和三类共存场景；
3. `noexcept` 裁决、Boost 动态覆盖、`pen-wave`、aarch64、D02/D05 等未完成项；
4. 已排除或受严格限制的路线及其原因；
5. 材料路径索引、禁止触碰的并行编译工作和证据纪律；
6. 一个新会话接手 `noexcept` 决策的最短阅读与裁决路径。

自检命令确认所要求的章节、关键结论、状态标记和材料目录均存在，且
`git diff --check` 通过。原始结果见 `raw/008_handover_self_review.*`。

## 命令与异常

每条实际命令、stdout、stderr 与退出码分别保存在 `raw/`。执行中有一次技术性
错误：首次调用日志包装器时把编号与标签合成了一个参数，包装器返回 127；检查
接口后仅修正调用参数并成功重跑。该错误不涉及 Git 判据、分支内容或已有材料，
完整记录为 `raw/006_read_additional_reports_bash.*`，成功重跑为
`raw/006_read_additional_reports.*`。

## 修改与推送边界

- 本次新增交接说明和 R107 证据材料；
- 未修改任何既有进度材料、平台源码、补丁、spec 或配置；
- 工作区原有的无关未跟踪文件保持未改、未纳入提交；
- 不合并、不 force，不推送 `codex/r5-ehabi-diagnosis`、`main` 或其他分支。

## 自行判断与疑问

- 自行判断：把 R92/R93 的后续源码补扫状态也写入扫描结论边界，避免新会话把
  R87 的 371 包零命中误写为全平台绝对结论；没有改变 R87 的原始结论。
- 自行判断：在材料索引中加入 R75 的 `shrink_to_fit` 调用栈结论，作为“其他
  运行时差异”的已知案例，便于后续区分主动终止与 terminate handler 二次崩溃。
- 尚存疑问：无。`noexcept` 的未决项是明确留给人工的方案裁决，不是本次建分支
  操作的歧义。
