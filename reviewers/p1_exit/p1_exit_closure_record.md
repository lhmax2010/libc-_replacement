# P1 出口关账记录

- 关账日期：2026-07-30
- 最终 corpus HEAD：`c61abe4d02a721ea7b8dbf7c2c596c0be2e93aa8`
- 终局：`P1_EXIT_REVIEW_PASSED`
- 范围：r0 全量评审基线，以及 r1–r6 六轮整改/定点复核

## 评审沿革

| 轮次 | 评审输入及 SHA256 | corpus HEAD | 阻塞、整改与复核方 | 该轮结论 |
|---|---|---|---|---|
| r0 | `p1_exit_review_v1.tar.gz` — `1ca767dfb826291e7e9b118521b59d3595c99db6aeb865a2512a5d83b57712ec` | `8fc31c637690fe1882cbf3cfcb28224e208e1264` | 全量对抗评审。Claude 报 B1–B3（波次归属治理、门禁假绿、签核 NEVRA 继承）；Kimi 报 D5 未落地、D-G1 新边 triage 缺失、排期仍为 12 包；GPT/三方整改汇总形成 C1–C6。复核方：GPT、Claude、Kimi。 | `FAIL`（全量） |
| r1 | `p1_exit_review_v2_remediation.tar.gz` — `0d924fad75ce503090a6e5d680bd550948a870d7f65bffc2473d59d6cb8466f8` | `40e99437e7e34e57e7da2fa543a2eb0f1a2913b6` | 完成 C1–C6：签核继承、快照新边 triage、混合标签消解、门禁 v1.1、执行态/S6、排期同步。Claude、Kimi 定点复核为 PASS；GPT 仍判 T1-0008 分支闭包及 plan 状态同步三项 `NOT_CLOSED`。 | `NOT_CLOSED` |
| r2 | `p1_exit_review_v3.tar.gz` — `4cc5e814c94c34943403b3c3eff891616ca7837862ef8965ff53537f18450f55` | `2e93c633d54e2b9d86e5dca3b4da55ba2d0d2889` | 修复 T1-0008：两条 cross-repo CPP_ABI 边在任一 S4 分支均保持四包同批；同步 plan、启动权威及稳定证据引用。Claude、Kimi确认；后续复核要求补 source-output 闭包和 D-G4 分层执行规则。 | `NOT_CLOSED` |
| r3 | `p1_exit_review_v4.tar.gz` — `e83e77aeb032b4893c19038d96447b65f34c305671bf9bae589a5a7b46406fff` | `916822ef249d94b20965866718d7c6352f078bc2` | 建立 source-unit census、D-G4、ADMIT/HOLD 晋级模板及三场景规则。Claude、Kimi确认；GPT r3 复核仍指出 noarch 输出域遗漏、第四门未形成可信可执行闭包等关闭条件。 | `NOT_CLOSED` |
| r4 | `p1_exit_review_v5.tar.gz` — `d379012baf8206d976a6f5e121caf72709fd2914f5d6ae34f392cbdbb97e541c` | `a0727098b1e6d38de53d3601e798dfe3ccca6f0f` | 补 noarch/共享输出，重分类 `boost-license`，新增第四门 v1.2 和三组台账实例。Claude、Kimi确认；GPT 复核发现 R4-NB-01：幽灵行、payload 身份未绑定、闭包信任 manifest 自报字段。 | `P1_EXIT_STILL_FAIL` |
| r5 | `p1_exit_review_v6.tar.gz` — `b58e564b594a02446494e5f31b02ac4d0e3795301dc3f68f72ace49605876c37` | `72f4ff613b006214073e712b85230b9443f78155` | 第四门 v2：双向完备、五元身份与三方 SHA、认证 census 闭包，v1.3 全矩阵重冻。Claude、Kimi确认；GPT 复核发现 R5-NB-01：`ADMIT_STDLIB_NEUTRAL` 资格仍可由台账自报。 | `P1_EXIT_STILL_FAIL` |
| r6 | `p1_exit_review_v7.tar.gz` — `52413f783f5de9ee6bc4848da6265bf328372f44e6fc0ea0928debc3b5aa98e2` | `c61abe4d02a721ea7b8dbf7c2c596c0be2e93aa8` | 建立受信 neutral 登记册并挂入固化 authority 根；第四门 v2.1 精确匹配；删除自报 proof 列；v1.4 全矩阵 42/42（5 绿 + 37 红）重冻。GPT 判 `P1_EXIT_PASS`，Claude、Kimi均 `CONFIRM`，无新阻塞。 | `P1_EXIT_REVIEW_PASSED` |

## 终局三方文件

| 复核方 | 仓内归档路径 | 原始文件 SHA256 | 终局原文 |
|---|---|---|---|
| GPT | `reviewers/p1_exit/r6/gpt_closure_verdict_r6.md` | `163f54f7076bf6cd9d2e84c8b70773743303fd118bc7738552cf7860101f4210` | `P1_EXIT_PASS` |
| Claude | `reviewers/p1_exit/r6/claude_confirmation_neutral-registry.md` | `109aa9de9ccfe9b45f2ae7fda2b6be0f6f1013988444c6db732ae26ac4822524` | `CONFIRM`；`P1_EXIT_PASS` 总裁定继续成立 |
| Kimi | `reviewers/p1_exit/r6/kimi_confirmation_r6.md` | `769abb562e491fca2e2f06cde5905bc15a871af3242e2f4d76673714cdc282b6` | `CONFIRM`；残余阻塞为无 |

## 归档边界

仓内 `reviewers/p1_exit/` 保存本地工作区中实际落盘的全部 P1 出口评审文件，
并以 `MANIFEST.sha256` 逐文件锚定。r0–r3 的 GPT 详细意见在当时以整改合同
进入 C1–C6、R2、R3、R4 的任务与报告，工作区没有独立的 reviewer 文件；
本记录不虚构缺失文件，以对应整改报告和逐轮输入包 SHA 作为沿革证据。

本关账只表示 P1 出口评审已通过，不把仍需在执行期完成的 S1 签署、D5 正式
并入、S6 v2 重冻、候选构建和板测自动改判为 PASS；这些状态继续以
`wave1/startup_conditions.md` 为唯一权威。
