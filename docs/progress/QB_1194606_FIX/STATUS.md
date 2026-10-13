# 状态

## R4 当前续跑

**PARTIAL，构建已停止，待人工审阅门禁差异。** cereal 追加 a681c106（父 d62ccf97 不动），四组宏、完整构建、performance 链接/NEEDED、前版清单与 90 个头文件内容检查全部 PASS，已推指定 sandbox 并回核。

ICU 构建退出 0、原 tz2icu 错误消失，但 libicudata/libicui18n 不含直接 libc++ NEEDED，按原门禁 FAIL、不推。qemu-armv7l 与 aarch64 构建退出 0、无 unpackaged；aarch64 同变体清单新增 8 项而非 2 项，FAIL、不推；依“失败停该包”未启动 riscv64，记 NOT_OBSERVED。所有现场保留，未改 ICU/qemu 修法。

R5 13 行远端逐行 MATCH，但**不可作为第二轮 QuickBuild 开工清单**。R101 cereal 已推，判定计数仍 12/55/6/0。详见 FINAL_RESULT_R4.md、ICU_FIRST_ERROR_R4.md、QEMU_FIRST_ERROR_R4.md；项目推送回执见 PUSH_RECEIPT_R4.md（收尾写入）。

## R3 当前续跑

**R3：STOPPED_ON_CEREAL_VALIDATION_FAILURE。** 三个精确源码基线已确认，本地候选已建立，五份 spec × 四种宏组合检查 PASS（旧根与本轮 RBS 根分别验证）。cereal 的 x86_64 GBS 构建退出 1，约 450.5 秒，在 98% performance 编译时 uniform_int_distribution<char> 触发 libc++ 静态断言。按原 FIX 第 8 条停止，不调整修法、不启动 ICU/qemu 构建，三个包均未推送。原 R1/R2 ref 阻断已解决，记录仅作历史。

现场：tmp/QB_1194606_FIX/GBS-ROOT/local/BUILD-ROOTS/scratch.x86_64.0；首错与关键输入另存 failure-cereal_R3/。R101 cereal 已依人工裁决改为需要改、未推送；判定计数 12/55/6/0，不声称 12 已推。R5 只列已存在的 12 条远端分支，不加入未验收候选。详见 FINAL_RESULT_R3.md。

## R1/R2 历史

**最新：BLOCKED_PATCH_NOT_APPLICABLE，已停止。** 人工已授权从896a21f0起建；本轮实际克隆并建本地sandbox分支后发现该提交为空树，spec/CMakeLists均不存在，git apply --check退出1。无补丁实施、构建、包提交/推送，R5与台账未改。见FINAL_RESULT_R2.md及raw/016–028。以下为首次阻断的历史状态。

| 阶段 | 状态 | 证据 |
|---|---|---|
| medium资源门禁 | PASS，退出0 | raw/001_resource.* |
| 磁盘 | 226GiB可用，退出0 | raw/003_disk.* |
| cereal tizen基线 | MISMATCH，ls-remote本身退出0 | raw/002_cereal_refs.* |
| 其它仓核对、补丁、包提交 | NOT_STARTED | 第1条要求停止 |
| 本地构建/验收、包推送 | NOT_STARTED | 同上 |
| R5、已推送台账 | 未生成/未修改 | 不能提前登记完成 |
| 项目材料提交推送 | 收尾后见回执 | PUSH_RECEIPT.md |

下一步：人工明确含真实源码的cereal基线或如何从空仓引入源码后再恢复。三个既定修法未改变，未自行退回28aedc95。
