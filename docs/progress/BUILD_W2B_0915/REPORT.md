# bcc-tools armv7l 续跑与 bpftrace 前置状态

## 结论

`PARTIAL`。本轮没有新增通过格，也没有包提交或包推送。

开工资源检查与小规模 I/O 测试通过，但 LLVM 重建期间再次观测到明显
I/O 延迟。按任务“耗时明显异常则停下报告，不要硬跑”的要求中止。
既有 bcc-tools 四格结果保留；armv7l 两格仍为 `NOT_OBSERVED`。
bpftrace 因前置未闭合未实施。

## 已执行与证据

### 资源、I/O 和残留审计

- `raw/001_resource_gate.*`：medium 退出 0，load1=3.33/20，
  MemAvailable=13,521,032 KiB，磁盘可用 334,482,752 KiB。
- `raw/002_io_probe.*`：在实际 `tmp/GBS-ROOT` 下新建临时目录，写入
  8,912,896 字节，fsync，读回并逐字节比对，再删除。写入及 fsync
  0.091996 秒，读回比对 0.007689 秒，总计 0.150790 秒，退出 0。
- `raw/003_initial_audit.*`：未见遗留 GBS/Ninja/编译器进程。旧 LLVM
  工作树与构建根仍在，未清空。
- 后续 `raw/013_io_recheck.*`：同一脚本、同一目录、同样数据量，
  30 秒超时，退出 124；没有形成成功的读回结果。
- `raw/014_io_anomaly_stop.*`：同时记录到 git/clang 的
  `folio_wait_bit_common` 磁盘等待；探针运行期间另观测到
  `wait_on_buffer`。不能从这些现象确定具体磁盘或内核故障原因；
  本轮只据实判定 I/O 不满足继续构建条件。

`io_probe.py` 的首轮是热/新文件的小规模读写测试，不证明大型源码树
的所有后续 I/O 都正常。复测失败是本轮新增的直接证据。

### armv7l LLVM 输入定位与重建

- `raw/006_targeted_arm_inputs.*`：既有目录剩余运行库是
  `libllvm-22.1.8-109.3.armv7l.rpm`，此前已验证为 libstdc++ 对照产物。
  `108.2` 的 debuginfo 仍在，但不能替代 libc++ 运行库。
- 原 libc++ 构建日志末尾为 6338 秒；同架构 GCC 基线耗时 7188 秒。
  本轮按 2–3 小时估计，低于四小时；估计依据已落盘，没有据此保证完成。
- 使用 `tmp/BUILD_W1BC_0911/target-fetch`，HEAD 精确为
  `f203923a1508c9344f5fc6b17bd8822f011655c4`，工作树干净。
  spec SHA256 为
  `e6b52c4391c605741dadada1faa9f5376e592bb5b3f05949f13d3eaebfd08e67`。
- 新建构建根 `tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l`，没有复用或清理旧根。
- `raw/009_rebuild_llvm_armv7l.*` 为完整日志；进程以 nice 15、ionice 3
  启动，GBS threads=1、Ninja jobs=2。
- `raw/012_llvm_configuration_observation.*` 确认实际 C/C++ 编译器均为
  Clang 22.1.8，CMake 启用 `LLVM_ENABLE_LIBCXX=ON`，解包的是
  `mlgo_arm_model_libcxx_tf218.tar.gz`，并嵌入对应 XLA runtime 对象。
- 编译到 `801/7147` 后按 I/O 停止要求中止；日志有
  `ninja: build stopped: interrupted by user`。未写出本轮 LLVM RPM，
  因此标准库依赖的最终产物核查为 `NOT_OBSERVED`。

停止后顶层包装器记录退出码 137，Ninja 的原始输出明确为人工中断；不将
该退出码解释为源码编译回归。`raw/017_final_process_snapshot.*` 已确认
本次列出的构建进程全部消失、I/O 探针临时目录无残留。中断后的构建根
保留约 6 GiB 的输入和半成品（由可用空间差额估计，未做全树精确计量），
未再次发起大规模删除。

### bcc-tools 和 bpftrace

| 包 | armv7l libc++ | armv7l GCC | aarch64 libc++/GCC | x86_64 libc++/GCC | 本轮推送 |
| --- | --- | --- | --- | --- | --- |
| bcc-tools | NOT_OBSERVED | NOT_OBSERVED | PASS / PASS，沿用 | PASS / PASS，沿用 | 无 |
| bpftrace | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED / NOT_OBSERVED | NOT_OBSERVED / NOT_OBSERVED | 无 |

bcc-tools 未提交的 15 行 spec 候选保持原样。两格均尚未开始，要求的
“各 5 次核查”也未执行；仅已准备核查脚本，不计为验证通过。
既有四格证据为 `docs/progress/BUILD_W2_0912/checks/`，本轮不重跑。

这两格原计划是构建与 readelf/符号检查，不需要物理板。未连接或占用
开发板；当前项目工作树中 `docs/progress/ADAPTOR_STATUS_0912.md` 不存在，
因此也没有自行推断板子已释放。

## 兼容性交叉核查

已取回并逐包检查 11 包 spec，完整快照、revision、SHA256 见
`spec_audit/inputs.tsv`，关键代码行号见 `spec_audit/guard_context.txt`，
逐包说明见 `COMPATIBILITY_REVIEW.md`。

已完成适配的 9 包及 bcc-tools 本地候选均将本次新增 libc++ 选项置于
约定条件内。bpftrace 尚无适配改动。该结构核查不能替代尚未执行的
armv7l GCC 构建；原已完成格的实测限定仍保留，包括 tensorflow2 的
armv7l/GCC 是“与基线同点同因失败”，不是完整构建成功。

## 技术性调整、自行判断与疑问

1. 初次广域查找旧 RPM 遍历范围过大，主动终止（143），改为查询已登记的
   输出目录；保留原始命令与部分输出，不声称扫描了全部磁盘。
2. 选用全新构建根，避免上轮旧根清理问题；使用上一轮成功的 Base-only
   配置形态和本地 libc++ devel 辅助仓。配置及 URL 已存档。reference
   是滚动输入，不将其描述为固定快照。
3. 在负载和磁盘等待增加后复测 I/O，并因同一探针超时而停止。这是基于
   实测的停止判断；没有把环境阻断称为 bcc-tools 的代码失败。
4. bpftrace 现有 `ExclusiveArch: armv7l aarch64` 与六格要求冲突。已提出
   人工确认，本轮没有自行扩展架构、删除声明或放宽门禁。
5. 下一次需要 I/O 稳定的构建环境，或可验证的 armv7l libc++ LLVM RPM。
   已保留新根及部分日志，是否能增量恢复需在稳定环境中核查，不能保证。
6. 材料自检脚本首次有括号笔误，退出 1（`raw/020_*`）。仅修正 Python
   语法并排除正在执行的自检自身后，`raw/021_*` 退出 0：矩阵 6 行、
   状态表 11 行、列数一致、已结束命令记录齐全、11 包 spec 哈希吻合。

最终包远端核对见 `raw/018_final_refs.*`：bcc-tools 仍为
`dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2`，bpftrace 仍为
`30e51cd665360f85b665308dc13ba27df0c5f739`。

未修改上游 C++ 源码；未修改本轮两包的 spec；没有包提交或包推送。
