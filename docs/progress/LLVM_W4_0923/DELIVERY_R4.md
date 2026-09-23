# R4 项目仓交付回执

- 材料提交：`ea96b547e31ed9c0b1316d051966a03aee341822`。
- 项目分支：`codex/r5-ehabi-diagnosis`。普通push退出0；随后ls-remote实测同一SHA，见raw/R4_030–033。
- 本回执与提交/推送日志作为后续回执提交归档；归档后的项目远端SHA写在最终回复中。
- 包仓未推送，本轮只登记人工已推的LLVM `5c169afc97945bfb88786d9f38477f6973fe5285`。
- 12行QuickBuild分支逐行远端MATCH；26项历史差异按人工裁决关闭，27项范围外登记，6个获准副本改动已在远端。
- 安全与范围自检PASS；旧R2/R3材料不改、PACKAGE_STATUS其它10包行不改、无构建/上板/QuickBuild操作。
- runtime独立源码构建单元依据固定20260828.101647快照；实时OBS实现方式NOT_OBSERVED，当前reference快照缺runtime的观测已单独申报。未隐藏这一限制。
