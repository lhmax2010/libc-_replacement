# 交付材料阶段记录

状态 `PARTIAL`；标注“待人工审阅后交付”。前置 W2 未完成，W3 相应数据也未齐，
本阶段只完成可独立整理和只读核对的部分，不声称完整交付。

产物：[中文说明与验证汇总](DELIVERY_ZH.md)、[英文说明与验证汇总](DELIVERY_EN.md)、
[五提交补丁快照](patches/)、[提交与摘要索引](COMMIT_INDEX.tsv)、[证据索引](EVIDENCE_INDEX.tsv)。

五提交均在原 sandbox 远端；补丁快照与原提交的稳定 patch-id 一致，源码工作树干净，
四个既有补丁及其覆盖内容未变。没有实施新改动、改写提交或推送 Gerrit。
已说明第二批与第一批分别解决不同问题，以及双版本、头文件重编和部署时点的限制。

五个提交说明的英文主题、原因、中英文解释和验证段落已核对；五个均缺 Change-Id，
后续通过只读配置继承链查明 `requireChangeId=true`，见 raw/019 和 GERRIT_CONFIG_CHAIN.tsv。
故现有提交至少未满足该项 review 接收前置；未试推 refs/for，实际服务器拒绝输出为 NOT_OBSERVED。
不能从 sandbox 推送成功推断 review 可直接通过。这个问题留给人工的 review 准备阶段，
本包没有自行补 footer、amend、rebase 或 force。

两架构 470 次定向证据、正常路径值、三方析构对照、现有官方对照及 ARM 3,554 项缺口
均已逐项链接。表中的官方 FAIL 原数不改写，overflow 按 W1 分类为已接受行为变化。
本包 ARM 续跑结果及支持声明相应数据仍待补，不能虚填完成。
W2 后续已清除十四项旧残留并核验，保留八份诊断包的主机备份，板子已清理并释放；
测试编译范围未收到答复，补测未启动，见 ../W2/REPORT.md。

资源 medium PASS；无构建、无板卡操作。导出补丁仅为交付快照，不改原四补丁。
脚本快照为 [verify_delivery.py](verify_delivery.py)、[validate_documents.py](validate_documents.py)，
SHA256 和验证输出见 raw/005、008；既有补丁保护检查见 raw/006。
配置继承查询脚本为 read_config_chain.py，追加核验及最新摘要见 raw/021。
用时以 raw/001 至封存记录为准，未触及四小时上限。
