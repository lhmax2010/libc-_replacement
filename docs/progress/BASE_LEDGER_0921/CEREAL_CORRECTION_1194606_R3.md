# cereal 后续修订（2026-10-13）

原 BASE_73_LEDGER / OUTSIDE_ELEVEN_62 是固定快照的历史对账，保留原文。QuickBuild #1194606 的实际构建表明：cereal 的安装产物虽为头文件，其构建期 performance 仍消费 Boost.Serialization；原“无需适配”未覆盖这条构建期标准库一致性要求。人工已裁决将 cereal 纳入需改集合。

当前判定计数为 **12 需要改 / 55 无需改 / 6 按设计排除 / 0 未评估**。这是判定计数，**不是 12 个已推送**：现有 11 包已推送，新增 cereal 未推送。R101 表仅修改 cereal 行，不把失败候选标成“需要改，已推送”。

cereal 的 tizen HEAD 896a21f0 是 Initial empty repository；本轮按 manifest 的 28aedc95（tizen_base 同 SHA）建立指定 sandbox 本地候选 d62ccf97。四组宏检查 PASS，但 RBS 输入下 x86_64 构建退出 1，在 performance.cpp:261/280 的 uniform_int_distribution<char> 处失败，尚未链接、未写 RPM。候选不推，头文件 RPM 对照与产物 NEEDED 验收 NOT_OBSERVED。

证据：`../QB_1194606_FIX/FINAL_RESULT_R3.md`、`failure-cereal_R3/FIRST_ERROR.txt`、`build-cereal-attempt2_R3/build.log`、`raw/063_R3_cereal_build.*`。后续须人工裁决新的 performance 源码修复；不得转为跳过 performance。未启动第二轮 QuickBuild。
