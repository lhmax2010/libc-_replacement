# ICU 首个未满足的验收项（不是编译错误）

> 后续状态：人工修订门禁后已补测全部六库；两份例外条件均成立，ICU_ADJUDICATION_R4.json 全 PASS，b7ab6868 已推。以下旧门禁记录不删除。

GBS 退出 0，tz2icu 的 -iquote 已实测，原 version 遮蔽错误未再出现。未修改候选 b7ab6868。

严格按人工 R4“libicu* 各 .so 的 NEEDED 含 libc++.so.1”检查，首个不满足的是 libicudata.so.78.1，随后 libicui18n.so.78.1 也不满足。六份 ELF 均无直接 libstdc++.so.6 依赖，但不能据此将直接 NEEDED 门禁改成间接依赖门禁。

- libicudata.so.78.1：libc++abi.so.1、libc.so.6。
- libicui18n.so.78.1：libc++abi.so.1、libicuuc.so.78、libm.so.6、libgcc_s.so.1、libc.so.6。
- libicuio、libicutest、libicutu、libicuuc：直接含 libc++.so.1。

原文：raw/R4_018_icu_NEEDED.stdout.txt；机器判定：verify-icu_R4/result.json。构建与提取现场在 tmp/QB_1194606_FIX/GBS-R4/icu/、tmp/QB_1194606_FIX/verified_R4/icu/，保持不清理。人工未授权例外前不推 ICU，不自行添加链接参数。
