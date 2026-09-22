# W4 准备结果（含续轮）：本地提交已备好，待人工签字后推送

**最新结论：** 人工已批准完整配方范围，上一轮阻断解除。包提交 `72fda9941031fc35d8825e73446ca43153c4b69b` 已本地创建，spec/patch 与已验证 recipe 逐字节一致，ARM prep 通过，工作树干净、ahead 1 / behind 0。未推包仓。条件组合核查及未覆盖的两个组合已写入签字材料。以下先保留上一轮记录，再追加本轮三部分结果；当前状态以末节为准。

## 上一轮记录（范围裁决前，保留）

四份误改主包副本已全部恢复并保留事故文件，安全提取脚本已修正。提速线索完成只读核查，但不足以确认历史编译器分派差异及唯一原因。**包提交发现任务范围与真实基线冲突，已停下待人工确认；没有创建或推送包提交。** 不能把本轮标为“提交已备好待签字”。

## 一、恢复与防复发

| 项 | 状态 | 证据 |
|---|---|---|
| 四个原 RPM 身份 | 4/4 PASS | RESTORE_RECORDS.json，原 RPM_INVENTORY 对照 |
| 四份恢复后 SHA = 改前 SHA | 4/4 PASS | RESTORE_RESULT.md |
| 四份事故副本保留 | 4/4 PASS | 原路径后缀 .objcopy-modified-0922，事故 SHA 不变 |
| 修正 objcopy 明确输出新 ELF | PASS | BUILD_AUDIT_FIX.diff、build_audit.py |
| 一次性副本验证 | PASS | FIX_SMOKE.json，输入字节和元数据未变 |

## 二、构建提速线索

| 项 | 结论 | 证据 |
|---|---|---|
| 两轮 launcher 显式 QEMU 包裹 make/cmake | 所查脚本无该语句 | BUILD_DISPATCH.md，带原文行号 |
| 两轮当前根 QEMU 与 /emul | 同一 ARM QEMU SHA、均有原生工具 | read-checks/*-root.out |
| 当前 binfmt | arm=P/qemu-arm-binfmt；qemu-arm=空 flags/qemu-arm-static | read-checks/host-*.out |
| 历史调度器差异 | 已观测 QEMU make 与直接 make 的不同记录 | W3R history；不等同于编译器差异 |
| W1 历史编译器 /emul 分派、提速唯一原因 | NOT_OBSERVED | 日志没有逐编译进程映像 |
| 正式 OBS 与本轮完全同形 | NOT_OBSERVED | 本地 GBS 保留根具备 accel，但缺 OBS 实际执行记录 |

## 三、包配方提交

| 项 | 状态 | 证据 |
|---|---|---|
| fetch 远端基线 | PASS，f895f8c0373d224847fc7d3ecbeaac3bf926a1a1 | raw/010、012 |
| 两份 libbpf patch 内容一致 | PASS | PATCH_IDENTITY.json |
| 原基线只增两行能否等于完整验证 recipe | 不能，缺 static 构建/安装等前置改动 | BASELINE_TO_VERIFIED_RECIPE.diff |
| 本地包提交 / format-patch | 未做，BLOCKED_SCOPE_CONFIRMATION | DECISIONS.md |
| 包仓工作树 / ahead、behind | 干净；相对 FETCH_HEAD 0/0 | raw/018、019 |
| 本轮 prep-only 验证 | NOT_OBSERVED，范围未确认 | W4_SIGNOFF.md |
| sandbox/包仓推送、QuickBuild、构建、上板 | 均未执行 | 本轮日志范围 |

请确认是否允许将完整已验证 recipe 的前置变更一并纳入本地提交；否则只能准备两行 patch，但它不是已验证 static 配方。未做此裁决。项目材料按任务结尾授权单独提交推送；回执见交付时远端 SHA。原 spec、Source1002、codes 不变。

## 续轮一、本地包提交与 prep

| 项 | 结果 | 证据 |
|---|---|---|
| 再次 fetch 基线 | f895f8c0373d224847fc7d3ecbeaac3bf926a1a1，初始干净 | raw/038–039 |
| 新本地提交 | **72fda9941031fc35d8825e73446ca43153c4b69b** | LOCAL_COMMIT_RESULT.json |
| 变更范围 | 仅 packaging/bpftrace.spec、packaging/static-link-libbpf.patch | SANDBOX_COMMIT.diff |
| 文件身份 | 两文件均与 recipe 逐字节一致 | spec SHA ccb538d0…；patch SHA abe6d2fa…，完整值见 LOCAL_COMMIT_RESULT.json |
| 差异一致性 | spec hunk 坐标/内容与批准差异一致；新增 patch 单独字节校验 | 比较时忽略 Git 附加的 hunk 上下文提示，不忽略增删内容 |
| Source1002 与其它文件 | 未修改 | Source1002 SHA 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123 |
| prep | PASS；setup、Patch0、原资产解包、test 1=1、sed 均通过 | PREP_RESULT.md、prep-check/ |
| 包仓状态 | 干净，ahead 1 / behind 0，远端仍 f895f8c… | raw/052；**未推送** |
| 审阅文件 | 完整 git show、format-patch、英文提交信息，无代签 | SANDBOX_COMMIT.diff、0001-build-static-subpackage-from-source.patch、COMMIT_MESSAGE.txt |

## 续轮二、条件组合

| 项 | 结论 |
|---|---|
| 项目配置 | 全项目默认 `_toolchain=clang`，可由包级 override 选 GCC；不定义 build_with_libcxx |
| gbs_llvm.conf | 仅 profile/repos/buildroot，没有定义这两个宏 |
| build_with_libcxx | 由本包 spec:1–5 在 Clang 条件下派生为 1，不是独立的全局开关 |
| W1 六格 | 两架构分别 clang/1、gcc/未定义、未定义/未定义；原 argv 与日志逐项列于 CONDITION_COMBINATIONS.md |
| 两个不一致组合 | 构建阶段 clang/未定义与 gcc/1 均 NOT_OBSERVED；解析前未定义不冒充额外测试 |
| QuickBuild | 已核本地配置默认 Clang 路径两条件同步；未获服务器最终宏快照，不无条件保证所有配置 |

## 续轮三、签字与下一步

| 项 | 状态 |
|---|---|
| W4 | **本地提交已备好，待人工签字后推送** |
| 人工审阅 | 完整 diff/patch/提交信息、条件组合边界、-lstdc++fs、Source1002 回退、安装验证边界 |
| 未闭合事项 | rpm 安装/%post 留待镜像阶段；-lstdc++fs 不改；历史提速唯一原因仍未定 |
| 推送与 QuickBuild | 人工推包仓并核对 72fda994… → 人工批准 QuickBuild；本轮均未执行 |
| 项目材料 | 仅本轮目录追加与 LINE_STATUS 提交推送，远端回执另列 |

自行判断及两次检查器技术性非零见 DECISIONS.md 续轮补记；没有修改 recipe 来凑验收。
