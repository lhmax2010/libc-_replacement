# W4 人工审阅项：本地提交已备好，待人工签字后推送

| 审阅项 | 材料 | 当前状态 |
|---|---|---|
| 恢复事故副本及保留事故证据 | RESTORE_RESULT.md、RESTORE_RECORDS.json | 4/4 PASS，可核对 |
| 明确包仓允许改动范围 | DECISIONS.md、BASELINE_TO_VERIFIED_RECIPE.diff | 已获人工授权完整配方，原“两行”限制取消 |
| 基线与本地提交 | raw/038、039、052；LOCAL_COMMIT_RESULT.json | 远端 f895f8c…；本地 72fda994…，干净、ahead 1 / behind 0 |
| spec 完整 diff | SANDBOX_COMMIT.diff | 与已批准 spec 差异内容一致；与 recipe 字节相同 |
| libbpf patch | ../BPF_STATIC_0922/recipe/static-link-libbpf.patch、PATCH_IDENTITY.json | 新增文件与原已验证 patch 字节一致，SHA abe6d2fa… |
| 英文提交信息 | COMMIT_MESSAGE.txt、SANDBOX_COMMIT.diff | 全变更已描述；无 Signed-off-by、不代签 |
| git format-patch | 0001-build-static-subpackage-from-source.patch | 本地提交导出，可供人工审阅 |
| prep-only 检查 | PREP_RESULT.md、prep-check/ | PASS，只到 %prep，计数 test 1=1，sed 实际执行 |
| 条件组合 | CONDITION_COMBINATIONS.md、condition-check/ | 默认 Clang 由 spec 派生为 1；两个不一致组合 NOT_OBSERVED |
| -lstdc++fs | 前轮 BPF_STATIC_0922/DECISIONS.md | 不改，仍待人工定配方 |
| Source1002 | 未动，SHA 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123 | GCC/未定义工具链路径仍安装该原资产，%post 逻辑不变 |
| 板上/安装边界 | ../BPF_W3R_0922/FINAL_RESULT.md | 原验收沿用；安装验证留待镜像阶段 |

## 人工推送前核对

包仓工作目录：`/home/toolchain/development/libc++_replacement/tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push`。

期望本地 HEAD：`72fda9941031fc35d8825e73446ca43153c4b69b`。本轮只创建一个本地提交，未推送。提交前后仅两个文件改变，Source1002 与其它文件保持。

请审阅实际英文信息中四项 static 配方改动、libbpf patch、验证范围；实际板测 12/12 和共享 libgcc 取消 20/20 来自已完成的前轮，不是本轮重跑。若人工加 Signed-off-by 而 amend，SHA 会改变，应重新核对并登记实际新 SHA。

签字材料还须明确：`-lstdc++fs` 保持原状；条件组合未覆盖项不等于通过；rpm 正常安装及 %post 覆盖仍留待镜像阶段，当前可写区验证不能替代。

人工自行执行（本轮未执行）：

```bash
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-migration
git ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-migration
```

若未 amend，推送后远端必须为 `72fda9941031fc35d8825e73446ca43153c4b69b`。不得 force；拒绝即停报。QuickBuild 前置为：**人工审阅签字 → 人工推送并核对远端 SHA → 人工批准 QuickBuild**。本轮没有启动 QuickBuild。
