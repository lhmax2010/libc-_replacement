# 入口 mode 单点增量观测

2026-09-21 17:00 +08:00 开始，沿用六小时时限，截止 23:00 +08:00。
先原样归档并推送 ARM_LZMA_0921B 最终材料，再进行本轮试验。

本轮工作记录暂存于此独立目录，避免归档上一轮时读到正在写入的自身日志；最终材料归入 docs/progress/ARM_LZMA_0921B/mode-entry/。
不改上一轮最终报告的历史交付状态说明；由本轮远端校验记录补充。

## 先行交付已完成

17:02 左右普通推送成功，远端核实为 `85e8238b72a2fab9505a9fc306b57ecf2ff69134`。完整清单含 895 个源文件，三份大型记录无损压缩并解压回验；没有遗漏源文件。提交前凭据扫描通过，只提交指定文档子树。见 raw/004、009、010、011、012、013。

## 本轮单点增量与边界

保留上轮失败配置的三个观察点，仅新增 `lzopen_internal` 入口，全部条件为 `lzclose` 计数等于 18。不再启用上一任务的条件 b 额外返回断点；若条件变化只记录，不扩展本任务仪表。
入口使用已核对的实际 ARMv7 ELF 函数起点 0x132b4（运行时加库基址），r0 为 mode。仅读 16 字节，不调用 inferior 函数，不写内存，不 finish 或单步；读取后继续。
首个和后续终止符若未出现在这 16 字节内，只报告边界截断，不越界补读或推断完整字符串。可打印形式中非 ASCII 可打印字节用点表示；十六进制是无损原文。

rpmbuild/QEMU/bwrap argv 与上轮有效 GDB 轮逐项比较，只有 strace 输出证据路径变更。strace 过滤器、guest 参数、输出 RPM 目录均不变。共享增量 BUILD 树的时变限制仍在，不能声称恢复了历史文件的全部状态。

实际资源为总内存 50%（16536457216 字节）、nice 19、ionice idle、构建并行度 1。启动后由 guard 校验实际 cgroup 与调度进程。初始门禁退出 0，磁盘 186 GiB，I/O 0.116 秒。
先将上一失败输出树复制到 `tmp/ARM_LZMA_0921B/previous-gdb-failure`，逐文件 SHA256 核对后才复现；不删除原现场。

准备中的 diff 退出 1 表示有预期的仪表脚本差异，不是构建失败；完整差异留在 raw/017。脚本复制与准备均保留源码快照。创建日志器所用的 `cp progress/ARM_LZMA_0921B/run.py progress/ARM_LZMA_MODE_0921/run.py` 退出 0；文件与原版相同。

开场两次只读检查未经过日志器，命令及退出码追记如下（输出已在工具记录中，不能伪称为原始日志器输出）：

```sh
pwd; git branch --show-current; git status --short; ls progress/ARM_LZMA_0921B; ls docs/progress/ARM_LZMA_0921B | head
# exit 0
pwd; git status --short; ls progress/ARM_LZMA_0921B; sed -n '1,200p' docs/progress/ARM_LZMA_0921B/PUBLISH_SCRIPT.py
# exit 0
sed -n '1,160p' progress/ARM_LZMA_0921B/run.py; sed -n '1,200p' progress/ARM_LZMA_0921B/check_staged.py; sed -n '1,180p' progress/ARM_LZMA_0921B/FINAL_RESULT.md; sed -n '1,200p' progress/ARM_LZMA_0921B/gdb_observer.py; date -Is
# exit 0
```
