# 状态与交接

验证及汇编增补完成，待人工审阅。资源检查均通过；串行，分析/编译使用 nice 19、ionice 3，RLIMIT_AS 为 9,921,875,558 字节（物理内存 30%）。未使用开发板。

开始资源检查：2026-09-21 06:39:49 UTC；完成实测与自检：06:55 UTC，约 16 分钟，不含提交推送。

| 项目 | 状态 | 产物 |
| --- | --- | --- |
| 两目标架构四项复测 | 完成；每架构 4/4 分歧，两库五轮一致 | TYPES.tsv、MEASUREMENTS.json、raw/ |
| 真 ARM podofo 头与 Seek | 完成编译核查；两侧名称不同，虚调用索引均为 6；未作产品运行判断 | RPM_BINDING.json、SEEK_SYMBOLS.json、VIRTUAL_CALL.json |
| 已登记公开签名的消费方查询 | 正向对照通过；5 条声明、4 个身份，限定范围内无新增确认边 | BOUNDARY_CONTROL.json、BOUNDARY_QUERIES.json |
| 汇编增补与旧版保留 | 完成；只改五份既有文件，新增版本说明 | SUMMARY.diff、SUMMARY_UPDATE.json、summary_v1/ |
| 自检 | PASS；旧版 24 文件与原提交一致 | AUDIT.json、SCRIPT_SHA256.json |

结果、限制与自行判断见 [FINAL.md](FINAL.md)。不因未找到 UND 排除虚调用等间接消费者；其余 568 项没有重开。

提交推送入口为 code/deliver.py，只暂存本轮目录及汇编六个具名文件，不暂存其他工作。Git 使用普通 I/O 优先级。Git 操作的命令、原始输出、退出码及远端核对记录另存本地 tmp/R119_ENUM_RETEST/delivery/，避免把提交自身的日志再次递归提交；最终提交与远端 SHA 在交接答复中报告。
