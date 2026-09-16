# bpftrace-static 收尾核查交接

**候选当前不能替换发布资产。** 本轮出现实际静态取消失败，不只是缺少功能权限。
静态探针 0/20，动态对照 20/20；另有功能与完整 RPM 未闭合项。

阅读顺序：STATUS.md → W3_REPORT.md（实测失败）→ W1_REPORT.md → W2_REPORT.md → QUESTIONS.md。

原候选二进制、strip 副本、探针、完整运行时 RPM 与未完成构建根都留在
`tmp/NIGHT_0917/`，不覆盖原资产、不推包仓。本目录提交证据、哈希与脚本，
不把大型构建根和二进制放进项目 Git。RPM 的路径/大小/SHA256 在 rpm_outputs.tsv。
三路径 bpftrace RPM 未生成；不会用上轮的手动安装结果替代本轮 RPM 验证。

主要复核材料：

- w1_commands.json / w1_comparisons.json / w1_sizes.json：完整 CLI 命令、逐行差异、体积。
- runtime-aarch64-rpm.build.log、events.jsonl：真正 rpmbuild -bb、资源/I/O 检查。
- rpm_source_manifest.json / rpm_outputs.tsv：完整源输入及四个生成 RPM。
- w3_commands.json / cancel_matrix.json：两种链接配置、40 次输出和逐次判据。
- static_gdb_abort.json / static_gdb_registers_pc.json：SIGABRT 回溯、实际 PC 指令。
- static_abi_identity.json / *-unwinder-members.json：补丁符号、归档对象及 link map。
- raw/：逐条命令、退出码、起止时间及输出。053 退出 1 是**行为门禁失败**。
- final_source_checks.json / selfcheck.json：输入未覆盖和统计自检，不是任务验收通过。

脚本和执行副本均有 SHA256 清单。`.executed_*.py` 是按当时差异另存的旧版本，
用于对应技术性重跑；这些旧副本是事后恢复记录，不冒称当时自动快照。
真正执行过的 bwrap/编译/GDB 完整参数均在 JSON 或监控 events 中。
探针副本位于 inputs/condition_cancel_probe.cpp；其来源是运行时分支，未合并该分支。

本轮没有修改上游 C++、spec 逻辑或运行时行为。只补充临时构建路径、复制候选
输入、创建验证脚本及 probe 副本。项目工作树原有无关修改未纳入提交。
