# 原 argv 复现与仅增加 `-vv` 的对照结果

## 结论与停止原因

**STOP_VV_FAILURE_DISAPPEARED：原 argv 复现失败；仅增加 `-vv` 的下一轮成功。已按人工规定立即停止后续实验，等待审阅。**

| 轮次 | 实際开始与结束（北京时间） | 返回码 | 写出 RPM | 观测 |
|---|---|---:|---:|---|
| 原 argv，保留原 host strace | 09-20 14:29:38 → 15:23:23 | 1 | 18 | 第7559/7560行再次出现与历史相同的最后Wrote和归档错误 |
| 只增加 `-vv`；host strace配置不变 | 09-20 15:30:22 → 16:22:35 | 0 | 22 | 第57586行写出lldb-debuginfo，整轮成功结束 |

控制器计时分别为3224.600638秒（含失败后立即取证）与3133.192044秒。两轮均记录96个增量构建目标完成。不是从空目录全量重编LLVM，也不是重复验证候选功能或迁移门禁。

本次证明：在恢复的原命令条件下，目标失败仍可复现；在随后仅增加`-vv`的命令条件下，目标失败消失。这是仪表相关的**线索**，不是`-vv`修复问题的证明，也不足以排除共享中间树状态、时序等尚未识别因素。没有把第二轮成功归结成已确定的根因。

## 1. 命令与输入约束

- `original-argv.json`为原`progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json`的字节副本，SHA256：`38b3dd533205c58777e9fce51fe0b26ecefb174e6144eef3bed3bfea69e89b4f`。
- 第一轮只改数组索引10，即host strace的`-o`输出路径，改为本轮`host-original-config.strace`；原trace证据未覆盖。见`argv-only-difference.json`。
- 第二轮相对第一轮只插入一个`-vv`（在`/bin/rpmbuild`后），并将trace另存`host-vv-original-config.strace`。移除该参数并恢复trace文件名后，argv数组逐元素完全相等；见`vv-only-difference.json`、`FINAL_AUDIT.json`。
- 两轮均保持binfmt入口、原host `status=failed`过滤、原bwrap绑定、原可写`/tmp/task`、`/tmp/task/rpm-arm-llvm/`输出路径、`--noprep --noclean -bb`、`_binary_payload w5T1.xzdio`与`_build_create_debug 1`。未显式调用qemu-arm、未加guest `-strace`。
- spec SHA256在复现前后均为`cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`。控制器两次执行的SHA均为`42c9d64d88faba137c31fec68679adbbc1776b03ec7a1c5ddf7feb5390004510`。
- 固定PATH、实际命令与资源核验均在各轮`events.jsonl`；执行脚本快照及SHA在`script-snapshots*/`。命令原文、stdout/stderr和退出码在`raw/`。

“argv相同”不等于“整棵BUILD树恢复历史字节状态”：两次按授权在同一共享增量树运行，没有回滚或重建所有中间文件。这一限制必须保留，不能把顺次两轮当成冻结所有时变因素的重复试验。

## 2. 第一轮失败的原文与现场

`cells/original-argv-repro/build.log:7559`：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
```

紧接`:7560`：

```text
error: create archive failed: cpio: write failed - Function not implemented
```

与历史两次失败的行号、末包与文本一致；本次第一步不是“成功后直接停止”的分支。18份RPM逐一`rpm -K --nosignature`通过，但**整轮失败**。完整命令、包名、校验与SHA见`STEP1_RESULT.json`，说明见`STEP1_RESULT.md`。

失败后立即记录目录`ls -la`、五个相关debug文件和RPM的大小/mtime/ctime/权限/属主，再计算SHA；相关文件在该cell的`failure-permissions.txt`、`failure-file-metadata-immediate.json`、`failure-file-metadata.json`。失败后进程列表仅是事后快照，不是归档期间持续写入监控。

在第二轮启动前，完整复制第一轮输出树至`tmp/ARM_REPRO_0920/step1-failure/`，16987项校验通过，见`step1-failure-preserved-manifest.json`和`raw/084_*`。历史周末树另存`tmp/ARM_REPRO_0920/previous-weekend-rpm-arm-llvm/`，同样16987项通过，见`preserved-input-manifest.json`。未清理这两份现场。备份保留RPM、BUILDROOT、TMP等输出树，不宣称包含整个共享BUILD输入树。

## 3. 第二轮成功的原文与核验

`cells/vv-only-repro/build.log:57575`仍先写出`clang-devel-debuginfo`，`:57586`明确为：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/lldb-debuginfo-22.1.8-1.armv7l.rpm
```

`:57612`写出最后一份`libomp-debuginfo`；`events.jsonl`末条记录`finished`、`exitcode: 0`，时间为`2026-09-20T16:22:35.828689+08:00`。

前18个包的名称及写出顺序与失败轮完全相同。本轮多出以下4份（观察来自本轮成功日志，**不能用其顺序反推旧失败轮具体的内部失败成员**）：

1. `lldb-debuginfo-22.1.8-1.armv7l.rpm`
2. `lldb-devel-debuginfo-22.1.8-1.armv7l.rpm`
3. `compiler-rt-debuginfo-22.1.8-1.armv7l.rpm`
4. `libomp-debuginfo-22.1.8-1.armv7l.rpm`

实存22份RPM，名称集合与22条Wrote一致，22/22通过`rpm -K --nosignature`；每包SHA、大小、核验命令/输出/退出码在`VV_RESULT.json`。逐项顺序对照为`RPM_ORDER_COMPARISON.tsv`。摘要校验不是签名验证、安装测试或功能验收；未推进下游bpftrace。

**日志并非零报错**：两轮均有8条相同的`SOURCES/llvm.spec`打开/查询报错（第一轮7086–7105，第二轮7140–7159）；两轮均继续执行，第二轮最终退出0。这些原文收录于`FINAL_AUDIT.json`，未掩盖、未修复，也未据此推断与归档失败的关系。

## 4. 便宜预检结果

详见`PRECHECK.md`、`payload-precheck.json`及`raw/011`、`018`、`021`等：

- ARM rpm为32位ELF，但实际librpm里的cpio偏移字段及相关加法按64位处理；aarch64对应字段也是64位。证据为实际二进制反汇编，不以“32位进程”推定32位off_t。
- 历史`_FILE_OFFSET_BITS=64`编译命令原文：**NOT_OBSERVED**；不能把未找到宏记录说成偏移字段32位。
- ARM五个普通文件载荷合计**344668300字节**，小于2GiB；不支持据此提出2GiB溢出解释。
- aarch64既有完整12包集合不含`lldb-debuginfo`，对应包同项载荷**NOT_AVAILABLE**；已查其RPM清单，不能自行构造对照值。

## 5. 按停止条件未执行的项目

| 项目 | 状态 | 原因 |
|---|---|---|
| 更换host strace，记录成功write/pwrite64等 | 未执行 | `-vv`轮已使目标失败消失，按指令停止 |
| 显式qemu-arm客体-strace | 未执行 | 同上 |
| gdb/独立RPM副本插桩 | 未执行 | 同上；未替换根内RPM |
| fileend、offset实际值及差值 | NOT_OBSERVED | 原轮未采集局部变量；带-vv轮未失败 |
| 精确失败成员、写头时st_size、Fwrite返回、失败瞬间errno、rpmPackageFilesArchive返回码 | NOT_OBSERVED | 未到允许的归档层取值步骤，未从消息或顺序倒推 |
| 按差值区分源文件短读与压缩层失败 | 未判定 | 必要的实测差值缺失 |

本任务目前得到的是可复现失败与仪表变化的对照，**并未完成根因定位**。不得把`Function not implemented`当成本轮抓到了失败系统调用，也不得把`-vv`列为修复方案。

## 6. 资源、交付与自行判断

- 开工medium门禁0，I/O探针均在30秒内（初始0.062054秒，第二轮前0.092558秒）；磁盘始终在已记录检查点高于20GiB。
- 两轮实际串行Ninja、nice19、ionice idle、cgroup `memory.max=16536457216`均有现场核验；第二轮结束后scope为inactive/dead（`raw/148`）。没有sudo、提权或开发板操作。
- 每轮只有96个增量目标，未达到“每500目标”的中途I/O探测触发点；不能宣称构建期间连续测量I/O。没有触发I/O暂停重试。
- 第一轮证据已单独commit/push：`20fddc48b2b1c96bbffc8d409751981a0686772b`。第二轮与本报告只推项目`codex/r5-ehabi-diagnosis`，推送收据另存。第一轮推送前后`codex/runtime-validation`远端SHA曾变化（`raw/078`与`083`）；本次仅显式推编译线，不能宣称观察到的其他分支均未变化。
- 大trace无损压缩分片入库，`DELIVERY_MANIFEST.json`记原始SHA/大小、分片路径和还原核验；没有因体积静默删节。RPM与完整BUILDROOT留在上述本地路径，不作为小型证据文档提交；逐文件清单与SHA入库。
- 自行判断限于：按退出码、原错误文本和Wrote清单判定停止门槛；使用源码与实际二进制反汇编判定偏移字段宽度；把两轮结果的因果关系保留为未知。没有选定修法、未修改平台源码/spec/config、未动根内RPM、未推包仓。
- 尚存疑问：为何增加`-vv`后的这一轮成功，以及失败时归档层真实数值为何。按指令先停下，不通过追加重复试验自行寻找答案。
