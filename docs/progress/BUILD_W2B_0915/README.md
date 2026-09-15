# bcc-tools / bpftrace 续跑材料

结论：`PARTIAL`。2026-09-15 的 LLVM armv7l 重建因 I/O 复测超时中止；
bcc-tools 原四格 PASS 不变，armv7l 两格未测，bpftrace 未实施。
没有包提交或包推送。

阅读顺序：

1. `REPORT.md`：结果、输入身份、I/O 异常与未完成项。
2. `MATRIX.tsv`：两包验证矩阵。
3. `PACKAGE_STATUS.tsv`：11 包当前状态（沿用既有提交和已通过格）。
4. `COMPATIBILITY_REVIEW.md`：逐包条件与 GCC 路径的结构核查。
5. `spec_audit/`：11 包完整 spec 快照、revision、哈希与实际代码行号。
6. `raw/`：命令、stdout/stderr、退出码及开始/结束时间。

本轮脚本在本目录保存，SHA256 见 `raw/011_script_snapshot.*`。
`verify_bcc_cell.sh` 已准备，但本轮没有新 bcc RPM，未执行它；不能把
脚本准备工作或语法自检当成五次产品核查已通过。
