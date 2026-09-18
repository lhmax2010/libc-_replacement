# 周末任务最终结论

**PARTIAL：bpftrace-static 新候选尚不能替换发布资产。** 没有推包仓、没有覆盖 Source1002；11个包既有源码适配发布状态不变。

1. **W1**：bpftrace四个GCC/未定义工具链完整RPM格通过，static载荷与原件字节相同；aarch64 libc++写包成功但多出动态libbpf，ARM libc++在时限内未完成（约88%，断点保留）。
2. **W2**：ARM LLVM整轮再次退出1，仍为cpio `Function not implemented`；18份新RPM单独核验通过，但整轮状态不升级。跟踪未能归属根因。
3. **W3**：ARM物理板共享libgcc_s取消20/20通过，**静态libgcc对照也20/20通过**。不可外推x86崩溃机制。候选RPM缺；原包安装预检缺BCC/Clang库，功能等价未闭合。板已清理、恢复权限并释放。
4. **W4**：11个已发布提交的spec条件保护复核通过，远端HEAD均匹配；历史构建结果沿用，不冒称11包本轮重建。

## 阅读顺序

- `W4_REPORT.md`：完整验收矩阵、未闭合项和自行判断。
- `PACKAGE_STATUS.md` / `.tsv`、`SPEC_GUARD_AUDIT.tsv`：11包状态及代码行证据。
- `W1_REPORT.md`、`W1_MATRIX.tsv`、`W1_RPM_MANIFEST.json`：六格与RPM身份。
- `W2_REPORT.md`：整轮失败、18包核验、历史rsync区别。
- `W3_REPORT.md`、`BOARD_CANCEL_MATRIX.tsv`：物理板结果和清理。
- `STATIC_CANCEL_WEEKLY.md`：带范围限制的周报用机制说明。
- `QUESTIONS.md`：需要人工裁决/补材料的事项。

执行文件在 `progress/BUILD_WEEKEND_0918/`；按阶段固定快照上传到 `docs/progress/BUILD_WEEKEND_0918/w1/`、`w2/`、`w3/`、`w4/`。大文本gzip保存，MANIFEST记录解压后SHA256；阶段完成后的Git回执顺延至下一阶段或最终交付回执目录，不伪造自引用提交记录。
