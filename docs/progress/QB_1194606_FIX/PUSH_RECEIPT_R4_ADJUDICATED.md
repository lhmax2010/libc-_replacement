# R4 人工裁决后推送回执

| 包 | sandbox 远端 SHA | 原始输出 |
|---|---|---|
| cereal | a681c106a5866b6113b90a25307bf3abb3e76334 | raw/R4_012–013（沿用） |
| icu | b7ab6868d425e3190057e6d9f3c2b69646793df1 | raw/R4_054、058、060；dcbb3fb4 快进 |
| qemu-accel | 3b8d6274822cc2c2ce373eff7b921f5b3de3062e | raw/R4_068、070–071；新建授权名称分支 |

两次新增推送均 exit 0，均为普通 HEAD:refs/heads/sandbox/lhmax2025/libcxx-migration；没有 force、没有 Gerrit master 操作。qemu 推送的提交信息长度 warning 不影响成功，原文保留。

R5 14 行 ls-remote 全 MATCH：REMOTE_CHECK_R4_adjudicated.json、raw/R4_remote_adjudicated_01–14。材料普通推送到项目仓 codex/r5-ehabi-diagnosis，最终项目远端 SHA 在交接回复给出；不启动第二轮 QuickBuild。

补验主体材料提交与已回核项目远端：eb7e52e9f36603d367cb83f99b03708082d820d0。push 退出 0，原文 raw/R4_078_push_adjudicated.*；远端回核 raw/R4_079_remote_adjudicated.*。本回执及这些输出另作证据提交，最终远端随交接回复给出。

提交前暂存扫描 raw/R4_075：753 文件，超范围 0、单文件超过 10MB 为 0、常见凭据模式命中 0；未读取任何凭据文件，公开标识不当作秘密。后续回执提交另行同样扫描。无关的既有工作区改动不纳入提交。
