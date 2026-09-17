# LLVM aarch64 完整 RPM：已完成

本项结果为 PASS；这不是 bpftrace 两架构三路径写包或板上验证完成。

- 最终命令：`raw/116_rpm_complete_retry.command.txt`；完整 rpmbuild（--noprep -bb，非 short-circuit）于 2026-09-17 14:17:21 +08 结束，退出码 0。
- 完整输出：`llvm-aarch64.log`，SHA256 `d7dd694d69cd4f75f341da4a10ec50999e8ef0bcdd54c9e8581958277626a701`。
- 12 个二进制 RPM 保留于 `tmp/STATIC_0917B/rpm-aarch64/RPMS/aarch64/`。逐包名称、版本、架构、大小、SHA256、文件清单见 `verify_aarch64/result.json`；逐包 RPM 摘要校验全部通过。大 RPM 不提交公开项目仓。
- 从实际 `llvm-static-devel` RPM 提取 `/usr/lib64/libclang.a`，SHA256 为 `84aef989b7fad82ba205a99943430f37a1e33e84e8098857d197e8588b288f87`。strings 中 `_ZNSt3__1` 命中 388 行，`_ZNSt7__cxx11` 命中 0 行；归档成员及 readelf ELF 头原文已保存。数字是命中行数，不是去重符号数，也不代替功能测试。
- 从实际 `libllvm` RPM 提取 `libLLVM.so.22.1`，readelf 确认依赖 `libc++.so.1`，不依赖 `libstdc++.so`。完整动态段见 `verify_aarch64/libllvm_dynamic.stdout`。
- aarch64 代码由已获批准的原生交叉工具集生成。工具切换等价性的依据是上一轮五目标抽查，不是逐目标等价证明。原工具链身份与本轮实际命令/资源证据见 `raw/055–057` 及上轮 `native-equivalence-summary.json`。

## 资源与技术性事件

并行 1、nice19、ionice idle；实际 cgroup memory.max=16,536,461,312 字节。十五次 500 目标周期 I/O 检查均通过；首轮打包 memory.max 曾触及上限，但 oom/oom_kill 为 0，没有放宽限制。详细资源事件与每小时记录保存。

首轮完整构建已于 13:37 返回 0（7546 个目标，含 6084 次对象编译公告）。随后外层记录器重复启动同一命令；这是执行记录方面的失误，不是新一格验证。已停止重复执行、可恢复地隔离混合/不完整 RPM 集合，换用一次性 Python 记录器并重新完整写包。最终集合不复用被中止写入的 RPM。事故、推断边界、修正、额外 10 次对象编译公告与约 40 分 20 秒额外墙钟时间见 STATUS.md；原始记录不改写。

首轮、重复轮、最终轮各记录 129 行 Missing build-id 警告（可能重复指向同一文件，不称 129 个文件）。这是观察到的打包警告，不能写成无警告；没有因此修改构建参数或平台源码。最终退出 0、RPM 摘要与目标载荷检查独立通过。

本项没有新增 sudo、板上操作、包仓提交或包仓推送。Source1002 保持原状。当前不能批准 bpftrace-static 候选替换发布资产，后续完整 RPM 与物理板验证仍待完成。
