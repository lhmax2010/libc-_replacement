# w5.xzdio 单参数复现

开始 2026-09-21 19:03 +08:00；截止 23:03 +08:00（四小时）。
不改 spec、平台源码、配置或构建根工具。codes/ 只读。仅本轮命令行改变压缩参数；去掉前一轮 GDB 入口，恢复原始 binfmt argv，保留原 strace。

## 执行约束与证据保护

完整 argv 与原 JSON 对照，只有 index 61 从 `_binary_payload w5T1.xzdio` 变为 `_binary_payload w5.xzdio`。包括 strace -o 在内，其余每个参数逐字节相同。为避免破坏历史证据，原 strace 先 cp -a 备份并校验 SHA256；执行结束后把本轮 trace 保存到本轮 cell，再恢复历史路径的原文件并核验。上一轮失败输出树也完整备份，不删除。

宏预检在原 bwrap 环境中，复用原 rpm 宏源、rcfile、目标及命令行 --define，`rpm --eval '%{_binary_payload}'` 退出 0、输出 `w5.xzdio`。未写配置。见 PAYLOAD_PRECHECK.json。

线程数仅由宿主 /proc 只读采样；不注入 guest、不加 GDB、不加 LD_PRELOAD。记录构建日志位置与当前 Wrote 数，区别构建期和压缩期。host rpmbuild/QEMU 的总线程数不能直接等同于压缩 worker 数。历史 T1 轮 12:57:30 的 Threads=2、13:27:43 的 Threads=3，均原样报告，不强行对齐预期。

构建总内存上限 16536457216 字节（50%），nice 19、ionice idle、Ninja -j1；资源约束由实际进程检查。截止时间固定 23:03 +08:00。共享增量树限制保留，不声称历史环境逐文件相同。

read-only 辅助脚本复制前的下列读取均 exit 0，工具输出用于 apply_patch，现追记：

```sh
sed -n '1,300p' progress/ARM_REPRO_0920/guard.py
sed -n '1,300p' progress/ARM_REPRO_0920/capture_failure.py
sed -n '1,200p' progress/ARM_LZMA_MODE_0921/preserve_previous.py
```

raw/005、006 的 rg 退出 1 表示指定范围无匹配；随后在 .status 与宿主快照 JSON 中找到线程记录。raw/018 退出 1 是最后的进程名检索无匹配（复制已进入校验），不是构建失败。

开场只读命令未经日志器，现追记（exit 0；宽泛检索的工具显示输出被截断，后续改为限定文件检索，不把截断结果当作完整证据）：

```sh
pwd; date -Is; git branch --show-current; ls progress/ARM_REPRO_0920; sed -n '1,220p' progress/ARM_REPRO_0920/original-argv.json; rg -n '四份|第五份|9884348b|6b5e306d|Threads|线程数' progress/ARM_RPM_DIAG progress/ALIGN_0920 progress/ARM_REPRO_0920 progress/ARM_LZMA_0921B --glob '*.md' --glob '*.json' --glob '*.py'
# exit 0
```
