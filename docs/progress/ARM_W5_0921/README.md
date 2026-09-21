# armv7l LLVM w5.xzdio 整轮写包验证

先读 FINAL_RESULT.md；命令的唯一差异见 ARGV_DIFF.json，隔离宏实测见 PAYLOAD_PRECHECK.json。
完整写包记录在 cells/w5-original/；22 包校验见 RPM_RESULTS.json，归档身份对照见 FIVE_STATIC_IDENTITIES.tsv。
线程原始采样与汇总分别为 cells/w5-original/threads.jsonl 和 THREAD_SUMMARY.json。
DELIVERY_MANIFEST.json 列出全部材料 SHA256。大型日志按 stored 顺序连接 gzip 分片后解压；已回验无损。
RPM、提取的静态归档与旧失败树保留在本机 tmp/，不提交大型二进制；报告列出路径和身份。
