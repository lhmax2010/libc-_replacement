# 第一步：原 argv 复现成功（构建按预期失败）

**STEP_1_EXACT_FAILURE_REPRODUCED。允许按任务顺序进入仅加 -vv 的下一轮。**

- 开始2026-09-20 14:29:38.902870+08，控制器完成2026-09-20 15:23:23.503508+08，约53分45秒；rpmbuild失败后立即取证的记录时间15:23:19.607808+08。
- 进程返回 **1**，写出18份RPM，均通过独立 `rpm -K --nosignature`。包名集合与本轮18条Wrote逐项一致；这不是整轮构建成功。
- `cells/original-argv-repro/build.log:7559`：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
```

- 紧接第7560行：

```text
error: create archive failed: cpio: write failed - Function not implemented
```

与9月17日、周末W2两轮日志的位置和文本一致。本轮没有因为用回原入口而成功；不是任务2b或其他失败形态。此结果证明在本次恢复的原命令条件下目标失败仍能重现，不单凭三次同点失败断言所有时刻都确定失败。

## 单变量核验

原JSON字节副本是original-argv.json；实际启动argv见cell_commands/original-argv-repro.json和events.jsonl的start项。本轮与原数组逐元素比较，唯一不同为索引10（strace -o之后的文件路径）：改成本轮host-original-config.strace。完整差异在argv-only-difference.json。没有-vv、没有显式qemu-arm、没有客体-strace；原status=failed、w5T1.xzdio、_build_create_debug 1、--noprep --noclean -bb、可写/tmp/task绑定、RPM输出目录全部保留。

spec SHA256 cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68；原命令JSON SHA256 38b3dd533205c58777e9fce51fe0b26ecefb174e6144eef3bed3bfea69e89b4f。实际binfmt进程命令见raw/070；实际Ninja -j1、nice19、idle、memory.max=16536457216见events.jsonl。未触及内存上限/OOM（raw/072）；全部资源观测限于已记录时点，不伪称逐纳秒采样。

## 失败现场

控制器失败后立即执行ls -la，结果在failure-permissions.txt与events.jsonl；所有相关文件先记录大小、mtime/ctime、模式、uid/gid、inode，再读文件计算SHA256，分别在failure-file-metadata-immediate.json和failure-file-metadata.json。失败后的进程列表在failure-processes.json，不能拿它代替归档期间的持续写入监测。

本轮未清理失败BUILDROOT。旧周末现场已在运行前完整复制到tmp/ARM_REPRO_0920/previous-weekend-rpm-arm-llvm，16987项校验通过；保存本轮失败树副本后才可再次对原路径执行，以免覆盖唯一现场。原树路径不变，备份不是恢复旧BUILD中间状态。

## 本轮未取得的值

cpio的fileend/offset实际值、差值、写头时st_size、Fwrite返回值、失败瞬间errno与准确的内部失败成员：**NOT_OBSERVED**。本轮故意只用原仪表，不从文本倒推这些值；第一步的目的只是确认复现条件。

off_t/载荷预检见PRECHECK.md：实际ARM及aarch64的cpio偏移字段均按64位处理；ARM五个普通文件合计344668300字节，不支持2GiB溢出解释。历史_FILE_OFFSET_BITS编译宏原文未取得。

下一轮仅加-vv，host strace配置仍保持旧值；若失败消失，立即停止，不继续变更host trace或加guest trace。不改平台源码、spec、根内rpm，不推包仓。
