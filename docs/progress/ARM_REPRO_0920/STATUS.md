# 原 argv 复现：预检与现场保护

**最终状态（2026-09-20 16:22:35+08）：STOP_VV_FAILURE_DISAPPEARED。** 第一轮原argv复现退出1、18包；第二轮只增加-vv（另存trace）退出0、22包，22份摘要校验通过。已停止后续实验，不改host过滤、不加guest跟踪、不插桩；归档局部变量仍NOT_OBSERVED。详见FINAL_RESULT.md、FINAL_AUDIT.json、VV_RESULT.json。以下均为历史阶段快照，不能当作仍在运行的状态。

第二轮（vv-only-repro）已启动：2026-09-20约15:30。唯一构建变量为新增-vv，host strace保留旧status=failed配置、binfmt入口和原输出树；trace另存host-vv-original-config.strace。第一步失败树已完整备份并核验16987项。第一步证据已推送20fddc48b2b1c96bbffc8d409751981a0686772b。第二轮若成功立即停止；总截止仍2026-09-21 02:15:44+08。以下为此前阶段记录。

第一步已完成：2026-09-20 15:23:23，退出1，18份RPM，第7559/7560行与历史失败完全相同；失败现场已取证。详见STEP1_RESULT.md/JSON。下一步先推送本阶段证据、保存失败树，再仅增加-vv进行第二轮。下列14:29起的内容是运行中快照，不是最新结论。

14:29:38 已启动第一步。原现场16987项备份逐文件核验通过。实际Ninja PID608023的cgroup为arm-repro-0920.scope，memory.max=16536457216、nice19、ionice idle；命令含-j1，原96个增量目标已完成，进入调试信息提取/写包前处理。当前未观察到最终退出码。后续只读反汇编明确看到两架构cpio偏移字段均为64位；历史_FILE_OFFSET_BITS宏选项仍未观测，详见PRECHECK.md，不再把两者混淆。

人工已确认保留原宿主 strace 前缀；唯一 argv 偏离为 -o 后输出文件改为本轮目录。RPM/BUILDROOT/TMP 路径、可写绑定和构建入口不变。旧输出树约4.9GiB先完整复制到 tmp/ARM_REPRO_0920/previous-weekend-rpm-arm-llvm 并逐文件校验，原树不移走、不删文件。这样后续同路径执行不会丢失旧证据；复制不等于重置 BUILD 中间状态。

本次沿用最早预检时间计时，第一步截止2026-09-20 17:15:44+08，总截止2026-09-21 02:15:44+08。资源实际 MemoryMax=16536457216，nice19/idle；构建 guard 会核验实际 Ninja 的 cgroup 与 -j1。第一步只运行一次；成功或非原形态失败立即停。

已完成廉价预检：armv7l rpm 为32位ELF，aarch64 rpm为64位ELF；两套 rpmio 均导入 lseek64/stat64/fstat64。精确历史编译宏和 off_t 类型调试信息未找到，NOT_OBSERVED。五个ARM文件合计344668300字节，小于2GiB，现存旧BUILDROOT逐项大小与包清单一致；不作为溢出根因。aarch64既有12包没有lldb-debuginfo，对应包的同项载荷NOT_AVAILABLE。原始命令见raw/009、011、014、015。

以下保留确认前记录，不作为当前阻断状态。

2026-09-20 14:15 +08 开始预检；未启动构建、未变更输入或输出树。

- medium 资源门禁退出 0。
- 磁盘可用 222325358592 字节，大于 20 GiB。
- 原读写/fsync/校验探针退出 0，0.062054 秒；仅删除探针自身新建文件。
- 当前分支 codex/r5-ehabi-diagnosis。
- 原 argv JSON SHA256：38b3dd533205c58777e9fce51fe0b26ecefb174e6144eef3bed3bfea69e89b4f。
- spec SHA256：cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68，符合任务给定值。

阻断是执行定义冲突，不是资源问题：原 JSON 已含宿主 strace 前缀，并非无仪表命令：

```text
strace -f -yy -e trace=%file,read,pread64,write,pwrite64,lseek,ioctl,copy_file_range,sendfile -e status=failed -s 160 -o /home/toolchain/development/libc++_replacement/progress/BUILD_WEEKEND_0918/llvm-arm-rpm-failed-syscalls.strace bwrap ...
```

若原 argv 逐字节不动，则首轮已带宿主 strace；若首轮无仪表，须删该前缀，不再是原 argv。第二步第4条“增加宿主 strace”也需随首轮选择明确是新增还是替换原过滤项。

请求人工选择：首轮保留原宿主 strace，还是首轮删除 strace 前缀、只保持 bwrap 起全部参数不变。未自行选择，不启动复现。历史 trace 和 RPM/BUILDROOT 仍未触碰；后续执行前须保存旧现场，防止同路径覆盖证据。

32/64位 off_t 与包载荷预检尚未执行，NOT_OBSERVED；后续待明确首轮定义后继续。完整命令、输出、退出码见 raw/001–004。最初只读查看的命令为 pwd、rg --files 检索 guard/upstream-cpio、sed guard.py、cat 原 argv；执行退出0，原 argv 全文随后由 raw/004 落盘。
