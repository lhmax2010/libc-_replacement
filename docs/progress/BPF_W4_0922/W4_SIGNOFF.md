# W4 人工审阅项（当前不能签为提交完成）

| 审阅项 | 材料 | 当前状态 |
|---|---|---|
| 恢复事故副本及保留事故证据 | RESTORE_RESULT.md、RESTORE_RECORDS.json | 4/4 PASS，可核对 |
| 明确包仓允许改动范围 | DECISIONS.md、BASELINE_TO_VERIFIED_RECIPE.diff | **待裁决，阻断包提交** |
| 基线 | raw/010、012 | 远端与本地 f895f8c0373d224847fc7d3ecbeaac3bf926a1a1，干净、0/0 |
| spec 完整 diff | BASELINE_TO_VERIFIED_RECIPE.diff | 仅比较差异，尚未应用；不同于原 SPEC_DIFF.patch 的起点 |
| libbpf patch | ../BPF_STATIC_0922/recipe/static-link-libbpf.patch、PATCH_IDENTITY.json | 两份 SHA 一致，未在包仓添加 |
| 英文提交信息 | 下列待用内容 | 未提交、无代签 |
| SANDBOX_COMMIT.diff / git format-patch | NOT_AVAILABLE | 因范围冲突没有包提交，未制造空文件冒充 |
| prep-only 检查 | NOT_OBSERVED | 待范围确认及实际候选提交；不能以旧配方历史通过代替本轮 |
| -lstdc++fs | 前轮 BPF_STATIC_0922/DECISIONS.md | 不改，仍待人工定配方 |
| Source1002 | 当前未动 | 完整 recipe 仍引用原资产，GCC/未定义回退；需随实际获准 diff 再审 |
| 板上/安装边界 | ../BPF_W3R_0922/FINAL_RESULT.md | 原验收沿用；安装验证留待镜像阶段 |

若完整配方获准，提交信息还需准确描述**获准的完整前置变更**，不能只描述最后两行 patch。用户要求的原拟正文可作为其中一部分：

```text
Use the selected libbpf library in static builds

The local LIBCLANG_STATIC_PATH extension linked -lbpf despite libbpf.a
being available. Use ${LIBBPF_LIBRARIES} so that the static binary depends
only on glibc and libgcc_s.

Verified exact DT_NEEDED dependencies on armv7l and aarch64. The armv7l
board paired tests passed 12/12, and the shared-libgcc cancellation
probes passed 20/20.
```

上述为草案，未创建 git commit；无 Signed-off-by，留给人工。QuickBuild 前置：范围裁决 → 本地完整配方及 prep 核对 → 人工签字、授权推送包仓 → 核实远端身份 → 人工批准 QuickBuild。本轮不跨过这些门禁。
